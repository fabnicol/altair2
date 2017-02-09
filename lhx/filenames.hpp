#ifndef FILENAMES_HPP
#define FILENAMES_HPP
#ifdef __linux__
#include <array>

// doit être encodé en UTF-8

static std::array<std::string, 13> types_extension = { "traitements",
                                                 "indemnités-résidence",
                                                 "sft",
                                                 "avantages-nature",
                                                 "indemnités",
                                                 "divers",
                                                 "déductions",
                                                 "acompte",
                                                 "rappels",
                                                 "retenues",
                                                 "cotisations",
                                                 "commentaires",
                                                 "na"};

#endif
#endif
