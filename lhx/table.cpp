

#include <inttypes.h>
#include <cstring>
#include <array>
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

static inline void GCC_INLINE ECRIRE_LIGNE_l_COMMUN(int i, uint32_t agent, int l, char* type, std::ofstream& base, char sep, std::vector<info_t> &Info, int GCC_UNUSED rang)
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
           << VAR(Grade) << sep
           << VAR(NIR) << "\n";
}

static inline void GCC_INLINE ECRIRE_LIGNE_l_GENERER_RANG(int i, uint32_t agent, int l, char* type, std::ofstream& base, char sep, std::vector<info_t> &Info, int rang)
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
}

static inline void GCC_INLINE ECRIRE_LIGNE_l_SIRET(int i, uint32_t agent, int l, char* type, std::ofstream& base, char sep, std::vector<info_t> &Info, int GCC_UNUSED rang)
{
    base  << VAR(Annee) << sep
          << VAR(Mois) << sep;
    
    base  << VAR(Budget) << sep
          << VAR(Employeur) << sep
          << VAR(Siret) << sep
          << VAR(Etablissement) << sep;

    ECRIRE_LIGNE_l_COMMUN(i, agent, l, type, base, sep, Info, rang);
    
}

static inline void GCC_INLINE ECRIRE_LIGNE_l(int i, uint32_t agent, int l, char* type, std::ofstream& base, char sep, std::vector<info_t> &Info, int GCC_UNUSED rang)
{
    base  << VAR(Annee) << sep
          << VAR(Mois) << sep;
    
    ECRIRE_LIGNE_l_COMMUN(i, agent, l, type, base, sep, Info, rang);
}

static inline void GCC_INLINE ECRIRE_LIGNE_BULLETIN_COMMUN(int i, uint32_t agent, std::ofstream& bulletins, char sep, std::vector<info_t> &Info, int GCC_UNUSED rang)
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
              << VAR(Grade) << sep
              << VAR(NIR) << "\n";
}

static inline void GCC_INLINE  ECRIRE_LIGNE_BULLETIN_GENERER_RANG(int i, uint32_t agent, std::ofstream& bulletins, char sep, std::vector<info_t> &Info, int rang)
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
}

static inline void GCC_INLINE  ECRIRE_LIGNE_BULLETIN_SIRET(int i, uint32_t agent, std::ofstream& bulletins, char sep, std::vector<info_t> &Info, int GCC_UNUSED rang)
{
    
    bulletins << VAR(Annee) << sep
              << VAR(Mois) << sep;
    
    bulletins  << VAR(Budget) << sep
               << VAR(Employeur) << sep
               << VAR(Siret) << sep
               << VAR(Etablissement) << sep;

    ECRIRE_LIGNE_BULLETIN_COMMUN(i, agent, bulletins, sep, Info, rang);
}

static inline void GCC_INLINE  ECRIRE_LIGNE_BULLETINS(int i, uint32_t agent, std::ofstream& bulletins, char sep, std::vector<info_t> &Info, int GCC_UNUSED rang)
{
    
    bulletins << VAR(Annee) << sep
              << VAR(Mois) << sep;
    
    ECRIRE_LIGNE_BULLETIN_COMMUN(i, agent, bulletins, sep, Info, rang);
}

static void (*ecrire_ligne_table)(int, uint32_t, int, char*, std::ofstream&, char, std::vector<info_t> &, int);
static void (*ecrire_ligne_bulletin)(int i, uint32_t, std::ofstream& , char, std::vector<info_t>& , int );

