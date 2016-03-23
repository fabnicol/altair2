/*  Programme �crit par Fabrice NICOL sous licence CECILL 3
 *  Attention : lorsqu'il est �dit�, le pr�sent code doit �tre converti soit en UTF-8 soit en ISO-5589-1 (Latin-1)avant d'�tre compil�.
 *  En entr�e d'Altair pr�ciser encodage.entr�e en conformit� avec l'encodage du pr�sent fichier, qui sera celui de la base g�n�r�e.
 */

/* Constantes de compilation pouvant �tre red�finies : NA_STRING, MAX_LIGNES_PAYE, MAX_NB_AGENTS, NO_DEBUG*/

#include <iomanip>
#include <iostream>
#include <cstring>
#include "validator.hpp"
#include "fonctions_auxiliaires.hpp"
#include "table.hpp"
#include "ligne_paye.hpp"
#include "tags.h"

using namespace std;

extern mutex mut;
extern  inline uint64_t   parseLignesPaye(xmlNodePtr cur, info_t& info, ofstream& log);
extern vector<errorLine_t>  errorLineStack;
extern int rang_global;
template <typename Allocator = allocator<char>>
static inline string read_stream_into_string(
    ifstream& in,
    Allocator alloc = {})
{
  basic_ostringstream<char, char_traits<char>, Allocator>
    ss(basic_string<char, char_traits<char>, Allocator>(move(alloc)));

  if (!(ss << in.rdbuf()))
    throw ios_base::failure{"[ERR] Erreur d'allocation de lecture de fichier.\n"};

  return ss.str();
}

/* agent_total est une variable de contr�le pour info->NCumAgent */


// Probl�me des accumulations de champs <DonneesIndiv> non r�solu !

// -1 erreur, 0 : fichier petit; sinon nombre de fichiers d�coup�s.

int redecouper(info_t& info)
{

        #if defined(STRINGSTREAM_PARSING) || defined(MMAP_PARSING)
             string filest = info.threads->in_memory_file.at(info.fichier_courant);
        #else
             ifstream c(info.threads->argv.at(info.fichier_courant));
             string filest;
             filest = read_stream_into_string(c);
             uint64_t taille = info.taille.at(info.fichier_courant);
        #endif

        info.threads->argv_cut.push_back(vector<string>{});

        if (info.taille.at(info.fichier_courant) < CUTFILE_CHUNK) return 0;

        if (verbeux)
            cerr << PROCESSING_HTML_TAG "Red�coupage du fichier n�" << info.fichier_courant << ENDL;

        string document_tag = "", enc = "";
        string::iterator iter = filest.begin();

        while (*++iter != '?' && iter != filest.end()) continue;

        while (++iter != filest.end())
        {
            if (*iter == '?') break;
            if (*iter != 'e') continue;
            string::iterator iter2 = iter;
            //encoding="ISO-8859-1"
            if (*++iter2 != 'n' || *++iter2 != 'c') continue;
            while (*++iter2 != '?') continue;

            enc = string(iter, iter2);
            if (enc.find("encoding") == string::npos)
                enc = "";
            break;
        }

        while (iter != filest.end())
        {
            if (*++iter != 'D') continue;
            ++iter;
            const string s = string(iter, iter + 11);
            // <...:DocumentPaye>
            if (s != "ocumentPaye") continue;
            string::iterator iter2 = iter;
            while (*--iter2 != '<' ) if (iter2 == filest.begin()) break;
            document_tag = string(iter2 + 1, iter + 11);
            break;
        }

        int r = 0;

        uint64_t i = 0;
        uint64_t init_pos= 0;
        bool depuis_debut = true;
        static bool open_di;

        while (filest.at(i) != '\0')
        {
            if (i + CUTFILE_CHUNK < taille)
                i += CUTFILE_CHUNK;
            else
                depuis_debut = false;

            ++r;

            string s = "";
            bool fermeture_PI = false;

            if (depuis_debut)
            {
                while (filest.at(i) != '\0')
                {
                    while (filest.at(++i) != '<') continue;
                    if (filest.at(++i) != '/') continue;

                    if (filest.at(++i) != 'P') continue;

                    s = filest.substr(i, 16);

                    if (s == "PayeIndivMensuel")
                    {
                        fermeture_PI = true;
                        break;
                    }
                    // </PayeIndivMensuel>
                }
            }
            else
            {
                i = taille - 16;
                while (init_pos < i)
                {
                    while (filest.at(--i) != 'P') continue;
                    if (filest.at(--i) != '/') continue;

                    if (filest.at(--i) != '<') continue;

                    s = filest.substr(i + 2, 16);

                    if (s == "PayeIndivMensuel")
                    {
                        fermeture_PI = true;

                        break;
                    }
                }
            }

            if (! fermeture_PI) return -1;

            i += 16;

            while (filest.at(++i) != '<') continue;

            s = filest.substr(i + 1 , 13);
            bool insert_di = (s != "/DonneesIndiv");
            if (! insert_di) i += 14;

            if (filest.at(i) != '<') while (filest.at(++i) != '<') continue;

            string header = (r > 1)? (string("<?xml version=\"1.0\" ") + enc + string("?>")
                                     + string("\n<DocumentPaye>")
                                     + ((! open_di)? "\n<DonneesIndiv>\n" : "")) : string("");

            string filest_cut = header + filest.substr(init_pos, i - init_pos)
                                       + ((insert_di)? string("\n</DonneesIndiv>") : "")
                                       + (string("\n</") + ((r > 1)? "DocumentPaye" : document_tag) + string(">"));

            s = filest.substr(i + 1 , 12);
            open_di = (s == "DonneesIndiv");

            init_pos = i;
            string filecut_path = info.threads->argv.at(info.fichier_courant) +"_" + to_string(r) + ".xhl";

            ofstream filecut(filecut_path);
            filecut << filest_cut;
            filecut.close();
            info.threads->argv_cut[info.fichier_courant].push_back(filecut_path);
            if (! depuis_debut) break;
            fermeture_PI = false;
        }

        return r;
}


