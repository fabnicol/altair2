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
#ifndef OPTIONS_H
#define OPTIONS_H

#include "flineframe.hpp"
#include "flistframe.h"
#include "common.h"


class options;
class Altair;
class RAltairPage;
class QToolDirButton;
class FListFrame;
class FLineFrame;


class processPage : public common
{
    Q_OBJECT

public :
    processPage();
    FLineFrame *logFrame;

private:
    FComboBox
      *processTypeWidget,
      *memoryUseWidget;
    
#   ifdef INSERT_MAXN
      FLineEdit
        *nLineEdit,
        *NLineEdit;
#   endif
      
    QGroupBox
      *processTypeBox;

    FCheckBox
      *logCheckBox,
      *consoleCheckBox;

    FString
      standardMsg;

    QLabel *nLineLabel, *NLineLabel;

};



class standardPage : public common
{
    Q_OBJECT

public :
    standardPage();
    FCheckBox
      *tableCheckBox, *FPHCheckBox;

private:
    FComboBox
      *baseTypeWidget,
      *processTypeWidget;

    FLineEdit
     *maxNLigneLineEdit;

    QGroupBox
      *baseTypeBox;

    FCheckBox
      *etabCheckBox,
      *rangCheckBox;

    FString
      standardMsg;

    QLabel *maxNLigneLabel;

private slots:
    void substituer_versant();

};

#ifdef INSERT_DIRPAGE
class dirPage : public common
{
    Q_OBJECT

public :
    dirPage();
    FLineFrame *donneesCSV, *applicationNoyau;

};
#endif

class codePage : public common
{
    Q_OBJECT

public :
    codePage();
    void resetLabel() {label->setText("") ;}

private:
    QString prologue_codes_path;
    QList<FLineEdit*> listeCodes;
    QList<QString> listeLabels;
    QLabel *label;
    QString init_label_text ;
    QToolButton* appliquerCodes ;
    QGridLayout *vLayout = new QGridLayout;

    inline int ajouterVariable(const QString& nom);
    bool reinitialiser_prologue();

private slots:
    void substituer_valeurs_dans_script_R();
};


class options :  public common
{
    Q_OBJECT

public:

    options(Altair* parent=0);
    standardPage* standardTab;
    processPage* processTab;
#   ifdef INSERT_DIRPAGE
      dirPage* dirTab;
#   endif      
    codePage* codeTab;
    static std::uint16_t RefreshFlag;
    QListWidget *contentsWidget;
    void clearOptionData();

signals:

    void defaultClick(bool);


private:

    QDialogButtonBox *closeButton;
    QStackedWidget *pagesWidget;

    void createIcons();
    void createIcon(const char* path, const char* text);

private slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);


};
#endif // OPTIONS_H
