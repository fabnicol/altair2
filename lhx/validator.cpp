// Copyright Cour des comptes, 2017
// Contributeur :
// Fabrice Nicol, années 2012 à 2017
// fabrice.nicol@crtc.ccomptes.fr
//
// Ce logiciel est un programme informatique servant à extraire et analyser les fichiers de paye
// produits au format spécifié par l'annexe de la convention-cadre nationale de dématérialisation
// en vigueur à compter de l'année 2008.
//
// Ce logiciel est régi par la licence CeCILL soumise au droit français et
// respectant les principes de diffusion des logiciels libres. Vous pouvez
// utiliser, modifier et/ou redistribuer ce programme sous les conditions
// de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
// sur le site "http://www.cecill.info".
//
// En contrepartie de l'accessibilité au code source et des droits de copie,
// de modification et de redistribution accordés par cette licence, il n'est
// offert aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons,
// seule une responsabilité restreinte pèse sur l'auteur du programme, le
// titulaire des droits patrimoniaux et les concédants successifs.
//
// A cet égard l'attention de l'utilisateur est attirée sur les risques
// associés au chargement, à l'utilisation, à la modification et/ou au
// développement et à la reproduction du logiciel par l'utilisateur étant
// donné sa spécificité de logiciel libre, qui peut le rendre complexe à
// manipuler et qui le réserve donc à des développeurs et des professionnels
// avertis possédant des connaissances informatiques approfondies. Les
// utilisateurs sont donc invités à charger et tester l'adéquation du
// logiciel à leurs besoins dans des conditions permettant d'assurer la
// sécurité de leurs systèmes et ou de leurs données et, plus généralement,
// à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.
//
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
// pris connaissance de la licence CeCILL, et que vous en avez accepté les
// termes.
//
//


/// \file    validator.cpp
/// \author  Fabrice Nicol
/// \brief   Ce fichier contient le code relatif au décoidage de la partie "bulletins de paye" (variables communes à toutes les lignes de paye)
/// ainsi qu'à la gestion des allocations de mémoire.
/// Il contient aussi une fonction auxiliaire relative à la normalisation de certains caractères accentués, afin d'accélerer le traitement par libxml2


#include "validator.h"
#include "fonctions_auxiliaires.h"
#include "table.h"
#include "ligne_paye.h"

#ifdef INCLURE_REG_ELUS
#  include "expression_reg_elus.h"
#endif

using namespace std;

extern mutex mut;
extern uint64_t   parseLignesPaye (xmlNodePtr cur, info_t& info);
extern vector<errorLine_t>  errorLineStack;
extern int rang_global;


/// Définit le besoin de mémoire en octets pour un bulletin de paye de données XML (Table)
/// \param info  Structure info_t contenant les données XML
/// \param agent rang de l'agent considéré dans info.NLigne (qui donne le nombre de lignes de paye pour l'agent)
/// \return Besoin de mémoire par bulletin de paye
/// \details Dépend de trois constantes de compilation : \b #BESOIN_MEMOIRE_ENTETE, \b #TYPE_LOOP_LIMIT et \b #INDEX_MAX_COLONNNES

static inline  int GCC_INLINE memoire_p_ligne (const info_t& info, const unsigned agent)
{
    // Attention on peut "rembobiner" les types dans la limite de TYPE_LOOP_LIMIT, ce qui implique que l'on doit allouer ex ante TYPE_LOOP_LIMIT fois nbType
    //  pour les drapeux de séparation des catégories

    return BESOIN_MEMOIRE_ENTETE  // chaque agent a au moins BESOIN_MEMOIRE_ENTETE champs du bulletins de paye en colonnes
           // sans la table ces champs sont répétés à chaque ligne de paye.
           + nbType  *  TYPE_LOOP_LIMIT                   // espace pour les drapeaux de séparation des champs (taille de type_remuneration).
           + (info.NLigne[agent]) * (INDEX_MAX_COLONNNES + 1);   // nombre de lignes de paye x nombre maximum de types de balises distincts de lignes de paye
    // soit INDEX_MAX_COLONNES = N pour les écritures du type Var(l+i), i=0,...,N dans ECRIRE_LIGNE_l_COMMUN
}

/// Décode une structure info_t contenant les données de paye à l'état brut "fichier" (<pre> info.threads->in_memory_file </pre> ou <pre> info.threads->argv </pre>)
/// Commence par décoder Année, Mois, Budget, Siret, Employeur, puis lance le décodage des données de paye individuelles, agent par agent, pour l'ensemble du fichier
/// A chaque bulletin de paye de l'agent, lance  parseLignesPaye
/// \param info Structure info_t contenant les fichiers de paye en mémoire vive et les champs à remplir par le décodage libxml2.
/// \return #SKIP_FILE si le fichier doit être sauté pour une raison ou pour une autre \n
/// ou bien #RETRY si le décodage est essayé à nouveau \n
/// sinon 0 en cas de succès.

