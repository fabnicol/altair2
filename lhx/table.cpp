

#include <inttypes.h>
#include <cstring>
#include <array>
#include <sstream>  // attention l'oubli de cette déclaration induit des erreurs de log de compilation
#include "table.hpp"
#include "fonctions_auxiliaires.hpp"
#include "tags.h"

#include <iostream>
#include <fstream>

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
#define table_t std::ofstream
#else
#define table_t std::ostringstream
#endif


static inline void GCC_INLINE ECRIRE_LIGNE_l_COMMUN(int i, uint32_t agent, int l, char* type, table_t& base, char sep, std::vector<info_t> &Info, int GCC_UNUSED rang)
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

static inline void GCC_INLINE ECRIRE_LIGNE_l_GENERER_RANG(int i, uint32_t agent, int l, char* type, table_t& base, char sep, std::vector<info_t> &Info, int rang)
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

static inline void GCC_INLINE ECRIRE_LIGNE_l_GENERER_RANG_ECHELON(int i, uint32_t agent, int l, char* type, table_t& base, char sep, std::vector<info_t> &Info, int rang)
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

static inline void GCC_INLINE ECRIRE_LIGNE_l_SIRET(int i, uint32_t agent, int l, char* type, table_t& base, char sep, std::vector<info_t> &Info, int GCC_UNUSED rang)
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

static inline void GCC_INLINE ECRIRE_LIGNE_l_SIRET_ECHELON(int i, uint32_t agent, int l, char* type, table_t& base, char sep, std::vector<info_t> &Info, int GCC_UNUSED rang)
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

static inline void GCC_INLINE ECRIRE_LIGNE_l(int i, uint32_t agent, int l, char* type, table_t& base, char sep, std::vector<info_t> &Info, int GCC_UNUSED rang)
{
    base  << VAR(Annee) << sep
          << VAR(Mois) << sep;
    
    ECRIRE_LIGNE_l_COMMUN(i, agent, l, type, base, sep, Info, rang);

    base << VAR(Categorie) << sep
         << VAR(NIR) << "\n";
}

static inline void GCC_INLINE ECRIRE_LIGNE_l_ECHELON(int i, uint32_t agent, int l, char* type, table_t& base, char sep, std::vector<info_t> &Info, int GCC_UNUSED rang)
{
    base  << VAR(Annee) << sep
          << VAR(Mois) << sep;

    ECRIRE_LIGNE_l_COMMUN(i, agent, l, type, base, sep, Info, rang);

    base << VAR(Echelon) << sep
         << VAR(Categorie) << sep
         << VAR(NIR) << "\n";

}

static inline void GCC_INLINE ECRIRE_LIGNE_BULLETIN_COMMUN(int i, uint32_t agent, table_t& bulletins, char sep, std::vector<info_t> &Info, int GCC_UNUSED rang)
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

static inline void GCC_INLINE  ECRIRE_LIGNE_BULLETIN_GENERER_RANG(int i, uint32_t agent, table_t& bulletins, char sep, std::vector<info_t> &Info, int rang)
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

static inline void GCC_INLINE  ECRIRE_LIGNE_BULLETIN_GENERER_RANG_ECHELON(int i, uint32_t agent, table_t& bulletins, char sep, std::vector<info_t> &Info, int rang)
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

static inline void GCC_INLINE  ECRIRE_LIGNE_BULLETIN_SIRET(int i, uint32_t agent, table_t& bulletins, char sep, std::vector<info_t> &Info, int GCC_UNUSED rang)
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

static inline void GCC_INLINE  ECRIRE_LIGNE_BULLETIN_SIRET_ECHELON(int i, uint32_t agent, table_t& bulletins, char sep, std::vector<info_t> &Info, int GCC_UNUSED rang)
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

static inline void GCC_INLINE  ECRIRE_LIGNE_BULLETINS(int i, uint32_t agent, table_t& bulletins, char sep, std::vector<info_t> &Info, int GCC_UNUSED rang)
{
    bulletins << VAR(Annee) << sep
              << VAR(Mois) << sep;
    
    ECRIRE_LIGNE_BULLETIN_COMMUN(i, agent, bulletins, sep, Info, rang);

    bulletins    << VAR(Categorie) << sep
                 << VAR(NIR) << "\n";
}

