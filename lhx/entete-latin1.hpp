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
#ifndef __linux__
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
#endif
#endif // ENTETE_HPP