static int parseFile (info_t& info)
{
// REFERENCE
//
//        <DocumentPaye xmlns="http://www.minefi.gouv.fr/cp/helios/pes_v2/paye_1_1">
//          <IdVer V="">{1,1}</IdVer>
//          <Année V="">{1,1}</Année>
//          <Mois V="">{1,1}</Mois>
//          <Train V="">{0,1}</Train>
//          <Budget>{0,1}</Budget>
//          <Employeur>{1,1}</Employeur>
//          <DonneesIndiv>{1,unbounded}</DonneesIndiv>
//          <Nomenclatures>{1,1}</Nomenclatures>
//          <RepartitionParNature>{1,1}</RepartitionParNature>
//        </DocumentPaye>


//    Note sur les modes d'exécution :
//    Le mode == strict == envoie une exception dès la première non-conformité,
//    sauf exceptions contraires signalées en commentaire.
//    Le mode == tolérant == arrête l'exécution du fichier et passe suivant.
//    Cela peut poser des problèmes de (ré)allocation/réservation/libération de mémoire
//    Le mode == souple == continue l'exécution le plus possible dans le même fichier.
//    Le mode strict fait l'objet d'une compilation séparée. Les modes tolérant ou souple
//    choisis au cas par cas en fonction d'une évaluation plus ou moins subjective de la gravité
//    de la non-conformité. */

    ofstream log;
    xmlDocPtr doc;
    xmlNodePtr cur = nullptr;
    info.NAgent[info.fichier_courant] = 0;
    xmlNodePtr cur_save = cur;
    xmlChar *annee_fichier = nullptr,
             *mois_fichier = nullptr,
              *employeur_fichier = nullptr,
               *etablissement_fichier = nullptr,
                *siret_fichier = nullptr,
                 *budget_fichier = nullptr;

#if defined(STRINGSTREAM_PARSING)
    doc = xmlReadDoc (reinterpret_cast<const xmlChar*> (info.threads->in_memory_file.at (info.fichier_courant).c_str()), nullptr, nullptr, XML_PARSE_COMPACT | XML_PARSE_BIG_LINES);
//#elif defined (MMAP_PARSING)
//    doc = xmlParseDoc (reinterpret_cast<const xmlChar*> (info.threads->in_memory_file.at (info.fichier_courant).c_str()));
#else
    doc = xmlReadFile (info.threads->argv.at (info.fichier_courant).c_str(), nullptr, XML_PARSE_BIG_LINES);
#endif

    memory_debug ("parseFile : xmlParseFile");

    if (doc == nullptr)
        {
            cerr << ERROR_HTML_TAG " problème d'allocation mémoire pour le scan XML." ENDL;
            return SKIP_FILE;
        }

    if (! info.chemin_log.empty())
        {
            log.open (info.chemin_log, ios::app);
        }

    cur = xmlDocGetRootElement (doc);

    if (cur == nullptr)
        {
            cerr << ERROR_HTML_TAG "document vide" ENDL;
            xmlFreeDoc (doc);

            if (verbeux)
                cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode tolérant)." ENDL;

            if (log.is_open())
                log.close();

            return SKIP_FILE;
        }

    {
        LOCK_GUARD
        cerr << STATE_HTML_TAG "Fichier "
#ifdef GENERATE_RANK_SIGNAL
             << "n°" <<  rang_global + 1
#endif
             << " -- " << info.threads->argv[info.fichier_courant] << ENDL;

        if (verbeux)
            {
                cerr << STATE_HTML_TAG << "Fil n°" << info.threads->thread_num + 1 << " -- "
                     << "Fichier n°" << info.fichier_courant + 1 << " dans ce fil." ENDL;
            }
    }

    cur =  cur->xmlChildrenNode;

    cur = atteindreNoeud ("Annee", cur);

    if (cur != nullptr)
        {
            annee_fichier = xmlGetProp (cur, (const xmlChar *) "V");
            int annee;
            annee = (annee_fichier[0] == '\0') ? 0 : atoi ((const char*) annee_fichier);

            // Altaïr est écrit pour durer 100 ans :)

            if (annee <= 2000 || annee >= 2100)
                {
                    cerr << ERROR_HTML_TAG " pas de données sur l'année." ENDL;

#ifdef STRICT

                    if (log.is_open()) log.close();

                    exit (-517);
#endif
                    // Il faudra sans doute ajuster les différences entre le parsing C et l'analyse Xml, qui vont diverger

                    if (verbeux) cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode tolérant)." ENDL;

                    return SKIP_FILE;
                }

            cur = (cur) ? cur->next : nullptr;
        }
    else
        {
            cerr << ERROR_HTML_TAG "Année non détectable" ENDL;

            if (log.is_open())
                log.close();

#ifdef STRICT

            if (log.is_open())
                log.close();

            exit (-502);
#else
            // Il faudra sans doute ajuster les différences entre le parsing C et l'analyse Xml, qui vont diverger

            if (verbeux) cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode tolérant)." ENDL;

            return SKIP_FILE;
#endif
        }

    if (cur != nullptr)
        {
            mois_fichier = xmlGetProp (cur, (const xmlChar *) "V");
            int mois = atoi ((const char*) mois_fichier);

            if (mois <= 0 || mois > 12)
                {
                    cerr << ERROR_HTML_TAG " pas de données sur le mois." ENDL;

#ifdef STRICT

                    if (log.is_open()) log.close();

                    exit (-517);
#endif
                    // Il faudra sans doute ajuster les différences entre le parsing C et l'analyse Xml, qui vont diverger

                    if (verbeux) cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode tolérant)." ENDL;

                    return SKIP_FILE;
                }

        }
    else
        {
            cerr << ERROR_HTML_TAG "Mois non détectable" ENDL;

#ifdef STRICT

            if (log.is_open())
                log.close();

            exit (-503);
#else

            // Il faudra sans doute ajuster les différences entre le parsing C et l'analyse Xml, qui vont diverger

            if (verbeux) cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode tolérant)." ENDL;

            return SKIP_FILE;
