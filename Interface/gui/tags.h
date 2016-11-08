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

