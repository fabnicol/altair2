

#include <inttypes.h>
#include <cstring>
#include <array>
#include <sstream>  // attention l'oubli de cette déclaration induit des erreurs de log de compilation
#include "table.hpp"
#include "fonctions_auxiliaires.hpp"
#include "tags.h"

#include <iostream>
#include <fstream>

using namespace std;

static const char* type_remuneration_traduit[] = {
    "T",   // Traitement
    "IR", // Indemnité de résidence
    "S",  // Supplément familial
    "AV", // Avantage en nature
    "I",  // Indemnité
    "A", //Autres rémunérations
    "D", //Déduction
    "AC", //Acompte
    "R", // Rappel
    "RE", //Retenue
    "C", //Cotisation
    "CO" //Commentaire
};

#define VAR(X) Info[i].Table[agent][X]

// la méthode OFSTREAM est nettement moins performante sous Windows. La performance linux est comparable.

#ifdef OFSTREAM_TABLE_OUTPUT
#define table_t ofstream
#else
#define table_t ostringstream
#endif


static inline void GCC_INLINE ECRIRE_LIGNE_l_COMMUN(int i, uint32_t agent, int l, char* type, table_t& base, char sep, vector<info_t> &Info, int GCC_UNUSED rang)
{
    base   << VAR(Nom) << sep
           << VAR(Prenom) << sep
           << VAR(Matricule) << sep
           << VAR(Service) << sep
           << VAR(NbEnfants) << sep
           << VAR(Statut) << sep
           << VAR(QuotiteTrav) << sep
           << VAR(NbHeureSup) << sep
           << VAR(NbHeureTotal) << sep
           << VAR(Indice) << sep
           << VAR(MtBrut) << sep
           << VAR(MtNet) << sep
           << VAR(MtNetAPayer) << sep
           << VAR(NBI) << sep
           << VAR(l) << sep
           << VAR(l + 1) << sep
           << VAR(l + 2) << sep
           << VAR(l + 3) << sep
           << VAR(l + 4) << sep
           << VAR(l + INDEX_MAX_COLONNNES) << sep     // Il doit y avoir au plus INDEX_MAX_COLONNES + 1 types de colonnes de lignes de paye différents
           << type << sep
           << VAR(EmploiMetier) << sep
           << VAR(Grade) << sep;
}

static inline void GCC_INLINE ECRIRE_LIGNE_l_GENERER_RANG(int i, uint32_t agent, int l, char* type, table_t& base, char sep, vector<info_t> &Info, int rang)
{
    base <<  rang << sep;
    base  << VAR(Annee) << sep
          << VAR(Mois) << sep;
    
    if (Info[0].select_siret)
    {
        base  << VAR(Budget) << sep
              << VAR(Employeur) << sep
              << VAR(Siret) << sep
              << VAR(Etablissement) << sep;
    }
    
    ECRIRE_LIGNE_l_COMMUN(i, agent, l, type, base, sep, Info, rang);

    base << VAR(Categorie) << sep
         << VAR(NIR) << "\n";
}

static inline void GCC_INLINE ECRIRE_LIGNE_l_GENERER_RANG_ECHELON(int i, uint32_t agent, int l, char* type, table_t& base, char sep, vector<info_t> &Info, int rang)
{
    base <<  rang << sep;
    base  << VAR(Annee) << sep
          << VAR(Mois) << sep;

    if (Info[0].select_siret)
    {
        base  << VAR(Budget) << sep
              << VAR(Employeur) << sep
              << VAR(Siret) << sep
              << VAR(Etablissement) << sep;
    }

    ECRIRE_LIGNE_l_COMMUN(i, agent, l, type, base, sep, Info, rang);

    base << VAR(Echelon) << sep
         << VAR(Categorie) << sep
         << VAR(NIR) << "\n";
}

static inline void GCC_INLINE ECRIRE_LIGNE_l_SIRET(int i, uint32_t agent, int l, char* type, table_t& base, char sep, vector<info_t> &Info, int GCC_UNUSED rang)
{
    base  << VAR(Annee) << sep
          << VAR(Mois) << sep;
    
    base  << VAR(Budget) << sep
          << VAR(Employeur) << sep
          << VAR(Siret) << sep
          << VAR(Etablissement) << sep;

    ECRIRE_LIGNE_l_COMMUN(i, agent, l, type, base, sep, Info, rang);

   base  << VAR(Categorie) << sep
         << VAR(NIR) << "\n";
}

static inline void GCC_INLINE ECRIRE_LIGNE_l_SIRET_ECHELON(int i, uint32_t agent, int l, char* type, table_t& base, char sep, vector<info_t> &Info, int GCC_UNUSED rang)
{
    base  << VAR(Annee) << sep
          << VAR(Mois) << sep;

    base  << VAR(Budget) << sep
          << VAR(Employeur) << sep
          << VAR(Siret) << sep
          << VAR(Etablissement) << sep;

    ECRIRE_LIGNE_l_COMMUN(i, agent, l, type, base, sep, Info, rang);

    base << VAR(Echelon) << sep
         << VAR(Categorie) << sep
         << VAR(NIR) << "\n";
}