static int parseFile(const xmlDocPtr doc, info_t& info, int cont_flag = PREMIER_FICHIER)
{
   ofstream log;
   xmlNodePtr cur = nullptr;
   xmlNodePtr cur_save = cur;
   static xmlChar  *annee_fichier,
                   *mois_fichier,
                   *employeur_fichier,
                   *etablissement_fichier,
                   *siret_fichier,
                   *budget_fichier;


   info.NAgent[info.fichier_courant] = 0;

    memory_debug("parseFile : parseFile doc");

    if (doc == nullptr)
    {
        cerr << ERROR_HTML_TAG " probl�me d'allocation m�moire pour le scan XML." ENDL;
        return SKIP_FILE;
    }

    if (! info.chemin_log.empty())
    {
        log.open(info.chemin_log, ios::app);
    }

    cur = xmlDocGetRootElement(doc);

    memory_debug("parseFile : parseFile get root");

    if (cur == nullptr)
    {
        cerr << ERROR_HTML_TAG "document vide" ENDL;
        xmlFreeDoc(doc);
        if (verbeux) cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode tol�rant)." ENDL;
        if (log.is_open())
            log.close();
        return SKIP_FILE;
    }

    cur =  cur->xmlChildrenNode;

    if (cont_flag != PREMIER_FICHIER) goto donnees_indiv;

    cur = atteindreNoeud("Annee", cur);

    if (cur != nullptr)
    {
        annee_fichier = xmlGetProp(cur, (const xmlChar *) "V");
        int annee;
        annee = (annee_fichier[0] == '\0')? 0 : atoi((const char*) annee_fichier);

        /* Alta�r est �crit pour durer 100 ans :) */

        if (annee <= 2000 || annee >= 2100)
        {
            cerr << ERROR_HTML_TAG " pas de donn�es sur l'ann�e." ENDL;

#ifdef STRICT
            if (log.open()) log.close();
            exit(-517);
#endif
            /* Il faudra sans doute ajuster les diff�rences entre le parsing C et l'analyse Xml, qui vont diverger */
            if (verbeux) cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode tol�rant)." ENDL;
            return SKIP_FILE;
        }

        cur = (cur)? cur->next : nullptr;
    }
    else
    {
        cerr << ERROR_HTML_TAG "Ann�e non d�tectable" ENDL;
        if (log.is_open())
            log.close();
#ifdef STRICT
        if (log.is_open())
            log.close();
        exit(-502);
#else
        /* Il faudra sans doute ajuster les diff�rences entre le parsing C et l'analyse Xml, qui vont diverger */
        if (verbeux) cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode tol�rant)." ENDL;
        return SKIP_FILE;
#endif
    }

    if (cur != nullptr)
    {
        mois_fichier = xmlGetProp(cur, (const xmlChar *) "V");
        int mois = atoi((const char*) mois_fichier);
        if (mois <= 0 || mois > 12)
        {
            cerr << ERROR_HTML_TAG " pas de donn�es sur le mois." ENDL;

#ifdef STRICT
            if (log.open()) log.close();
            exit(-517);
#endif
            /* Il faudra sans doute ajuster les diff�rences entre le parsing C et l'analyse Xml, qui vont diverger */
            if (verbeux) cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode tol�rant)." ENDL;
            return SKIP_FILE;
        }

    }
    else
    {
        cerr << ERROR_HTML_TAG "Mois non d�tectable" ENDL;

#ifdef STRICT
        if (log.is_open())
            log.close();
        exit(-503);
#else
        /* Il faudra sans doute ajuster les diff�rences entre le parsing C et l'analyse Xml, qui vont diverger */
        if (verbeux) cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode tol�rant)." ENDL;
        return SKIP_FILE;
