#ifndef LIGNE_PAYE
#define LIGNE_PAYE
/*  Programme écrit par Fabrice NICOL sous licence CECILL 3
 *  Attention : lorsqu'il est édité, le présent code doit être converti soit en UTF-8 soit en ISO-5589-1 (Latin-1)avant d'être compilé.
 *  En entrée d'Altair préciser encodage.entrée en conformité avec l'encodage du présent fichier, qui sera celui de la base générée.
 */

/* Constantes de compilation pouvant être redéfinies : NA_STRING, MAX_LIGNES_PAYE, MAX_NB_AGENTS, NO_DEBUG, NO_REGEXP */

#include <iomanip>
#include <iostream>
#include <cstring>

#include "fonctions_auxiliaires.hpp"
#include "table.hpp"

extern std::mutex mut;

xmlNodePtr GCC_INLINE atteindreNoeud(const char* noeud, xmlNodePtr cur, bool  opt = true, int normalJump = 0);


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

void afficher_environnement_xhl(const info_t& info);

#endif // LIGNE_PAYE

