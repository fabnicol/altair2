//#ifdef __cplusplus
//extern "C" {
//#endif // __cplusplus

#include "table.hpp"
#include "fonctions_auxiliaires.hpp"
#include <inttypes.h>



int64_t generer_table_standard(const char* chemin_table, info_t* info)
{

return 0;

}

/* Il doit y avoir BESOIN_MEMOIRE_ENTETE + 6 champs plus Type, soit 18 + 6 +1 = 23 champs et 24 séparateurs + saut de ligne = 48 char + \0*/



#define VAR(X) Info[i].Table[agent][X]

static inline void  ECRIRE_LIGNE_l(int i, uint32_t agent, int l, char* type, FILE* base, char* format_base, char sep, info_t* Info, int rang)
{
if (Info[0].generer_rang)
    fprintf(base, format_base,
                            rang, sep,
                            VAR(Annee), sep,
                            VAR(Mois), sep,
                            VAR(Nom), sep,
                            VAR(Prenom), sep,
                            VAR(Matricule), sep,
                            VAR(Service), sep,
		                    VAR(NbEnfants), sep,
                            VAR(Statut), sep,
                            VAR(QuotiteTrav), sep,
                            VAR(NbHeureSup), sep,
                            VAR(NbHeureTotal), sep,
                            VAR(Indice), sep,
                            VAR(MtBrut), sep,
                            VAR(MtNet), sep,
                            VAR(MtNetAPayer), sep,
                            VAR(NBI), sep,
                            VAR(l), sep,
                            VAR(l+1), sep,
                            VAR(l+2), sep,
                            VAR(l+3), sep,
                            VAR(l+4), sep,
                            VAR(l+5), sep,
                            type, sep,
                            VAR(EmploiMetier), sep,
                            VAR(Grade), sep,
                            VAR(NIR));
else
    fprintf(base, format_base,
                            VAR(Annee), sep,
                            VAR(Mois), sep,
                            VAR(Nom), sep,
                            VAR(Prenom), sep,
                            VAR(Matricule), sep,
                            VAR(Service), sep,
		                    VAR(NbEnfants), sep,
                            VAR(Statut), sep,
                            VAR(QuotiteTrav), sep,
                            VAR(NbHeureSup), sep,
                            VAR(NbHeureTotal), sep,
                            VAR(Indice), sep,
                            VAR(MtBrut), sep,
                            VAR(MtNet), sep,
                            VAR(MtNetAPayer), sep,
                            VAR(NBI), sep,
                            VAR(l), sep,
                            VAR(l+1), sep,
                            VAR(l+2), sep,
                            VAR(l+3), sep,
                            VAR(l+4), sep,
                            VAR(l+5), sep,
                            type, sep,
                            VAR(EmploiMetier), sep,
                            VAR(Grade), sep,
                            VAR(NIR));

}


static inline void  ECRIRE_LIGNE_BULLETIN(int i, uint32_t agent, FILE* bulletins, char* format_bulletins, char sep, info_t* Info, int rang)
{
if (Info[0].generer_rang)
 fprintf(bulletins, format_bulletins,
                            rang, sep,
                            VAR(Annee), sep,
                            VAR(Mois), sep,
                            VAR(Nom), sep,
                            VAR(Prenom), sep,
                            VAR(Matricule), sep,
                            VAR(Service), sep,
   		                    VAR(NbEnfants), sep,
                            VAR(Statut), sep,
                            VAR(QuotiteTrav), sep,
                            VAR(NbHeureSup), sep,
                            VAR(NbHeureTotal), sep,
                            VAR(Indice), sep,
                            VAR(MtBrut), sep,
                            VAR(MtNet), sep,
                            VAR(MtNetAPayer), sep,
                            VAR(NBI), sep,
                            VAR(EmploiMetier), sep,
                            VAR(Grade), sep,
                            VAR(NIR));
else
  fprintf(bulletins, format_bulletins,
                            VAR(Annee), sep,
                            VAR(Mois), sep,
                            VAR(Nom), sep,
                            VAR(Prenom), sep,
                            VAR(Matricule), sep,
                            VAR(Service), sep,
   		                    VAR(NbEnfants), sep,
                            VAR(Statut), sep,
                            VAR(QuotiteTrav), sep,
                            VAR(NbHeureSup), sep,
                            VAR(NbHeureTotal), sep,
                            VAR(Indice), sep,
                            VAR(MtBrut), sep,
                            VAR(MtNet), sep,
                            VAR(MtNetAPayer), sep,
                            VAR(NBI), sep,
                            VAR(EmploiMetier), sep,
                            VAR(Grade), sep,
                            VAR(NIR));

}

