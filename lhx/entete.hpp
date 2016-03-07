#ifndef ENTETE_HPP
#define ENTETE_HPP

/* ce fichier doit être encodé en UTF-8 pour Unix */

static const char* entete_char[]={"R", "Année", "Mois", "Budget", "Employeur",  "Siret", "Etablissement", "Nom", "Prénom", "Matricule", "Service", "Nb.Enfants", "Statut", "Temps.de.travail",
                                  "Heures.Sup.", "Heures", "Indice", "Brut", "Net", "Net.à.Payer", "NBI",
                                  "Libellé", "Code",   "Base", "Taux", "Nb.Unité", "Montant", "Type",
                                  "Emploi", "Grade", "Echelon", "Catégorie", "Nir"};

static const char* entete_char_bulletins[]={"R", "Année", "Mois", "Budget", "Employeur", "Siret", "Etablissement", "Nom", "Prénom", "Matricule", "Service", "Nb.Enfants", "Statut", "Temps.de.travail",
                                  "Heures.Sup.", "Heures", "Indice", "Brut", "Net", "Net.à.Payer", "NBI",
                                            "Emploi", "Grade", "Echelon", "Catégorie", "Nir"};

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
#endif // ENTETE_HPP

