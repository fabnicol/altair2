#ifndef ENTETE_HPP
#define ENTETE_HPP

/* ce fichier doit être encodé en ISO-8859-1 pour Windows */

static const char* entete_char[]={"R", "Année", "Mois", "Budget", "Employeur",  "Siret", "Etablissement", "Nom", "Prénom", "Matricule", "Service", "Nb.Enfants", "Statut", "Temps.de.travail",
                                  "Heures.Sup.", "Heures", "Indice", "Brut", "Net", "Net.à.Payer", "NBI", "Libellé", "Code",
                                  "Base", "Taux", "Nb.Unité", "Montant", "Type", "Emploi", "Grade", "Catégorie", "Nir"};

static const char* entete_char_bulletins[]={"R", "Année", "Mois", "Budget", "Employeur", "Siret", "Etablissement", "Nom", "Prénom", "Matricule", "Service", "Nb.Enfants", "Statut", "Temps.de.travail",
                                  "Heures.Sup.", "Heures", "Indice", "Brut", "Net", "Net.à.Payer", "NBI", "Emploi", "Grade", "Catégorie", "Nir"};

std::array<std::string, 12> types_extension = { "traitements",
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
                                                 "commentaires" };

#endif // ENTETE_HPP

