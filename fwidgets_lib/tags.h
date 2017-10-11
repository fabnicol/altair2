/// Copyright Fabrice Nicol
// Contributeur :
// Fabrice Nicol, années 2008 à 2012
// fabrnicol@gmail.com
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

