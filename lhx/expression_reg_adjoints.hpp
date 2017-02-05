#ifndef EXPRESSION_REG_ADJOINTS_HPP
#define EXPRESSION_REG_ADJOINTS_HPP

#include "expression_reg_commun.hpp"

#define ADMINISTRATIFS   "adm"
#define ANIMATION            "ani"
#define TECHNIQUES         "tech"
#define PATRIMOINE          "pat"
#define OPERATEUR           "ope.*"
#define AIDE_LABO            "aide.*\\blab"  
#define AUXILIAIRE             "aux.*"
#define PUERICULTRICE      "puer"  
#define SOIN                       "soin"  
#define GARDIEN                "gard(?:ien|.*ch)"
#define POLICIER                 "brigadier|sapeur|capor|sous.*off"  
#define RECEVEUR              "receveur"  

static constexpr auto EXPRESSION_REG_ADJOINTS = AUCUN_MOT
                                                                                               SOIT ADJOINTS 
                                                                                                                 SOIT 
                                                                                                                            ADMINISTRATIFS OU ANIMATION OU TECHNIQUES OU PATRIMOINE 
                                                                                                                 FIN_SOIT
                                                                                                      OU OPERATEUR
                                                                                                                  SOIT
                                                                                                                           "a\\.?p\\.?s\\.?|act"
                                                                                                                  FIN_SOIT
                                                                                                      OU AIDE_LABO
                                                                                                      OU AUXILIAIRE 
                                                                                                                  SOIT 
                                                                                                                             PUERICULTRICE OU SOIN
                                                                                                                  FIN_SOIT
                                                                                                      OU GARDIEN
                                                                                                      OU POLICIER
                                                                                                      OU RECEVEUR
                                                                                              FIN_SOIT 
                                                                                     ETC;

#endif // EXPRESSION_REG_ADJOINTS_HPP
