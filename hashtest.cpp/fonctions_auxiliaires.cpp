/*  Programme écrit par Fabrice NICOL sous licence CECILL 3
 *  Attention : lorsqu'il est édité, le présent code doit être converti soit en UTF-8 soit en ISO-5589-1 (Latin-1)avant d'être compilé.
 *  En entrée d'Altair préciser encodage.entrée en conformité avec l'encodage du présent fichier, qui sera celui de la base générée.
 */


#ifdef __cplusplus
extern "C" {
#endif
#include "fonctions_auxiliaires.hpp"

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

#ifdef __cplusplus
}
#endif // __cplusplus
