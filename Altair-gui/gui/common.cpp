#include "common.h"

FString common::htmlLogPath;
QString common::tempdir=QDir::homePath ()+QDir::separator()+"tempdir";  // should be equal to main app globals.settings.tempdir=TEMPDIR;


qint64 common::getDirectorySize(const QString &path, const QString &extension)
{
    QDir dir(path);
    qint64 size=0;
    QStringList filters;
    filters+=extension;

    foreach (QString file, dir.entryList(filters, QDir::Files))
        size+=QFileInfo(dir, file).size();

    foreach (QString subDir, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
        size+=getDirectorySize(path+QDir::separator()+subDir, extension);

    return size;
}

/* Disregard compiler whinig here, a spurious argument is used to ensure a higher-order approach to FProgressBar */

qint64 common::getFileSize(const QString &fullFilePath, const QString & voidstring)
{
    return QFileInfo(fullFilePath).size();
}


void common::writeFile(QString & path, const QStringList &list, QFlags<QIODevice::OpenModeFlag> flag)
{
    QFile data(path);
    if (data.open(flag))
    {
        QTextStream out(&data);
        QStringListIterator i(list);
        while (i.hasNext())
            out  << i.next() << "\n";
    }
    data.close();
}


int common::readFile(QString &path, QStringList &list, int start, int stop, int width)
{
QFile file(path);
int j=0;
if (file.open(QIODevice::ReadOnly | QIODevice::Text))
{
  QTextStream in(&file);
  in.seek(0);
  while (++j < start)  in.readLine();
  while (!in.atEnd() )
  {
      QString line = in.readLine(width);
      list << line;
      if  (j == stop) break;
      j++;
  }
}
else QMessageBox::warning(this, tr("Attention"), tr("Impossible d'ouvrir les bulles: ") + path );
file.close();
return j;

}

QString common::readFile(QString &path,  int start, int stop, int width)
{
QFile file(path);
QStringList L=QStringList();
readFile(path, L, start, stop, width);
QString string=L.join("\n");
return string;
}

QString common::generateDatadirPath(const QString &path)
{
  QString pathstr= QDir::cleanPath(  QStandardPaths::writableLocation(QStandardPaths::DataLocation) + ((path.isEmpty())?"":QString("/"))+path);
  return pathstr;
}

QString common::generateDatadirPath(const char* path)
{
  const QString str= QString(path);
  QString pathstr= QDir::cleanPath(  QStandardPaths::writableLocation(QStandardPaths::DataLocation) + ((path[0]=='\0')?"":"/")+ str);
  return pathstr;
}

void common::setWhatsThisText(QWidget* widget, int start, int stop)
{
  widget->setWhatsThis("<html>"+readFile(whatsThisPath, 2, 2)+readFile(whatsThisPath, start, stop)+"</html>");
}

void common::openDir(QString path)
{
  if (path.isEmpty()) return;
  if (!QFileInfo(path).isDir())
    {
      QMessageBox::warning(this, "", path + " n'est pas un répertoire.");
      return;
    }

QUrl url("file:///" + path);
QDesktopServices::openUrl(url);
}

bool common::openDirDialog()
{
QString path=QFileDialog::getExistingDirectory(this, QString("Sélection du répertoire"),
                                               QDir::currentPath(),
                                               QFileDialog::ShowDirsOnly
                                               | QFileDialog::DontResolveSymlinks);
if (path.isEmpty()) return false;

qint64 size=common::getDirectorySize(path, "*");

if (size)
{
    if (QMessageBox::warning(0, QString("Répertoire"), QString("Le répertoire %1 n'est pas vide (Taille %2B). Ecraser et recréer ? ").arg(path,QString::number(size)), QMessageBox::Ok | QMessageBox::Cancel)
            == QMessageBox::Ok)
    {
        QDir targetDirObject(path);
        if (!targetDirObject.removeRecursively())    QMessageBox::information(0, QString("Supprimer le répertoire"),
                                                       QString("Le répertoire n'a pas été supprimé' %1").arg(QDir::toNativeSeparators(path)));

        else
        if (targetDirObject.mkpath(path) == false)
        {
            QMessageBox::warning(0, QString("Répertoire"), QString("Le répertoire %1 n'a pas été créé").arg(path), QMessageBox::Ok);
            return false;
        }
    }
}

return true;
}


// dynamic allocation is obligatory
//  ImageViewer *v = new ImageViewer(videoMenuLineEdit->setXmlFromWidget());
//  v->show();