#endif

    }

    cur_save = cur;
    cur = atteindreNoeud("Budget", cur);

    if (cur != nullptr)
    {
        cur_save = cur;
        cur =  cur->xmlChildrenNode;
        budget_fichier = xmlGetProp(cur, (const xmlChar *) "V");
        if (budget_fichier[0] == '\0')
        {
            xmlFree(budget_fichier);
            budget_fichier = xmlStrdup(NA_STRING);
            lock_guard<mutex> lock(mut);
            if (verbeux) cerr << STATE_HTML_TAG "Aucune information sur le budget [optionnel]." ENDL;
            if (verbeux) cerr << PROCESSING_HTML_TAG "Poursuite du traitement." ENDL;
        }

        cur = cur_save->next;
    }
    else
    {
        budget_fichier = xmlStrdup(NA_STRING);
        lock_guard<mutex> lock(mut);
        if (verbeux) cerr << STATE_HTML_TAG "Aucune information sur le budget [optionnel]." ENDL;
        if (verbeux) cerr << PROCESSING_HTML_TAG "Poursuite du traitement." ENDL;
        cur = cur_save;
    }

    cur_save = cur;

    /* REFERENCE */

    /* <Employeur>
    *      <Nom V="">{1,1}</Nom>
    *      <APE V="">{1,1}</APE>
    *      <Adresse>{1,1}</Adresse>
    *      <NumUrssaf V="">{1,1}</NumUrssaf>
    *      <Siret V="">{1,1}</Siret>
    * </Employeur>
    */

    if (nullptr == (cur = atteindreNoeud("Employeur", cur)))
    {

        warning_msg("la balise Employeur [non-conformit� � la norme].", info, cur);

#ifdef STRICT
        if (log.is_open())
            log.close();
        exit(-515);
#endif                 
        if (verbeux) cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode souple)." ENDL;
        employeur_fichier = xmlStrdup(NA_STRING);
        siret_fichier = xmlStrdup(NA_STRING);
        cur = cur_save;
    }
    else
    {
        cur_save = cur;
        /* On recherche le nom, le siret de l'employeur */
        do {
            cur =  cur->xmlChildrenNode;

            if (cur == nullptr || xmlIsBlankNode(cur))
            {
                cerr << ERROR_HTML_TAG "Pas de donn�es sur le nom de l'employeur [non-conformit� � la norme]." ENDL;
                if (mut.try_lock())
                {
                  errorLineStack.emplace_back(afficher_environnement_xhl(info, cur));
                  mut.unlock();
                }
                employeur_fichier = xmlStrdup(NA_STRING);
                siret_fichier = xmlStrdup(NA_STRING);
                break;
            }

            cur = atteindreNoeud("Nom", cur);
            if (cur != nullptr)
            {
                employeur_fichier = xmlGetProp(cur, (const xmlChar *) "V");
                cur = (cur)? cur->next : nullptr;
            }
            else
            {
                cerr << ERROR_HTML_TAG "Employeur non identifi� [non-conformit� � la norme]." ENDL;
                employeur_fichier = xmlStrdup(NA_STRING);
            }

            if (cur != nullptr) cur = atteindreNoeud("Siret", cur);

            if (cur != nullptr)
            {
                siret_fichier = xmlGetProp(cur, (const xmlChar *) "V");
                if (siret_fichier[0] == '\0')
                {
                    warning_msg("la balise Siret", info, cur);
#ifdef STRICT
                    if (log.open()) log.close();
                    exit(-517);
#endif
                   if (verbeux)  cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode souple)." ENDL;
                   xmlFree(siret_fichier);
                   siret_fichier = xmlStrdup(NA_STRING);

                }
            }
            else
            {
                cerr  << ERROR_HTML_TAG "Siret de l'empoyeur non identifi� [non-conformit� � la norme]." ENDL;
               if (verbeux)  cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode souple)." ENDL;
                cerr << "Ann�e " << annee_fichier
                          << " Mois "  << mois_fichier << ENDL;
                siret_fichier = xmlStrdup(NA_STRING);
            }

        } while (false);

        /* on remonte d'un niveau */

        cur = cur_save;
    }

    /* On effectue un saut direct par �tiquetage ici dans le cas de fichiers d�coup�s dont les informations
     * d'ent�te (ann�e, mois...) sont d�finits par les pointeurs statiques d�clar�s en d�but de fonction */