static inline void GCC_INLINE ECRIRE_LIGNE_l(int i, uint32_t agent, int l, char* type, table_t& base, char sep, vector<info_t> &Info, int GCC_UNUSED rang)
{
    base  << VAR(Annee) << sep
          << VAR(Mois) << sep;
    
    ECRIRE_LIGNE_l_COMMUN(i, agent, l, type, base, sep, Info, rang);

    base << VAR(Categorie) << sep
         << VAR(NIR) << "\n";
}

static inline void GCC_INLINE ECRIRE_LIGNE_l_ECHELON(int i, uint32_t agent, int l, char* type, table_t& base, char sep, vector<info_t> &Info, int GCC_UNUSED rang)
{
    base  << VAR(Annee) << sep
          << VAR(Mois) << sep;

    ECRIRE_LIGNE_l_COMMUN(i, agent, l, type, base, sep, Info, rang);

    base << VAR(Echelon) << sep
         << VAR(Categorie) << sep
         << VAR(NIR) << "\n";

}

static inline void GCC_INLINE ECRIRE_LIGNE_BULLETIN_COMMUN(int i, uint32_t agent, table_t& bulletins, char sep, vector<info_t> &Info, int GCC_UNUSED rang)
{
    bulletins << VAR(Nom) << sep
              << VAR(Prenom) << sep
              << VAR(Matricule) << sep
              << VAR(Service) << sep
              << VAR(NbEnfants) << sep
              << VAR(Statut) << sep
              << VAR(QuotiteTrav) << sep
              << VAR(NbHeureSup) << sep
              << VAR(NbHeureTotal) << sep
              << VAR(Indice) << sep
              << VAR(MtBrut) << sep
              << VAR(MtNet) << sep
              << VAR(MtNetAPayer) << sep
              << VAR(NBI) << sep
              << VAR(EmploiMetier) << sep
              << VAR(Grade) << sep;

}

static inline void GCC_INLINE  ECRIRE_LIGNE_BULLETIN_GENERER_RANG(int i, uint32_t agent, table_t& bulletins, char sep, vector<info_t> &Info, int rang)
{
    bulletins <<  rang << sep;

    bulletins << VAR(Annee) << sep
              << VAR(Mois) << sep;

    if (Info[0].select_siret)
    {
        bulletins  << VAR(Budget) << sep
                   << VAR(Employeur) << sep
                   << VAR(Siret) << sep
                   << VAR(Etablissement) << sep;
    }

    ECRIRE_LIGNE_BULLETIN_COMMUN(i, agent, bulletins, sep, Info, rang);

      bulletins   << VAR(Categorie) << sep
                  << VAR(NIR) << "\n";
}

static inline void GCC_INLINE  ECRIRE_LIGNE_BULLETIN_GENERER_RANG_ECHELON(int i, uint32_t agent, table_t& bulletins, char sep, vector<info_t> &Info, int rang)
{
    bulletins <<  rang << sep;
    
    bulletins << VAR(Annee) << sep
              << VAR(Mois) << sep;
    
    if (Info[0].select_siret)
    {
        bulletins  << VAR(Budget) << sep
                   << VAR(Employeur) << sep
                   << VAR(Siret) << sep
                   << VAR(Etablissement) << sep;
    }
    
    ECRIRE_LIGNE_BULLETIN_COMMUN(i, agent, bulletins, sep, Info, rang);

    bulletins    << VAR(Echelon)   << sep
                 << VAR(Categorie) << sep
                 << VAR(NIR) << "\n";
}

static inline void GCC_INLINE  ECRIRE_LIGNE_BULLETIN_SIRET(int i, uint32_t agent, table_t& bulletins, char sep, vector<info_t> &Info, int GCC_UNUSED rang)
{
    
    bulletins << VAR(Annee) << sep
              << VAR(Mois) << sep;
    
    bulletins  << VAR(Budget) << sep
               << VAR(Employeur) << sep
               << VAR(Siret) << sep
               << VAR(Etablissement) << sep;

    ECRIRE_LIGNE_BULLETIN_COMMUN(i, agent, bulletins, sep, Info, rang);

    bulletins    << VAR(Categorie) << sep
                 << VAR(NIR) << "\n";
}

static inline void GCC_INLINE  ECRIRE_LIGNE_BULLETIN_SIRET_ECHELON(int i, uint32_t agent, table_t& bulletins, char sep, vector<info_t> &Info, int GCC_UNUSED rang)
{

    bulletins << VAR(Annee) << sep
              << VAR(Mois) << sep;

    bulletins  << VAR(Budget) << sep
               << VAR(Employeur) << sep
               << VAR(Siret) << sep
               << VAR(Etablissement) << sep;

    ECRIRE_LIGNE_BULLETIN_COMMUN(i, agent, bulletins, sep, Info, rang);

    bulletins    << VAR(Echelon)   << sep
                 << VAR(Categorie) << sep
                 << VAR(NIR) << "\n";
}

