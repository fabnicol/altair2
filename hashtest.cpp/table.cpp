#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "table.hpp"

uint64_t generer_table_bulletins(const char* chemin_base, info_t* info)
{
    FILE* base = fopen(chemin_base, "wb");
    fseek(base, 0, SEEK_SET);
    if (base == NULL)
    {
        fprintf(stderr, "%s\n", "Impossible d'ouvrir le fichier de sortie.");
        exit(-1000);
    }
    else
        printf("%s\n", "Enregistrement de la base csv");

    unsigned i;
    for (i = 0; i < sizeof(entete_char)/sizeof(char*) -1; i++)
        fprintf(base, "%s%c", entete_char[i], info->separateur);

    fprintf(base, "%s\n", entete_char[i]);

    uint64_t compteur=0;

    compteur = boucle_ecriture(base, info);

    fclose(base);
    return compteur;
}

int64_t generer_table_standard(const char* chemin_table, info_t* info)
{



}

/* Il doit y avoir BESOIN_MEMOIRE_ENTETE + 6 champs plus Type, soit 18 + 6 +1 = 25 champs et 24 séparateurs + saut de ligne = 50 char + \0*/

uint64_t boucle_ecriture(FILE* base, info_t* info)
{
    int ligne = 0;
    uint64_t compteur = 0;

#define TAILLE_FORMAT (info->besoin_memoire_par_ligne + 6 +1 +2)*4

    char format_base[TAILLE_FORMAT];

    for (int i = 0; i <= TAILLE_FORMAT - 8; i += 4)
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

    char sep = info->separateur;

    // Un peu low-level C, mais beaucoup plus rapide que de coder un fprintf pour chaque item.
    // Gain d'exécution : 30s pour fprintf par item
    //                    22s sur une ligne

    for (uint32_t agent = 0; agent < info->NCumAgentLibxml2; agent++)
    {
        #define VAR(X) info->Table[agent][X]

        /* BOUCLER SUR L */

        unsigned l = info->besoin_memoire_par_ligne;
        char* type =  (char*) type_remuneration_traduit[0];

        while (ligne < info->NLigne[agent])
        {
            if (l + 6 == ((info->reduire_consommation_memoire)?
                          info->besoin_memoire_par_ligne + nbType + (info->NLigne[agent])*6
                          : info->besoin_memoire_par_ligne + nbType + MAX_LIGNES_PAYE*6) * sizeof(xmlChar*))
            {
                perror("Max lignes de paye atteint !");
                exit(-1002);
            }

            if (VAR(l) && VAR(l)[0] == '_')
            {
                char* p = (char*) VAR(l);
                int val = atoi(p+1);
                if (val > 0 && val < nbType)
                  type =  (char*) type_remuneration_traduit[val];
                l++;
            }

            fprintf(base, format_base,
                    VAR(Annee), sep,
                    VAR(Mois), sep,
                    VAR(Nom), sep,
                    VAR(Prenom), sep,
                    VAR(Matricule), sep,
                    VAR(Service), sep,
                    VAR(Statut), sep,
                    VAR(QuotiteTrav), sep,
                    VAR(NbHeureSup), sep,
                    VAR(NbHeureTotal), sep,
                    VAR(Indice), sep,
                    VAR(MtBrut), sep,
                    VAR(MtNet), sep,
                    VAR(MtNetAPayer), sep,
                    VAR(NBI), sep,
                    "*",'$',
                    VAR(l), sep,
                    VAR(l+1), sep,
                    VAR(l+2), sep,
                    VAR(l+3), sep,
                    VAR(l+4), sep,
                    VAR(l+5), sep,
                    "*", '$',
                    type, sep,
                    VAR(EmploiMetier), sep,
                    VAR(Grade), sep,
                    VAR(NIR));
            l += 6;
            ligne++;
        }
        compteur += ligne;
        ligne = 0;
    }
    return compteur;
}
//#undef VAR
#ifdef __cplusplus
}
#endif // __cplusplus