donnees_indiv:


    cur = atteindreNoeud("DonneesIndiv", cur);

    if (cur == nullptr || xmlIsBlankNode(cur))
    {
        cerr << ERROR_HTML_TAG "Pas de donn�es individuelles de paye [DonneesIndiv, non conformit� � la norme]." ENDL;
        warning_msg("la balise DonneesIndiv", info, cur);

#ifdef STRICT
        if (log.is_open())
            log.close();
        exit(-525);
#else
        /* Il faudra sans doute ajuster les diff�rences entre le parsing C et l'analyse Xml, qui vont diverger */
        return SKIP_FILE;
#endif
    }


    bool siret_etablissement =  false;

    while(cur != nullptr)
    {
        cur_save = cur;
        xmlNodePtr cur_save2 = nullptr;
        cur =  cur->xmlChildrenNode;  // Niveau Etablissement et PayeIndivMensuel
        cur_save2 = cur;

        if (cur == nullptr || xmlIsBlankNode(cur))
        {
            cerr << ERROR_HTML_TAG "Pas de donn�es individuelles de paye [non conformit� � la norme]." ENDL;
            if (mut.try_lock())
            {
               errorLineStack.emplace_back(afficher_environnement_xhl(info, cur));
               mut.unlock();
            }
#ifdef STRICT
            if (log.is_open())
                log.close();
            exit(-515);
#else
            /* Il faudra sans doute ajuster les diff�rences entre le parsing C et l'analyse Xml, qui vont diverger */
            return SKIP_FILE;
#endif
        }

        /* Les donn�es sur l'�tablissement sont optionnelles */

        /* REFERENCE
         *
         * <DonneesIndiv>
         *   <Etablissement>{0,1}</Etablissement>
         *   <PayeIndivMensuel>{1,unbounded}</PayeIndivMensuel>
         * </DonneesIndiv>
         *
         */

        cur = atteindreNoeud("Etablissement", cur);
        if (cur == nullptr)
        {
            cur = cur_save2;
            cerr << STATE_HTML_TAG "Pas d'information sur l'Etablissement" ENDL;
            etablissement_fichier = xmlStrdup(NA_STRING);
        }
        else
        {

            /* REFERENCE */
            /*
                 * <Etablissement>
                 *   <Nom V="">{1,1}</Nom>
                 *   <NumUrssaf V="">{1,1}</NumUrssaf>
                 *   <Adresse>{1,1}</Adresse>
                 *   <Siret V="">{1,1}</Siret>
                 * </Etablissement>
                */

            /* Alta�r a ici deux modes : strict, le non-repect de la norme entraine une exception
                 * ou souple : on continue l'ex�cution en consid�rant que bien que le nom/siret soit obligatoire,
                 * l'ensemble du champ lui-m�me est en fait optionnel. [la norme est quelque peu inconsistante sur ce point]
                 */

            cur_save2 = cur;

            /* On recherche le nom, le siret de l'�tablissement */
      do {
                cur =  cur->xmlChildrenNode;

                if (cur == nullptr || xmlIsBlankNode(cur))
                {
                    warning_msg("les donn�es nominales de l'�tablissement [non-conformit�]", info, cur);

#ifdef STRICT
                    if (log.open()) log.close();
                    exit(-517);
#endif
                    if (verbeux) cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode souple)." ENDL;
                    etablissement_fichier = xmlStrdup(NA_STRING);
                    /* on garde le siret de l'employeur */
                    break;
                }

                cur = atteindreNoeud("Nom", cur);

                if (cur != nullptr)
                {
                    etablissement_fichier = xmlGetProp(cur, (const xmlChar *) "V");
                    if (etablissement_fichier[0] == '\0')
                    {
                        warning_msg("les donn�es nominales de l'�tablissement [non-conformit�]", info, cur);
#ifdef STRICT
                        if (log.open()) log.close();
                        exit(-517);
#endif
                        if (verbeux) cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode souple)." ENDL;
                        xmlFree(etablissement_fichier);
                        etablissement_fichier = xmlStrdup(NA_STRING);
                    }

                    cur = (cur)? cur->next : nullptr;

                }
                else
                {
                    warning_msg("les donn�es nominales de l'�tablissement [non-conformit�]", info, cur);
#ifdef STRICT
                    if (log.open()) log.close();
                    exit(-517);
#endif
                    if (verbeux) cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode souple)." ENDL;
                    etablissement_fichier = xmlStrdup(NA_STRING);

                    /* on garde le siret de l'employeur */
                }

                if (cur != nullptr)
                    cur = atteindreNoeud("Siret", cur);

                siret_etablissement =  true;
                if (cur != nullptr)
                {
                    xmlFree(siret_fichier);
                    siret_fichier = xmlGetProp(cur, (const xmlChar *) "V");
                    if (siret_fichier[0] == '\0')
                    {
                        warning_msg("les donn�es de Siret de l'�tablissement [non-conformit�]", info, cur);
#ifdef STRICT
                        if (log.open()) log.close();
                        exit(-517);
#endif
                        if (verbeux) cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode souple)." ENDL;
                        xmlFree(siret_fichier);
                        siret_fichier = xmlStrdup(NA_STRING);
                    }
                }
                else
                {
#ifdef STRICT
                    if (log.open()) log.close();
                    exit(-517);
#endif
                    warning_msg("les donn�es de Siret de l'�tablissement [non-conformit�]", info, cur);
                    siret_fichier = xmlStrdup(NA_STRING);
                }

            } while (false);

            /* on remonte d'un niveau */

            cur = cur_save2;
        }

        /* REFERENCE */

        /* <PayeIndivMensuel>
              <Agent>{1,1}</Agent>
              <Evenement>{0,unbounded}</Evenement>
              <Service V="">{1,1}</Service>
              <NBI V="">{1,unbounded}</NBI>
              <QuotiteTrav V="">{1,1}</QuotiteTrav>
              <Periode>{1,1}</Periode>
              <Remuneration>{1,1}</Remuneration>
              <NbHeureTotal V="">{0,1}</NbHeureTotal>
              <TauxHor V="">{0,1}</TauxHor>
              <NbHeureSup V="">{1,1}</NbHeureSup>
              <MtBrut V="">{1,1}</MtBrut>
              <MtNet V="">{1,1}</MtNet>
              <MtNetAPayer V="">{1,1}</MtNetAPayer>
              <DatePaiement V="">{1,1}</DatePaiement>
              <MtImposable V="">{1,1}</MtImposable>
              <CumulMtImposable V="">{1,1}</CumulMtImposable>
              <CumulMtBrut V="">{1,1}</CumulMtBrut>
              <CumulBaseSS V="">{1,1}</CumulBaseSS>
              <RepartitionBudget>{0,unbounded}</RepartitionBudget>
              <PJRef>{0,unbounded}</PJRef>
           </PayeIndivMensuel>
       */

        while(cur != nullptr)
        {

            cur_save2 =  cur;
            cur = atteindreNoeud("PayeIndivMensuel", cur);

            if (cur == nullptr || cur->xmlChildrenNode == nullptr || xmlIsBlankNode(cur->xmlChildrenNode))
            {
                cerr << ERROR_HTML_TAG "Pas d'information sur les lignes de paye [non-conformit� � la norme : PayeIndivMensuel]." ENDL;

                if (cur == nullptr)
                        warning_msg("la balise PayeIndivMensuel de l'�tablissement [non-conformit�]", info, cur);
#ifdef STRICT
                if (log.open()) log.close();
                exit(-518);
#endif
                if (verbeux) cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode souple)." ENDL;
                /* Ici on ne risque pas d'avoir une divergence entre le parsage C et Xml */
                if (cur == nullptr)
                    cur = cur_save2->next;
                else
                    cur = cur->next;

                continue;
            }

            /* ici on sait que cur->xmlChildrenNode est non vide */
            cur_save2 = cur;
            cur = cur->xmlChildrenNode;  // Niveau Agent

            /* remarque atoi retourne z�ro s'il rencontre "" ou des caract�res non num�riques */

            info.Table[info.NCumAgentXml][Annee] = xmlStrdup(annee_fichier);
            info.Table[info.NCumAgentXml][Mois]  = xmlStrdup(mois_fichier);
            info.Table[info.NCumAgentXml][Budget] = xmlStrdup(budget_fichier);
            info.Table[info.NCumAgentXml][Employeur]  = xmlStrdup(employeur_fichier);

            /* Nota : le Siret est, si l'�tablissement existe, celui de l'�tablissement, sinon celui de l'Employeur */

            info.Table[info.NCumAgentXml][Siret]  = xmlStrdup(siret_fichier);
            info.Table[info.NCumAgentXml][Etablissement]  = xmlStrdup(etablissement_fichier);

            /* LECTURE DES LIGNES DE PAYE STRICTO SENSU */

            int32_t ligne_p = parseLignesPaye(cur, info, log);

            /*  */

            info.drapeau_cont = true;

            int diff = info.NLigne[info.NCumAgentXml] - ligne_p;

            if (info.reduire_consommation_memoire)
            {
                if (diff != 0)
                {
                  if (verbeux)
                    cerr << ERROR_HTML_TAG "Incoh�rence des d�comptes de lignes entre le contr�le C : "
                              << info.NLigne[info.NCumAgentXml]
                              << "et l'analyse Libxml2 : "
                              << ligne_p
                              << ENDL "Pour l'agent "
                              << "de rang  " << info.NCumAgentXml << " dans le fichier" ENDL
                              << info.Table[info.NCumAgentXml][Matricule]
                              << " Ann�e "
                              << info.Table[info.NCumAgentXml][Annee]
                              << " Mois "
                              << info.Table[info.NCumAgentXml][Mois]
                              << ENDL   ;

                    ecrire_log(info, log, diff);
                    if (log.is_open())
                        log.close();
#ifdef STRICT
                    exit(-1278);
#else
                    if (ligne_p > info.NLigne[info.NCumAgentXml])
                        info.NLigne[info.NCumAgentXml] = ligne_p;

                    /* il faut tout r�allouer */
                    exit(0);
                    return RETRY;
#endif
                }
            }
            else
                info.NLigne[info.NCumAgentXml] = ligne_p;

            info.nbLigne += ligne_p;

            ecrire_log(info, log, diff);

            // Ici il est normal que cur = nullptr

            cur = cur_save2->next;

            AFFICHER_NOEUD(cur->name)

            ++info.NAgent[info.fichier_courant];
            ++info.NCumAgentXml;
        }

        if (cont_flag == DERNIER_FICHIER_DECOUPE)
        {
        //  xmlFree(etablissement_fichier);
        }
        /* si pas d'�tablissement (NA_STRING) alors on utilise le siret de l'employeur, donc
         * ne pas lib�rer dans ce cas ! */

        cur = cur_save->next;  // next DonneesIndiv
        if (cur == nullptr || xmlStrcmp(cur->name, (const xmlChar*) "DonneesIndiv")) break;   // on ne va pas envoyer un message d'absence de DonneesIndiv si on a fini la boucle...
    }

