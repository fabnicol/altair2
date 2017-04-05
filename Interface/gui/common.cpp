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
#include "common.h"

FString common::htmlLogPath;
QString common::tempdir=QDir::homePath ()+QDir::separator()+"tempdir";  // should be equal to main app globals.settings.tempdir=TEMPDIR;
QString  const common::systemPathPrefix = "/../../../";
QString common::userdatadir;

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

qint64 common::getFileSize(const QString &fullFilePath)
{
    return QFileInfo(fullFilePath).size();
}

int common::readFile(const QString &path, QStringList &list, int start, int stop, int width)
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
else
{
    Warning(tr("Attention"), tr("Impossible d'ouvrir le fichier: ") + path );
}

file.close();
return j;

}

bool common::renommer(const QString& ancien, const QString& nouveau)
{
    QFile fout(ancien);
    QFile(nouveau).remove();
    return fout.rename(nouveau);
}

const QString common::getEmbeddedPath(QString s, QString subDir)
{
#   ifdef Q_OS_WIN
      s = QDir::toNativeSeparators(s);
      subDir = QDir::toNativeSeparators(subDir);
#   endif

    QString rootDir;

    int j = 0;

    if (subDir.isEmpty())
    {
#     ifdef Q_OS_WIN
        const QString &cdROM = "D:\\";
        const QString &testCDROM = "D:/";
#     else
        const QString &cdROM = "cdrom/";
        const QString &testCDROM = cdROM;
#     endif

      if (v(XHL).contains(testCDROM))
      {
          rootDir = cdROM;
#         ifndef Q_OS_WIN
            j = 1;
#         endif
      }
      else
      {
          rootDir = userdatadir;
      }
    }
    else
    {
        rootDir = subDir;
    }

    const QString section = s.section(rootDir, j, j, QString::SectionSkipEmpty);

    if (section.isEmpty())
        return (QFileInfo(s).fileName());
    return section;
}

QString common::dump(const QString &chaine)
{
    QString temp_path = path_access("temp");

    QFile fout(temp_path);
    fout.open(QIODevice::WriteOnly);
    QTextStream out (&fout);
    out << chaine;
    fout.close();
    return temp_path;
}

bool common::substituer(const QString& s, const QString& repl,  QString& file_str)
{
    QRegExp reg = QRegExp(s);
    reg.setPatternSyntax(QRegExp::RegExp2);

    file_str.replace(reg, repl);
    return (! file_str.isEmpty());
}


void common::exporter_identification_controle(QString &file_str)
{
    const QString &employeur = Hash::aplatir(Hash::Employeur, ", ");
    const QString &budget = Hash::aplatir(Hash::Budget, ", ");
    const QString &siret = Hash::aplatir(Hash::Siret, " - ");
    const QString &etablissement = Hash::aplatir(Hash::Etablissement, " - ");

    substituer("controle<-c\\(\"\",\"\",\"\",\"\"\\)", "controle<-c(\""
                                                 + employeur +"\",\""
                                                 + siret + "\",\""
                                                 + etablissement + "\",\""
                                                 + budget + "\")",
               file_str);
}

void common::exporter_identification_controle()
{
    const QString &prologue_code_path = path_access("Tests/Exemple/prologue_codes.R");
    QString file_str = readFile(prologue_code_path);
    exporter_identification_controle(file_str);
    renommer(dump(file_str), prologue_code_path);
}

QString common::readFile(const QString &path,  int start, int stop, int width)
{
    QStringList L=QStringList();
    readFile(path, L, start, stop, width);
    QString string=L.join("\n");
    return string;
}