static inline void GCC_INLINE  ECRIRE_LIGNE_BULLETINS(int i, uint32_t agent, table_t& bulletins, char sep, vector<info_t> &Info, int GCC_UNUSED rang)
{
    bulletins << VAR(Annee) << sep
              << VAR(Mois) << sep;
    
    ECRIRE_LIGNE_BULLETIN_COMMUN(i, agent, bulletins, sep, Info, rang);

    bulletins    << VAR(Categorie) << sep
                 << VAR(NIR) << "\n";
}

static inline void GCC_INLINE  ECRIRE_LIGNE_BULLETINS_ECHELON(int i, uint32_t agent, table_t& bulletins, char sep, vector<info_t> &Info, int GCC_UNUSED rang)
{
    bulletins << VAR(Annee) << sep
              << VAR(Mois) << sep;

    ECRIRE_LIGNE_BULLETIN_COMMUN(i, agent, bulletins, sep, Info, rang);

    bulletins    << VAR(Echelon)   << sep
                 << VAR(Categorie) << sep
                 << VAR(NIR) << "\n";
}

static void (*ecrire_ligne_table)(int, uint32_t, int, char*, table_t&, char, vector<info_t> &, int);
static void (*ecrire_ligne_bulletin)(int i, uint32_t, table_t& , char, vector<info_t>& , int );

