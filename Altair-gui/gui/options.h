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
    //FLineEdit* aspectRatioLineEdit;
    //FLineEdit* baseTypeLineEdit;

private:
    FComboBox
      *baseTypeWidget,
      *processTypeWidget;
    
    FLineEdit 
     *sepLineEdit,
     *decLineEdit,
     *maxNLigneLineEdit,
     *nLineEdit,
     *NLineEdit;

    QGroupBox
      *baseTypeBox,
      *processTypeBox;

    FCheckBox
      *tableCheckBox,
      *economeCheckBox;

    FString
      //aspectRatioMsg,
      standardMsg;

 private slots :
    void on_baseTypeWidgetChanged(int){}
    void on_processTypeWidgetChanged(int){}

};


class options :  public common
{
    Q_OBJECT

public:

    options(Altair* parent=0);
    standardPage* standardTab;
    static int RefreshFlag;
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