#endif

        }

    cur_save = cur;
    cur = atteindreNoeud ("Budget", cur);

    if (cur != nullptr)
        {
            cur_save = cur;
            cur =  cur->xmlChildrenNode;
            budget_fichier = xmlGetProp (cur, (const xmlChar *) "V");

            if (budget_fichier == nullptr || budget_fichier[0] == '\0')
                {
                    if (budget_fichier  != nullptr) xmlFree (budget_fichier);
                    budget_fichier = nullptr;

                    if (verbeux)
                        {
                            LOCK_GUARD
                            cerr << STATE_HTML_TAG "Aucune information sur le budget [optionnel]." ENDL;
                            cerr << PROCESSING_HTML_TAG "Poursuite du traitement." ENDL;
                        }

                }

            cur = cur_save->next;
        }
    else
        {
            budget_fichier = nullptr;

            if (verbeux)
                {
                    LOCK_GUARD
                    cerr << STATE_HTML_TAG "Aucune information sur le budget [optionnel]." ENDL;
                    cerr << PROCESSING_HTML_TAG "Poursuite du traitement." ENDL;
                }

            cur = cur_save;
        }

    bool skip_budget = false;

    if (budget_fichier && ! info.exclure_budget.empty())
        {
            if (find (info.exclure_budget.cbegin(), info.exclure_budget.cend(), string ((const char*) budget_fichier))
                    != info.exclure_budget.cend())
                {
                    // on exclut ce fichier
                    if (verbeux) cerr << PROCESSING_HTML_TAG "Budget " << budget_fichier << " exclu." ENDL;

                    skip_budget = true;
                }
        }

    cur_save = cur;

    // REFERENCE

    // <Employeur>
    //      <Nom V="">{1,1}</Nom>
    //      <APE V="">{1,1}</APE>
    //      <Adresse>{1,1}</Adresse>
    //      <NumUrssaf V="">{1,1}</NumUrssaf>
    //      <Siret V="">{1,1}</Siret>
    // </Employeur>
    //

    if (nullptr == (cur = atteindreNoeud ("Employeur", cur)))
        {

            warning_msg ("la balise Employeur [non-conformité à la norme].", info, cur);

#ifdef STRICT

            if (log.is_open())
                log.close();

            exit (-515);
#endif

            if (verbeux) cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode souple)." ENDL;

            employeur_fichier = nullptr;
            siret_fichier = nullptr;
            cur = cur_save;
        }
    else
        {
            cur_save = cur;

            // On recherche le nom, le siret de l'employeur

            do
                {
                    cur =  cur->xmlChildrenNode;

                    if (cur == nullptr || xmlIsBlankNode (cur))
                        {
                            cerr << ERROR_HTML_TAG "Pas de données sur le nom de l'employeur [non-conformité à la norme]." ENDL;

                            if (mut.try_lock())
                                {
                                    errorLineStack.emplace_back (afficher_environnement_xhl (info, cur));
                                    mut.unlock();
                                }

                            employeur_fichier = nullptr;
                            siret_fichier = nullptr;
                            break;
                        }

                    cur = atteindreNoeud ("Nom", cur);

                    if (cur != nullptr)
                        {
                            employeur_fichier = xmlGetProp (cur, (const xmlChar *) "V");
                            cur = (cur) ? cur->next : nullptr;
                        }
                    else
                        {
                            cerr << ERROR_HTML_TAG "Employeur non identifié [non-conformité à la norme]." ENDL;
                            employeur_fichier = nullptr;
                        }

                    if (cur != nullptr) cur = atteindreNoeud ("Siret", cur);

                    if (cur != nullptr)
                        {
                            siret_fichier = xmlGetProp (cur, (const xmlChar *) "V");

                            if (siret_fichier[0] == '\0')
                                {
                                    warning_msg ("la balise Siret", info, cur);
#ifdef STRICT

                                    if (log.is_open()) log.close();

                                    exit (-517);
#endif

                                    if (verbeux)  cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode souple)." ENDL;

                                    xmlFree (siret_fichier);
                                    siret_fichier = nullptr;

                                }
                        }
                    else
                        {
                            cerr  << ERROR_HTML_TAG "Siret de l'empoyeur non identifié [non-conformité à la norme]." ENDL;

                            if (verbeux)  cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode souple)." ENDL;

                            cerr << "Année " << annee_fichier
                                 << " Mois "  << mois_fichier << ENDL;
                            siret_fichier = nullptr;
                        }

                }
            while (false);

            // on remonte d'un niveau

            cur = cur_save;
        }

    bool skip_employeur = false;
    bool siret_etablissement =  false;

    if (employeur_fichier && ! info.exclure_employeur.empty())
        {
            if (find (info.exclure_employeur.cbegin(), info.exclure_employeur.cend(), string ((const char*) employeur_fichier))
                    != info.exclure_employeur.cend())
                {
                    // on exclut ce fichier
                    if (verbeux) cerr << PROCESSING_HTML_TAG "Employeur " << employeur_fichier << " exclu." ENDL;

                    skip_employeur = true;
                }
        }

    cur = atteindreNoeud ("DonneesIndiv", cur);

    if (cur == nullptr || xmlIsBlankNode (cur))
        {
            cerr << ERROR_HTML_TAG "Pas de données individuelles de paye [DonneesIndiv, non conformité à la norme]." ENDL;
            warning_msg ("la balise DonneesIndiv", info, cur);

#ifdef STRICT

            if (log.is_open())
                log.close();

            return SKIP_FILE;
#else
            // Il faudra sans doute ajuster les différences entre le parsing C et l'analyse Xml, qui vont diverger

            if (verbeux) cerr << PROCESSING_HTML_TAG "Reste du fichier omis";

            long lineN = xmlGetLineNo (cur);

            if (lineN != 65535 && lineN != -1)
                {
                    cerr <<  ", ligne  " << lineN << ENDL;
                }
            else
                {
                    if (info.ligne_fin.size() > info.NCumAgentXml)
                        {
                            cerr <<  " un peu après la ligne "  << info.ligne_fin.at (info.NCumAgentXml)[0] + 1 << ENDL;
                        }
                }

            goto out;

#endif
        }

    while (cur != nullptr)
        {
            cur_save = cur;
            xmlNodePtr cur_save2 = nullptr;
            cur =  cur->xmlChildrenNode;  // Niveau Etablissement et PayeIndivMensuel
            cur_save2 = cur;

            if (cur == nullptr || xmlIsBlankNode (cur))
                {
                    cerr << ERROR_HTML_TAG "Pas de données individuelles de paye [non conformité à la norme]." ENDL;

#ifdef STRICT

                    if (log.is_open())
                        log.close();

                    exit (-515);
                    return SKIP_FILE;
#else

                    cur = cur_save->next;

                    if (verbeux) cerr << PROCESSING_HTML_TAG "La paire de balises DonneesIndiv vides est omise, ";

                    long lineN = xmlGetLineNo (cur);

                    if (lineN != 65535 && lineN != -1)
                        {
                            cerr << "ligne  " << lineN << ENDL;
                        }
                    else
                        {
                            if (info.ligne_fin.size() > info.NCumAgentXml)
                                {
                                    cerr << "un peu après la ligne "  << info.ligne_fin.at (info.NCumAgentXml)[0] + 1 << ENDL;
                                }
                        }

                    goto out;

#endif
                }

// Les données sur l'établissement sont optionnelles
//         REFERENCE
//         <DonneesIndiv>
//           <Etablissement>{0,1}</Etablissement>
//           <PayeIndivMensuel>{1,unbounded}</PayeIndivMensuel>
//         </DonneesIndiv>


            cur = atteindreNoeud ("Etablissement", cur);

            if (cur == nullptr)
                {
                    cur = cur_save2;

                    if (verbeux)
                        {
                            LOCK_GUARD
                            cerr << STATE_HTML_TAG "Pas d'information sur l'Etablissement" ENDL;
                        }

                }
            else
                {

// REFERENCE
//             <Etablissement>
//               <Nom V="">{1,1}</Nom>
//               <NumUrssaf V="">{1,1}</NumUrssaf>
//               <Adresse>{1,1}</Adresse>
//               <Siret V="">{1,1}</Siret>
//             </Etablissement>


// Altaïr a ici deux modes : strict, le non-repect de la norme entraine une exception
// ou souple : on continue l'exécution en considérant que bien que le nom/siret soit obligatoire,
// l'ensemble du champ lui-même est en fait optionnel. [la norme est quelque peu inconsistante sur ce point]


                    cur_save2 = cur;

// On recherche le nom, le siret de l'établissement

                    do
                        {
                            cur =  cur->xmlChildrenNode;

                            if (cur == nullptr || xmlIsBlankNode (cur))
                                {
                                    warning_msg ("les données nominales de l'établissement [non-conformité]", info, cur);

#ifdef STRICT

                                    if (log.is_open()) log.close();

                                    exit (-517);
#endif

                                    if (verbeux) cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode souple)." ENDL;

                                    etablissement_fichier = nullptr;

                                    // on garde le siret de l'employeur

                                    break;
                                }

                            cur = atteindreNoeud ("Nom", cur);

                            if (cur != nullptr)
                                {
                                    etablissement_fichier = xmlGetProp (cur, (const xmlChar *) "V");

                                    if (etablissement_fichier[0] == '\0')
                                        {
                                            warning_msg ("les données nominales de l'établissement [non-conformité]", info, cur);
#ifdef STRICT

                                            if (log.is_open()) log.close();

                                            exit (-517);
#endif

                                            if (verbeux) cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode souple)." ENDL;

                                            xmlFree (etablissement_fichier);
                                            etablissement_fichier = nullptr;
                                        }

                                    cur = (cur) ? cur->next : nullptr;

                                }
                            else
                                {
                                    warning_msg ("les données nominales de l'établissement [non-conformité]", info, cur);
#ifdef STRICT

                                    if (log.is_open()) log.close();

                                    exit (-517);
#endif

                                    if (verbeux) cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode souple)." ENDL;

                                    etablissement_fichier = nullptr;

                                    // on garde le siret de l'employeur
                                }

                            if (cur != nullptr)
                                cur = atteindreNoeud ("Siret", cur);

                            siret_etablissement =  true;

                            if (cur != nullptr)
                                {
                                    xmlFree (siret_fichier);
                                    siret_fichier = xmlGetProp (cur, (const xmlChar *) "V");

                                    if (siret_fichier[0] == '\0')
                                        {
                                            warning_msg ("les données de Siret de l'établissement [non-conformité]", info, cur);
#ifdef STRICT

                                            if (log.is_open()) log.close();

                                            exit (-517);
#endif

                                            if (verbeux) cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode souple)." ENDL;

                                            xmlFree (siret_fichier);
                                            siret_fichier = nullptr;
                                        }
                                }
                            else
                                {
#ifdef STRICT

                                    if (log.is_open()) log.close();

                                    exit (-517);
#endif
                                    warning_msg ("les données de Siret de l'établissement [non-conformité]", info, cur);
                                    siret_fichier = nullptr;
                                }

                        }
                    while (false);

                    // on remonte d'un niveau

                    cur = cur_save2;
                }

            bool skip_siret = false;

            if (siret_etablissement && siret_fichier && ! info.exclure_siret.empty())
                {
                    if (find (info.exclure_siret.cbegin(), info.exclure_siret.cend(), string ((const char*) siret_fichier))
                            != info.exclure_siret.cend())
                        {
                            if (verbeux) cerr << PROCESSING_HTML_TAG "SIRET " << siret_fichier << " exclu." ENDL;

                            skip_siret = true;
                        }
                }


