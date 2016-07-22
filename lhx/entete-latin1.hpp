#ifndef ENTETE_HPP
#define ENTETE_HPP

/* ce fichier doit �tre encod� en Latin-1 */

static const char* entete_char[]={"R", "Ann�e", "Mois", "Budget", "Employeur",  "Siret", "Etablissement", "Nom", "Pr�nom", "Matricule", "Service", "Nb.Enfants", "Statut", "Temps.de.travail",
                                  "Heures.Sup.", "Heures", "Indice", "Brut", "Net", "Net.�.Payer", "NBI",
                                  "Libell�", "Code",   "Base", "Taux", "Nb.Unit�", "Montant", "Type",
                                  "Emploi", "Grade", "Echelon", "Cat�gorie", "Nir"};

static const char* entete_char_bulletins[]={"R", "Ann�e", "Mois", "Budget", "Employeur", "Siret", "Etablissement", "Nom", "Pr�nom", "Matricule", "Service", "Nb.Enfants", "Statut", "Temps.de.travail",
                                  "Heures.Sup.", "Heures", "Indice", "Brut", "Net", "Net.�.Payer", "NBI",
                                            "Emploi", "Grade", "Evenement", "Echelon", "Cat�gorie", "Nir" };

static std::array<std::string, 13> types_extension = { "traitements",
                                                 "indemnit�s-r�sidence",
                                                 "sft",
                                                 "avantages-nature",
                                                 "indemnit�s",
                                                 "divers",
                                                 "d�ductions",
                                                 "acompte",
                                                 "rappels",
                                                 "retenues",
                                                 "cotisations",
                                                 "commentaires",
                                                 "na"};
#endif // ENTETE_HPP

