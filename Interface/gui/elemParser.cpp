#ifndef REGEX_PARSING_FOR_HEADERS
#include "elemParser.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

//extern "C" {


struct Header* elem_parser(const char* buffer)
{
   #ifdef DEBUG_MSG
    fprintf(stderr, PROCESSING_HTML_TAG "Premier scan du fichier pour vérifier les années, mois et siret... \n");
   #endif

    Header* elemPar = (Header*) calloc(1, sizeof(Header));

    if (elemPar == nullptr)
    {
        perror("Allocation de mémoire Header.");    // cautious no-op
        return nullptr;
    }


   int i = NB_CHAR_SAUT_ENTETE;  //350+
   bool test = false;

   while (i < MAX_COUNT)
    {
        if  (buffer[++i]  != '<') continue;
        if  (buffer[++i]  != 'A') continue;
        if  (buffer[++i]  != 'n') continue;
        if  (buffer[++i]  != 'n') continue;
        if  (buffer[++i]  != 'e') continue;
        if  (buffer[++i]  != 'e') continue;
        if  (buffer[++i]  != ' ') continue;
        if  (buffer[++i]  != 'V') continue;
        if  (buffer[++i]  != '=') continue;
        if  (buffer[++i]  != '\"') continue;

        elemPar->annee[0] = buffer[++i] ;
        elemPar->annee[1] = buffer[++i] ;
        elemPar->annee[2] = buffer[++i] ;
        elemPar->annee[3] = buffer[++i] ;
        test =     elemPar->annee[0] >= '0' && elemPar->annee[0] <= '9'
               &&  elemPar->annee[1] >= '0' && elemPar->annee[1] <= '9'
               &&  elemPar->annee[2] >= '0' && elemPar->annee[2] <= '9'
               &&  elemPar->annee[3] >= '0' && elemPar->annee[3] <= '9';

        i += SHIFT_MOIS_ANNEE;
        break;
    }

   if (! test)
       fprintf(stderr, "%s %s", "Erreur d'encodage numérique sur Année : \n", elemPar->annee);

   bool test2 = true;

   while (i < MAX_COUNT)
    {
        if  (buffer[++i]  != '<') continue;
        if  (buffer[++i]  != 'M') continue;
        if  (buffer[++i]  != 'o') continue;
        if  (buffer[++i]  != 'i') continue;
        if  (buffer[++i]  != 's') continue;
        if  (buffer[++i]  != ' ') continue;
        if  (buffer[++i]  != 'V') continue;
        if  (buffer[++i]  != '=') continue;
        if  (buffer[++i]  != '\"') continue;

        elemPar->mois[0] = buffer[++i] ;
        elemPar->mois[1] = (buffer[++i] == '\"')? '\0' :  buffer[i++];

        test2 &=   elemPar->mois[0] >= '0' && elemPar->mois[0] <= '9'       // Il y a des encodages du type "01" et d'autres du type "1"
               && (elemPar->mois[1] == '\0' || (elemPar->mois[1] >= '0' && elemPar->mois[1] <= '9'));


        i += SHIFT_SIRET_MOIS;
        break;
    }

   if (! test2)
       fprintf(stderr, "%s", "Erreur d'encodage numérique sur Mois\n");

   bool test3 = true;

   while (i < MAX_COUNT)
    {
        if  (buffer[++i]  != '<') continue;
        if  (buffer[++i]  != 'S') continue;
        if  (buffer[++i]  != 'i') continue;
        if  (buffer[++i]  != 'r') continue;
        if  (buffer[++i]  != 'e') continue;
        if  (buffer[++i]  != 't') continue;
        if  (buffer[++i]  != ' ') continue;
        if  (buffer[++i]  != 'V') continue;
        if  (buffer[++i]  != '=') continue;
        if  (buffer[++i]  != '\"') continue;

        for (int j=0; j < 14; j++)
            test3 &= elemPar->siret[j] >= '0' && elemPar->siret[j] <= '9' ;

        memcpy(elemPar->siret, buffer + i + 1, 14);
        break;
    }

   if (! test3)
       fprintf(stderr, "%s", "Erreur d'encodage numérique sur Siret\n");

    elemPar->test = test && test2 && test3;


    return elemPar;
}
//}
#endif