// REFERENCE
//         <PayeIndivMensuel>
//              <Agent>{1,1}</Agent>
//              <Evenement>{0,unbounded}</Evenement>
//              <Service V="">{1,1}</Service>
//              <NBI V="">{1,unbounded}</NBI>
//              <QuotiteTrav V="">{1,1}</QuotiteTrav>
//              <Periode>{1,1}</Periode>
//              <Remuneration>{1,1}</Remuneration>
//              <NbHeureTotal V="">{0,1}</NbHeureTotal>
//              <TauxHor V="">{0,1}</TauxHor>
//              <NbHeureSup V="">{1,1}</NbHeureSup>
//              <MtBrut V="">{1,1}</MtBrut>
//              <MtNet V="">{1,1}</MtNet>
//              <MtNetAPayer V="">{1,1}</MtNetAPayer>
//              <DatePaiement V="">{1,1}</DatePaiement>
//              <MtImposable V="">{1,1}</MtImposable>
//              <CumulMtImposable V="">{1,1}</CumulMtImposable>
//              <CumulMtBrut V="">{1,1}</CumulMtBrut>
//              <CumulBaseSS V="">{1,1}</CumulBaseSS>
//              <RepartitionBudget>{0,unbounded}</RepartitionBudget>
//              <PJRef>{0,unbounded}</PJRef>
//           </PayeIndivMensuel>

            uint32_t step = 0;

            while (cur != nullptr)
                {

                    cur_save2 =  cur;
                    cur = atteindreNoeud ("PayeIndivMensuel", cur);


                    if (cur == nullptr || cur->xmlChildrenNode == nullptr || xmlIsBlankNode (cur->xmlChildrenNode))
                        {
                            LOCK_GUARD
                            cerr << ERROR_HTML_TAG "Pas d'information sur les lignes de paye [non-conformité à la norme : absence de balise PayeIndivMensuel après DonneesIndiv]." ENDL;

                            if (verbeux)
                                {
                                    if (info.NCumAgentXml) cerr << ERROR_HTML_TAG "La balise PayeIndivMensuel n'existe pas en dessous de la balise "
                                                                    << (char*) cur_save2->name
                                                                    << " vers la ligne n°" << info.ligne_fin.at (info.NCumAgentXml - 1)[0] + 2 << ENDL;
                                }

//                if (cur == nullptr)
//                        warning_msg("la balise PayeIndivMensuel de l'établissement [non-conformité]", info, cur);
#ifdef STRICT

                            if (log.is_open()) log.close();

                            exit (-518);
#endif

                            if (verbeux) cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode souple)." ENDL;

                            // Ici on ne risque pas d'avoir une divergence entre le parsage C et Xml

                            cur = cur_save2->next;

                            continue;
                        }

                    // ici on sait que cur->xmlChildrenNode est non vide

                    cur_save2 = cur;
                    cur = cur->xmlChildrenNode;  // Niveau Agent

                    if (skip_siret || skip_employeur || skip_budget)
                        {
                            info.Table[info.NCumAgentXml][Annee] = xmlStrdup ((const xmlChar*)"*");

                            for (int i = Annee + 1 ; i < memoire_p_ligne (info, info.NCumAgentXml) ; ++i)
                                info.Table[info.NCumAgentXml][i] = xmlStrdup ((const xmlChar*)""); // pas nullptr
                        }
                    else
                        {
                            // remarque atoi retourne zéro s'il rencontre "" ou des caractères non numériques

                            info.Table[info.NCumAgentXml][Annee] = xmlStrdup (annee_fichier);
                            info.Table[info.NCumAgentXml][Mois]  = xmlStrdup (mois_fichier);
                            info.Table[info.NCumAgentXml][Budget] = xmlStrdup (budget_fichier);
                            info.Table[info.NCumAgentXml][Employeur]  = xmlStrdup (employeur_fichier);

                            // Nota : le Siret est, si l'établissement existe, celui de l'établissement, sinon celui de l'Employeur

                            info.Table[info.NCumAgentXml][Siret]  = xmlStrdup (siret_fichier);
                            info.Table[info.NCumAgentXml][Etablissement]  = xmlStrdup (etablissement_fichier);

                            // LECTURE DES LIGNES DE PAYE STRICTO SENSU

                            int32_t ligne_p = parseLignesPaye (cur, info);

                            info.drapeau_cont = true;

                            int diff = info.NLigne[info.NCumAgentXml] - ligne_p;

                            if (info.reduire_consommation_memoire)
                                {
                                    if (diff < 0)
                                        {
                                            if (verbeux)
                                                {

#ifdef USE_STRING_EXEC
                                                    char cmd[999] = {0};
                                                    snprintf (cmd, 999, "grep -n 'Matricule V=\"%s\"' %s | cut -f 1 -d:", info.Table[info.NCumAgentXml][Matricule], env.filePath.c_str());
                                                    string lineN = string_exec (cmd);
#else
                                                    string lineN;

                                                    if (info.ligne_debut.size() > info.NCumAgentXml
                                                            && info.ligne_fin.size() > info.NCumAgentXml )
                                                        {
                                                            lineN = to_string (info.ligne_debut.at (info.NCumAgentXml)[0] + 1)
                                                                    + " - "
                                                                    + to_string (info.ligne_fin.at (info.NCumAgentXml)[0]);
                                                        }

#endif
                                                    LOCK_GUARD
                                                    cerr << ERROR_HTML_TAG "L'allocation de mémoire initiale a prévu : "
                                                         << info.NLigne[info.NCumAgentXml]
                                                         << " ligne(s) de paye mais le décompte précis donne : "
                                                         << ligne_p
                                                         << ENDL "Pour l'agent "
                                                         << "de matricule"
                                                         << info.Table[info.NCumAgentXml][Matricule] << ENDL
                                                         << " Année "
                                                         << info.Table[info.NCumAgentXml][Annee]
                                                         << " Mois "
                                                         << info.Table[info.NCumAgentXml][Mois] << ENDL
                                                         << "Ligne(s) "
                                                         << lineN
                                                         << ENDL;
                                                }

                                            ecrire_log (info, log, diff);

                                            if (log.is_open())
                                                log.close();

#ifdef STRICT
                                            exit (-1278);
#else

                                            // il faut tout réallouer

                                            return RETRY;
#endif
                                        }
                                }

                            info.NLigne[info.NCumAgentXml] = ligne_p;

                            info.nbLigne += ligne_p;

                            ecrire_log (info, log, diff);
                        }

                    // Ici il est normal que cur = nullptr

                    cur = cur_save2->next;

                    AFFICHER_NOEUD (cur->name)

                    ++info.NAgent[info.fichier_courant];
                    ++info.NCumAgentXml;
