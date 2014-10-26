/*  Programme écrit par Fabrice NICOL sous licence CECILL 3
 *  Attention : lorsqu'il est édité, le présent code doit être converti soit en UTF-8 soit en ISO-5589-1 (Latin-1)avant d'être compilé.
 *  En entrée d'Altair préciser encodage.entrée en conformité avec l'encodage du présent fichier, qui sera celui de la base générée.
 */


//#ifdef __cplusplus
//extern "C" {
//#endif
#include "fonctions_auxiliaires.hpp"
#include <stdint.h>

char* ecrire_chemin_base(char* chemin_base, int rang_fichier_base)
{
    int s = strlen(chemin_base);
    char chemin[s + 1 + 3];   // chemin_base + _ + 3 chiffres
    int cut = s - strlen(CSV);
    strncpy(chemin, chemin_base, cut);
    sprintf(chemin + cut, "_%d%s", rang_fichier_base, CSV);
    return(strdup(chemin));
}

FILE* ouvrir_fichier_base(info_t* info, int rang)
{
    char* chemin = ecrire_chemin_base(info[0].chemin_base, rang);
    FILE* base = fopen(chemin, "wb");
    fseek(base, 0, SEEK_SET);
    if (base == NULL)
    {
        fprintf(stderr, "%s\n", "Impossible d'ouvrir le fichier de sortie.");
        exit(-1000);
    }

    unsigned i;
    for (i = 0; i < sizeof(entete_char)/sizeof(char*) -1; i++)
        fprintf(base, "%s%c", entete_char[i], info[0].separateur);

    fprintf(base, "%s\n", entete_char[i]);
    return base;
}

int32_t lire_argument(int argc, const char* const c_str)
{
    if (argc > 2)
    {
        char *end;
        errno = 0;
        const long sl = strtol(c_str, &end, 10);

        if (end == c_str)
        {
            fprintf(stderr, "%s: pas un décimal\n", c_str);
        }
        else if (sl > INT32_MAX)
        {
            fprintf(stderr, "%ld entier excédant la limite des entiers à 16 bits\n", sl);
        }
        else if (sl < 0)
        {
            fprintf(stderr, "%ld l'entier doit être positif\n", sl);
        }
        else
        {
            return((uint32_t)sl);
        }
        return(-1);
    }
    else
    {
        fprintf(stderr, "%s\n", "Préciser le nombre de bulletins mensuels attendus (majorant du nombre).");
        return(-1);
    }
}

int calculer_memoire_requise(info_t* info)
{
    errno = 0;
    info->NLigne = (uint16_t*) calloc(info->threads->argc, MAX_NB_AGENTS * sizeof(uint16_t));  // nm total de bulletins
    info->NCumAgent = 0;
    puts("Premier scan des fichiers pour déterminer les besoins mémoire ... ");

    /* par convention  un agent avec rémunération non renseignées (balise sans fils) a une ligne */
    for (unsigned i = 0; i < info->threads->argc ; i++)
    {
        FILE* c;
        errno = 0;
        c = fopen(info->threads->argv[i], "r");
        if (c) fseek(c, 0, SEEK_SET);
        else if(c == NULL)
        {
            perror("Ouverture Fichiers.");    // cautious no-op
            exit(-120);
        }

        if (errno)
        {
            perror("Fichier .xhl");
            exit(-122);
        }
        int d = 0;

        while (d != EOF && (d = fgetc(c)) != EOF)
        {
            if  (d != '<') continue;
            if  ((d = fgetc(c)) != 'R') continue;
            if  ((d = fgetc(c)) != 'e') continue;
            if  ((d = fgetc(c)) != 'm') continue;
            if  ((d = fgetc(c)) != 'u') continue;
            if  ((d = fgetc(c)) != 'n') continue;
            if  ((d = fgetc(c)) != 'e') continue;
            if  ((d = fgetc(c)) != 'r') continue;
            if  ((d = fgetc(c)) != 'a') continue;
            if  ((d = fgetc(c)) != 't') continue;
            if  ((d = fgetc(c)) != 'i') continue;
            if  ((d = fgetc(c)) != 'o') continue;
            if  ((d = fgetc(c)) != 'n') continue;
            if  ((d = fgetc(c)) == '/')
            {
                // info->NAgent[i]++;
                info->NLigne[info->NCumAgent]=1;
                info->NCumAgent++;

                continue;  // Balise simple vide
            }

            while (d != EOF && (d = fgetc(c)) != EOF)
            {
                if (d != '<') continue;
                if ((d = fgetc(c)) != 'C')
                {
                    if (d != '/') continue;
                    else if ((d = fgetc(c)) != 'R')   continue;
                    else if ((d = fgetc(c)) != 'e')   continue;
                    else if ((d = fgetc(c)) != 'm')   continue;
                    else if ((d = fgetc(c)) != 'u')   continue;
                    else if ((d = fgetc(c)) != 'n')   continue;

                    if (info->NLigne[info->NCumAgent] == 0) info->NLigne[info->NCumAgent] = 1;
                    //info->NAgent[i]++;
                    info->NCumAgent++;
                    break;
                }
                else
                {
                    if ((d = fgetc(c)) != 'o') continue;
                    else
                    {
                        if ((d = fgetc(c)) != 'd')   continue;
                        else
                        {
                            if ((d = fgetc(c)) != 'e')   continue;
                            else
                            {
                                if ((d = fgetc(c)) != ' ')   continue;
                                info->NLigne[info->NCumAgent]++;
                            }
                        }
                    }
                }
            }
        }

        fclose(c);
    }

    info->NLigne = (uint16_t*) realloc(info->NLigne, info->NCumAgent * sizeof(uint16_t));

    return errno;
}

//#ifdef __cplusplus
//}
//#endif // __cplusplus
