//#ifdef __cplusplus
//extern "C" {
//#endif // __cplusplus

#include "table.hpp"
#include "fonctions_auxiliaires.hpp"



int64_t generer_table_standard(const char* chemin_table, info_t* info)
{



}

/* Il doit y avoir BESOIN_MEMOIRE_ENTETE + 6 champs plus Type, soit 18 + 6 +1 = 23 champs et 24 séparateurs + saut de ligne = 48 char + \0*/

inline FILE* ouvrir_nouvelle_base(info_t* info, unsigned* rang_fichier_base, FILE* base)
{
    if (fclose(base) == EOF)
    {
        perror("Problème fermeture fichier base");
        exit(-902);
    }

    if (*rang_fichier_base >= 1000)
    {
        fprintf(stderr, "%s", "Ne peut générer que 999 bases au plus\n");
        exit(-904);
    }

    return ouvrir_fichier_base(info, ++*rang_fichier_base);
}

#define TAILLE_FORMAT (Info[0].minimum_memoire_p_ligne + 6 +1)*4
#define VAR(X) Info[i].Table[agent][X]

void boucle_ecriture(info_t* Info)
{
    int ligne = 0;
    uint64_t compteur = 0;
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

    char sep = Info[0].separateur;
    char* annee_courante = (char*) Info[0].Table[0][Annee];
    unsigned rang_fichier_base = 1;

    // Un peu low-level C, mais beaucoup plus rapide que de coder un fprintf pour chaque item.
    // Gain d'exécution : 30s pour fprintf par item
    //                    22s sur une ligne

    FILE* base = ouvrir_fichier_base(Info, rang_fichier_base);

    for (int i = 0; i < Info[0].nbfil; i++)
    {
        for (uint32_t agent = 0; agent < Info[i].NCumAgentXml; agent++)
        {
            /* BOUCLER SUR L */

            if (Info[i].taille_base == PAR_ANNEE && strcmp((const char*)VAR(Annee), annee_courante))
            {
                base = ouvrir_nouvelle_base(&Info[i], &rang_fichier_base, base);
                fprintf(stderr, "Année : %s Table générée.\n", annee_courante);
                annee_courante = (char*) VAR(Annee);
            }

            unsigned l = Info[i].minimum_memoire_p_ligne;
            char* type =  (char*) type_remuneration_traduit[0];

            int allocation_memoire = (Info[i].reduire_consommation_memoire)?
                                     Info[i].minimum_memoire_p_ligne + nbType + Info[i].NLigne[agent] * 6
                                     : Info[i].minimum_memoire_p_ligne + nbType + Info[i].nbLigneUtilisateur * 6 * sizeof(xmlChar*);

            while (ligne < Info[i].NLigne[agent])
            {
                if (Info[i].taille_base > MONOLITHIQUE && compteur + ligne == rang_fichier_base*Info[i].taille_base -1)
                {
                    fprintf(stderr, "Table n° %d de %d lignes générée, lignes %d à %d.\n",
                            rang_fichier_base,
                            Info[i].taille_base,
                            (rang_fichier_base-1) * Info[i].taille_base +1,
                            rang_fichier_base * Info[i].taille_base);
                    base = ouvrir_nouvelle_base(&Info[i], &rang_fichier_base, base);
                }

                if (l + 6 == allocation_memoire)
                {
                    perror("Max lignes de paye atteint !");
                    exit(-1002);
                }

                int val;

                if (VAR(l) && (val = VAR(l)[0]) >= 1 && (val <= nbType))
                {
                    type =  (char*) type_remuneration_traduit[val-1];
                    l++;
                }

                fprintf(base, format_base,
                        VAR(Annee),sep,
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
                l += 6;
                ligne++;
            }
            compteur += ligne;
            ligne = 0;
        }

        if (i) Info[0].nbLigne += Info[i].nbLigne;
    }

    if (base)
    {
        fclose(base);
        switch (Info[0].taille_base)
        {
        case MONOLITHIQUE :
            fprintf(stderr, "Table de %" PRIu64 " lignes générée.\n", compteur);
            break;
        case PAR_ANNEE    :
            fprintf(stderr, "Année : %s Table générée.\n", annee_courante);
            break;
        default :  /* Taille définie par l'utilisateur */
            fprintf(stderr, "Table n° %d de %" PRIu64 " lignes générée, lignes %d à %" PRIu64 ".\n",
                    rang_fichier_base,
                    compteur - (rang_fichier_base-1)*Info[0].taille_base,
                    (rang_fichier_base-1)*Info[0].taille_base +1,
                    compteur);

        }
    }
}
#undef VAR
//#ifdef __cplusplus
//}
//#endif // __cplusplus