#ifdef GENERATE_RANK_SIGNAL
                    ++step;

                    if (step > info.NCumAgent / 5)
                        {
                            generate_rank_signal (ceil ((float) (info.NCumAgentXml * 100) / (float) info.NCumAgent));
                            cerr << " \n";
                            step = 0;
                        }

#endif
                }

            xmlFree (etablissement_fichier);

            // si pas d'établissement (NA_STRING) alors on utilise le siret de l'empoyeur, donc
            // ne pas libérer dans ce cas !

            cur = cur_save->next;  // next DonneesIndiv

            if (cur == nullptr || xmlStrcmp (cur->name, (const xmlChar*) "DonneesIndiv")) break;  // on ne va pas envoyer un message d'absence de DonneesIndiv si on a fini la boucle...
        }


out :

#ifdef GENERATE_RANK_SIGNAL
    generate_rank_signal();
#endif


    if (verbeux)
        {
            LOCK_GUARD
            cerr << STATE_HTML_TAG << "Total : " <<  info.NCumAgentXml << " bulletins -- " << info.nbLigne << " lignes cumulées." ENDL ENDL;
        }


    if (siret_etablissement )
        xmlFree (siret_fichier);

    xmlFree (annee_fichier);
    xmlFree (mois_fichier);
    xmlFree (budget_fichier);
    xmlFree (employeur_fichier);

    if (! info.generer_bulletins)
        info.threads->in_memory_file.at (info.fichier_courant).clear();

    xmlFreeDoc (doc);

    if (log.is_open())
        log.close();

    return 0;
}