bool common::IOControl(const QString& in,
                       const QString& out,
                       const QString& comment,
                       bool require)
{
    QFile inFile(in);
    if (! inFile.exists())
    {
        if (require == true) QMessageBox::critical(nullptr,
                                                   "Erreur",
                                                   in + " n'a pas été trouvé.",
                                                   QMessageBox::Cancel);
        return false;
    }

    QFile newfile(out);

    if (newfile.exists())
    {
        int test = QMessageBox::Ok;
        if (! comment.isEmpty())
                test = QMessageBox::warning(nullptr, "Ecraser le fichier ?", comment + 
                                            " va être écrasé(e).\n"
                                            "Appuyer sur Oui pour confirmer, Non pour quitter.",
                                            QMessageBox::Ok|QMessageBox::Cancel);

        if (test != QMessageBox::Ok)   return false;
        else
        {
               if (newfile.remove() == false)
               {
                   if (! comment.isEmpty())
                       QMessageBox::critical(nullptr, 
                                             "Erreur", 
                                             "La version précédente du fichier " 
                                             + out + " n'a pas pu être effacée.",
                                             QMessageBox::Cancel);
                   return false;
               }
        }
    }

    const QString copyDirStr = QFileInfo(out).path();
    QDir copyDir(copyDirStr);
    if (! copyDir.exists())
    {
        if  (QFileInfo(copyDirStr).isFile())
        {
          int result = QMessageBox::warning(nullptr, 
                                            "Ecraser le fichier ?",
                                            copyDirStr
                                            + " sera écrasé.\n"
                                              "Appuyer sur Oui pour confirmer, Non pour quitter.",
                                            QMessageBox::Ok|QMessageBox::Cancel);

          if (result != QMessageBox::Ok)   return false;
          else
          {
                 QFile newfile(copyDirStr);
                 if (newfile.remove() == false)
                 {
                     QMessageBox::critical(nullptr, 
                                           "Erreur",
                                           "Le fichier "
                                           + copyDirStr + " n'a pas pu être effacé.",
                                           QMessageBox::Cancel);
                     return false;
                 }
          }
        }

        if (copyDir.mkpath(copyDirStr) == false)
        {
            QMessageBox::critical(nullptr, 
                                  "Erreur",
                                  "Le dossier "
                                  + copyDirStr
                                  + " n'a pas pu être créé.",
                                  QMessageBox::Cancel);
            return false;
        }
    }

    return true;
}


bool common::copyFile(const QString &in,
                      const QString &out,
                      const QString& comment,
                      bool require)
{
    if (false == IOControl(in, out, comment, require))
        return false;

    QFile inFile(in);

    if (inFile.copy(out) == false && require == true)
    {
        if (! comment.isEmpty()) 
            QMessageBox::critical(nullptr, 
                                  "Erreur", 
                                  comment + " n'a pas été copié(e).",
                                  QMessageBox::Cancel);
        return false;
    }

    if (! QFileInfo(out).isFile()) return false;

    return true;
}

bool common::copyDir(const QString &in, const QString &out, const QString& comment)
{
    QDir directory(in);
    if (! directory.exists()) return  false;

    QFileInfoList fileInfoList = directory.entryInfoList(QDir::Files 
                                                       | QDir::Dirs 
                                                       | QDir::NoDotAndDotDot);
    
    bool result = true;

    for (const QFileInfo& fileInfo : fileInfoList)
    {
      if (fileInfo.isDir())
          result &= copyDir(fileInfo.filePath(), 
                            out + "/" +fileInfo.fileName(),
                            comment);
      else
      if (fileInfo.isFile())
          result &= copyFile(fileInfo.filePath(),
                             out + "/" + fileInfo.fileName(),
                             comment);
    }

    return result;
}

QString common::generateDatadirPath(const QString &path)
{
  QString pathstr= QDir::cleanPath(QStandardPaths::writableLocation(QStandardPaths::DataLocation) 
                                   + ((path.isEmpty()) ? "" : QString("/")) + path);
  return pathstr;
}

QString common::generateDatadirPath(const char* path)
{
  const QString str= QString(path);
  QString pathstr= QDir::cleanPath(QStandardPaths::writableLocation(QStandardPaths::DataLocation)
                                   + ((path[0]=='\0') ? "" : "/") + str);
  return pathstr;
}

void common::setWhatsThisText(QWidget* widget, int start, int stop)
{
  widget->setWhatsThis("<html>"+readFile(whatsThisPath, 2, 2)
                       + readFile(whatsThisPath, start, stop)
                       + "</html>");
}

void common::openDir(QString path)
{
  if (path.isEmpty()) return;
  if (!QFileInfo(path).isDir())
    {
      Warning("", path + " n'est pas un répertoire.");
      return;
    }

  QDesktopServices::openUrl(QUrl::fromLocalFile(path));

}

const QString common::remAccents(QString str, bool toUpper)
{
    QRegExp rem("[éèê]");
    str.replace(rem, "e").replace("à", "a").replace("ô","o").replace("\'", " ");
    if (toUpper)
        return str.toUpper();

    return str;
}

