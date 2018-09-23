#ifndef XMLUNDEF_H
#define XMLUNDEF_H

#ifdef XMLCONV_H
#undef xmlFree 
#undef XMLDocument  
#undef XMLElement* 
#undef char    
#undef xmlGetRootElement
#undef FirstChild()  
#undef next 

#undef xmlStrcmp
#undef XMLCONV_H
#endif

#endif // XMLUNDEF_H