#ifdef GENERATE_RANK_SIGNAL
        generate_rank_signal();
#endif

    {

        if (verbeux)
        {
            lock_guard<mutex> guard(mut);
            cerr << STATE_HTML_TAG "Fichier "
                   #ifdef GENERATE_RANK_SIGNAL
                      << "n�" <<  rang_global
                   #endif
                      << " : " << info.threads->argv[info.fichier_courant] << ENDL;

            cerr << STATE_HTML_TAG << "Fil n�" << info.threads->thread_num << " : " << "Fichier courant : " << info.fichier_courant + 1 << ENDL;
            cerr << STATE_HTML_TAG << "Total : " <<  info.NCumAgentXml << " bulletins -- " << info.nbLigne <<" lignes cumul�es." ENDL;
        }
    }

    if (cont_flag == DERNIER_FICHIER_DECOUPE)
    {
//        xmlFree(annee_fichier);
//        xmlFree(mois_fichier);
//        xmlFree(budget_fichier);
//        xmlFree(employeur_fichier);
//        if (siret_etablissement)
//            xmlFree(siret_fichier);
    }

#if defined(STRINGSTREAM_PARSING) || defined(MMAP_PARSING)
    info.threads->in_memory_file.at(info.fichier_courant).clear();
#endif

    xmlFreeDoc(doc);

    if (log.is_open())
        log.close();

    return 0;
}

