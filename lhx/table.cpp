

#include <inttypes.h>
#include <cstring>
#include <array>
#include "table.hpp"
#include "fonctions_auxiliaires.hpp"


#include <iostream>
#include <fstream>

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
           << VAR(l + INDEX_MAX_CONNNES) << sep     // Il doit y avoir au plus INDEX_MAX_COLONNES + 1 types de colonnes de lignes de paye différents
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
              << VAR(Etablissement) << sep
              << VAR(Siret) << sep;
    }
    
    ECRIRE_LIGNE_l_COMMUN(i, agent, l, type, base, sep, Info, rang);
}

static inline void GCC_INLINE ECRIRE_LIGNE_l_SIRET(int i, uint32_t agent, int l, char* type, std::ofstream& base, char sep, std::vector<info_t> &Info, int GCC_UNUSED rang)
{
    base  << VAR(Annee) << sep
          << VAR(Mois) << sep;
    
    base  << VAR(Budget) << sep
          << VAR(Etablissement) << sep
          << VAR(Siret) << sep;
    
    ECRIRE_LIGNE_l_COMMUN(i, agent, l, type, base, sep, Info, rang);
    
}

static inline void GCC_INLINE ECRIRE_LIGNE_l(int i, uint32_t agent, int l, char* type, std::ofstream& base, char sep, std::vector<info_t> &Info, int GCC_UNUSED rang)
{
    base  << VAR(Annee) << sep
          << VAR(Mois) << sep;
    
    ECRIRE_LIGNE_l_COMMUN(i, agent, l, type, base, sep, Info, rang);
}

static inline void GCC_INLINE ECRIRE_LIGNEBULLETIN_OBLIGATOIRE_NUMERIQUEOPTIONNEL_COMMUN(int i, uint32_t agent, std::ofstream& bulletins, char sep, std::vector<info_t> &Info, int GCC_UNUSED rang)
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

static inline void GCC_INLINE  ECRIRE_LIGNEBULLETIN_OBLIGATOIRE_NUMERIQUEOPTIONNEL_GENERER_RANG(int i, uint32_t agent, std::ofstream& bulletins, char sep, std::vector<info_t> &Info, int rang)
{
    bulletins <<  rang << sep;
    
    bulletins << VAR(Annee) << sep
              << VAR(Mois) << sep;
    
    if (Info[0].select_siret)
    {
        bulletins  << VAR(Budget) << sep
                   << VAR(Etablissement) << sep
                   << VAR(Siret) << sep;
    }
    
    ECRIRE_LIGNEBULLETIN_OBLIGATOIRE_NUMERIQUEOPTIONNEL_COMMUN(i, agent, bulletins, sep, Info, rang);
}

static inline void GCC_INLINE  ECRIRE_LIGNEBULLETIN_OBLIGATOIRE_NUMERIQUEOPTIONNEL_SIRET(int i, uint32_t agent, std::ofstream& bulletins, char sep, std::vector<info_t> &Info, int GCC_UNUSED rang)
{
    
    bulletins << VAR(Annee) << sep
              << VAR(Mois) << sep;
    
    bulletins  << VAR(Budget) << sep
               << VAR(Etablissement) << sep
               << VAR(Siret) << sep;

    ECRIRE_LIGNEBULLETIN_OBLIGATOIRE_NUMERIQUEOPTIONNEL_COMMUN(i, agent, bulletins, sep, Info, rang);
}