void boucle_ecriture(info_t* Info)
{
    int ligne = 0;
    uint64_t compteur = 0;
    uint32_t compteur_lignes_bulletins = 0;
    int TAILLE_FORMAT = (Info[0].minimum_memoire_p_ligne + 6 + 1) * 4 + ((Info[0].generer_rang)? 4 : 0);
    int TAILLE_FORMAT_BULLETINS = Info[0].minimum_memoire_p_ligne * 4 + ((Info[0].generer_rang)? 4 : 0);

    char* format_base = (char*) calloc(TAILLE_FORMAT, sizeof(char));
    char* format_bulletins = (char*) calloc(TAILLE_FORMAT_BULLETINS, sizeof(char));

   if (Info[0].generer_rang)
   {
    format_base[0]='%';
    format_base[1]='d';
    format_base[2]='%';
    format_base[3]='c';

    format_bulletins[0]='%';
    format_bulletins[1]='d';
    format_bulletins[2]='%';
    format_bulletins[3]='c';
   }

    for (int i = (Info[0].generer_rang)? 4 : 0; i <= TAILLE_FORMAT - 8; i += 4)
    {
        format_base[i] = '%';
        format_base[i + 1] = 's';
        format_base[i + 2] = '%';
        format_base[i + 3] = 'c';
    }

    format_base[TAILLE_FORMAT - 4] = '%';
    format_base[TAILLE_FORMAT - 3] = 's';
    format_base[TAILLE_FORMAT - 2] = '\n';
    format_base[TAILLE_FORMAT - 1] = '\0';

    for (int i = (Info[0].generer_rang)? 4 : 0; i <= TAILLE_FORMAT_BULLETINS - 8; i += 4)
    {
        format_bulletins[i] = '%';
        format_bulletins[i + 1] = 's';
        format_bulletins[i + 2] = '%';
        format_bulletins[i + 3] = 'c';
    }

    format_bulletins[TAILLE_FORMAT_BULLETINS - 4] = '%';
    format_bulletins[TAILLE_FORMAT_BULLETINS - 3] = 's';
    format_bulletins[TAILLE_FORMAT_BULLETINS - 2] = '\n';
    format_bulletins[TAILLE_FORMAT_BULLETINS - 1] = '\0';

    char sep = Info[0].separateur;
    char* annee_courante = (char*) Info[0].Table[0][Annee];
    unsigned rang_fichier_base = 1;
    static FILE* base = NULL;
    static FILE* bulletins = NULL;
    static FILE** fichier_base = (FILE**) calloc(nbType, sizeof(FILE*));

    // Un peu low-level C, mais beaucoup plus rapide que de coder un fprintf pour chaque item.
    // Gain d'exécution : 30s pour fprintf par item
    //                    22s sur une ligne

    bulletins = ouvrir_fichier_bulletins(Info);

    switch (Info[0].taille_base)
    {

        case MONOLITHIQUE :
                 base = ouvrir_fichier_base(Info, 0);
                 break;

        case PAR_ANNEE :
                 base = ouvrir_fichier_base(Info, atoi(annee_courante) + nbType + 1);
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
                                       base = ouvrir_fichier_base(Info, -Info[0].taille_base - 2);
                                       break;

        case TOUTES_CATEGORIES  :
                 for (int d = 0; d < nbType; ++d) fichier_base[d] = ouvrir_fichier_base(Info, d + 1);
                 break;

        default : base = ouvrir_fichier_base(Info, rang_fichier_base + nbType + 1);
                 // cas où une vraie taille de base en lignes est entrée.

    }

    for (int i = 0; i < Info[0].nbfil; ++i)
    {
        for (uint32_t agent = 0; agent < Info[i].NCumAgentXml; ++agent)
        {
            /* BOUCLER SUR L */

            ++compteur_lignes_bulletins;

            ECRIRE_LIGNE_BULLETIN(i, agent, bulletins, format_bulletins, sep, Info, compteur_lignes_bulletins);

            if (Info[i].taille_base == PAR_ANNEE  && strcmp((const char*)VAR(Annee), annee_courante))
            {
                if (fclose(base) == EOF)
                {
                    perror("Erreur : Problème fermeture fichier base");
                    exit(-902);
                }

                fprintf(stderr, "Année : %s Table générée.\n", annee_courante);
                annee_courante = (char*) VAR(Annee);
                base = ouvrir_fichier_base(&Info[i], atoi(annee_courante) + nbType + 1);
            }

            unsigned l = Info[i].minimum_memoire_p_ligne;

            char type[3]={0};
            strcpy(type, type_remuneration_traduit[0]);

            int allocation_memoire = (Info[i].minimum_memoire_p_ligne + nbType + Info[i].NLigne[agent]*6) * sizeof(xmlChar*);

            while (ligne < Info[i].NLigne[agent])
            {
                bool nouveau_type = false;

                if (Info[i].taille_base > MONOLITHIQUE   // soit : il existe un nombre de lignes maximal par base
                    && (compteur  == rang_fichier_base * Info[i].taille_base))
                {
                    fprintf(stderr, "Table n° %d de %d lignes générée, lignes %d à %d.\n",
                            rang_fichier_base,
                            Info[i].taille_base,
                            (rang_fichier_base-1) * Info[i].taille_base +1,
                            rang_fichier_base * Info[i].taille_base);
                            if (fclose(base) == EOF)
                            {
                                perror("Erreur : Problème fermeture fichier base");
                                exit(-902);
                            }
                            ++rang_fichier_base;
                            if (rang_fichier_base >= 1000)
                            {
                                fprintf(stderr, "%s", "Erreur : Ne peut générer que 999 bases au plus\n");
                                exit(-904);
                            }

                            base = ouvrir_fichier_base(&Info[i], rang_fichier_base + nbType + 1);
                }

                if (l + 6 == allocation_memoire)
                {
                    fprintf(stderr, "Max lignes de paye atteint (%d) !\n", allocation_memoire);
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

                if (Info[0].taille_base > PAR_TRAITEMENT)
                {
                    ++compteur;

                    ECRIRE_LIGNE_l(i, agent, l, type, base, format_base, sep, Info, compteur);
                }
                else
                {
                    if (Info[0].taille_base != TOUTES_CATEGORIES)
                    {
                        if (valeur_drapeau_categorie + 2 == -Info[0].taille_base)
                        {
                            ++compteur;

                            ECRIRE_LIGNE_l(i, agent, l, type, base, format_base, sep, Info, compteur);
                        }
                    }
                    else
                    {
                        ++compteur;
                        if (nouveau_type)
                        {
                            base = fichier_base[valeur_drapeau_categorie - 1];
                        }

                        ECRIRE_LIGNE_l(i, agent, l, type, base, format_base, sep, Info, compteur);
                    }
                }

                l += 6;
                ++ligne;
            }

            ligne = 0;
        }

        if (i) Info[0].nbLigne += Info[i].nbLigne;
    }

    // Dans les autres cas, les bases ont déjà été refermées sauf une (cas par année et par taille maximale)
    if (Info[0].taille_base == TOUTES_CATEGORIES)
        for (int d = 0; d < nbType - 1; ++d)
            fclose(fichier_base[d]);

    if (base)
    {
        fclose(base);
        switch (Info[0].taille_base)
        {
        case  MONOLITHIQUE            :
            goto message;
        case  PAR_TRAITEMENT          :
            puts("Catégorie : Traitement.");
            goto message;
        case  PAR_INDEMNITE_RESIDENCE :
            puts("Catégorie : Indemnité de résidence.");
            goto message;
        case  PAR_SFT                 :
            puts("Catégorie : Supplément familial de traitement.");
            goto message;
        case  PAR_AVANTAGE_NATURE     :
            puts("Catégorie : Avantage en nature.");
            goto message;
        case  PAR_INDEMNITE           :
            puts("Catégorie : Indemnité.");
            goto message;
        case  PAR_REM_DIVERSES        :
            puts("Catégorie : Rémunérations diverses.");
            goto message;
        case  PAR_DEDUCTION           :
            puts("Catégorie : Déduction.");
            goto message;
        case  PAR_ACOMPTE             :
            puts("Catégorie : Acompte.");
            goto message;
        case  PAR_RAPPEL              :
            puts("Catégorie : Rappel.");
            goto message;
        case  PAR_RETENUE             :
            puts("Catégorie : Retenue.");
            goto message;
        case  PAR_COTISATION          :
            puts("Catégorie : Cotisation.");
            goto message;
        case  TOUTES_CATEGORIES       :
            puts("Toutes catégories.");
            fprintf(stderr, "Total de %" PRIu64 " lignes générée dans 11 bases.\n", compteur);
            break;

        case PAR_ANNEE    :
            fprintf(stderr, "Année : %s Table générée.\n", annee_courante);
            break;
        default :  /* Taille définie par l'utilisateur */
            fprintf(stderr, "Table n° %d de %" PRIu64 " lignes, lignes %d à %" PRIu64 ".\n",
                    rang_fichier_base,
                    compteur - (rang_fichier_base-1) * Info[0].taille_base,
                    (rang_fichier_base-1) * Info[0].taille_base +1,
                    compteur);
        }

        return;
message :
        fprintf(stderr, "Table de %" PRIu64 " lignes.\n", compteur);
    }

    if (bulletins)
    {
        fclose(bulletins);
        fprintf(stderr, "Base des bulletins de paye de %" PRIu32 " lignes.\n", compteur_lignes_bulletins);
    }

}
#undef VAR
//#ifdef __cplusplus
//}
//#endif // __cplusplus

