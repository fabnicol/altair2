#ifndef LIGNE_PAYE
#define LIGNE_PAYE
/*  Programme écrit par Fabrice NICOL sous licence CECILL 3
 *  Attention : lorsqu'il est édité, le présent code doit être converti soit en UTF-8 soit en ISO-5589-1 (Latin-1)avant d'être compilé.
 *  En entrée d'Altair préciser encodage.entrée en conformité avec l'encodage du présent fichier, qui sera celui de la base générée.
 */

/*
   Constantes de compilation pouvant être redéfinies : NA_STRING, MAX_LIGNES_PAYE, MAX_NB_AGENTS, NO_DEBUG
   MAX_NB_AGENTS détermine le nombre maximal d'agents par mois potentiellement traités
*/

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
       /* pour des raisons pratiques il peut être nécessaire de limiter le nombre de sorties de ce type */



    static int warning_count;
    static std::string fichier_last = "";

    std::lock_guard<std::mutex> guard(mut);

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
               std::cerr << WARNING_HTML_TAG "Impossible d'atteindre " << noeud << ". Messages d'avertissement supprimés par la suite."  ENDL;
               warning_count = WARNING_LIMIT + 1;
               errorLineStack.emplace_back(afficher_environnement_xhl(info, cur));
           }

       if (fichier_last !=  "" && info.threads->argv[info.fichier_courant] != fichier_last)
           warning_count = 0;

       /* on remet à zéro le maximum d'avertissements à chaque nouveau fichier */

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
                En excès du nombre de lignes de paye autorisé (" << info.nbLigneUtilisateur << ")."  ENDL
                "Omettre -n ... et utiliser -L fichier_log pour détecter le maximum de lignes de paye dans les fichiers."  ENDL
                "Utiliser -N ce_maximum ou bien recompiler en augmentant MAX_LIGNES_PAYE, à condition de disposer d'assez de mémoire."  ENDL;

        exit(-10);
    }
}



#endif // LIGNE_PAYE

