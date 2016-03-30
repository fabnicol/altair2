#ifndef THREAD_HANDLER_H
#define THREAD_HANDLER_H


#include "commandline_handler.h"
class Commandline;

class thread_handler
{
friend class Analyseur;

public:
    thread_handler(Commandline&, int rang_segment);

private:
    vector<thread> fils;
    vector<info_t> Info;
    void redecouper_volumineux(info_t& info);
    void redecouper(info_t& info);
    int nb_fil = 1;
};



#endif // THREAD_HANDLER_H