// Les expressions régulières correctes ne sont disponibles sur MINGW GCC qu'à partir du build 4.9.2

#ifdef __cplusplus
#include <regex>
#include <string>
#include <iostream>
#include <ctype.h>
#include <mutex>

using namespace std;
#else
#error "C++14 ou ultérieur doit être utilisé."
#endif

/// Alloue la mémoire de la table des données
/// \param info Structure info_t contenant les champs à réserver en mémoire

static inline void GCC_INLINE allouer_memoire_table (info_t& info)
{
    // utiliser NCumAgent ici et pas NCumAgentXml, puisqu'il s'agit d'une préallocation

    // Libération de la mémoire si elle est déjà allouée

    if (info.Table.size() > 0 && info.Table.size() == info.NCumAgent)
        {
            for (unsigned agent = 0; agent < info.NCumAgent; ++agent)
                {
                    for (xmlChar* a : info.Table[agent])
                        {
                          if (a)
                             xmlFree (a);
                        }
                }
        }

    // Utilisation des dimensions issues de calculer_memoire_requise (info.threads->argc et info.NCumAgent)

    info.NAgent.resize (info.threads->argc);
    info.Table.resize (info.NCumAgent);
    info.NCumAgentXml = 0;

    if (info.NAgent.empty())
        {
            cerr << ERROR_HTML_TAG "Mémoire insuffisante pour la table du nombre d'agents" ENDL;
            exit (-19);
        }

    if (info.Table.empty())
        {
            cerr << ERROR_HTML_TAG "Mémoire insuffisante pour la table de lignes de paye" ENDL;
            exit (-18);
        }

    // Pour chaque agent utiliser memoire_p_ligne(info, agent) pour retailler info.Table

    for (unsigned agent = 0; agent < info.NCumAgent; ++agent)
        {
            info.Table[agent].resize (memoire_p_ligne (info, agent));

            if (verbeux && info.Table[agent].empty())
                {
                    cerr <<  ERROR_HTML_TAG "Erreur d'allocation de drapeau I. pour l'agent "
                         <<  agent
                         <<  "et pour "
                         <<  memoire_p_ligne (info, agent)
                         <<  " B" ENDL;
                    exit (-63);
                }
        }
}

/// Fonction permettant de convertir très efficacement les caractères accentués UTF-8 ou Latin-1 en caractères non accentués
/// et d'effacer les octets superflus après remplacement par des caractères non accentués.
/// \param c Caractère au format xmlChar*

inline void GCC_INLINE normaliser_accents (xmlChar* c)
{
    // la représentation interne est UTF-8 donc les caractères accentués sont sur 2 octets : é = 0xc3a8 etc.

    while (c != nullptr && *c != 0)  // C++11 : on peut avoir c != nullptr et *c == 0.
        {
#ifdef CONVERTIR_LATIN_1

            switch (* (c))
                {
                case 0xe8 : // è
                case 0xe9 : // é
                case 0xea : // ê
                case 0xc8 : // E accent aigu
                case 0xc9 : // E accent grave
                case 0xca : // E accent circ
                    *c = 0x65;  // e (ignore casse)

                    // on peut très rarement avoir besoin de plus d'une seule conversion dans les faits pour l'expression régulière

                    break;

                case 0xee : // î
                case 0xce : // I accent circ
                    *c = 0x69; //i

                    break;

                case 0xd4 :  // ô
                case 0xf4 :  // O accent circ
                    *c = 0x6f;

                    break;

                }

#else   //UTF-8

            if (*c == 0xc3)
                {
                    if (c + 1 == nullptr) break;

                    switch (* (c + 1))
                        {
                        case 0xa8 : // è
                        case 0xa9 : // é
                        case 0xaa : // ê
                        case 0x88 : // E accent aigu
                        case 0x89 : // E accent grave
                        case 0x8a : // E accent circ
                            *c = 0x65;  // e (ignore casse)

                            effacer_char (c + 1);

                            // on peut très rarement avoir besoin de plus d'une seule conversion dans les faits pour l'expression régulière

                            break;

                        case 0xae : // î
                        case 0x8e : // I accent circ
                            *c = 0x69; //i

                            effacer_char (c + 1);

                            break;

                        case 0xb4 :  // ô
                        case 0x94 :  // O accent circ
                            *c = 0x6f;

                            effacer_char (c + 1);

                            break;

                        }
                }

#endif
            ++c;
        }

}

