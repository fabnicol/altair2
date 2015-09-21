#ifndef OPTIONS_H
#define OPTIONS_H


#include "flistframe.h"
#include "common.h"


class options;
class Altair;
class RAltairPage;
class QToolDirButton;
class FListFrame;

class standardPage : public common
{
    Q_OBJECT

public :
    standardPage();
    FLineEdit *xhlLineEdit;

private:
    FComboBox
      *baseTypeWidget,
      *processTypeWidget;
    
    FLineEdit 
     *baseLineEdit,
     *logLineEdit,
     *maxNLigneLineEdit,
     *nLineEdit,
     *NLineEdit;

    QGroupBox
      *baseTypeBox,
      *processTypeBox;

    FCheckBox
      *tableCheckBox,
      *rangCheckBox,
      *logCheckBox,
      *economeCheckBox;

    FString
      standardMsg;

 private slots :

    void on_processTypeWidgetChanged(int){}
    void on_openDirButton_clicked(const FLineEdit*);
    void selectBaseOutput(FLineEdit*, flags::directory = directory::noCheck);
    void selectLogOutput();
};


class options :  public common
{
    Q_OBJECT

public:

    options(Altair* parent=0);
    standardPage* standardTab;
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



};
#endif // OPTIONS_H
