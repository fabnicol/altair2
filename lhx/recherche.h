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
#include <fstream>
#include "validator.h"
#include "fonctions_auxiliaires.h"



/// Parcourt l'ensemble des données de paye, pour tous les fils d'exécution, après décodage \n
/// (lhx a complètement exécuté préalablement)\n
/// La recherche est restreinte à l'année, au mois et au matricule donnés \n
/// Lance  extraire_lignes pour extraire les lignes XML correspondantes. \n
/// Cela ne suffit pas à donner un fichier XML syntaxiquement correct. \n
/// A cette fin, rajouter un préambule et une fin de fichier en accord avec ce préambule. \n
/// \param Info Vecteur de structures \e info_t contenant l'information de paye.
/// \param annee Année du/des bulletins(s).
/// \param mois  Mois du/des bulletin(s).
/// \param matricule Matricule de l'agent.
/// \return Vecteur de chaînes de caractères, chacune des chaînes représentant un bulletin XML extrait.

vector<long long>  recherche (const vector<info_t> &Info, const xmlT* annee, const xmlT* mois, const xmlT* matricule);


/// Extrait le bulletin de paye correspondant à la ligne de début et de fin dans le fichier XML base de paye
/// \param info Structure info_t contenant la partie pertinente des données de paye décodées
/// \param debut Tableau de 3 entiers de 64 bits contenant l'indicatrice du début du bulletin particulier à extraire
/// \param fin Tableau de 2 entiers de 64 bits contenant l'indicatrice du fin du bulletin particulier à extraire
/// \return  Chaîne de caractères de type string contenant l'extraction du bulletin

const string extraire_lignes (const info_t& info, const array<uint64_t, 3>& debut, const array<uint64_t, 2>& fin);

/// Crée le répertoire d'exportation d'un bulletin de paye donné pour un matricule, un mois et une année donnés
/// et le fichier XHL minimal encapsulant ce bulletin individuel
/// \param chemin_repertoire Chemin compet du répertoire d'exportation contenant les bulletins extraits
/// \param Info vecteur de structures info_t contenant les données de paye décodées
/// \param matricule Matricule de l'agent
/// \param mois Mois de la paye
/// \param annee Année de la paye
/// \return Liste des chemins des fichiers \em .xml exportés dans le répertoire chemin_repertoire.

vector<string> bulletin_paye (const string& chemin_repertoire,
                    const vector<info_t> &Info,
                    const string& matricule,
                    const string& mois,
                    const string& annee);

/// Lance l'extraction d'un bulletin de paye pour un matricule, un mois et une année donnés
/// \param repertoire_bulletins Référence vers un dossier de bulletins extraits de type \e std::string
/// \param Info vers un vecteur de structures info_t contenant l'ensemble, pour tous les segments et fils, des données de paye décodées
/// \param matricule Référence vers une chaîne de caractère de type string contenant le matricule
/// \param mois Référence vers une chaîne de caractère de type string contenant le mois
/// \param annee Référence vers une chaîne de caractère de type string contenant l'année
/// \return Vecteur de chemins de bulletins \em xml extraits.

vector<string> scan_mois (const string &repertoire_bulletins,
                            const vector<info_t> &Info,
                            const string &matricule,
                            const string &mois,
                            const string &annee);
