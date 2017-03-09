#ifndef EXPRESSION_REG_ELUS_HPP
#define EXPRESSION_REG_ELUS_HPP

#include "expression_reg_commun.hpp"


#define MAIRE                                    "maire"
#define PRESIDENT                            "pr..?sident"
#define ELUS                                      "elus?"
#define ADJOINT_MAIRE                     "adj.*\\bmaire\\b"
#define VICE_PRESIDENT                   "vi.*\\bpr..?sident\\b"
#define CONSEILLER_MUNIC              "cons.*\\bmuni"
#define CONSEILLER_COMMUN          "cons.*\\bcomm"
#define CONSEILLER_DELEGUE          "(?:cons.*\\bd|d..?..?gu)"


static constexpr auto EXPRESSION_REG_ELUS = DEBUT SOIT 
                                                MAIRE
                                                  OU PRESIDENT
                                                  OU ELUS
                                                  OU ADJOINT_MAIRE
                                                  OU VICE_PRESIDENT
                                                  OU CONSEILLER_MUNIC
                                                  OU CONSEILLER_COMMUN
                                                  OU CONSEILLER_DELEGUE
                                                FIN_SOIT
                                                ETC;

#endif // EXPRESSION_REG_ELUS_HPP