static int parseFile(info_t& info)
{
   /* REFERENCE */
   /*
        <DocumentPaye xmlns="http://www.minefi.gouv.fr/cp/helios/pes_v2/paye_1_1">
          <IdVer V="">{1,1}</IdVer>
          <Annee V="">{1,1}</Annee>
          <Mois V="">{1,1}</Mois>
          <Train V="">{0,1}</Train>
          <Budget>{0,1}</Budget>
          <Employeur>{1,1}</Employeur>
          <DonneesIndiv>{1,unbounded}</DonneesIndiv>
          <Nomenclatures>{1,1}</Nomenclatures>
          <RepartitionParNature>{1,1}</RepartitionParNature>
        </DocumentPaye>
    */

    /* Note sur les modes d'ex�cution :
     * Le mode == strict == envoie une exception d�s la premi�re non-conformit�,
     * sauf exceptions contraires signal�es en commentaire.
     * Le mode == tol�rant == arr�te l'ex�cution du fichier et passe suivant.
     * Cela peut poser des probl�mes de (r�)allocation/r�servation/lib�ration de m�moire
     * Le mode == souple == continue l'ex�cution le plus possible dans le m�me fichier.
     * Le mode strict fait l'objet d'une compilation s�par�e. Les modes tol�rant ou souple
     * choisis au cas par cas en fonction d'une �valuation plus ou moins subjective de la gravit�
     * de la non-conformit�. */


    #if defined(STRINGSTREAM_PARSING)
      doc = xmlParseDoc(reinterpret_cast<const xmlChar*>(info.threads->in_memory_file.at(info.fichier_courant).c_str()));
    #elif defined (MMAP_PARSING)
       doc = xmlParseDoc(reinterpret_cast<const xmlChar*>(info.threads->in_memory_file.at(info.fichier_courant).c_str()));
    #else

    int res = 0;
    int NDecoupe = info.threads->argv_cut.at(info.fichier_courant).size();
    if (NDecoupe == 0)
        return parseFile(xmlParseFile(info.threads->argv.at(info.fichier_courant).c_str()), info);


    int rang  = 0;
    int cont_flag = PREMIER_FICHIER;

    for (auto && s :  info.threads->argv_cut.at(info.fichier_courant))
    {
         cerr << ENDL;
         cerr << ".....     .....     ....." ENDL;
         cerr << ENDL;

         if (verbeux)
             cerr << PROCESSING_HTML_TAG "Analyse du fichier scind� fil " << info.threads->thread_num << " - n�" <<info.fichier_courant<<"-"<< ++rang << "/" << NDecoupe << " : " << s << ENDL;

         if (rang == 1)
             cont_flag = PREMIER_FICHIER;
         else
         if (rang == NDecoupe)
             cont_flag = DERNIER_FICHIER_DECOUPE;
         else
             cont_flag = FICHIER_SUIVANT_DECOUPE;


         xmlDocPtr doc = xmlParseFile(s.c_str());
         cerr << ENDL;
         if (doc == nullptr)
         {
            cerr << ERROR_HTML_TAG "L'analyse du parseur XML n'a pas pu �tre r�alis�e." ENDL;
         }
         else
            res = parseFile(doc, info, cont_flag);

         if (verbeux)
            cerr << PROCESSING_HTML_TAG "Fin de l'analyse du fichier scind� fil " << info.threads->thread_num << " n�" <<info.fichier_courant<<"-"<< rang << "/" << NDecoupe << " : " << s << ENDL;

         if (res == 0 && ! info.preserve_tempfiles)
         {
            remove(s.c_str());
            if (verbeux)
               cerr << PROCESSING_HTML_TAG "Effacement du fichier scind� fil " << info.threads->thread_num << " n�" <<info.fichier_courant<<"-"<< rang  << "/" << NDecoupe << " : " << s << ENDL;
         }

         if (res == SKIP_FILE || res == RETRY) return res;

     }

 return 0;

#endif
}


/* Les expressions r�guli�res correctes ne sont disponibles sur MINGW GCC qu'� partir du build 4.9.2 */

#if !defined GCC_REGEX && !defined NO_REGEX && (defined __WIN32__ || defined GCC_4_8)
#include <regex.h>

bool regex_match(const char *string, const char *pattern)
{
    int status;
    regex_t re;
    if (string == nullptr) return false;
    if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB|REG_ICASE|REG_NEWLINE) != 0)
    {
        return(false); /* Report error. */
    }
    status = regexec(&re, string, (size_t) 0, nullptr, 0);
    regfree(&re);
    if (status != 0)
    {
        return(false); /* Report error. */
    }
    return(true);
}

const char* pat = EXPRESSION_REG_ELUS;
const char* pat2 = EXPRESSION_REG_VACATIONS;