static inline void GCC_INLINE  ECRIRE_LIGNE_BULLETINS(int i, uint32_t agent, std::ofstream& bulletins, char sep, std::vector<info_t> &Info, int GCC_UNUSED rang)
{
    
    bulletins << VAR(Annee) << sep
              << VAR(Mois) << sep;
    
    ECRIRE_LIGNEBULLETIN_OBLIGATOIRE_NUMERIQUEOPTIONNEL_COMMUN(i, agent, bulletins, sep, Info, rang);
}


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

    int32_t taille_base = Info[0].taille_base;

    switch (taille_base)
    {
    
    case MONOLITHIQUE :
        ouvrir_fichier_base(Info[0], 0, base);
        break;
        
    case PAR_ANNEE :
        ouvrir_fichier_base(Info[0], atoi(annee_courante) + nbType + 1, base);
        break;
        
    case PAR_TRAITEMENT          :
    case PAR_INDEMNITE_RESIDENCE :
    case PAR_SFT                 :
    case PAR_AVANTAGE_NATURE     :
    case PAR_INDEMNITE           :
    case PAR_REM_DIVERSES        :
    case PAR_DEDUCTION           :
    case PAR_ACOMPTE             :
    case PAR_RAPPEL              :
    case PAR_RETENUE             :
    case PAR_COTISATION          :
        ouvrir_fichier_base(Info[0], - taille_base - 2, base);
        break;
        
    case TOUTES_CATEGORIES  :
          for (int d = 0; d < nbType; ++d)
            ouvrir_fichier_base(Info[0], d + 1, fichier_base[d]);
        break;
        
    default : ouvrir_fichier_base(Info[0], rang_fichier_base + nbType + 1, base);
        // cas où une vraie taille de base en lignes est entrée.
        
    }
    
    if (! base.is_open())
    {
       std::cerr << "Erreur : impossible d'ouvrir la base de lignes de paye.\n";
       return;
    }
    if (! bulletins.is_open())
    {
       std::cerr << "Erreur : impossible d'ouvrir la base de bulletins de paye.\n";
       return;
    }

    static void (*ecrire_ligne_table)(int, uint32_t, int, char*, std::ofstream&, char, std::vector<info_t> &, int);
    static void (*ecrire_ligne_bulletin)(int i, uint32_t agent, std::ofstream& bulletins, char sep, std::vector<info_t> &Info, int rang);
    
    if (Info[0].generer_rang)
    {
        ecrire_ligne_table = ECRIRE_LIGNE_l_GENERER_RANG;
        ecrire_ligne_bulletin = ECRIRE_LIGNEBULLETIN_OBLIGATOIRE_NUMERIQUEOPTIONNEL_GENERER_RANG;
    }
    else
        if (Info[0].select_siret)
        {
            ecrire_ligne_table = ECRIRE_LIGNE_l_SIRET;
            ecrire_ligne_bulletin = ECRIRE_LIGNEBULLETIN_OBLIGATOIRE_NUMERIQUEOPTIONNEL_SIRET;
        }
        else
        {
            ecrire_ligne_table = ECRIRE_LIGNE_l;
            ecrire_ligne_bulletin = ECRIRE_LIGNE_BULLETINS;
        }


    for (int i = 0; i < Info[0].nbfil; ++i)
    {

        for (uint32_t agent = 0; agent < Info[i].NCumAgentXml; ++agent)
        {

            unsigned l = Info[i].minimum_memoire_p_ligne;
            uint16_t NLigneAgent = Info[i].NLigne[agent];

            ++compteur_lignes_bulletins;
            
            ecrire_ligne_bulletin(i, agent, bulletins, sep, Info, compteur_lignes_bulletins);
            
            if (taille_base == PAR_ANNEE  && strcmp((const char*)VAR(Annee), annee_courante))
            {
                base.close();
                
                std::cerr << "Année : " << annee_courante << " Table générée.\n";
                annee_courante = (char*) VAR(Annee);
                ouvrir_fichier_base(Info[i], atoi(annee_courante) + nbType + 1, base);
                if (! base.is_open()) return;
            }

            char type[3]={0};
            strcpy(type, type_remuneration_traduit[0]);
            
            unsigned allocation_memoire = (l + nbType + NLigneAgent * (INDEX_MAX_CONNNES + 1)) * sizeof(xmlChar*);
            
            while (ligne < NLigneAgent)
            {
                bool nouveau_type = false;
                
                if (taille_base > MONOLITHIQUE   // soit : il existe un nombre de lignes maximal par base
                        && (compteur  == rang_fichier_base * taille_base))
                {
                    std::cerr << "Table n°" << rang_fichier_base << " de " << taille_base
                              << "lignes générée, lignes "  << (rang_fichier_base - 1) * taille_base + 1
                              << " à " << rang_fichier_base * taille_base << " .\n";
                    
                    base.close();
                    if (! base.good())
                    {
                        perror("Erreur : Problème fermeture fichier base");
                        exit(-902);
                    }
                    
                    ++rang_fichier_base;
                    
                    if (rang_fichier_base >= 1000)
                    {
                        std::cerr << "Erreur : Ne peut générer que 999 bases au plus\n";
                        exit(-904);
                    }
                    
                    ouvrir_fichier_base(Info[i], rang_fichier_base + nbType + 1, base);
                    if (! base.is_open()) return;
                }
                
                if (l + INDEX_MAX_CONNNES + 1 == allocation_memoire)
                {
                    std::cerr << "Erreur : Max lignes de paye atteint (" << allocation_memoire << ") ! \n";
                    exit(-1002);
                }
                
                int valeur_drapeau_categorie = 0, test_drapeau_categorie = 0;
                
                while (VAR(l) && (test_drapeau_categorie = VAR(l)[0]) >= 1 && (test_drapeau_categorie <= nbType))
                {
                    valeur_drapeau_categorie = test_drapeau_categorie;
                    strcpy(type, type_remuneration_traduit[valeur_drapeau_categorie - 1]);
                    nouveau_type = true;
                    ++l;
                }
                
                if (taille_base > PAR_TRAITEMENT)
                {
                    ++compteur;
                    ecrire_ligne_table(i, agent, l, type, base, sep, Info, compteur);
                }
                else
                {
                    if (taille_base != TOUTES_CATEGORIES)
                    {
                        if (valeur_drapeau_categorie + 2 == -Info[0].taille_base)
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
                
                l += INDEX_MAX_CONNNES + 1;
                ++ligne;
            }
            
            ligne = 0;
        }
        
        if (i) Info[0].nbLigne += Info[i].nbLigne;
    }
    
    // Dans les autres cas, les bases ont déjà été refermées sauf une (cas par année et par taille maximale)
    if (taille_base == TOUTES_CATEGORIES)
        for (int d = 0; d < nbType - 1; ++d)
        {
            fichier_base[d].close();
        }
    
//    if (base.good())
//    {
        base.close();
        switch (taille_base)
        {
        case  MONOLITHIQUE            :
            goto message;
        case  PAR_TRAITEMENT          :
            std::cout << "[MSG] Catégorie : Traitement.\n";
            goto message;
        case  PAR_INDEMNITE_RESIDENCE :
            std::cout << "[MSG] Catégorie : Indemnité de résidence.\n";
            goto message;
        case  PAR_SFT                 :
            std::cout << "[MSG] Catégorie : Supplément familial de traitement.\n";
            goto message;
        case  PAR_AVANTAGE_NATURE     :
            std::cout << "[MSG] Catégorie : Avantage en nature.\n";
            goto message;
        case  PAR_INDEMNITE           :
            std::cout << "[MSG] Catégorie : Indemnité.\n";
            goto message;
        case  PAR_REM_DIVERSES        :
            std::cout << "[MSG] Catégorie : Rémunérations diverses.\n";
            goto message;
        case  PAR_DEDUCTION           :
            std::cout << "[MSG] Catégorie : Déduction.\n";
            goto message;
        case  PAR_ACOMPTE             :
            std::cout << "[MSG] Catégorie : Acompte.\n";
            goto message;
        case  PAR_RAPPEL              :
            std::cout << "[MSG] Catégorie : Rappel.\n";
            goto message;
        case  PAR_RETENUE             :
            std::cout << "[MSG] Catégorie : Retenue.\n";
            goto message;
        case  PAR_COTISATION          :
            std::cout << "[MSG] Catégorie : Cotisation.\n";
            goto message;
        case  TOUTES_CATEGORIES       :
            std::cout << "[MSG] Toutes catégories.\n";
            std::cout << "[MSG] Total de " << compteur << " lignes générée dans 11 bases.\n";
            break;
            
        case PAR_ANNEE    :
            std::cout << "Année : " << annee_courante << " Table générée.\n";
            break;
        default :  /* Taille définie par l'utilisateur */
            std::cout << "[MSG] Table n°" << rang_fichier_base
                      << " de " <<  compteur - (rang_fichier_base-1) * taille_base
                      << " lignes, lignes " << (rang_fichier_base-1) * taille_base + 1
                      << " à " << compteur << ".\n";
        }
        
        return;
        
message :
        std::cerr << "[MSG] Table de " << compteur << " lignes.\n";
    //}
    
    if (bulletins.good())
    {
        bulletins.close();
        std::cerr << "[MSG] Base des bulletins de paye de " << compteur_lignes_bulletins << " lignes.\n";
    }
    
}

#undef VAR

