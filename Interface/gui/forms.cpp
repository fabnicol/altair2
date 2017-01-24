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
