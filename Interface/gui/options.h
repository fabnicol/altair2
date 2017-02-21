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
      *tableCheckBox;

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
