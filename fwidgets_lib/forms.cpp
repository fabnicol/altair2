
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
#include "forms.h"



FRichLabel::FRichLabel(const QString &title, flags::font flag) : QWidget()
{
  QHBoxLayout* mainLayout=new QHBoxLayout;
  QLabel *label=new QLabel;
  
  switch (flag)
    {
      case flags::font::regularTitle:
        label->setText(title);break;

      case flags::font::boldTitle:
        label->setText("<b>"+title+"</b>"); break;

      case flags::font::italicTitle:
        label->setText("<i>"+title+"</i>"); break;
    }

  label->setFixedHeight(20);
  

  mainLayout->addStretch(100);
  mainLayout->addWidget(label, Qt::AlignRight);
  
  setLayout(mainLayout);
}