pair<uint64_t, uint32_t> boucle_ecriture(vector<info_t>& Info, int nsegment)
{
    int ligne = 0;
    uint64_t compteur = 0,
            dernier_compteur = 0,
            compteur_annee_courante = 0;

    uint32_t compteur_lignes_bulletins = 0;

    char sep = Info[0].separateur;
    char* annee_courante = (char*) Info[0].Table[0][Annee];
    unsigned rang_fichier_base = 1, rang_fichier_base_annee_courante = 1;
    static ofstream base;
    static ofstream bulletins;
    static array<ofstream, nbType + 1> tableau_base;

#ifdef OFSTREAM_TABLE_OUTPUT    // cas de l'écriture directe dans le fichier base

#define t_base base
#define t_bulletins bulletins
#define t_tableau_base tableau_base

#else                          //  case de l'écriture dans un tampon string avant écriture sur le disque (nécessite plus de mémoire)

    ostringstream t_base;
    ostringstream t_bulletins;
    static array<ostringstream, nbType + 1> t_tableau_base;

#endif

    ouvrir_fichier_bulletins(Info[0], bulletins, nsegment);

    uint32_t taille_base = Info[0].taille_base;
    BaseType type_base = Info[0].type_base;


    if (Info[0].generer_rang)
    {
        if (Info[0].select_echelon)
        {
          ecrire_ligne_table = ECRIRE_LIGNE_l_GENERER_RANG_ECHELON;
          ecrire_ligne_bulletin = ECRIRE_LIGNE_BULLETIN_GENERER_RANG_ECHELON;
        }
        else
        {
          ecrire_ligne_table = ECRIRE_LIGNE_l_GENERER_RANG;
          ecrire_ligne_bulletin = ECRIRE_LIGNE_BULLETIN_GENERER_RANG;
        }
    }
    else
        if (Info[0].select_siret)
        {
            if (Info[0].select_echelon)
            {
              ecrire_ligne_table = ECRIRE_LIGNE_l_SIRET_ECHELON;
              ecrire_ligne_bulletin = ECRIRE_LIGNE_BULLETIN_SIRET_ECHELON;
            }
            else
            {
              ecrire_ligne_table = ECRIRE_LIGNE_l_SIRET;
              ecrire_ligne_bulletin = ECRIRE_LIGNE_BULLETIN_SIRET;
            }

        }
        else
        {
            if (Info[0].select_echelon)
            {
                ecrire_ligne_table = ECRIRE_LIGNE_l_ECHELON;
                ecrire_ligne_bulletin = ECRIRE_LIGNE_BULLETINS_ECHELON;
            }
            else
            {
                ecrire_ligne_table = ECRIRE_LIGNE_l;
                ecrire_ligne_bulletin = ECRIRE_LIGNE_BULLETINS;
            }
        }

    switch (type_base)
    {
    case BaseType::TOUTES_CATEGORIES  :
        for (int d = 0; d <= nbType; ++d)  // Il faut tenir compte des types NA
        {
            ouvrir_fichier_base(Info[0], static_cast<BaseType>(d + 1), tableau_base[d], nsegment);
            if (! tableau_base[d].is_open())
            {
                cerr << ERROR_HTML_TAG "impossible d'ouvrir la base de lignes de paye."  ENDL;
                exit(2003);
            }
        }

        break;

    default : ouvrir_fichier_base(Info[0], type_base,  base, nsegment);
        if (! base.is_open())
        {
            cerr << ERROR_HTML_TAG "impossible d'ouvrir la base de lignes de paye."  ENDL;
            exit(2004);
            return make_pair(0,0);
        }

    }
    
    if (! bulletins.is_open())
    {
        cerr << ERROR_HTML_TAG "impossible d'ouvrir la base de bulletins de paye."  ENDL;
        return make_pair(0,0);
    }

#ifdef GENERATE_RANK_SIGNAL
    uint64_t NCumLignes = 0;
    int progression = 0;
    unsigned int step = 0;

    for (unsigned i = 0; i < Info[0].nbfil; ++i)
    {
        NCumLignes += Info[i].nbLigne;
    }

    reset_rank_signal();
    generate_rank_signal();

#endif

    static uint64_t compteur_ancien, compteur_bulletins_ancien;
    compteur = compteur_ancien;
    compteur_lignes_bulletins = compteur_bulletins_ancien;

    if (type_base == BaseType::MONOLITHIQUE)
    {
        for (unsigned i = 0; i < Info[0].nbfil; ++i)
        {
            for (uint32_t agent = 0; agent < Info[i].NCumAgentXml; ++agent)
            {
                unsigned l = BESOIN_MEMOIRE_ENTETE;
                uint16_t NLigneAgent = Info[i].NLigne[agent];

                ++compteur_lignes_bulletins;

                ecrire_ligne_bulletin(i, agent, t_bulletins, sep, Info, compteur_lignes_bulletins);

                char* type = nullptr;

                while (ligne < NLigneAgent)
                {
                    int test_drapeau_categorie;

                    // teste si un drapeau de nouvelle catégorie de ligne de paye (T, I,...) a été introduit en base
                    if (VAR(l+1) && xmlStrcmp(VAR(l+1), NA_STRING) == 0)
                    {
                        type = (char*) NA_STRING;
                    }
                    else
                    while (VAR(l) &&  (test_drapeau_categorie = VAR(l)[0], test_drapeau_categorie <= nbType) && (test_drapeau_categorie >= 1))
                    {
                        type = const_cast<char*>(type_remuneration_traduit[test_drapeau_categorie - 1]);
                        ++l;
                        // no break !!!!
                    }

                    ++compteur;
                    ecrire_ligne_table(i, agent, l, type, t_base, sep, Info, compteur);

                    l += INDEX_MAX_COLONNNES + 1;
                    ++ligne;
                }

                ligne = 0;

#ifdef GUI_TAG_MESSAGES
#ifdef GENERATE_RANK_SIGNAL
                progression = ceil((float) (compteur * 100) / (float) NCumLignes );

                ++step;

                if (step > Info[i].NCumAgentXml / 5)
                {
                    generate_rank_signal(progression);
                    cerr << " \n";
                    step = 0;
                }
#endif
#endif
            }
        }
    }
    else
        if (type_base == BaseType::PAR_ANNEE)
        {
            for (unsigned i = 0; i < Info[0].nbfil; ++i)
            {
                for (uint32_t agent = 0; agent < Info[i].NCumAgentXml; ++agent)
                {
                    unsigned l = BESOIN_MEMOIRE_ENTETE;
                    uint16_t NLigneAgent = Info[i].NLigne[agent];

                    ++compteur_lignes_bulletins;

                    ecrire_ligne_bulletin(i, agent, t_bulletins, sep, Info, compteur_lignes_bulletins);

                    if (strcmp((const char*)VAR(Annee), annee_courante))
                    {
#ifndef OFSTREAM_TABLE_OUTPUT      // Il faut écrire dans le fichier OFSTREAM la chaine de caractères temporaires
                        base << t_base.str();
                        t_base.str("");
#endif

                        base.close();

                        cerr << "Année : " << annee_courante <<  ENDL;
                        cerr << "Table de " << compteur - dernier_compteur
                                  << " lignes, lignes "  << dernier_compteur + 1
                                  << " à " << compteur << "."  ENDL ENDL;

                        dernier_compteur = compteur;
                        compteur_annee_courante = 0;
                        rang_fichier_base_annee_courante = 1;

                        annee_courante = (char*) VAR(Annee);
                        ouvrir_fichier_base(Info[i],  type_base, base, nsegment);
                        if (! base.is_open()) return make_pair(0,0);
                    }

                    char* type = nullptr;

                    while (ligne < NLigneAgent)
                    {
                        int      test_drapeau_categorie;

                        // teste si un drapeau de nouvelle catégorie de ligne de paye (T, I,...) a été introduit en base
                        if (VAR(l+1) && xmlStrcmp(VAR(l+1), NA_STRING) == 0)
                        {
                            type = (char*) NA_STRING;
                        }
                        else
                        while (VAR(l) &&  (test_drapeau_categorie = VAR(l)[0], test_drapeau_categorie <= nbType) && (test_drapeau_categorie >= 1))
                        {
                            type = const_cast<char*>(type_remuneration_traduit[test_drapeau_categorie - 1]);
                            ++l;
                            // NO BREAK !
                        }

                        ++compteur;
                        ecrire_ligne_table(i, agent, l, type, t_base, sep, Info, compteur);

                        l += INDEX_MAX_COLONNNES + 1;
                        ++ligne;
                    }
                    ligne = 0;

#ifdef GUI_TAG_MESSAGES
#ifdef GENERATE_RANK_SIGNAL
                    progression = ceil((float) (compteur * 100) / (float) NCumLignes );

                    ++step;

                    if (step > Info[i].NCumAgentXml / 5)
                    {

                        generate_rank_signal(progression);
                        cerr << " \n";
                        step = 0;
                    }
#endif
#endif
                }
            }
        }
        else
            if (type_base == BaseType::MAXIMUM_LIGNES
                    // soit : il existe un nombre de lignes maximal par base spécifié en ligne de commande après -T
                    && taille_base > 0)
            {
                rang_fichier_base = compteur / taille_base + 1;

                for (unsigned i = 0; i < Info[0].nbfil; ++i)
                {
                    for (uint32_t agent = 0; agent < Info[i].NCumAgentXml; ++agent)
                    {
                        unsigned l = BESOIN_MEMOIRE_ENTETE;
                        uint16_t NLigneAgent = Info[i].NLigne[agent];

                        ++compteur_lignes_bulletins;

                        ecrire_ligne_bulletin(i, agent, t_bulletins, sep, Info, compteur_lignes_bulletins);

                        char* type = nullptr;

                        while (ligne < NLigneAgent)
                        {
                            if (compteur  == rang_fichier_base * taille_base)
                                // soit : il existe un nombre de lignes maximal par base spécifié en ligne de commande après -T

                            {

                                cerr << "Table n°" << rang_fichier_base << " de " << taille_base
                                          << " lignes, lignes "  << (rang_fichier_base - 1) * taille_base + 1
                                          << " à " << rang_fichier_base * taille_base << "."  ENDL;

#ifndef OFSTREAM_TABLE_OUTPUT      // Il faut écrire dans le fichier OFSTREAM la chaine de caractères temporaires
                                base << t_base.str();
                                t_base.str("");
#endif
                                base.close();

                                if (! base.good())
                                {
                                    cerr << ERROR_HTML_TAG "Problème fermeture fichier base"  ENDL;
                                    exit(-902);
                                }

                                ++rang_fichier_base;

                                if (rang_fichier_base >= 1000)
                                {
                                    cerr << ERROR_HTML_TAG "Ne peut générer que 999 bases au plus"  ENDL;
                                    exit(-904);
                                }

                                ouvrir_fichier_base(Info[i], type_base, base, nsegment);
                                if (! base.is_open()) return make_pair(0,0);
                            }

                            int test_drapeau_categorie;

                            // teste si un drapeau de nouvelle catégorie de ligne de paye (T, I,...) a été introduit en base
                            if (VAR(l+1) && xmlStrcmp(VAR(l+1), NA_STRING) == 0)
                            {
                                type = (char*) NA_STRING;
                            }
                            else
                            while (VAR(l) &&  (test_drapeau_categorie = VAR(l)[0], test_drapeau_categorie <= nbType) && (test_drapeau_categorie >= 1))
                            {
                                type = const_cast<char*>(type_remuneration_traduit[test_drapeau_categorie - 1]);
                                ++l;
                                // NO BREAK !
                            }

                            ++compteur;

                            ecrire_ligne_table(i, agent, l, type, t_base, sep, Info, compteur);

                            l += INDEX_MAX_COLONNNES + 1;
                            ++ligne;
                        }

                        ligne = 0;

#ifdef GUI_TAG_MESSAGES
#ifdef GENERATE_RANK_SIGNAL
                        progression = ceil((float) (compteur * 100) / (float) NCumLignes );

                        ++step;

                        if (step > Info[i].NCumAgentXml / 5)
                        {

                            generate_rank_signal(progression);
                            cerr << " \n";
                            step = 0;
                        }
#endif
#endif
                    }
                }
            }
            else
                if (type_base == BaseType::MAXIMUM_LIGNES_PAR_ANNEE
                        && taille_base > 0)
                {

                    compteur_annee_courante = compteur_ancien;
                    rang_fichier_base_annee_courante = compteur_annee_courante  / taille_base + 1;

                    for (unsigned i = 0; i < Info[0].nbfil; ++i)
                    {
                        for (uint32_t agent = 0; agent < Info[i].NCumAgentXml; ++agent)
                        {
                            unsigned l = BESOIN_MEMOIRE_ENTETE;
                            uint16_t NLigneAgent = Info[i].NLigne[agent];

                            ++compteur_lignes_bulletins;

                            ecrire_ligne_bulletin(i, agent, t_bulletins, sep, Info, compteur_lignes_bulletins);

                            if (strcmp((const char*)VAR(Annee), annee_courante))
                            {
#ifndef OFSTREAM_TABLE_OUTPUT      // Il faut écrire dans le fichier OFSTREAM la chaine de caractères temporaires
                                base << t_base.str();
                                t_base.str("");
#endif

                                base.close();

                                cerr << SPACER "Table n°" << rang_fichier_base << " de " <<  compteur - dernier_compteur - (rang_fichier_base_annee_courante - 1) * taille_base
                                          << " lignes, lignes "  << dernier_compteur + (rang_fichier_base_annee_courante - 1) * taille_base + 1
                                          << " à " << compteur << "."  ENDL;
                                cerr << "Année : " << annee_courante <<  ENDL;
                                cerr << "Total annuel de " << compteur - dernier_compteur
                                          << " lignes, lignes "  << dernier_compteur + 1
                                          << " à " << compteur << "."  ENDL ENDL;

                                dernier_compteur = compteur;
                                compteur_annee_courante = 0;
                                rang_fichier_base_annee_courante = 1;
                                ++rang_fichier_base;

                                annee_courante = (char*) VAR(Annee);
                                ouvrir_fichier_base(Info[i],  type_base, base, nsegment);
                                if (! base.is_open()) return make_pair(0,0);
                            }

                            char* type = nullptr;

                            while (ligne < NLigneAgent)
                            {
                                if (compteur_annee_courante  == rang_fichier_base_annee_courante * taille_base)
                                {

                                    cerr << SPACER "Table n°" << rang_fichier_base << " de " << taille_base
                                              << " lignes, lignes "  << compteur - taille_base + 1
                                              << " à " << compteur << "."  ENDL;

#ifndef OFSTREAM_TABLE_OUTPUT      // Il faut écrire dans le fichier OFSTREAM la chaine de caractères temporaires
                                    base << t_base.str();
                                    t_base.str("");
#endif
                                    base.close();

                                    if (! base.good())
                                    {
                                        cerr << ERROR_HTML_TAG "Problème fermeture fichier base"  ENDL;
                                        exit(-902);
                                    }

                                    ++rang_fichier_base;
                                    ++rang_fichier_base_annee_courante;

                                    if (rang_fichier_base >= 1000)
                                    {
                                        cerr << ERROR_HTML_TAG "Ne peut générer que 999 bases au plus"  ENDL;
                                        exit(-904);
                                    }

                                    ouvrir_fichier_base(Info[i], type_base, base, nsegment);
                                    if (! base.is_open()) return make_pair(0,0);
                                }

                                int      test_drapeau_categorie;

                                // teste si un drapeau de nouvelle catégorie de ligne de paye (T, I,...) a été introduit en base
                                if (VAR(l+1) && xmlStrcmp(VAR(l+1), NA_STRING) == 0)
                                {
                                    type = (char*) NA_STRING;
                                }
                                else
                                while (VAR(l) &&  (test_drapeau_categorie = VAR(l)[0], test_drapeau_categorie <= nbType) && (test_drapeau_categorie >= 1))
                                {
                                    type = const_cast<char*>(type_remuneration_traduit[test_drapeau_categorie - 1]);
                                    ++l;
                                    // NO BREAK !
                                }


                                ++compteur;

                                ++compteur_annee_courante;

                                ecrire_ligne_table(i, agent, l, type, t_base, sep, Info, compteur);

                                l += INDEX_MAX_COLONNNES + 1;
                                ++ligne;
                            }
                            ligne = 0;

#ifdef GUI_TAG_MESSAGES
#ifdef GENERATE_RANK_SIGNAL
                            progression = ceil((float) (compteur * 100) / (float) NCumLignes );

                            ++step;

                            if (step > Info[i].NCumAgentXml / 5)
                            {

                                generate_rank_signal(progression);
                                cerr << " \n";
                                step = 0;
                            }
#endif
#endif
                        }
                    }

                    compteur_ancien = compteur_annee_courante;
                }
                else
                    if (type_base == BaseType::TOUTES_CATEGORIES)
                    {

                        // La différence par rapport à la base monolithique, ce sera les catégories de lignes NA

                        for (unsigned i = 0; i < Info[0].nbfil; ++i)
                        {
                            for (uint32_t agent = 0; agent < Info[i].NCumAgentXml; ++agent)
                            {
                                unsigned l = BESOIN_MEMOIRE_ENTETE;
                                uint16_t NLigneAgent = Info[i].NLigne[agent];

                                ++compteur_lignes_bulletins;

                                ecrire_ligne_bulletin(i, agent, t_bulletins, sep, Info, compteur_lignes_bulletins);

                                char* type = nullptr;

                                int int_drapeau_categorie = -1;

                                if (VAR(l+1) && xmlStrcmp(VAR(l+1), NA_STRING))
                                {
                                   while (ligne < NLigneAgent)
                                    {

                                        int  test_drapeau_categorie;

                                        // teste si un drapeau de nouvelle catégorie de ligne de paye (T, I,...) a été introduit en base

                                        while (VAR(l) &&  (test_drapeau_categorie = VAR(l)[0], test_drapeau_categorie <= nbType) && (test_drapeau_categorie >= 1))
                                        {
                                            int_drapeau_categorie = test_drapeau_categorie - 1;

                                            type = const_cast<char*>(type_remuneration_traduit[int_drapeau_categorie]);

                                            ++l;
                                            // NO BREAK!
                                        }

                                        ++compteur;
                                        ecrire_ligne_table(i, agent, l, type, t_tableau_base[int_drapeau_categorie], sep, Info, compteur);

                                        l += INDEX_MAX_COLONNNES + 1;
                                        ++ligne;
                                    }
                                }
                                else
                                {
                                    while (ligne < NLigneAgent)
                                    {
                                        ++compteur;
                                        ecrire_ligne_table(i, agent, l, "NA", t_tableau_base[nbType], sep, Info, compteur);

                                        l += INDEX_MAX_COLONNNES + 1;
                                        ++ligne;
                                    }
                                }

                                ligne = 0;

#ifdef GUI_TAG_MESSAGES
#ifdef GENERATE_RANK_SIGNAL
                                progression = ceil((float) (compteur * 100) / (float) NCumLignes );

                                ++step;

                                if (step > Info[i].NCumAgentXml / 5)
                                {

                                    generate_rank_signal(progression);
                                    cerr << " \n";
                                    step = 0;
                                }
#endif
#endif
                            }

                        }
                    }
                    else  //par catégorie spécifique
                    {
                        for (unsigned i = 0; i < Info[0].nbfil; ++i)
                        {
                            for (uint32_t agent = 0; agent < Info[i].NCumAgentXml; ++agent)
                            {
                                unsigned l = BESOIN_MEMOIRE_ENTETE;
                                uint16_t NLigneAgent = Info[i].NLigne[agent];

                                ++compteur_lignes_bulletins;

                                ecrire_ligne_bulletin(i, agent, t_bulletins, sep, Info, compteur_lignes_bulletins);

                                char* type = nullptr;

                                BaseType valeur_drapeau_categorie = BaseType::MONOLITHIQUE;

                                if (VAR(l+1) && xmlStrcmp(VAR(l+1), NA_STRING))
                                {
                                    while (ligne < NLigneAgent)
                                    {
                                        int      test_drapeau_categorie;

                                        // teste si un drapeau de nouvelle catégorie de ligne de paye (T, I,...) a été introduit en base
                                        // Var(l)[0] est un xmlChar càd un unsigned char; on doit le caster en int pour indicier les tableau des types
                                        // mais il ne peut être inférieur à 0 par construction.

                                        while (VAR(l) &&  (test_drapeau_categorie = (int) VAR(l)[0], test_drapeau_categorie <= nbType) && (test_drapeau_categorie != 0))
                                        {
                                            valeur_drapeau_categorie = static_cast<BaseType>(test_drapeau_categorie);

                                            type = const_cast<char*>(type_remuneration_traduit[test_drapeau_categorie - 1]);
                                            ++l;
                                            //break;  NO BREAK!
                                        }

                                        // test de catégorie

                                        if (valeur_drapeau_categorie  == Info[0].type_base)
                                        {
                                            ++compteur;
                                            ecrire_ligne_table(i, agent, l, type, t_base, sep, Info, compteur);
                                        }

                                        l += INDEX_MAX_COLONNNES + 1;
                                        ++ligne;

                                        // Si on a trouvé une catégorie, alors on peut avoir plusieurs enregistrements d'affilée sur cette catégorie.
                                        // Pour intégrer ces lignes, il est important de ne pas réinitialiser valeur_drapeau_categorie en début de boucle
                                        // En effet test_drapeau_categorie ne serait dans les bornes du tableau des types en l'absence de tout drapeau
                                        // et alors le test de catégorie supra serait inadéquatement négatif
                                    }
                                }

                                ligne = 0;

#ifdef GUI_TAG_MESSAGES
#ifdef GENERATE_RANK_SIGNAL
                                progression = ceil((float) (compteur * 100) / (float) NCumLignes );

                                ++step;

                                if (step > Info[i].NCumAgentXml / 5)
                                {

                                    generate_rank_signal(progression);
                                    cerr << " \n";
                                    step = 0;
                                }
#endif
#endif
                            }
                        }

                    }


 compteur_ancien = compteur;
 compteur_bulletins_ancien = compteur_lignes_bulletins;
    
#ifndef OFSTREAM_TABLE_OUTPUT
    bulletins << t_bulletins.str();
#endif

    // Dans les autres cas, les bases ont déjà été refermées sauf une (cas par année et par taille maximale)
    if (type_base == BaseType::TOUTES_CATEGORIES)
    {
        for (int d = 0; d <= nbType; ++d)
        {
#ifndef OFSTREAM_TABLE_OUTPUT
            tableau_base[d] << t_tableau_base[d].str();
            t_tableau_base[d].str("");
#endif
            tableau_base[d].close();
        }
    }
    else
        if (base.is_open())
        {
#ifndef OFSTREAM_TABLE_OUTPUT      // Il faut écrire dans le fichier OFSTREAM la chaine de caractères temporaires
            base << t_base.str();
            t_base.str("");
#endif

            base.close();
        }

    uint64_t compteur_lignes = compteur;

    switch (type_base)
    {
    case  BaseType::MONOLITHIQUE            :
        cerr << STATE_HTML_TAG "Table intégrée."  ENDL;
        break;

    case  BaseType::PAR_TRAITEMENT          :
        cerr << STATE_HTML_TAG "Catégorie : Traitement."  ENDL;
        break;

    case  BaseType::PAR_INDEMNITE_RESIDENCE :
        cerr << STATE_HTML_TAG "Catégorie : Indemnité de résidence."  ENDL;
        break;

    case  BaseType::PAR_SFT                 :
        cerr << STATE_HTML_TAG "Catégorie : Supplément familial de traitement."  ENDL;
        break;

    case  BaseType::PAR_AVANTAGE_NATURE     :
        cerr << STATE_HTML_TAG "Catégorie : Avantage en nature."  ENDL;
        break;

    case  BaseType::PAR_INDEMNITE           :
        cerr << STATE_HTML_TAG "Catégorie : Indemnité."  ENDL;
        break;

    case  BaseType::PAR_REM_DIVERSES        :
        cerr << STATE_HTML_TAG "Catégorie : Rémunérations diverses."  ENDL;
        break;

    case  BaseType::PAR_DEDUCTION           :
        cerr << STATE_HTML_TAG "Catégorie : Déduction."  ENDL;
        break;

    case  BaseType::PAR_ACOMPTE             :
        cerr << STATE_HTML_TAG "Catégorie : Acompte."  ENDL;
        break;

    case  BaseType::PAR_RAPPEL              :
        cerr << STATE_HTML_TAG "Catégorie : Rappel."  ENDL;
        break;

    case  BaseType::PAR_RETENUE             :
        cerr << STATE_HTML_TAG "Catégorie : Retenue."  ENDL;
        break;

    case  BaseType::PAR_COTISATION          :
        cerr << STATE_HTML_TAG "Catégorie : Cotisation."  ENDL;
        break;

    case  BaseType::TOUTES_CATEGORIES       :
        cerr << STATE_HTML_TAG "Toutes catégories."  ENDL;
        break;

    case BaseType::PAR_ANNEE    :
        cerr << "Année : " << annee_courante  << ENDL;
        cerr << "Table de " << compteur - dernier_compteur
                  << " lignes, lignes "  << dernier_compteur + 1
                  << " à " << compteur << "."  ENDL;
        break;

    case BaseType::MAXIMUM_LIGNES_PAR_ANNEE :
        cerr << SPACER "Table n°" << rang_fichier_base << " de " <<  compteur - dernier_compteur - (rang_fichier_base_annee_courante - 1) * taille_base
                  << " lignes, lignes "  << dernier_compteur + (rang_fichier_base_annee_courante - 1) * taille_base + 1
                  << " à " << compteur << "."  ENDL;
        cerr << "Année : " << annee_courante <<  ENDL;
        cerr << "Total annuel de " << compteur - dernier_compteur
                  << " lignes, lignes "  << dernier_compteur + 1
                  << " à " << compteur << "."  ENDL;
        break;

    case BaseType::MAXIMUM_LIGNES  :  /* Taille définie par l'utilisateur */
        cerr << STATE_HTML_TAG "Table n°" << rang_fichier_base
                  << " de " <<  compteur - (rang_fichier_base-1) * taille_base
                  << " lignes, lignes " << (rang_fichier_base-1) * taille_base + 1
                  << " à " << compteur << "."  ENDL;
       // compteur_lignes = compteur - compteur_ancien;
        break;

    default:  break;
    }

        cerr << ENDL;

        cerr << STATE_HTML_TAG "Nombre total de lignes de paye : " << compteur << " lignes."  ENDL;

#if defined(__WIN32__) && defined(USE_ICONV)
        convertir(Info[0].chemin_base);
#endif

    if (bulletins.good())
    {
        bulletins.close();

        cerr << STATE_HTML_TAG "Base des bulletins de paye de " << compteur_lignes_bulletins << " lignes."  ENDL;

#if defined(__WIN32__) && defined(USE_ICONV)
        convertir(Info[0].chemin_bulletins);
#endif
    }
    else
        cerr << ERROR_HTML_TAG "Problème de qualité du fichier " << Info[0].chemin_bulletins << ENDL;
    
 return make_pair(compteur_lignes, compteur_lignes_bulletins);
}

#undef VAR

