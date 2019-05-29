
// Auteur : Fabrice Nicol
// Copyright Fabrice Nicol
// Contributeur :
// Fabrice Nicol, années 2008 à 2012, 2017
// fabrnicol@gmail.com
//
// Ce logiciel est régi par les dispositions du code de la propriété
// intellectuelle (CPI). 

// L'auteur se réserve le droit d'exploitation du présent logiciel, 
// et notamment de reproduire et de modifier le logiciel, conformément aux 
// dispositions de l'article L.122-6 du même code. L'auteur se réserve le droit
// de modifier et de corriger les erreurs du logiciel, conformément aux termes 
// du deuxième alinéa du I de l'article L.122-6-1 du même code.

// Le présent logiciel doit être regardé comme un "Module Externe", pour 
// l'application des stipulations de la licence CeCILL, lorsque le code source, 
// ou une bibliothèque compilée à partir du présent code, sont utilisés pour 
// compiler tout logiciel faisant usage du présent code.

// Tout fichier contenant le présent code, ou des versions antérieures du 
// présent code, et faisant apparaître des termes contraires aux stipulations
// précédentes doit être regardé comme faisant partie d'actes préparatoires à
// l'élaboration du présent logiciel et non comme le logiciel lui-même, ou 
// comme des erreurs matérielles dans la copie des licences. Seules les 
// stipulations précédentes doivent être regardées comme s'appliquant au 
// présent logiciel.
//
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
// pris connaissance de ces stipulations et que vous en avez accepté les
// termes.

// Sans préjudice des dispositions du CPI, une autorisation d'usage et de 
// reproduction du présent code est donnée à tout agent employé par les
// juridictions financières pour l'exercice de leurs fonctions publiques. 
// Le code ainsi mis à disposition ne peut être transmis à d'autres utilisateurs.
//
#ifndef TAGS_H
#define TAGS_H

#if defined(GUI_OUTPUT) || defined (GUI_TAG_MESSAGES)

#define PARAMETER_HTML_TAG "<img src=\":/images/parameter.png\" />&nbsp;"
#define STATE_HTML_TAG "<img src=\":/images/msg.png\"/>&nbsp;"
#define ERROR_HTML_TAG "<img src=\":/images/error.png\"/>&nbsp;"
#define WARNING_HTML_TAG "<img src=\":/images/warning.png\"/>&nbsp;"
#define PROCESSING_HTML_TAG "<img src=\":/images/information.png\"/>&nbsp;"
#define LICENSE_HTML_TAG "<img src=\":/images/gplv3.png\"/>&nbsp;"
#define DBG_HTML_TAG "<img src=\":/images/debug.png\"/>&nbsp;"
#define HTML_TAG(X) "<span style=\"color: " #X ";\">&nbsp;"
#define ENDL  "<br>\n"  // pour une utilisation GUI, "\n" en ligne de commande de console pure.
#define SPACER  "&nbsp;&nbsp;\n"

#elif defined(COLOR_CONSOLE)
    #define COLOR_WHITE  "\033[1;37m"
    #define COLOR_BLACK  "\033[0;30m"
    #define COLOR_BLUE  "\033[0;34m"
    #define COLOR_LIGHT_BLUE  "\033[1;34m"
    #define COLOR_GREEN  "\033[0;32m"
    #define COLOR_LIGHT_GREEN  "\033[1;32m"
    #define COLOR_CYAN  "\033[0;36m"
    #define COLOR_LIGHT_CYAN  "\033[1;36m"
    #define COLOR_RED  "\033[0;31m"
    #define COLOR_LIGHT_RED  "\033[1;31m"
    #define COLOR_PURPLE  "\033[0;35m"
    #define COLOR_NORMAL "\033[0m"

    #define  STATE_HTML_TAG  COLOR_LIGHT_BLUE "[MSG] " COLOR_NORMAL
    #define  PARAMETER_HTML_TAG COLOR_LIGHT_RED "[PAR] " COLOR_NORMAL
    #define  PROCESSING_HTML_TAG  COLOR_LIGHT_GREEN "[INF] " COLOR_NORMAL
    #define  ERROR_HTML_TAG  COLOR_RED "[ERR] " COLOR_NORMAL
    #define  WARNING_HTML_TAG  COLOR_PURPLE "[ATT] " COLOR_NORMAL
    #define  ENDL "\n"
    #define  SPACER "\t"

#else

  #define  STATE_HTML_TAG  "[MSG] "
  #define  PARAMETER_HTML_TAG "[PAR] "
  #define  PROCESSING_HTML_TAG  "[INF] "
  #define  ERROR_HTML_TAG  "[ERR] "
  #define  WARNING_HTML_TAG  "[ATT] "
  #define  ENDL "\n"
  #define  SPACER "\t"

#endif

#endif // TAGS_H

