#ifndef XML_LIB_H
#define XML_LIB_H

#include <libxml/xinclude.h>
/// Macro tendant à forcer l'inlining sous GCC
#define GCC_INLINE __attribute__((always_inline))

#define verifier_libxml_version  LIBXML_TEST_VERSION

using xmlT = xmlChar;
using xmlN = xmlNode;
using xmlD = xmlDoc;

inline void GCC_INLINE initialiser_xml() ///< Routine d'initialxmlIsBlankNodeisation
{
    xmlKeepBlanksDefault (0);
    
    xmlInitMemory();
    xmlInitParser();
}

inline void GCC_INLINE nettoyer_xml()  ///< Nettoyage du parseur xml
{
     xmlCleanupParser();
}

inline void GCC_INLINE free_xml(xmlT* u)  ///< Libération de la mémoire d'une chaine de caractères
{
    xmlFree(u);
}
 
inline xmlT* GCC_INLINE copier_xml(const xmlT* u) ///< Copie d'une chaine de caractères
{
    return xmlStrdup(u);
}
 
inline xmlN* GCC_INLINE fils_xml(const xmlN* cur)  ///< Premier noeud fils
{
    return cur->xmlChildrenNode;
}

inline long GCC_INLINE ligne_xml(const xmlN* cur)  ///< Numéro de ligne du fichier
{
   return xmlGetLineNo(cur);
}

inline xmlT* GCC_INLINE prop_xml(const xmlN* cur, const char* V) ///< Propriété V dans <Tag ... V=.... />
{
    return xmlGetProp(cur, (const xmlT*) V);
}

inline int GCC_INLINE comp_xml(const xmlT* a, const xmlT* b)  ///< Comparaison de deux chaines. Vaut 0 ssi les deux chaines sont égales
{
    return xmlStrcmp(a, b);
}

inline int GCC_INLINE longueur_xml(const xmlT* s)  ///< Longueur d'une chaine de caractères
{
    return xmlStrlen(s);
}

inline int GCC_INLINE blanc_xml(const xmlN* s)  ///< Vaut 0 si le noeud xml est non vide ou blanc.
{
    return xmlIsBlankNode(s);
}

#endif // XML_LIB_H