#else
#ifdef __cplusplus
#include <regex>
#include <string>
#include <iostream>
#include <ctype.h>
#include <mutex>

using namespace std;
#else
#error "C++14 doit �tre utilis�."
#endif


#endif // defined


static inline  int GCC_INLINE memoire_p_ligne(const info_t& info, const unsigned agent)
{
    /* Attention on peut "rembobiner" les types dans la limite de TYPE_LOOP_LIMIT, ce qui implique que l'on doit allouer ex ante TYPE_LOOP_LIMIT fois nbType
     *  pour les drapeux de s�paration des cat�gories */

    return BESOIN_MEMOIRE_ENTETE  // chaque agent a au moins BESOIN_MEMOIRE_ENTETE champs du bulletins de paye en colonnes
                                                        // sans la table ces champs sont r�p�t�s � chaque ligne de paye.
            + nbType  *  TYPE_LOOP_LIMIT                   // espace pour les drapeaux de s�paration des champs (taille de type_remuneration).
            + (info.NLigne[agent]) * (INDEX_MAX_COLONNNES + 1);   // nombre de lignes de paye x nombre maximum de types de balises distincts de lignes de paye
                                                                                                     // soit INDEX_MAX_COLONNES = N pour les �critures du type Var(l+i), i=0,...,N dans ECRIRE_LIGNE_l_COMMUN
}


static inline void GCC_INLINE allouer_memoire_table(info_t& info)
{
    // utiliser NCumAgent ici et pas NCumAgentXml, puisqu'il s'agit d'une pr�allocation

    info.Memoire_p_ligne.resize(info.NCumAgent);

    for (unsigned agent = 0; agent < info.NCumAgent; ++agent)
    {
        info.Memoire_p_ligne[agent] = memoire_p_ligne(info, agent);
    }


    if (info.Table.size() > 0 && info.Table.size() == info.NCumAgent)
    {
        for (unsigned agent = 0; agent < info.NCumAgent; ++agent)
        {
            for (int i = 0; i < info.Memoire_p_ligne[agent] ; ++i)
            {
                xmlFree(info.Table[agent][i]);
            }

        }
    }

    info.NAgent.resize(info.threads->argc);
    info.Table.resize(info.NCumAgent);
    info.NCumAgentXml = 0;

    if (info.NAgent.empty())
    {
        cerr << ERROR_HTML_TAG "M�moire insuffisante pour la table du nombre d'agents" ENDL;
        exit(-19);
    }


    if (info.Table.empty())
    {
        cerr << ERROR_HTML_TAG "M�moire insuffisante pour la table de lignes de paye" ENDL;
        exit(-18);
    }

    for (unsigned agent = 0; agent < info.NCumAgent; ++agent)
    {

        info.Table[agent].resize(info.Memoire_p_ligne[agent]); // ne pas oublier d'initialiser � nullptr !

        if (verbeux && info.Table[agent].empty())
        {
            cerr <<  ERROR_HTML_TAG "Erreur d'allocation de drapeau I. pour l'agent "
                      <<  agent
                      <<  "et pour "
                      <<  info.Memoire_p_ligne[agent]
                      <<  " B" ENDL;
            exit(-63);
        }
    }
}


