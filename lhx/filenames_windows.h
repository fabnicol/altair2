// Copyright Cour des comptes, 2017
// Contributeur :
// Fabrice Nicol, ann�es 2012 � 2017
// fabrice.nicol@crtc.ccomptes.fr
//
// Ce logiciel est un programme informatique servant � extraire et analyser les fichiers de paye
// produits au format sp�cifi� par l'annexe de la convention-cadre nationale de d�mat�rialisation
// en vigueur � compter de l'ann�e 2008.
//
// Ce logiciel est r�gi par la licence CeCILL soumise au droit fran�ais et
// respectant les principes de diffusion des logiciels libres. Vous pouvez
// utiliser, modifier et/ou redistribuer ce programme sous les conditions
// de la licence CeCILL telle que diffus�e par le CEA, le CNRS et l'INRIA
// sur le site "http://www.cecill.info".
//
// En contrepartie de l'accessibilit� au code source et des droits de copie,
// de modification et de redistribution accord�s par cette licence, il n'est
// offert aux utilisateurs qu'une garantie limit�e. Pour les m�mes raisons,
// seule une responsabilit� restreinte p�se sur l'auteur du programme, le
// titulaire des droits patrimoniaux et les conc�dants successifs.
//
// A cet �gard l'attention de l'utilisateur est attir�e sur les risques
// associ�s au chargement, � l'utilisation, � la modification et/ou au
// d�veloppement et � la reproduction du logiciel par l'utilisateur �tant
// donn� sa sp�cificit� de logiciel libre, qui peut le rendre complexe �
// manipuler et qui le r�serve donc � des d�veloppeurs et des professionnels
// avertis poss�dant des connaissances informatiques approfondies. Les
// utilisateurs sont donc invit�s � charger et tester l'ad�quation du
// logiciel � leurs besoins dans des conditions permettant d'assurer la
// s�curit� de leurs syst�mes et ou de leurs donn�es et, plus g�n�ralement,
// � l'utiliser et l'exploiter dans les m�mes conditions de s�curit�.
//
// Le fait que vous puissiez acc�der � cet en-t�te signifie que vous avez
// pris connaissance de la licence CeCILL, et que vous en avez accept� les
// termes.
//
//
#ifndef FILENAMES_HPP
#define FILENAMES_HPP
#ifdef _WIN32
#include <array>

/// \file filenames.h
/// \brief Noms donn�s aux fichiers CSV en sortie, lorsque l'exportation est par
/// type de ligne de paye (cat�gorie de ligne de paye dans le lexique Xemelios)
/// \note Ce fichier doit �tre encod� en Latin-1

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
                                                       "na"
                                                     };

#endif
#endif
