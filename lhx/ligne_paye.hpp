#ifndef LIGNE_PAYE
#define LIGNE_PAYE
/*  Programme �crit par Fabrice NICOL sous licence CECILL 3
 *  Attention : lorsqu'il est �dit�, le pr�sent code doit �tre converti soit en UTF-8 soit en ISO-5589-1 (Latin-1)avant d'�tre compil�.
 *  En entr�e d'Altair pr�ciser encodage.entr�e en conformit� avec l'encodage du pr�sent fichier, qui sera celui de la base g�n�r�e.
 */

/* Constantes de compilation pouvant �tre red�finies : NA_STRING, MAX_LIGNES_PAYE, MAX_NB_AGENTS, NO_DEBUG, NO_REGEXP */

#include <iomanip>
#include <iostream>
#include <cstring>

#include "fonctions_auxiliaires.hpp"
#include "table.hpp"
#include "tags.h"

extern bool verbeux;
extern std::mutex mut;
extern std::vector<errorLine_t> errorLineStack;


#if 0
static inline xmlNodePtr GCC_INLINE atteindreNoeudArret(const char* noeud, xmlNodePtr cur, const char* arret)
{
    while (cur && xmlIsBlankNode(cur))
    {
        cur = cur -> next;
    }

    while (cur != nullptr)
    {
        if (xmlStrcmp(cur->name, (const xmlChar *) noeud))  // cur->name != noeud
        {
            if (xmlStrcmp(cur->name, (const xmlChar *) arret))
                cur = cur->next;
            else
                return nullptr;
        }
        else
        {
            AFFICHER_NOEUD(noeud)
            break;
        }
    }
    return cur;
}
#endif

inline void warning_msg(const char* noeud, const info_t& info, const xmlNodePtr cur)
{
       /* pour des raisons pratiques il peut �tre n�cessaire de limiter le nombre de sorties de ce type */

    static int warning_count;
    static char* fichier_last = nullptr;

      #ifdef WARNING_LIMIT
       if (warning_count < WARNING_LIMIT)
       {
           ++warning_count;
           if (verbeux) std::cerr << WARNING_HTML_TAG "Impossible d'atteindre " << noeud << ENDL;
           errorLineStack.emplace_back(afficher_environnement_xhl(info, cur));
       }
       else
           if (warning_count == WARNING_LIMIT)
           {
               std::cerr << WARNING_HTML_TAG "Impossible d'atteindre " << noeud << ". Messages d'avertissement supprim�s par la suite."  ENDL;
               warning_count = WARNING_LIMIT + 1;
              errorLineStack.emplace_back(afficher_environnement_xhl(info, cur));
           }

       if (fichier_last !=  nullptr && strcmp(info.threads->argv[info.fichier_courant], fichier_last) != 0)
           warning_count = 0;

       /* on remet � z�ro le maximum d'avertissements � chaque nouveau fichier */

       fichier_last = info.threads->argv[info.fichier_courant];

      #else
           if (verbeux) std::cerr << WARNING_HTML_TAG "Impossible d'atteindre " << noeud << ENDL;
           errorLineStack.emplace_back(afficher_environnement_xhl(info, cur));
      #endif

}

static inline void GCC_INLINE  verifier_taille(const int nbLignePaye, info_t& info)
{
    if (nbLignePaye >= info.nbLigneUtilisateur)
    {
        std::cerr << "\
                En exc�s du nombre de lignes de paye autoris� (" << info.nbLigneUtilisateur << ")."  ENDL
                "Omettre -n ... et utiliser -L fichier_log pour d�tecter le maximum de lignes de paye dans les fichiers."  ENDL
                "Utiliser -N ce_maximum ou bien recompiler en augmentant MAX_LIGNES_PAYE, � condition de disposer d'assez de m�moire."  ENDL;

        exit(-10);
    }
}




#endif // LIGNE_PAYE

