/*  Programme écrit par Fabrice NICOL sous licence CECILL 3
 *  Attention : lorsqu'il est édité, le présent code doit être converti soit en UTF-8 soit en ISO-5589-1 (Latin-1)avant d'être compilé.
 *  En entrée d'Altair préciser encodage.entrée en conformité avec l'encodage du présent fichier, qui sera celui de la base générée.
 */


//#ifdef __cplusplus
//extern "C" {
//#endif
#include "fonctions_auxiliaires.hpp"
#include <stdint.h>
    int lire_argument(int argc, const char* const c_str)
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
            else if ((LONG_MIN == sl || LONG_MAX == sl) && ERANGE == errno)
            {
                fprintf(stderr, "%s entier excédant la limite des entiers longs\n", c_str);
            }
            else if (sl > UINT16_MAX)
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
        info->NLigne = (uint16_t*) calloc(info->threads->argc, MAX_NB_AGENTS * info->threads->argc * sizeof(uint16_t));  // nm total de bulletins
        info->NAgent = (int32_t*)  calloc(info->threads->argc, sizeof(int32_t));
        info->NCumAgent = 0;
        puts("Premier scan des fichiers pour déterminer les besoins mémoire ... ");

        /* par convention  un agent avec rémunération non renseignées (balise sans fils) a une ligne */
        for (int i = 0; i < info->threads->argc ; i++)
        {
            FILE* c;
            errno = 0;
            c = fopen(info->threads->argv[i], "r");
            if (c) fseek(c, 0, SEEK_SET);
            else
             if(c == NULL)
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
                    info->NAgent[i]++;
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
                        info->NAgent[i]++;
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
        return errno;
    }

//#ifdef __cplusplus
//}
//#endif // __cplusplus
