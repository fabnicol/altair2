#ifndef EXPRESSION_REG_ADJOINTS_HPP
#define EXPRESSION_REG_ADJOINTS_HPP

#include "expression_reg_commun.hpp"

#define ADMINISTRATIFS   "adm.*"
#define ANIMATION        "ani.*"
#define TECHNIQUES       "tech.*"
#define PATRIMOINE       "pat.*"
#define OPERATEUR        "ope.*"
#define AIDE_LABO        "aide.*\\blab.*"
#define AUXILIAIRE       "aux.*"
#define PUERICULTRICE    "puer.*"
#define SOIN             "soin"
#define GARDIEN          "gard(?:ien|.*ch.*)"
#define SOUS_OFF         "brigadier.*|sapeur.*|capor.*|sous.*off.*|adju.*|serg.*|major"
#define OFFICIER         "(?:sous-?)lieut.*(?:col.*)|capit.*|com.*d.*t|colon.*|g.?.?n.?.?ral|.*amiral"
#define RECEVEUR         "receveur"
#define ADJOINT          "adj.*"


//  SOIT A OU B OU ...  FIN_SOIT  OU ... OU ...

static constexpr auto EXPRESSION_REG_ADJOINTS = AUCUN_MOT
                                                 SOIT
                                                   ADJOINT
                                                       SOIT
                                                         ADMINISTRATIFS
                                                         OU ANIMATION
                                                         OU TECHNIQUES
                                                         OU PATRIMOINE
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
                                                   OU SOUS_OFF
                                                   OU RECEVEUR
                                                FIN_SOIT
                                                ETC;

#endif // EXPRESSION_REG_ADJOINTS_HPP