void boucle_ecriture(std::vector<info_t>& Info)
{
    int ligne = 0;
    uint64_t compteur = 0;
    uint32_t compteur_lignes_bulletins = 0;
    char sep = Info[0].separateur;
    char* annee_courante = (char*) Info[0].Table[0][Annee];
    unsigned rang_fichier_base = 1;
    static std::ofstream base;
    static std::ofstream bulletins;
    static std::array<std::ofstream, nbType> fichier_base;
    
    ouvrir_fichier_bulletins(Info[0], bulletins);

    uint32_t taille_base = Info[0].taille_base;
    BaseType type_base = Info[0].type_base;

    if (Info[0].generer_rang)
    {
        ecrire_ligne_table = ECRIRE_LIGNE_l_GENERER_RANG;
        ecrire_ligne_bulletin = ECRIRE_LIGNE_BULLETIN_GENERER_RANG;
    }
    else
        if (Info[0].select_siret)
        {
            ecrire_ligne_table = ECRIRE_LIGNE_l_SIRET;
            ecrire_ligne_bulletin = ECRIRE_LIGNE_BULLETIN_SIRET;
        }
        else
        {
            ecrire_ligne_table = ECRIRE_LIGNE_l;
            ecrire_ligne_bulletin = ECRIRE_LIGNE_BULLETINS;
        }

    switch (type_base)
    {
    case BaseType::TOUTES_CATEGORIES  :
        for (int d = 0; d < nbType; ++d)
        {
            ouvrir_fichier_base(Info[0], type_base, fichier_base[d]);
            if (! fichier_base[d].is_open())
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

    for (int i = 0; i < Info[0].nbfil; ++i)
    {
        for (uint32_t agent = 0; agent < Info[i].NCumAgentXml; ++agent)
        {
            unsigned l = BESOIN_MEMOIRE_ENTETE;
            uint16_t NLigneAgent = Info[i].NLigne[agent];

            ++compteur_lignes_bulletins;
            
            ecrire_ligne_bulletin(i, agent, bulletins, sep, Info, compteur_lignes_bulletins);
            
            if (type_base == BaseType::PAR_ANNEE
                    && strcmp((const char*)VAR(Annee), annee_courante))
            {
                base.close();
                
                std::cerr << "Année : " << annee_courante << " Table générée."  ENDL;
                annee_courante = (char*) VAR(Annee);
                ouvrir_fichier_base(Info[i],  type_base, base);
                if (! base.is_open()) return;
            }

            char type[3]={0};
            strcpy(type, type_remuneration_traduit[0]);

            while (ligne < NLigneAgent)
            {
                bool nouveau_type = false;
                
                if (taille_base > 0   // soit : il existe un nombre de lignes maximal par base sépcifié en ligne de commande après -T
                        && (compteur  == rang_fichier_base * taille_base))
                {
                    std::cerr << "Table n°" << rang_fichier_base << " de " << taille_base
                              << "lignes générée, lignes "  << (rang_fichier_base - 1) * taille_base + 1
                              << " à " << rang_fichier_base * taille_base << " ."  ENDL;
                    
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

                int valeur_drapeau_categorie = 0, test_drapeau_categorie = 0;
                
                while (VAR(l) && (test_drapeau_categorie = VAR(l)[0]) >= 1 && (test_drapeau_categorie <= nbType))
                {
                    valeur_drapeau_categorie = test_drapeau_categorie;
                    strcpy(type, type_remuneration_traduit[valeur_drapeau_categorie - 1]);
                    nouveau_type = true;
                    ++l;
                }
                
                if (type_base == BaseType::PAR_ANNEE || type_base == BaseType::MONOLITHIQUE)
                {
                    ++compteur;
                    ecrire_ligne_table(i, agent, l, type, base, sep, Info, compteur);
                }
                else
                {
                    if (type_base != BaseType::TOUTES_CATEGORIES)
                    {
                        if (valeur_drapeau_categorie + 2 == - static_cast<int>(Info[0].type_base))
                        {
                            ++compteur;
                            ecrire_ligne_table(i, agent, l, type, base, sep, Info, compteur);
                        }
                    }
                    else
                    {
                        ++compteur;
                        if (nouveau_type)
                        {
                            ecrire_ligne_table(i, agent, l, type, fichier_base[valeur_drapeau_categorie - 1], sep, Info, compteur);
                        }
                        
                    }
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
    
    // Dans les autres cas, les bases ont déjà été refermées sauf une (cas par année et par taille maximale)
    if (type_base == BaseType::TOUTES_CATEGORIES)
        for (int d = 0; d < nbType - 1; ++d)
        {
            fichier_base[d].close();
        }
    
   if (base.good())
    {
        base.close();
        switch (type_base)
        {
        case  BaseType::MONOLITHIQUE            :
            goto message;
        case  BaseType::PAR_TRAITEMENT          :
            std::cerr << STATE_HTML_TAG "Catégorie : Traitement."  ENDL;
            goto message;
        case  BaseType::PAR_INDEMNITE_RESIDENCE :
            std::cerr << STATE_HTML_TAG "Catégorie : Indemnité de résidence."  ENDL;
            goto message;
        case  BaseType::PAR_SFT                 :
            std::cerr << STATE_HTML_TAG "Catégorie : Supplément familial de traitement."  ENDL;
            goto message;
        case  BaseType::PAR_AVANTAGE_NATURE     :
            std::cerr << STATE_HTML_TAG "Catégorie : Avantage en nature."  ENDL;
            goto message;
        case  BaseType::PAR_INDEMNITE           :
            std::cerr << STATE_HTML_TAG "Catégorie : Indemnité."  ENDL;
            goto message;
        case  BaseType::PAR_REM_DIVERSES        :
            std::cerr << STATE_HTML_TAG "Catégorie : Rémunérations diverses."  ENDL;
            goto message;
        case  BaseType::PAR_DEDUCTION           :
            std::cerr << STATE_HTML_TAG "Catégorie : Déduction."  ENDL;
            goto message;
        case  BaseType::PAR_ACOMPTE             :
            std::cerr << STATE_HTML_TAG "Catégorie : Acompte."  ENDL;
            goto message;
        case  BaseType::PAR_RAPPEL              :
            std::cerr << STATE_HTML_TAG "Catégorie : Rappel."  ENDL;
            goto message;
        case  BaseType::PAR_RETENUE             :
            std::cerr << STATE_HTML_TAG "Catégorie : Retenue."  ENDL;
            goto message;
        case  BaseType::PAR_COTISATION          :
            std::cerr << STATE_HTML_TAG "Catégorie : Cotisation."  ENDL;
            goto message;
        case  BaseType::TOUTES_CATEGORIES       :
            std::cerr << STATE_HTML_TAG "Toutes catégories."  ENDL;
            std::cerr << STATE_HTML_TAG "Total de " << compteur << " lignes générée dans 11 bases."  ENDL;
            break;

        case BaseType::PAR_ANNEE    :
            std::cerr << "Année : " << annee_courante << " Table générée."  ENDL;
            break;
        default :  /* Taille définie par l'utilisateur */
            std::cerr << STATE_HTML_TAG "Table n°" << rang_fichier_base
                      << " de " <<  compteur - (rang_fichier_base-1) * taille_base
                      << " lignes, lignes " << (rang_fichier_base-1) * taille_base + 1
                      << " à " << compteur << "."  ENDL;
        }
        
        return;
        
message :
        std::cerr << STATE_HTML_TAG "Table de " << compteur << " lignes."  ENDL;
    }
    
    if (bulletins.good())
    {
        bulletins.close();
        std::cerr << STATE_HTML_TAG "Base des bulletins de paye de " << compteur_lignes_bulletins << " lignes."  ENDL;
    }
    
}

#undef VAR