/// Lance  calculer_memoire_requise ou le calcul direct de la mémoire par par les saisies de -n et -N \n
/// ou alors retourne si <pre>--pretend</pre> est en ligne de commande.
/// \param info Structure info_t contenant les informations de ligne de commande
/// \return Pointeur nul. Exception en cas d'erreur.

void* decoder_fichier (info_t& info)
{
    if (info.pretend) return nullptr;

    if (info.reduire_consommation_memoire)
        {
            int err = calculer_memoire_requise (info);

            if (err)
                {
                    cerr << ERROR_HTML_TAG "Calcul de la mémoire requise" ENDL;
                    cerr << ERROR_HTML_TAG "Erreur : " << strerror (errno);
                    throw;
                }
        }
    else
        {
            info.NCumAgent = info.nbAgentUtilisateur * info.threads->argc;
            info.NLigne.resize (info.NCumAgent);

            memory_debug ("decoder_fichier_info.NLigne.resize(info.NCumAgent)");

            if (! info.NLigne.empty())
                {
                    for (unsigned i = 0 ; i < info.NCumAgent; ++i)
                        info.NLigne[i] = info.nbLigneUtilisateur;
                }
            else
                {
                    perror (ERROR_HTML_TAG "Problème d'allocation mémoire de info.NLigne");
                    throw;
                }
        }

    return nullptr;
}


/// Lance l'ensemble des processus de pré-traitement et de post-traitement des fichiers de paye. \n
/// Pour le pré-traitement : \n
/// <ul> <li> Allocation de la mémoire des tables des structures info_t </li>
/// <li>En cas d'incident de code #RETRY dans  parseFile, tente une réallocation </li>
/// <li>En cas d'incident de code #SKIP_FILE dans  parseFile, saute le fichier et passe au suivant </li></ul>
/// Pour le post-traitement : \n
/// <ul><li> Vérification de la cohérence de l'allocation mémoire ex-ante et de la mémoire consommée au décodage par libxml2. Exception en cas d'incohérence.</li>
/// <li>Tentative de codage dans le Grade des assistantes maternelles (A) et des vacataires (V) reconnus par expression régulière</li>
/// <li>Tentative de codage dans le Statut des élus reconnus par expression régulière (ELU) </li>
/// <li>Inférence de la catégorie statutaire A, B, C, à partir d'expressions régulières</li></ul>
/// \param info Structure info_t contenant les fichiers de paye en mémoire
/// \return nullptr ou Exception.


