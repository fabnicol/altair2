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

#else

  #define  STATE_HTML_TAG  "[MSG]"
  #define  PARAMETER_HTML_TAG "[PAR]"
  #define  PROCESSING_HTML_TAG  "[MSG]"
  #define  ERROR_HTML_TAG  "[ERR]"
  #define  WARNING_HTML_TAG  "[ATT]"
  #define  ENDL "\n"

#endif

#endif // TAGS_H
