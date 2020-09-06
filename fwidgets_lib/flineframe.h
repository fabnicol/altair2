// Auteur : Fabrice Nicol
// Copyright Fabrice Nicol
// Contributeur :
// Fabrice Nicol, années 2008 à 2012, 2017
// fabrnicol@gmail.com
//
// Ce logiciel est régi par les dispositions du code de la propriété
// intellectuelle (CPI).

// L'auteur se réserve le droit d'exploitation du présent logiciel, 
// et notamment de reproduire et de modifier le logiciel, conformément aux 
// dispositions de l'article L.122-6 du même code. L'auteur se réserve le droit
// de modifier et de corriger les erreurs du logiciel, conformément aux termes 
// du deuxième alinéa du I de l'article L.122-6-1 du même code.

// Le présent logiciel doit être regardé comme un "Module Externe", pour 
// l'application des stipulations de la licence CeCILL, lorsque le code source, 
// ou une bibliothèque compilée à partir du présent code, sont utilisés pour 
// compiler tout logiciel faisant usage du présent code.

// Tout fichier contenant le présent code, ou des versions antérieures du 
// présent code, et faisant apparaître des termes contraires aux stipulations
// précédentes doit être regardé comme faisant partie d'actes préparatoires à
// l'élaboration du présent logiciel et non comme le logiciel lui-même, ou 
// comme des erreurs matérielles dans la copie des licences. Seules les 
// stipulations précédentes doivent être regardées comme s'appliquant au 
// présent logiciel.
//
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
// pris connaissance de ces stipulations et que vous en avez accepté les
// termes.

// Sans préjudice des dispositions du CPI, une autorisation d'usage et de
// reproduction du présent code est donnée à tout agent employé par les
// juridictions financières pour l'exercice de leurs fonctions publiques.
// Le code ainsi mis à disposition ne peut être transmis à d'autres utilisateurs.
//

/// \file flineframe.h
/// \author Fabrice Nicol
/// \date Janvier 2010
/// \brief Ce fichier contient la classe FLineFrame, dérivée fonctionnelle de FLineEdit

#ifndef FLINEFRAME_HPP
#define FLINEFRAME_HPP
#include "fwidgets.h"
#include "fstring.h"
#include "enums.h"

/// Ligne de saisie de texte FLineEdit enrichie de fonctionnalités

class FLineFrame : public FLineEdit
{
Q_OBJECT

public:

FLineFrame(const QStringList& ={"", ""},                                   ///< Titre de la ligne et 3e argument de  lineEdit
           const QString& ="",                                             ///< Texte par défaut de  lineEdit
           const QString& ="",                                             ///< Balise XML de  lineEdit
           const QList<int>& pair = {0,0},                                 ///< Ligne et colonne d'insertion dans la disposition en grille
           QGridLayout* inputLayout = nullptr,                             ///< Disposition de rattachement s'il y a lieu
           const QString& commandLine ="",                                 ///< Ligne de commande générée par  lineEdit, s'il y en a une
           flags::directory check = directory::checkEmptyness,             ///< Vérifier que le répertoire indiqué dans  lineEdit est vide
           flags::flineframe category = flags::flineframe::isDirectoryPath, ///< Chemin par défaut = chemin de répertoire dans  lineEdit
           const QString& filter = "Fichier Log (*.log)"                    ///< Accesseur en écriture : précise le filtre de fichiers pour la boîte de dialogues d'importation dans la ligne (défaut : "Fichier Log (*.log)")  
        );

/// Accesseur en lecture : retourne le texte de la ligne
/// \return Chaîne de caractères \e QString de la ligne

QString getText(){ return lineEdit->text();}

/// Accesseur en écriture : édite le texte de la ligne

void setText(const QString &t){ lineEdit->setText(t);}

/// Accesseur en lecture de la disposition (Layout)
/// \return Pointeur vers une disposition de type \e QGridLayout

QGridLayout* getLayout(){ return frameLayout;}

/// Accesseur en lecture de la liste des composants  componentList
/// \return Vecteur de pointeurs vers chacun des QWidget composant

QVector<QWidget*> getComponentList() {return componentList;}

/// Accesseur en écriture de pathCategory, type de chemin représenté par lineEdit

void setPathCategory(flags::flineframe t)     {    pathCategory = t;    }

/// Accesseur en écriture de saveFileName

void setSaveFileName(bool value) { saveFileName = value; }

/// Fixer la police (type et taille)

void setFont(const char* type, const int size) { label->setFont(QFont(type, size)); lineEdit->setFont(QFont(type, size));}

void setToolTip(const QString& s) {label->setToolTip(s);}

private:

QGridLayout* frameLayout; ///< Disposition de la FLineFrame de type \e QGridLayout
FLineEdit* lineEdit;      ///< Ligne à éditer, noyau du composant
QLabel *label;            ///< Libellé du composant
QToolDirButton *sButton;  ///< Bouton permettant d'ouvrir un dialogue de sélection de répertoire
QToolDirButton *oButton;  ///< Bouton permettant d'ouvrir le répertoire ou le fichier sélectionné dans  lineEdit
QVector<QWidget*> componentList; ///< Liste des composants de type \e QWidget*
flags::flineframe pathCategory;  ///< Type de chemin représenté dans  lineEdit : soit chemin de fichier soit chemin de répertoire
bool saveFileName = true;  ///< Si \e false, utilise \e QFileDialog::getSaveFileName sinon utilise \e QFileDialog::getOpenFileName

signals:

void textChanged(const QString &);
};

#endif