void* parse_info (info_t& info)
{
//
// Pré-traitement des fichiers de paye
//

#ifdef INCLURE_REG_ELUS
    regex pat {EXPRESSION_REG_ELUS,  regex_constants::icase};
#endif    
#ifdef INCLURE_REG_VACATAIRES    
    regex pat2 {EXPRESSION_REG_VACATIONS, regex_constants::icase};
#endif 
#ifdef INCLURE_REG_ASSMAT    
    regex pat3 {EXPRESSION_REG_ASSISTANTES_MATERNELLES, regex_constants::icase};
#endif    
    regex pat_adjoints {EXPRESSION_REG_ADJOINTS, regex_constants::icase};
    regex pat_agents {EXPRESSION_REG_AGENTS, regex_constants::icase};
    regex pat_cat_a {EXPRESSION_REG_CAT_A, regex_constants::icase};
    regex pat_cat_b {EXPRESSION_REG_CAT_B, regex_constants::icase};

#   ifndef TINYXML2
      xmlKeepBlanksDefault (0);
#   endif      

    for (unsigned i = 0; i < info.threads->argc ; ++i)
        {
            if (i == 0)
                {
                    // première allocation ou réallocation à la suite d'un incident
                    allouer_memoire_table (info);
                    memory_debug ("decoder_fichier_allouer_memoire_table(info)");
                }

            if (info.verifmem) return nullptr;

            info.fichier_courant = i;

//
// Lancement du traitement des fichiers de paye
//

            switch (parseFile (info))
                {
                case RETRY:
                    i = 0;
                    // on réalloue tout depuis le début à la site d'un incident

                    cerr << ERROR_HTML_TAG " Il est nécessaire de réallouer la mémoire à la suite d'un incident dû aux données..." ENDL;
                    continue;

                case SKIP_FILE:
                    cerr << ERROR_HTML_TAG " Le fichier  " << info.threads->argv[info.fichier_courant] << " n'a pas pu être traité" ENDL
                         << "   Fichier suivant..." ENDL;
                    continue;

                default :
#ifndef NO_DEBUG
                    cerr << PROCESSING_HTML_TAG "Fichier " << info.threads->argv[info.fichier_courant]  << "  traité" ENDL;
#endif
                    break;
                }

            memory_debug ("decoder_fichier_parseFile(info");
        }

//
// Post-traitement
//

    // Vérifie la cohérence des deux calculs de préallocation de NCumAgentXml et NCumAgent
    // En cas d'incohérence, exception.

    if (info.reduire_consommation_memoire && info.NCumAgentXml > info.NCumAgent)
        {
            if (verbeux)
                {
                    cerr << ERROR_HTML_TAG "Incohérence de l'allocation mémoire ex-ante " << info.NCumAgent
                         << " unités et ex-post " <<  info.NCumAgentXml << " unités d'information." ENDL
                         << "Sortie pour éviter une erreur de segmentation." ENDL;
                    cerr << " Fichier : " << info.threads->argv[info.fichier_courant] << ENDL;
                }
            else
                cerr << ERROR_HTML_TAG "Erreur critique lors de l'analyse du fichier : " << info.threads->argv[info.fichier_courant] << ENDL;

            throw;
        }

    // attention, pas info<-NCumAgent ici

    // Le champ statut est modifié comme suit :
    //    ELU   pour un élu
    //    et le champ grade :
    //    V     pour un vacataire
    //    A     pour une assistante maternelle

#define VAR(X) info.Table[agent][X]

    for (unsigned agent = 0; agent < info.NCumAgentXml; ++agent)
        {
            // Les élus peuvent être identifiés soit dans le service soit dans l'emploi métier

#ifdef NORMALISER_ACCENTS

            xmlChar* em = VAR (EmploiMetier);
            xmlChar* gr = VAR (Grade);
#else
#if defined INCLURE_REG_ELUS || defined INCLURE_REG_VACATAIRES || defined INCLURE_REG_ASSMAT
            xmlChar* em = xmlStrdup (VAR (EmploiMetier));
            normaliser_accents (em);
#endif
            xmlChar* gr = xmlStrdup (VAR (Grade));

#endif

            // La normalisation des accents facilite et sécurise les analyses d'expressions régulières


            normaliser_accents (gr);
            
#           ifdef INCLURE_REG_ELUS
              if (regex_match ((const char*)em, pat))
                 {
                    xmlFree (VAR (Statut)) ;
                    VAR (Statut) =  xmlStrdup ((const xmlChar*)"ELU");
                    VAR (Catégorie) = nullptr;
                 }
            else
#           endif                                
                {
                    // Peu fiable
                    // vacataires                  
#                   ifdef INCLURE_REG_VACATAIRES 
                    if (regex_match ((const char*) em, pat2))
                        {
                            xmlFree (VAR (Grade));
                            VAR (Grade) = (xmlChar*) xmlStrdup ((const xmlChar*)"V");
                        }
                    else 
#                   endif
                    
                    // Peu fiable
                    // assistantes maternelles
#                   ifdef INCLURE_REG_ASSMAT                    
                    if (regex_match ((const char*) em, pat3))
                        {
                            xmlFree (VAR (Grade));
                            VAR (Grade) = (xmlChar*) xmlStrdup ((const xmlChar*)"A");
                        }
#                   endif
                    ///////////////////////////////////////////////
                    // identification des catégories A, B, C     //
                    ///////////////////////////////////////////////

                    // gestion de mémoire : ne pas allouer avec xmlStrdup et ne pas libérer
                    // à la fin de main.cpp dans la double boucle de libération de mémoire car
                    // A, B, C, NA ne sont pas alloués sur le tas.

                    if (gr)
                    {
                        if (regex_match ((const char*) gr, pat_adjoints)
                                || regex_match ((const char*) gr, pat_agents))
                            {
                                VAR (Categorie) = xmlStrdup ((xmlChar*)"C");
                            }
                        else if (regex_match ((const char*) gr, pat_cat_a))
                            {
                                VAR (Categorie) = xmlStrdup ((xmlChar*)"A");
                            }
                        else if (regex_match ((const char*) gr, pat_cat_b))
                            {
                                VAR (Categorie) = xmlStrdup ((xmlChar*)"B");
                            }
                        else
                            {
                                VAR (Categorie) = nullptr;
                            }

                    }
                    else
                    {
                        cerr << ERROR_HTML_TAG "Impossible de trouver le champ obligatoire Grade pour l'année : "
                             << VAR(Annee) << " et le mois: " << VAR(Mois) << ENDL;
                        cerr << WARNING_HTML_TAG "Réexécuter en mode verbeux pour disposer de plus d'informations. "
                             << ENDL;
                            if (verbeux
                                    && info.ligne_debut.size() > agent
                                    && info.ligne_fin.size() > agent)
                            {
                                 cerr << " entre les lignes "
                                 << info.ligne_debut.at (agent)[0] + 1
                                 << " et "
                                 <<   info.ligne_fin.at (agent)[0];
                            }

                        cerr <<  ENDL;
                        VAR(Categorie) = nullptr;
                    }

                    // Il faut tester d'abord cat A et seulement ensuite cat B

                }

            // Les vacations peuvent être indiquées comme telles dans les libellés de paie mais pas dans les emplois métiers.
            // On les récupère en parcourant les libellés
#        ifdef INCLURE_REG_VACATAIRES 
            if (info.reduire_consommation_memoire)
                {
                    // inutile de boucler sur la partie vide du tableau...

                    for (int j = BESOIN_MEMOIRE_ENTETE; j < info.NLigne[agent]; ++j)
                        if (regex_match ((const char*) VAR (j), pat2))
                            {
                                xmlFree (VAR (Grade));
                                VAR (Grade) = (xmlChar*) xmlStrdup ((const xmlChar*)"V");
                            }
                }
            else
                {
                    for (int j = BESOIN_MEMOIRE_ENTETE ; j < info.NLigne[agent] && info.Table[agent][j] != nullptr; ++j)
                        if (regex_match ((const char*) VAR (j), pat2))
                            {
                                xmlFree (VAR (Grade));
                                VAR (Grade) = (xmlChar*) xmlStrdup ((const xmlChar*)"V");
                            }
                }
#        endif 
#ifndef NORMALISER_ACCENTS
#if defined INCLURE_REG_ELUS || defined INCLURE_REG_VACATAIRES || defined INCLURE_REG_ASSMAT
            xmlFree (em);
#endif
            xmlFree (gr);
#endif
        }

#undef VAR

    return nullptr;
}

