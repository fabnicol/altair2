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

    FLineEdit
     *nLineEdit,
     *NLineEdit;

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

private:
    FComboBox
      *baseTypeWidget,
      *processTypeWidget;

    FLineEdit 
     *maxNLigneLineEdit;

    QGroupBox
      *baseTypeBox;

    FCheckBox
      *tableCheckBox,
      *etabCheckBox,
      *rangCheckBox;

    FString
      standardMsg;

    QLabel *maxNLigneLabel;

};

class dirPage : public common
{
    Q_OBJECT

public :
    dirPage();
    FLineFrame *donneesCSV, *applicationNoyau;

};


class codePage : public common
{
    Q_OBJECT

public :
    codePage();
    
private:    
    FLineEdit *NBILineEdit;
    FLineEdit *PFILineEdit;
    QList<FLineEdit*> listeCodes;
    QList<QString> listeLabels;
    QLabel *label;
    QString init_label_text ;
    QToolButton* appliquerCodes ;
private slots:    
    void substituer_valeurs_dans_script_R();
    bool reinitialiser_prologue();

};


class options :  public common
{
    Q_OBJECT

public:

    options(Altair* parent=0);
    standardPage* standardTab;
    processPage* processTab;
    dirPage* dirTab;
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
