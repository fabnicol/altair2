#ifndef LPLEX_H
#define LPLEX_H


#include "common.h"
#include "altair.h"

class FComboBox;
class FLineEdit;
class FCheckBox;
class altair;

class lplexPage : public common
{
    Q_OBJECT

public:
    lplexPage(altair* parent);

private slots:
   void on_lplexDirButton_clicked();
   void on_lplexInfoDirButton_clicked();
   void on_lplexBackgroundButton_clicked();
   void resetVideoProjectFeatures(bool);
private:
    altair* altairParent;
    FCheckBox* launchLplexAloneBox;
    FComboBox *lplexVideoType, *lplexSpliceType, *lplexShiftType, *lplexCreateType, *lplexMediaType;
    FLineEdit *lplexBackgroundLineEdit, *lplexInfoDirLineEdit, *lplexDirLineEdit;
    FCheckBox *lplexScreenParameterBox, *lplexMd5AwareBox, *lplexInfofilesBox, *lplexRescaleBox, *lplexInfoDirBox;
};


#endif // LPLEX_H