static inline void GCC_INLINE  ECRIRE_LIGNE_BULLETINS_ECHELON(int i, uint32_t agent, table_t& bulletins, char sep, std::vector<info_t> &Info, int GCC_UNUSED rang)
{
    bulletins << VAR(Annee) << sep
              << VAR(Mois) << sep;

    ECRIRE_LIGNE_BULLETIN_COMMUN(i, agent, bulletins, sep, Info, rang);

    bulletins    << VAR(Echelon)   << sep
                 << VAR(Categorie) << sep
                 << VAR(NIR) << "\n";
}

static void (*ecrire_ligne_table)(int, uint32_t, int, char*, table_t&, char, std::vector<info_t> &, int);
static void (*ecrire_ligne_bulletin)(int i, uint32_t, table_t& , char, std::vector<info_t>& , int );

void boucle_ecriture(std::vector<info_t>& Info)
{
    int ligne = 0;
    uint64_t compteur = 0,
            dernier_compteur = 0,
            compteur_annee_courante = 0;


    uint32_t compteur_lignes_bulletins = 0;

    char sep = Info[0].separateur;
    char* annee_courante = (char*) Info[0].Table[0][Annee];
    unsigned rang_fichier_base = 1, rang_fichier_base_annee_courante = 1;
    static std::ofstream base;
    static std::ofstream bulletins;
    static std::array<std::ofstream, nbType> tableau_base;

#ifdef OFSTREAM_TABLE_OUTPUT    // cas de l'écriture directe dans le fichier base

#define t_base base
#define t_bulletins bulletins
#define t_tableau_base tableau_base

#else                          //  case de l'écriture dans un tampon string avant écriture sur le disque (nécessite plus de mémoire)

    std::ostringstream t_base;
    std::ostringstream t_bulletins;

    static std::array<std::ostringstream, nbType> t_tableau_base;

#endif

    ouvrir_fichier_bulletins(Info[0], bulletins);


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
        for (int d = 0; d < nbType; ++d)
        {
            ouvrir_fichier_base(Info[0], type_base, tableau_base[d]);
            if (! tableau_base[d].is_open())
            {
                std::cerr << ERROR_HTML_TAG "impossible d'ouvrir la base de lignes de paye."  ENDL;
                exit(2003);
            }
        }
        break;

    default : ouvrir_fichier_base(Info[0], type_base,  base);
        if (! base.is_open())
        {
            std::cerr << ERROR_HTML_TAG "impossible d'ouvrir la base de lignes de paye."  ENDL;
            exit(2004);
            return;
        }

    }
    
    if (! bulletins.is_open())
    {
        std::cerr << ERROR_HTML_TAG "impossible d'ouvrir la base de bulletins de paye."  ENDL;
        return;
    }

#ifdef GENERATE_RANK_SIGNAL
    uint64_t NCumLignes = 0;
    int progression = 0;
    unsigned int step = 0;

    for (int i = 0; i < Info[0].nbfil; ++i)
    {
        NCumLignes += Info[i].nbLigne;
    }

    reset_rank_signal();
    generate_rank_signal();

