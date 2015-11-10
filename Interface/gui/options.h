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

class standardPage : public common
{
    Q_OBJECT

public :
    standardPage();
    FLineFrame *donneesCSV, *applicationNoyau, *logFrame;

private:
    FComboBox
      *baseTypeWidget,
      *processTypeWidget;
    
    FLineEdit 
     *maxNLigneLineEdit,
     *nLineEdit,
     *NLineEdit;

    QGroupBox
      *baseTypeBox,
      *processTypeBox;

    FCheckBox
      *tableCheckBox,
      *etabCheckBox,
      *rangCheckBox,
      *logCheckBox,
      *consoleCheckBox,
      *economeCheckBox;

    FString
      standardMsg;



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
