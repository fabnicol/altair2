// Copyright Cour des comptes, 2017
// Contributeur :
// Fabrice Nicol, années 2012 à 2017
// fabrice.nicol@crtc.ccomptes.fr
//
// Ce logiciel est un programme informatique servant à extraire et analyser les fichiers de paye
// produits au format spécifié par l'annexe de la convention-cadre nationale de dématérialisation
// en vigueur à compter de l'année 2008.
//
// Ce logiciel est régi par la licence CeCILL soumise au droit français et
// respectant les principes de diffusion des logiciels libres. Vous pouvez
// utiliser, modifier et/ou redistribuer ce programme sous les conditions
// de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
// sur le site "http://www.cecill.info".
//
// En contrepartie de l'accessibilité au code source et des droits de copie,
// de modification et de redistribution accordés par cette licence, il n'est
// offert aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons,
// seule une responsabilité restreinte pèse sur l'auteur du programme, le
// titulaire des droits patrimoniaux et les concédants successifs.
//
// A cet égard l'attention de l'utilisateur est attirée sur les risques
// associés au chargement, à l'utilisation, à la modification et/ou au
// développement et à la reproduction du logiciel par l'utilisateur étant
// donné sa spécificité de logiciel libre, qui peut le rendre complexe à
// manipuler et qui le réserve donc à des développeurs et des professionnels
// avertis possédant des connaissances informatiques approfondies. Les
// utilisateurs sont donc invités à charger et tester l'adéquation du
// logiciel à leurs besoins dans des conditions permettant d'assurer la
// sécurité de leurs systèmes et ou de leurs données et, plus généralement,
// à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.
//
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
// pris connaissance de la licence CeCILL, et que vous en avez accepté les
// termes.
//
//
#ifndef ENTETE_HPP
#define ENTETE_HPP

// Doit être encodé en Latin-1 pour pouvoir être lu sous Windows


#ifdef __linux__

// Ce tableau est utilisé pour les libellés de colonnes des bases CSV en sortie

// Pour les fichiers Tablesxxx.csv

static const char* entete_char[] = {"R",      // Rang
                                    "Annee",
                                    "Mois",
                                    "Budget",
                                    "Employeur",
                                    "Siret",
                                    "Etablissement",
                                    "Nom", "Prenom",
                                    "Matricule",
                                    "Service",
                                    "Nb.Enfants",
                                    "Statut",
                                    "Temps.de.travail",
                                    "Heures.Sup.",
                                    "Heures",
                                    "Indice",
                                    "Brut",
                                    "Net",
                                    "Net.a.Payer",
                                    "NBI",
                                    "Libelle",
                                    "Code",
                                    "Base",
                                    "Taux",
                                    "Nb.Unite",
                                    "Montant",
                                    "Debut",
                                    "Fin",
                                    "Type",
                                    "Emploi",
                                    "Grade",
                                    "Echelon",
                                    "Categorie",
                                    "Nir"
                                   };

// Pour les fichiers Bulletins.paiexxx.csv

static const char* entete_char_bulletins[] = {"R",
                                              "Annee",
                                              "Mois",
                                              "Budget",
                                              "Employeur",
                                              "Siret",
                                              "Etablissement",
                                              "Nom",
                                              "Prenom",
                                              "Matricule",
                                              "Service",
                                              "Nb.Enfants",
                                              "Statut",
                                              "Temps.de.travail",
                                              "Heures.Sup.",
                                              "Heures",
                                              "Indice",
                                              "Brut",
                                              "Net",
                                              "Net.a.Payer",
                                              "NBI",
                                              "Emploi",
                                              "Grade",
                                              "Evenement",
                                              "Echelon",
                                              "Categorie",
                                              "Nir"
                                             };

#endif

#endif // ENTETE_HPP