#endif


    if (type_base == BaseType::MONOLITHIQUE)
    {
        for (int i = 0; i < Info[0].nbfil; ++i)
        {
            for (uint32_t agent = 0; agent < Info[i].NCumAgentXml; ++agent)
            {
                unsigned l = BESOIN_MEMOIRE_ENTETE;
                uint16_t NLigneAgent = Info[i].NLigne[agent];

                ++compteur_lignes_bulletins;

                ecrire_ligne_bulletin(i, agent, t_bulletins, sep, Info, compteur_lignes_bulletins);

                char type[3]={0};
                strcpy(type, type_remuneration_traduit[0]);

                while (ligne < NLigneAgent)
                {

                    int      test_drapeau_categorie, int_drapeau_categorie = 0;

                    // teste si un drapeau de nouvelle catégorie de ligne de paye (T, I,...) a été introduit en base

                    while (VAR(l) &&  (test_drapeau_categorie = VAR(l)[0], test_drapeau_categorie <= nbType) && (test_drapeau_categorie >= 1))
                    {
                        int_drapeau_categorie = test_drapeau_categorie;

                        strcpy(type, type_remuneration_traduit[int_drapeau_categorie - 1]);
                        ++l;
                    }

                    ++compteur;
                    ecrire_ligne_table(i, agent, l, type, t_base, sep, Info, compteur);

                    l += INDEX_MAX_COLONNNES + 1;
                    ++ligne;
                }
                ligne = 0;

#ifdef GUI_TAG_MESSAGES
#ifdef GENERATE_RANK_SIGNAL
                progression = std::ceil((float) (compteur * 100) / (float) NCumLignes );

                ++step;

                if (step > Info[i].NCumAgentXml / 5)
                {

                    generate_rank_signal(progression);
                    std::cerr << " \n";
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
            for (int i = 0; i < Info[0].nbfil; ++i)
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

                        std::cerr << "Année : " << annee_courante <<  ENDL;
                        std::cerr << "Table de " << compteur - dernier_compteur
                                  << " lignes, lignes "  << dernier_compteur + 1
                                  << " à " << compteur << "."  ENDL ENDL;

                        dernier_compteur = compteur;
                        compteur_annee_courante = 0;
                        rang_fichier_base_annee_courante = 1;

                        annee_courante = (char*) VAR(Annee);
                        ouvrir_fichier_base(Info[i],  type_base, base);
                        if (! base.is_open()) return;
                    }

                    char type[3]={0};
                    strcpy(type, type_remuneration_traduit[0]);

                    while (ligne < NLigneAgent)
                    {
                        int      test_drapeau_categorie, int_drapeau_categorie = 0;

                        // teste si un drapeau de nouvelle catégorie de ligne de paye (T, I,...) a été introduit en base

                        while (VAR(l) &&  (test_drapeau_categorie = VAR(l)[0], test_drapeau_categorie <= nbType) && (test_drapeau_categorie >= 1))
                        {
                            int_drapeau_categorie = test_drapeau_categorie;

                            strcpy(type, type_remuneration_traduit[int_drapeau_categorie - 1]);
                            ++l;
                        }

                        ++compteur;
                        ecrire_ligne_table(i, agent, l, type, t_base, sep, Info, compteur);

                        l += INDEX_MAX_COLONNNES + 1;
                        ++ligne;
                    }
                    ligne = 0;

#ifdef GUI_TAG_MESSAGES
#ifdef GENERATE_RANK_SIGNAL
                    progression = std::ceil((float) (compteur * 100) / (float) NCumLignes );

                    ++step;

                    if (step > Info[i].NCumAgentXml / 5)
                    {

                        generate_rank_signal(progression);
                        std::cerr << " \n";
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
                for (int i = 0; i < Info[0].nbfil; ++i)
                {
                    for (uint32_t agent = 0; agent < Info[i].NCumAgentXml; ++agent)
                    {
                        unsigned l = BESOIN_MEMOIRE_ENTETE;
                        uint16_t NLigneAgent = Info[i].NLigne[agent];

                        ++compteur_lignes_bulletins;

                        ecrire_ligne_bulletin(i, agent, t_bulletins, sep, Info, compteur_lignes_bulletins);

                        char type[3]={0};
                        strcpy(type, type_remuneration_traduit[0]);

                        while (ligne < NLigneAgent)
                        {
                            if (compteur  == rang_fichier_base * taille_base)
                                // soit : il existe un nombre de lignes maximal par base spécifié en ligne de commande après -T

                            {

                                std::cerr << "Table n°" << rang_fichier_base << " de " << taille_base
                                          << " lignes, lignes "  << (rang_fichier_base - 1) * taille_base + 1
                                          << " à " << rang_fichier_base * taille_base << "."  ENDL;

#ifndef OFSTREAM_TABLE_OUTPUT      // Il faut écrire dans le fichier OFSTREAM la chaine de caractères temporaires
                                base << t_base.str();
                                t_base.str("");
#endif

                                base.close();

                                if (! base.good())
                                {
                                    std::cerr << ERROR_HTML_TAG "Problème fermeture fichier base"  ENDL;
                                    exit(-902);
                                }

                                ++rang_fichier_base;

                                if (rang_fichier_base >= 1000)
                                {
                                    std::cerr << ERROR_HTML_TAG "Ne peut générer que 999 bases au plus"  ENDL;
                                    exit(-904);
                                }

                                ouvrir_fichier_base(Info[i], type_base, base);
                                if (! base.is_open()) return;
                            }

                            int      test_drapeau_categorie, int_drapeau_categorie = 0;

                            // teste si un drapeau de nouvelle catégorie de ligne de paye (T, I,...) a été introduit en base

                            while (VAR(l) &&  (test_drapeau_categorie = VAR(l)[0], test_drapeau_categorie <= nbType) && (test_drapeau_categorie >= 1))
                            {
                                int_drapeau_categorie = test_drapeau_categorie;

                                strcpy(type, type_remuneration_traduit[int_drapeau_categorie - 1]);
                                ++l;
                            }


                            ++compteur;

                            ecrire_ligne_table(i, agent, l, type, t_base, sep, Info, compteur);

                            l += INDEX_MAX_COLONNNES + 1;
                            ++ligne;
                        }

                        ligne = 0;

#ifdef GUI_TAG_MESSAGES
#ifdef GENERATE_RANK_SIGNAL
                        progression = std::ceil((float) (compteur * 100) / (float) NCumLignes );

                        ++step;

                        if (step > Info[i].NCumAgentXml / 5)
                        {

                            generate_rank_signal(progression);
                            std::cerr << " \n";
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
                    for (int i = 0; i < Info[0].nbfil; ++i)
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

                                std::cerr << SPACER "Table n°" << rang_fichier_base << " de " <<  compteur - dernier_compteur - (rang_fichier_base_annee_courante - 1) * taille_base
                                          << " lignes, lignes "  << dernier_compteur + (rang_fichier_base_annee_courante - 1) * taille_base + 1
                                          << " à " << compteur << "."  ENDL;
                                std::cerr << "Année : " << annee_courante <<  ENDL;
                                std::cerr << "Total annuel de " << compteur - dernier_compteur
                                          << " lignes, lignes "  << dernier_compteur + 1
                                          << " à " << compteur << "."  ENDL ENDL;

                                dernier_compteur = compteur;
                                compteur_annee_courante = 0;
                                rang_fichier_base_annee_courante = 1;
                                ++rang_fichier_base;

                                annee_courante = (char*) VAR(Annee);
                                ouvrir_fichier_base(Info[i],  type_base, base);
                                if (! base.is_open()) return;
                            }

                            char type[3]={0};
                            strcpy(type, type_remuneration_traduit[0]);

                            while (ligne < NLigneAgent)
                            {
                                if (compteur_annee_courante  == rang_fichier_base_annee_courante * taille_base)
                                {

                                    std::cerr << SPACER "Table n°" << rang_fichier_base << " de " << taille_base
                                              << " lignes, lignes "  << compteur - taille_base + 1
                                              << " à " << compteur << "."  ENDL;

#ifndef OFSTREAM_TABLE_OUTPUT      // Il faut écrire dans le fichier OFSTREAM la chaine de caractères temporaires
                                    base << t_base.str();
                                    t_base.str("");
#endif

                                    base.close();

                                    if (! base.good())
                                    {
                                        std::cerr << ERROR_HTML_TAG "Problème fermeture fichier base"  ENDL;
                                        exit(-902);
                                    }

                                    ++rang_fichier_base;
                                    ++rang_fichier_base_annee_courante;

                                    if (rang_fichier_base >= 1000)
                                    {
                                        std::cerr << ERROR_HTML_TAG "Ne peut générer que 999 bases au plus"  ENDL;
                                        exit(-904);
                                    }

                                    ouvrir_fichier_base(Info[i], type_base, base);
                                    if (! base.is_open()) return;
                                }

                                int      test_drapeau_categorie, int_drapeau_categorie = 0;

                                // teste si un drapeau de nouvelle catégorie de ligne de paye (T, I,...) a été introduit en base

                                while (VAR(l) &&  (test_drapeau_categorie = VAR(l)[0], test_drapeau_categorie <= nbType) && (test_drapeau_categorie >= 1))
                                {
                                    int_drapeau_categorie = test_drapeau_categorie;

                                    strcpy(type, type_remuneration_traduit[int_drapeau_categorie - 1]);
                                    ++l;
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
                            progression = std::ceil((float) (compteur * 100) / (float) NCumLignes );

                            ++step;

                            if (step > Info[i].NCumAgentXml / 5)
                            {

                                generate_rank_signal(progression);
                                std::cerr << " \n";
                                step = 0;
                            }
#endif
#endif
                        }
                    }
                }
                else
                    if (type_base == BaseType::TOUTES_CATEGORIES)
                    {
                        for (int i = 0; i < Info[0].nbfil; ++i)
                        {
                            for (uint32_t agent = 0; agent < Info[i].NCumAgentXml; ++agent)
                            {
                                unsigned l = BESOIN_MEMOIRE_ENTETE;
                                uint16_t NLigneAgent = Info[i].NLigne[agent];

                                ++compteur_lignes_bulletins;

                                ecrire_ligne_bulletin(i, agent, t_bulletins, sep, Info, compteur_lignes_bulletins);


                                char type[3]={0};
                                strcpy(type, type_remuneration_traduit[0]);

                                while (ligne < NLigneAgent)
                                {
                                    bool nouveau_type = false;

                                    int      test_drapeau_categorie, int_drapeau_categorie = 0;

                                    // teste si un drapeau de nouvelle catégorie de ligne de paye (T, I,...) a été introduit en base

                                    while (VAR(l) &&  (test_drapeau_categorie = VAR(l)[0], test_drapeau_categorie <= nbType) && (test_drapeau_categorie >= 1))
                                    {
                                        int_drapeau_categorie = test_drapeau_categorie;

                                        strcpy(type, type_remuneration_traduit[int_drapeau_categorie - 1]);
                                        nouveau_type = true;
                                        ++l;
                                    }

                                    ++compteur;
                                    if (nouveau_type)
                                    {
                                        ecrire_ligne_table(i, agent, l, type, t_tableau_base[int_drapeau_categorie - 1], sep, Info, compteur);
                                    }

                                    l += INDEX_MAX_COLONNNES + 1;
                                    ++ligne;
                                }
                                ligne = 0;

#ifdef GUI_TAG_MESSAGES
#ifdef GENERATE_RANK_SIGNAL
                                progression = std::ceil((float) (compteur * 100) / (float) NCumLignes );

                                ++step;

                                if (step > Info[i].NCumAgentXml / 5)
                                {

                                    generate_rank_signal(progression);
                                    std::cerr << " \n";
                                    step = 0;
                                }
#endif
#endif
                            }

                        }
                    }
                    else  //par catégorie spécifique
                    {
                        for (int i = 0; i < Info[0].nbfil; ++i)
                        {
                            for (uint32_t agent = 0; agent < Info[i].NCumAgentXml; ++agent)
                            {
                                unsigned l = BESOIN_MEMOIRE_ENTETE;
                                uint16_t NLigneAgent = Info[i].NLigne[agent];

                                ++compteur_lignes_bulletins;

                                ecrire_ligne_bulletin(i, agent, t_bulletins, sep, Info, compteur_lignes_bulletins);

                                char type[3]={0};
                                strcpy(type, type_remuneration_traduit[0]);

                                while (ligne < NLigneAgent)
                                {
                                    BaseType valeur_drapeau_categorie = BaseType::MONOLITHIQUE;
                                    int      test_drapeau_categorie, int_drapeau_categorie = 0;

                                    // teste si un drapeau de nouvelle catégorie de ligne de paye (T, I,...) a été introduit en base

                                    while (VAR(l) &&  (test_drapeau_categorie = VAR(l)[0], test_drapeau_categorie <= nbType) && (test_drapeau_categorie >= 1))
                                    {
                                        valeur_drapeau_categorie = static_cast<BaseType>(test_drapeau_categorie);
                                        int_drapeau_categorie = test_drapeau_categorie;

                                        strcpy(type, type_remuneration_traduit[int_drapeau_categorie - 1]);
                                        ++l;
                                    }


                                    if (valeur_drapeau_categorie  == Info[0].type_base)
                                    {
                                        ++compteur;
                                        ecrire_ligne_table(i, agent, l, type, t_base, sep, Info, compteur);
                                    }

                                    l += INDEX_MAX_COLONNNES + 1;
                                    ++ligne;
                                }


                                ligne = 0;

#ifdef GUI_TAG_MESSAGES
#ifdef GENERATE_RANK_SIGNAL
                                progression = std::ceil((float) (compteur * 100) / (float) NCumLignes );

                                ++step;

                                if (step > Info[i].NCumAgentXml / 5)
                                {

                                    generate_rank_signal(progression);
                                    std::cerr << " \n";
                                    step = 0;
                                }
#endif
#endif
                            }
                        }

                    }


    
#ifndef OFSTREAM_TABLE_OUTPUT
    bulletins << t_bulletins.str();
#endif

    // Dans les autres cas, les bases ont déjà été refermées sauf une (cas par année et par taille maximale)
    if (type_base == BaseType::TOUTES_CATEGORIES)
    {
        for (int d = 0; d < nbType - 1; ++d)
        {
#ifndef OFSTREAM_TABLE_OUTPUT
            tableau_base[d] << t_tableau_base[d].str();
#endif
            tableau_base[d].close();
        }
    }
    else
        if (base.is_open())
        {
#ifndef OFSTREAM_TABLE_OUTPUT      // Il faut écrire dans le fichier OFSTREAM la chaine de caractères temporaires
            base << t_base.str();
            //  t_base.str("");  (devenu inutile)
#endif

            base.close();
        }

    if (base.good())
    {
        base.close();
        switch (type_base)
        {
        case  BaseType::MONOLITHIQUE            :
            break;

        case  BaseType::PAR_TRAITEMENT          :
            std::cerr << STATE_HTML_TAG "Catégorie : Traitement."  ENDL;
            break;

        case  BaseType::PAR_INDEMNITE_RESIDENCE :
            std::cerr << STATE_HTML_TAG "Catégorie : Indemnité de résidence."  ENDL;
            break;

        case  BaseType::PAR_SFT                 :
            std::cerr << STATE_HTML_TAG "Catégorie : Supplément familial de traitement."  ENDL;
            break;

        case  BaseType::PAR_AVANTAGE_NATURE     :
            std::cerr << STATE_HTML_TAG "Catégorie : Avantage en nature."  ENDL;
            break;

        case  BaseType::PAR_INDEMNITE           :
            std::cerr << STATE_HTML_TAG "Catégorie : Indemnité."  ENDL;
            break;

        case  BaseType::PAR_REM_DIVERSES        :
            std::cerr << STATE_HTML_TAG "Catégorie : Rémunérations diverses."  ENDL;
            break;

        case  BaseType::PAR_DEDUCTION           :
            std::cerr << STATE_HTML_TAG "Catégorie : Déduction."  ENDL;
            break;

        case  BaseType::PAR_ACOMPTE             :
            std::cerr << STATE_HTML_TAG "Catégorie : Acompte."  ENDL;
            break;

        case  BaseType::PAR_RAPPEL              :
            std::cerr << STATE_HTML_TAG "Catégorie : Rappel."  ENDL;
            break;

        case  BaseType::PAR_RETENUE             :
            std::cerr << STATE_HTML_TAG "Catégorie : Retenue."  ENDL;
            break;

        case  BaseType::PAR_COTISATION          :
            std::cerr << STATE_HTML_TAG "Catégorie : Cotisation."  ENDL;
            break;

        case  BaseType::TOUTES_CATEGORIES       :
            std::cerr << STATE_HTML_TAG "Toutes catégories."  ENDL;
            break;

        case BaseType::PAR_ANNEE    :
            std::cerr << "Année : " << annee_courante  << ENDL;
            std::cerr << "Table de " << compteur - dernier_compteur
                      << " lignes, lignes "  << dernier_compteur + 1
                      << " à " << compteur << "."  ENDL;
            break;

        case BaseType::MAXIMUM_LIGNES_PAR_ANNEE :
            std::cerr << SPACER "Table n°" << rang_fichier_base << " de " <<  compteur - dernier_compteur - (rang_fichier_base_annee_courante - 1) * taille_base
                      << " lignes, lignes "  << dernier_compteur + (rang_fichier_base_annee_courante - 1) * taille_base + 1
                      << " à " << compteur << "."  ENDL;
            std::cerr << "Année : " << annee_courante <<  ENDL;
            std::cerr << "Total annuel de " << compteur - dernier_compteur
                      << " lignes, lignes "  << dernier_compteur + 1
                      << " à " << compteur << "."  ENDL;
            break;

        case BaseType::MAXIMUM_LIGNES  :  /* Taille définie par l'utilisateur */
            std::cerr << STATE_HTML_TAG "Table n°" << rang_fichier_base
                      << " de " <<  compteur - (rang_fichier_base-1) * taille_base
                      << " lignes, lignes " << (rang_fichier_base-1) * taille_base + 1
                      << " à " << compteur << "."  ENDL;
            break;

        default:  break;
        }

        std::cerr << ENDL;

        std::cerr << STATE_HTML_TAG "Nombre total de lignes de paye : " << compteur << " lignes."  ENDL;

#if defined(__WIN32__) && defined(USE_ICONV)
        convertir(Info[0].chemin_base);
#endif
    }

    if (bulletins.good())
    {
        bulletins.close();

        std::cerr << STATE_HTML_TAG "Base des bulletins de paye de " << compteur_lignes_bulletins << " lignes."  ENDL;

#if defined(__WIN32__) && defined(USE_ICONV)
        convertir(Info[0].chemin_bulletins);
#endif
    }
    
}

#undef VAR

