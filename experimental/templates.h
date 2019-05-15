#ifndef TEMPLATES_H
#define TEMPLATES_H
#include "validator.hpp"

template<typename T=std::string, typename U=uint64_t, typename V=int, typename W=int> struct quad { T value;  U size; V elements; W status;};

/* Caractérisation des fichiers découpés ou pas :
 *    quad file;
 *    découpé : file.elements > 1 sinon entier
 */
#endif // TEMPLATES_H

