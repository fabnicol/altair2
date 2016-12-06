#ifndef ANALYSEUR_H
#define ANALYSEUR_H

#include <regex>
#include <string>
#include <iostream>
#include <ctype.h>
#include <mutex>

#include "commandline_handler.h"
#include "thread_handler.h"

extern bool verbeux;

//bclass Commandline;
//class thread_handler;

class Analyseur
{
friend class thread_handler;

public:
    Analyseur(Commandline&);
    ~Analyseur();


    static vector<errorLine_t>  errorLineStack;
    void lanceur(Commandline& );
    int produire_segment(Commandline&, int rang_segment);
    static void warning_msg(const char* noeud, const info_t& info, const xmlNodePtr cur);

private:
    static int parseFile(info_t& info);
    static int parseFile(const xmlDocPtr doc, info_t& info, int cont_flag, xml_commun* champ_commun);
    static void* decoder_fichier(info_t& info);
    static int memoire_p_ligne(const info_t& info, const unsigned agent);
    static void allouer_memoire_table(info_t& info);

    int nb_fil;
    int nb_segment;
    thread_handler* gestionnaire_fils;

};



#endif // ANALYSEUR_H

