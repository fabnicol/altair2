//#ifdef __cplusplus
//extern "C" {
//#endif // __cplusplus

#include "table.hpp"
#include "fonctions_auxiliaires.hpp"



int64_t generer_table_standard(const char* chemin_table, info_t* info)
{



}

/* Il doit y avoir BESOIN_MEMOIRE_ENTETE + 6 champs plus Type, soit 18 + 6 +1 = 23 champs et 24 s�parateurs + saut de ligne = 48 char + \0*/

inline FILE* ouvrir_nouvelle_base(info_t* info, unsigned* rang_fichier_base, FILE* base)
{
    if (fclose(base) == EOF)
    {
        perror("Probl�me fermeture fichier base");
        exit(-902);
    }

    if (*rang_fichier_base >= 1000)
    {
        fprintf(stderr, "%s", "Ne peut g�n�rer que 999 bases au plus\n");
        exit(-904);
    }

    return ouvrir_fichier_base(info, ++*rang_fichier_base);
}

#define TAILLE_FORMAT (Info[0].minimum_memoire_p_ligne + 6 +1)*4
#define VAR(X) Info[i].Table[agent][X]
#define ECRIRE_LIGNE_l fprintf(base, format_base, \
                            VAR(Annee),sep, \
                            VAR(Mois), sep, \
                            VAR(Nom), sep, \
                            VAR(Prenom), sep, \
                            VAR(Matricule), sep, \
                            VAR(Service), sep, \
                            VAR(Statut), sep, \
                            VAR(QuotiteTrav), sep, \
                            VAR(NbHeureSup), sep, \
                            VAR(NbHeureTotal), sep, \
                            VAR(Indice), sep, \
                            VAR(MtBrut), sep, \
                            VAR(MtNet), sep, \
                            VAR(MtNetAPayer), sep, \
                            VAR(NBI), sep, \
                            VAR(l), sep, \
                            VAR(l+1), sep, \
                            VAR(l+2), sep, \
                            VAR(l+3), sep, \
                            VAR(l+4), sep, \
                            VAR(l+5), sep, \
                            type, sep, \
                            VAR(EmploiMetier), sep, \
                            VAR(Grade), sep, \
                            VAR(NIR));

void boucle_ecriture(info_t* Info)
{
    int ligne = 0, ligne_typee = 0;
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
    FILE* base = NULL;

    // Un peu low-level C, mais beaucoup plus rapide que de coder un fprintf pour chaque item.
    // Gain d'ex�cution : 30s pour fprintf par item
    //                    22s sur une ligne

if (Info[0].taille_base == TOUTES_CATEGORIES)
   for (int rang = 11; rang >= 1; rang--)
        base= ouvrir_fichier_base(Info, rang);
 else
    base = ouvrir_fichier_base(Info, rang_fichier_base);

    for (int i = 0; i < Info[0].nbfil; i++)
    {
        for (uint32_t agent = 0; agent < Info[i].NCumAgentXml; agent++)
        {
            /* BOUCLER SUR L */

            if (Info[i].taille_base == PAR_ANNEE  && strcmp((const char*)VAR(Annee), annee_courante))
            {
                  base = ouvrir_nouvelle_base(&Info[i], &rang_fichier_base, base);
                  fprintf(stderr, "Ann�e : %s Table g�n�r�e.\n", annee_courante);
                  annee_courante = (char*) VAR(Annee);
            }

            unsigned l = Info[i].minimum_memoire_p_ligne;
            char* type =  (char*) type_remuneration_traduit[0]; //

            int allocation_memoire = (Info[i].reduire_consommation_memoire)?
                                     Info[i].minimum_memoire_p_ligne + nbType + Info[i].NLigne[agent] * 6
                                     : Info[i].minimum_memoire_p_ligne + nbType + Info[i].nbLigneUtilisateur * 6 * sizeof(xmlChar*);

            while (ligne < Info[i].NLigne[agent])
            {
                bool nouveau_type = false;

                if (Info[i].taille_base > MONOLITHIQUE && compteur + ligne == rang_fichier_base*Info[i].taille_base -1)
                {
                    fprintf(stderr, "Table n� %d de %d lignes g�n�r�e, lignes %d � %d.\n",
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

                int val = 0;

                if (VAR(l) && (val = VAR(l)[0]) >= 1 && (val <= nbType))
                {
                    type =  (char*) type_remuneration_traduit[val-1];
                    nouveau_type = true;
                    l++;
                }

                if (Info[0].taille_base > PAR_TRAITEMENT || (Info[0].taille_base != TOUTES_CATEGORIES && val + 2 == -Info[0].taille_base))
                {
                    ligne_typee++;

                    ECRIRE_LIGNE_l
                }
                else
                {
                 if (Info[0].taille_base == TOUTES_CATEGORIES)
                  {
                      ligne_typee++;
                      if (nouveau_type)
                      {
                        fclose(base);
                        base = ajouter_au_fichier_base(Info, val);
                      }

                      ECRIRE_LIGNE_l
                  }
                }

                l += 6;
                ligne++;
            }
            compteur += ligne_typee;
            ligne = 0;
            ligne_typee = 0;
        }

        if (i) Info[0].nbLigne += Info[i].nbLigne;
    }

    if (base)
    {
        fclose(base);
        switch (Info[0].taille_base)
        {
            case  MONOLITHIQUE            : goto message;
            case  PAR_TRAITEMENT          : puts("Cat�gorie : Traitement.");             goto message;
            case  PAR_INDEMNITE_RESIDENCE : puts("Cat�gorie : Indemnit� de r�sidence."); goto message;
            case  PAR_SFT                 : puts("Cat�gorie : Suppl�ment familial de traitement."); goto message;
            case  PAR_AVANTAGE_NATURE     : puts("Cat�gorie : Avantage en nature.");     goto message;
            case  PAR_INDEMNITE           : puts("Cat�gorie : Indemnit�.");              goto message;
            case  PAR_REM_DIVERSES        : puts("Cat�gorie : R�mun�rations diverses."); goto message;
            case  PAR_DEDUCTION           : puts("Cat�gorie : D�duction.");              goto message;
            case  PAR_ACOMPTE             : puts("Cat�gorie : Acompte.");                goto message;
            case  PAR_RAPPEL              : puts("Cat�gorie : Rappel.");                 goto message;
            case  PAR_RETENUE             : puts("Cat�gorie : Retenue.");                goto message;
            case  PAR_COTISATION          : puts("Cat�gorie : Cotisation.");             goto message;
            case  TOUTES_CATEGORIES       : puts("Toutes cat�gories."); fprintf(stderr, "Total de %" PRIu64 " lignes g�n�r�e dans 11 bases.\n", compteur); break;

            case PAR_ANNEE    :
                fprintf(stderr, "Ann�e : %s Table g�n�r�e.\n", annee_courante);
                break;
            default :  /* Taille d�finie par l'utilisateur */
                fprintf(stderr, "Table n� %d de %" PRIu64 " lignes, lignes %d � %" PRIu64 ".\n",
                        rang_fichier_base,
                        compteur - (rang_fichier_base-1) * Info[0].taille_base,
                        (rang_fichier_base-1) * Info[0].taille_base +1,
                        compteur);
        }

        return;
        message : fprintf(stderr, "Table de %" PRIu64 " lignes.\n", compteur);
    }
}
#undef VAR
//#ifdef __cplusplus
//}
//#endif // __cplusplus