void* decoder_fichier(info_t& info)
{
    /* environ 6000 bulletins par seconde en processus sumple, et 15000 en multithread ; rajoute 1/3 du temps */

if (info.pretend) return nullptr;

#if  defined GCC_REGEX && !defined NO_REGEX

    regex pat {EXPRESSION_REG_ELUS,  regex_constants::icase};
    regex pat2 {EXPRESSION_REG_VACATIONS, regex_constants::icase};
    regex pat3 {EXPRESSION_REG_ASSISTANTES_MATERNELLES, regex_constants::icase};
    regex pat_adjoints {EXPRESSION_REG_ADJOINTS, regex_constants::icase};
    regex pat_agents {EXPRESSION_REG_AGENTS, regex_constants::icase};
    regex pat_cat_a {EXPRESSION_REG_CAT_A, regex_constants::icase};
    regex pat_cat_b {EXPRESSION_REG_CAT_B, regex_constants::icase};

#endif

    xmlKeepBlanksDefault(0);

    if (info.reduire_consommation_memoire)
    {
        int err = calculer_memoire_requise(info);
        if (err)
        {
            std ::cerr << ERROR_HTML_TAG "Calcul de la m�moire requise" ENDL;
            exit(-1001);
        }
    }
    else
    {
        info.NCumAgent = info.nbAgentUtilisateur * info.threads->argc;
        info.NLigne.resize(info.NCumAgent);

        memory_debug("decoder_fichier : info.NLigne.resize(info.NCumAgent)");

        if (! info.NLigne.empty())
        {
            for (unsigned i = 0 ; i < info.NCumAgent; ++i)
                info.NLigne[i] = info.nbLigneUtilisateur;
        }
        else
        {
            perror(ERROR_HTML_TAG "Probl�me d'allocation m�moire de info.NLigne");
            exit(1003);
        }
    }

    for (unsigned i = 0; i < info.threads->argc ; ++i)
    {
        if (i == 0)
        {
            /* premi�re allocation ou r�allocation � la suite d'un incident */

            allouer_memoire_table(info);
            memory_debug("decoder_fichier : allouer_memoire_table(info)");
        }

    memory_debug("decoder_fichier : pre_parseFile(info)");

    if (info.verifmem) return nullptr;

    info.fichier_courant = i;

    if (info.decoupage_fichiers_volumineux) redecouper(info);

        switch(parseFile(info))
        {
        case RETRY:
            i = 0;
            /* on r�alloue tout depuis le d�but � la site d'un incident */
            cerr << ERROR_HTML_TAG " Il est n�cessaire de r�allouer la m�moire � la suite d'un incident d� aux donn�es..." ENDL;
            continue;

        case SKIP_FILE:
            cerr << ERROR_HTML_TAG " Le fichier  " << info.threads->argv[info.fichier_courant] << " n'a pas pu �tre trait�" ENDL
                      << "   Fichier suivant..." ENDL;
            continue;

        default :
#ifndef NO_DEBUG
            cerr << PROCESSING_HTML_TAG "Fichier " << info.threads->argv[info.fichier_courant]  << "  trait�" ENDL;
#endif
            break;
        }

        memory_debug("decoder_fichier : post_parseFile(info)");
    }

    if (info.reduire_consommation_memoire && info.NCumAgentXml != info.NCumAgent)
        {
          if (verbeux)
          {
            cerr << ERROR_HTML_TAG "Incoh�rence de l'allocation m�moire ex-ante " << info.NCumAgent
                      << " unit�s et ex-post " <<  info.NCumAgentXml << " unit�s d'information." ENDL
                      << "Sortie pour �viter une erreur de segmentation." ENDL;
            cerr << " Fichier : " << info.threads->argv[info.fichier_courant] << ENDL;
           }
            else
                 cerr << ERROR_HTML_TAG "Erreur critique lors de l'analyse du fichier : " << info.threads->argv[info.fichier_courant] << ENDL;
            exit(1005);
        }

    // attention, pas info<-NCumAgent ici

    /* Le champ statut est modifi� comme suit :
        ELU   pour un �lu
        et le champ grade :
        V     pour un vacataire
        A     pour une assistante maternelle */

#if !defined NO_REGEX
#define VAR(X) info.Table[agent][X]
    for (unsigned agent = 0; agent < info.NCumAgentXml; ++agent)
    {
        /* Les �lus peuvent �tre identifi�s soit dans le service soit dans l'emploi m�tier */


        if (regex_match((const char*) VAR(EmploiMetier), pat) || regex_match((const char*) VAR(Service), pat))
        {
            xmlFree(VAR(Statut)) ;
            VAR(Statut) = (xmlChar*) xmlStrdup((const xmlChar*)"ELU");
            VAR(Categorie) = (xmlChar*) NA_STRING;
        }
        else
        {
            /* vacataires */

            if (regex_match((const char*) VAR(EmploiMetier), pat2))
            {
                xmlFree(VAR(Grade));
                VAR(Grade) = (xmlChar*) xmlStrdup((const xmlChar*)"V");
            }

            /* assistantes maternelles */

            else if (regex_match((const char*) VAR(EmploiMetier), pat3))
            {
                xmlFree(VAR(Grade));
                VAR(Grade) = (xmlChar*) xmlStrdup((const xmlChar*)"A");
            }

            /* identification des cat�gories A, B, C */

                /* gestion de m�moire : ne pas allouer avec xmlStrdup et ne pas lib�rer
                 * � la fin de main.cpp dans la double boucle de lib�ration de m�moire car
                 * A, B, C, NA ne sont pas allou�s sur le tas. */

            if (regex_match((const char*) VAR(Grade), pat_adjoints)
                || regex_match((const char*) VAR(Grade), pat_agents))
            {
                VAR(Categorie) = (xmlChar*)"C";
            }
            else if (regex_match((const char*) VAR(Grade), pat_cat_a))
            {
                VAR(Categorie) = (xmlChar*)"A";
            }

            /* Il faut teste d'abord cat A et seulement ensuite cat B */

            else if (regex_match((const char*) VAR(Grade), pat_cat_b))
            {
                VAR(Categorie) = (xmlChar*)"B";
            }
            else
            {
                 VAR(Categorie) = (xmlChar*) NA_STRING;
            }
        }

        /* les vacations peuvent �tre indiqu�es comme telles dans les libell�s de paie mais pas dans les emplois m�tiers.
           On les r�cup�re en parcourant les libell�s */

        if (info.reduire_consommation_memoire)
        {
            /* inutile de boucler sur la partie vide du tableau... */

            for (int j = BESOIN_MEMOIRE_ENTETE; j < info.NLigne[agent]; ++j)
                if (regex_match((const char*) VAR(j), pat2))
                {
                    xmlFree(VAR(Grade));
                    VAR(Grade) = (xmlChar*) xmlStrdup((const xmlChar*)"V");
                }
        }
        else
        {
            for (int j = BESOIN_MEMOIRE_ENTETE ; j < info.NLigne[agent] && info.Table[agent][j] != nullptr; ++j)
                if (regex_match((const char*) VAR(j), pat2))
                {
                    xmlFree(VAR(Grade));
                    VAR(Grade) = (xmlChar*) xmlStrdup((const xmlChar*)"V");
                }
        }
    }


#undef VAR
#endif
    return nullptr;
}

