#ifndef ENTETE_HPP
#define ENTETE_HPP

// Doit être encodé en Latin-1 pour pouvoir être lu sous Windows

#ifndef CONVERTIR_LATIN_1
#ifdef __linux__ 

static const char* entete_char[]={"R", "Année", "Mois", "Budget", "Employeur",  "Siret", "Etablissement", "Nom", "Prénom", "Matricule", "Service", "Nb.Enfants", "Statut", "Temps.de.travail",
                                  "Heures.Sup.", "Heures", "Indice", "Brut", "Net", "Net.à.Payer", "NBI",
                                  "Libellé", "Code",   "Base", "Taux", "Nb.Unité", "Montant", "Type",
                                  "Emploi", "Grade", "Echelon", "Catégorie", "Nir"};

static const char* entete_char_bulletins[]={"R", "Année", "Mois", "Budget", "Employeur", "Siret", "Etablissement", "Nom", "Prénom", "Matricule", "Service", "Nb.Enfants", "Statut", "Temps.de.travail",
                                  "Heures.Sup.", "Heures", "Indice", "Brut", "Net", "Net.à.Payer", "NBI",
                                            "Emploi", "Grade", "Evenement", "Echelon", "Catégorie", "Nir" };

#endif
#endif
#endif // ENTETE_HPP

