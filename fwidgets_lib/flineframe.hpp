
// Auteur : Fabrice Nicol
// Copyright Fabrice Nicol
// Contributeur :
// Fabrice Nicol, années 2008 à 2012, 2017
// fabrnicol@gmail.com
//
// Ce logiciel est régi par les dispositions du code de la propriété
// intellectuelle. 

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

// Pour l'année 2017, une autorisation d'usage, de modification et de 
// reproduction du présent code est donnée à tout agent employé par les
// juridictions financières. Cette autorisation est temporaire et peut être 
// révoquée.
//


#ifndef FLINEFRAME_HPP
#define FLINEFRAME_HPP
#include "fwidgets.h"
#include "fstring.h"
#include "enums.h"


class FLineFrame : public FLineEdit
{
Q_OBJECT

public:

FLineFrame(const QStringList& ={"", ""},
           const QString& ="",
           const QString& ="",
           const QList<int>& pair = {0,0},
           QGridLayout* inputLayout = nullptr,
           const QString& commandLine ="",
           flags::directory check = directory::checkEmptyness,
           flags::flineframe category = flags::flineframe::isDirectoryPath);


QString getText(){ return lineEdit->text();}
void setText(const QString &t){ lineEdit->setText(t);}
QGridLayout* getLayout(){ return frameLayout;}
QVector<QWidget*> getComponentList() {return componentList;}
void setPathCategory(flags::flineframe t)     {    pathCategory = t;    }

private:

QGridLayout* frameLayout;
FLineEdit* lineEdit;
QLabel *label;
QToolDirButton *sButton;
QToolDirButton *oButton;
QVector<QWidget*> componentList;
flags::flineframe pathCategory;
};

#endif