const QString common::openDirDialog(flags::directory checkEmptyness)
{
QString path=QFileDialog::getExistingDirectory(nullptr, QString("Sélection du répertoire"),
                                               QDir::currentPath(),
                                               QFileDialog::ShowDirsOnly
                                             | QFileDialog::DontResolveSymlinks);
if (path.isEmpty()) return QString();

if (checkEmptyness == flags::directory::checkEmptyness)
{
    qint64 size=common::getDirectorySize(path, "*");

    if (size)
    {
        int result=-1;
        if ((result = Warning(QString("Répertoire"), 
                              QString("Le répertoire %1 n'est pas vide (Taille %2B)."
                                      "Ecraser et recréer ? ").arg(path,
                                                                   QString::number(size))))
             == 0)
        {
            QDir targetDirObject(path);
            if (!targetDirObject.removeRecursively())   
            {
              Warning(QString("Supprimer le répertoire de création des bases"),
                      QString("Le répertoire n'a pas été supprimé' %1").arg(QDir::toNativeSeparators(path)));
            }

            else
            if (targetDirObject.mkpath(path) == false)
            {
                Warning(QString("Répertoire"),
                        QString("Le répertoire %1 n'a pas été créé").arg(path));
                return QString();
            }
        }
    }
}

return QDir::toNativeSeparators(path);
}


bool common::zip (const QString& filename , const QString& zipfilename)
{
    IOControl(filename, zipfilename);
    QFile infile(filename);
    QFile outfile(zipfilename);
    qint64 n=0;
    bool result = true;
    
    result = infile.open(QIODevice::ReadOnly);
    result = outfile.open(QIODevice::WriteOnly);
    
    if (result == false) return false;
    n = infile.size();
    
    QByteArray uncompressedData = infile.readAll();
    
    int nBytesIn = uncompressedData.size();
    result &= (n == nBytesIn);
    
    QByteArray compressedData = qCompress(uncompressedData,9);
    
    int nBytesOut = compressedData.size();
    n = outfile.write(compressedData);
    
    result &= (n == nBytesOut);
    infile.close();
    outfile.close();
    return result;
}

bool common::zipDir (const QString& inPath , const QString& outPath)
{
    QDir directory(inPath);
    if (! directory.exists()) return  false;

    QFileInfoList fileInfoList = directory.entryInfoList(QDir::Files
                                                       | QDir::Dirs
                                                       | QDir::NoDotAndDotDot);
    bool result = true;

    for (const QFileInfo& fileInfo : fileInfoList)
    {
      if (fileInfo.isDir())
          result &= zipDir(fileInfo.filePath(), outPath + "/" + fileInfo.fileName());
      else
      if (fileInfo.isFile())
          result &= zip(fileInfo.filePath(),
                        outPath + "/" + fileInfo.fileName() + ".arch");
    }

    return result;
}

bool common::unzipDir (const QString& inPath , const QString& outPath)
{
    QDir directory(inPath);
    if (! directory.exists()) return  false;

    QFileInfoList fileInfoList = directory.entryInfoList(QDir::Files
                                                       | QDir::Dirs
                                                       | QDir::NoDotAndDotDot);
    bool result = true;

    for (const QFileInfo& fileInfo : fileInfoList)
    {
      if (fileInfo.isDir())
          result &= unzipDir(fileInfo.filePath(), outPath + "/" + fileInfo.fileName());
      else
      if (fileInfo.isFile())
      {
          QString fileNameChopped = fileInfo.fileName();
          fileNameChopped.chop(5);
          result &= unzip(fileInfo.filePath(), outPath + "/" + fileNameChopped);
      }
    }

    return result;
}

bool common::unzip (const QString& zipfilename , const QString& filename)
{
    IOControl(zipfilename, filename);
    QFile infile(zipfilename);
    QFile outfile(filename);
    
    qint64 n=0;
    bool result = true;
    
    result = infile.open(QIODevice::ReadOnly);
    result = outfile.open(QIODevice::WriteOnly);
    
    if (result == false) return false;
    n = infile.size();
    
    QByteArray uncompressedData = infile.readAll();
    
    int nBytesIn = uncompressedData.size();
    result &= (n == nBytesIn);
    
    QByteArray compressedData = qUncompress(uncompressedData);
    
    int nBytesOut = compressedData.size();
    
    n = outfile.write(compressedData);
    
    result &= (n == nBytesOut);
    infile.close();
    outfile.close();
    return result;
}


QString common::cdRomMounted()
{
    const QList<QString>& ISOLIST = {"CDFS", "UDF", "ISO-9660", "ISO9660"};

    for (const QStorageInfo &storage : QStorageInfo::mountedVolumes())
    {
           if (storage.isValid() && storage.isReady())
           {
                if (ISOLIST.contains(storage.fileSystemType().toUpper()))
                {
                 // #ifdef Q_OS_LINUX
                    if (QDir(storage.rootPath()).entryList(QDir::Files|QDir::Dirs).isEmpty())
                        return ("");

               //   #endif
                      // s'assure que se termine par un /
                      QString path = storage.rootPath();
                      if  (path.at(path.size() - 1) != '/')
                          path += "/";
                      
                    return(path);
                 break;
                }
           }
    }

    return("");
}
