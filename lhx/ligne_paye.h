// Copyright Cour des comptes, 2017
// Contributeur :
// Fabrice Nicol, années 2012 à 2017
// fabrice.nicol@crtc.ccomptes.fr
//
// Ce logiciel est un programme informatique servant à extraire et analyser les fichiers de paye
// produits au format spécifié par l'annexe de la convention-cadre nationale de dématérialisation
// en vigueur à compter de l'année 2008.
//
// Ce logiciel est régi par la licence CeCILL soumise au droit français et
// respectant les principes de diffusion des logiciels libres. Vous pouvez
// utiliser, modifier et/ou redistribuer ce programme sous les conditions
// de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
// sur le site "http://www.cecill.info".
//
// En contrepartie de l'accessibilité au code source et des droits de copie,
// de modification et de redistribution accordés par cette licence, il n'est
// offert aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons,
// seule une responsabilité restreinte pèse sur l'auteur du programme, le
// titulaire des droits patrimoniaux et les concédants successifs.
//
// A cet égard l'attention de l'utilisateur est attirée sur les risques
// associés au chargement, à l'utilisation, à la modification et/ou au
// développement et à la reproduction du logiciel par l'utilisateur étant
// donné sa spécificité de logiciel libre, qui peut le rendre complexe à
// manipuler et qui le réserve donc à des développeurs et des professionnels
// avertis possédant des connaissances informatiques approfondies. Les
// utilisateurs sont donc invités à charger et tester l'adéquation du
// logiciel à leurs besoins dans des conditions permettant d'assurer la
// sécurité de leurs systèmes et ou de leurs données et, plus généralement,
// à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.
//
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
// pris connaissance de la licence CeCILL, et que vous en avez accepté les
// termes.
//
//
#ifndef LIGNE_PAYE
#define LIGNE_PAYE

/// \file ligne_paye.h
/// \brief Définit deux fonctions auxiliaires utilisées dans lignes_paye.cpp
/// \note   Constantes de compilation pouvant être redéfinies : #WARNING_LIMIT, #MAX_LIGNES_PAYE

#include <iomanip>
#include <iostream>
#include <cstring>

#include "fonctions_auxiliaires.h"
#include "table.h"
#include "tags.h"

#define ATTEINDRE_NOEUD(X, Y)  {  cur = atteindreNoeud(#X, cur); \
    if (cur) \
      { \
        Y = xmlGetProp (cur, (const xmlChar *) "V"); \
        if (!Y) { result = false; cur = cur1;}\
      } \
    else {result = false; cur = cur1; } \
    }

#define ATTEINDRE_NOEUD_OPT(X, Y)  {  cur = atteindreNoeud(#X, cur); \
    if (cur) \
      { \
        Y = xmlGetProp (cur, (const xmlChar *) "V"); \
      } else cur = cur1;\
    }


extern bool verbeux;
extern std::mutex mut;
extern std::vector<errorLine_t> errorLineStack;

/// Affiche un message d'avertissement sur la sortie d'erreur lorsqu'un noeud XML ne peut être atteint
/// \param noeud Noeud courant XML
/// \param info  Structure info_t contenant les données de paye XMLà décoder
/// \param cur   Noeud libxml2 courant
/// \note Le nombre de message d'avertissement est au plus #WARNING_LIMIT si cette constante est définie

inline void warning_msg (const char* noeud, const info_t& info, const xmlNodePtr GCC_UNUSED cur)
{
    // pour des raisons pratiques il peut être nécessaire de limiter le nombre de sorties de ce type

    static int warning_count;
    static std::string fichier_last = "";

    std::lock_guard<std::mutex> guard (mut);

#ifdef WARNING_LIMIT

    if (warning_count < WARNING_LIMIT)
        {
            ++warning_count;

            if (verbeux)
            {

                std::cerr << WARNING_HTML_TAG "Impossible d'atteindre " << noeud << ENDL;
            }

            //    errorLineStack.emplace_back(afficher_environnement_xhl(info, cur));

        }
    else if (warning_count == WARNING_LIMIT)
        {
            std::cerr << WARNING_HTML_TAG "Impossible d'atteindre " << noeud << ". Messages d'avertissement supprimés par la suite."  ENDL;
            warning_count = WARNING_LIMIT + 1;
            //     errorLineStack.emplace_back(afficher_environnement_xhl(info, cur));
        }

    if (fichier_last !=  "" && info.threads->argv[info.fichier_courant] != fichier_last)
        warning_count = 0;

    /* on remet à zéro le maximum d'avertissements à chaque nouveau fichier */

    fichier_last = info.threads->argv[info.fichier_courant];

#else

    if (verbeux) std::cerr << WARNING_HTML_TAG "Impossible d'atteindre " << noeud << ENDL;

    errorLineStack.emplace_back (afficher_environnement_xhl (info, cur));
#endif

}


/// Vérifie que la taille des données de paye en nombre de lignes de paye au décodage effectif des fichiers XML
/// ne dépasse pas la préallocation en mémoire (en nombre d elignes)
/// \param nbLignePaye nombre de lignes de paye courant
/// \param info Structure info_t contenant les données de paye

inline void  verifier_taille (const int nbLignePaye, info_t& info)
{
    if (nbLignePaye >= info.nbLigneUtilisateur)
        {
            LOCK_GUARD
            std::cerr << "\
                En excès du nombre de lignes de paye autorisé (" << info.nbLigneUtilisateur << ")."  ENDL
                      "Omettre -n ... et utiliser -L fichier_log pour détecter le maximum de lignes de paye dans les fichiers."  ENDL
                      "Utiliser -N ce_maximum ou bien recompiler en augmentant MAX_LIGNES_PAYE, à condition de disposer d'assez de mémoire."  ENDL;

            exit (-10);
        }
}


#if LARGEUR >= 1

/// Sous-routine d'analyse du champ CptBancaire (compte bancaire)
/// \note Seulement si LARGEUR >= 1
/// \param cur Pointer XmlNodeePtr courant
/// \param info structure de données principales


inline void GCC_INLINE analyser_compte_bancaire(xmlNodePtr& cur, info_t& info);

/// Sous-routine d'analyse du champ Adresse
/// \note Seulement si LARGEUR >= 1
/// \param cur Pointer XmlNodeePtr courant
/// \param info structure de données principales
/// \return Booléen vrai si l'adresse est identifiée, faux sinon.

inline bool GCC_INLINE analyser_adresse(xmlNodePtr& cur, info_t& info);

#endif

#endif // LIGNE_PAYE

