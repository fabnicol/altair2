#ifndef BROWSER_H
#define BROWSER_H

#include <QtWidgets>
#ifndef STATIC
    #include <QtWebKitWidgets>
    #include <QWebView>
    class QWebView;
#endif
#include "common.h"

class QToolButton;


class browser : public QWidget
{
  Q_OBJECT
public:
#ifndef STATIC
   browser(const QUrl & url,  QWidget *parent = 0);
#endif
   static void showPage(const QUrl &url);
#ifndef STATIC
 private:
   QWebView *textBrowser  = new QWebView;
   QToolButton *homeButton = new QToolButton ;
   QToolButton *backButton = new QToolButton ;
   QToolButton *forwardButton = new QToolButton ;
   QToolButton *closeButton =new QToolButton;
   QUrl url;

public slots:
   void updateWindowTitle();
   void home();
#endif
};

#endif // BROWSER_H
