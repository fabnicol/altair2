#ifndef XMLUNDEF_H
#define XMLUNDEF_H

#ifdef XMLCONV_H
#undef xmlFree 
#undef xmlDocPtr  
#undef xmlNodePtr 
#undef xmlChar    
#undef xmlGetRootElement
#undef xmlChildrenNode  
#undef next 
#undef previous 
#undef xmlGetProp 
#undef xmlStrdup
#undef xmlGetLineNo
#undef xmlStrcmp
#undef XMLCONV_H
#endif

#endif // XMLUNDEF_H
