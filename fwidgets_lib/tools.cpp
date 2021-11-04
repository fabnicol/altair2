
// Auteur : Fabrice Nicol
// Copyright Fabrice Nicol
// Contributeur :
// Fabrice Nicol, années 2008 à 2012, 2017
// fabrnicol@gmail.com
//
// Ce logiciel est régi par les dispositions du code de la propriété
// intellectuelle (CPI). 

// L'auteur se réserve le droit d'exploitation du présent logiciel, 
// et notamment de reproduire et de modifier le logiciel, conformément aux 
// dispositions de l'article L.122-6 du même code. L'auteur se réserve le droit
// de modifier et de corriger les erreurs du logiciel, conformément aux termes 
// du deuxième alinéa du I de l'article L.122-6-1 du même code.

// Le présent logiciel doit être regardé comme un "Module Externe", pour 
// l'application des stipulations de la licence CeCILL, lorsque le code source, 
// ou une bibliothèque compilée à partir du présent code, sont utilisés pour 
// compiler tout logiciel faisant usage du présent code.

// Tout fichier contenant le présent code, ou des versions antérieures du 
// présent code, et faisant apparaître des termes contraires aux stipulations
// précédentes doit être regardé comme faisant partie d'actes préparatoires à
// l'élaboration du présent logiciel et non comme le logiciel lui-même, ou 
// comme des erreurs matérielles dans la copie des licences. Seules les 
// stipulations précédentes doivent être regardées comme s'appliquant au 
// présent logiciel.
//
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
// pris connaissance de ces stipulations et que vous en avez accepté les
// termes.

// Sans préjudice des dispositions du CPI, une autorisation d'usage et de 
// reproduction du présent code est donnée à tout agent employé par les
// juridictions financières pour l'exercice de leurs fonctions publiques. 
// Le code ainsi mis à disposition ne peut être transmis à d'autres utilisateurs.
//

//
#include "tools.h"


QString  const tools::systemPathPrefix = "/../../../";
QString tools::userdatadir;


QString tools::cdRomMounted()
{
    const QList<QString>& ISOLIST = {"CDFS", "UDF", "ISO-9660", "ISO9660"};

    for (const QStorageInfo &storage : QStorageInfo::mountedVolumes())
    {
           if (storage.isValid() && storage.isReady())
           {
                if (ISOLIST.contains(storage.fileSystemType().toUpper()))
                {
                    if (QDir(storage.rootPath()).entryList(QDir::Files|QDir::Dirs).isEmpty())
                        return ("");

                      // s'assure que se termine par un /
                      QString path = storage.rootPath();
                      if  (path.at(path.size() - 1) != '/')
                          path += "/";

                    return(path);
                }
           }
    }

    return("");
}


const QString tools::getEmbeddedPath(QString s, QString subDir)
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

qint64 tools::getDirectorySize(const QString &path, const QString &extension)
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

qint64 tools::getFileSize(const QString &fullFilePath)
{
    return QFileInfo(fullFilePath).size();
}

int tools::readFile(const QString &path, QStringList &list, int start, int stop, int width)
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
      ++j;
  }
}
else
{
    Warning("Attention", "Impossible d'ouvrir le fichier: " + path );
}

file.close();
return j;

}

bool tools::renommer(const QString& ancien, const QString& nouveau)
{
    QFile fout(ancien);
    QFile(nouveau).remove();
    return fout.rename(nouveau);
}


QString tools::dump(const QString &chaine)
{
    QString temp_path = path_access("temp");

    QFile fout(temp_path);
    fout.open(QIODevice::WriteOnly);
    QTextStream out (&fout);
    out << chaine;
    fout.close();
    return temp_path;
}

const QString tools::remAccents(QString &&str, bool toUpper)
  {
      QRegularExpression rem("[éèê]");
      str.replace(rem, "e").replace("à", "a").replace("ô","o").replace("\'", " ");
      if (toUpper)
          return str.toUpper();

      return str;
  }


bool tools::substituer(const QString& s, const QString& repl,  QString& file_str)
{
    QRegularExpression reg = QRegularExpression(s);

    file_str.replace(reg, repl);
    return (! file_str.isEmpty());
}



QString tools::readFile(const QString &path,  int start, int stop, int width)
{
    QStringList L=QStringList();
    readFile(path, L, start, stop, width);
    QString string=L.join("\n");
    return string;
}

bool tools::IOControl(const QString& in,
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


bool tools::copyFile(const QString &in,
                     const QString &out,
                     const QString &comment,
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

bool tools::copyDir(const QString &in, const QString &out, const QString& comment)
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

QString tools::generateDatadirPath(const QString &path)
{
  QString pathstr= QDir::cleanPath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                                   + ((path.isEmpty()) ? "" : QString("/")) + path);
  return pathstr;
}

QString tools::generateDatadirPath(const char* path)
{
  return generateDatadirPath(QString(path));
}


void tools::openDir(const QString& path)
{
  if (path.isEmpty()) return;
  if (! QFileInfo(path).isDir())
    {
      Warning("", path + " n'est pas un répertoire.");
      return;
    }

  QDesktopServices::openUrl(QUrl::fromLocalFile(path));

}

const QString tools::openDirDialog(flags::directory checkEmptyness)
{
QString path=QFileDialog::getExistingDirectory(nullptr, QString("Sélection du répertoire"),
                                               QDir::currentPath(),
                                               QFileDialog::ShowDirsOnly
                                             | QFileDialog::DontResolveSymlinks);
if (path.isEmpty()) return QString();

if (checkEmptyness == flags::directory::checkEmptyness)
{
    qint64 size=tools::getDirectorySize(path, "*");

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


bool tools::zip (const QString& filename , const QString& zipfilename)
{
    bool res = IOControl(filename, zipfilename);
    if (! res) return false;
    QFile infile(filename);
    QFile outfile(zipfilename);
    qint64 n=0;
    bool result;
    
    result = infile.open(QIODevice::ReadOnly);
    result &= outfile.open(QIODevice::WriteOnly);
    
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

bool tools::zipDir (const QString& inPath , const QString& outPath)
{
    QDir directory(inPath);
    if (! directory.exists()) return  false;

    QFileInfoList fileInfoList = directory.entryInfoList(QDir::Files
                                                       | QDir::Dirs
                                                       | QDir::NoDotAndDotDot);
    bool result = true;

    for (const QFileInfo& fileInfo : fileInfoList)
    {
      if (! fileInfo.exists()) continue;
      
      if (fileInfo.isDir())
          result &= zipDir(fileInfo.filePath(), outPath + "/" + fileInfo.fileName());
      else
      if (fileInfo.isFile())
      {
          if (fileInfo.size() == 0) continue;
          result &= zip(fileInfo.filePath(),
                        outPath + "/" + fileInfo.fileName() + ".arch");
      }
    }

    return result;
}

bool tools::cleanDir (const QString& inPath , const QStringList& filter, const QString& excl_dir)
{
    if (! QFileInfo(inPath).isDir()) return false;

    QDirIterator it (inPath,  filter, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    bool result = true;
    while (it.hasNext())
    {
          auto a = it.next();

          if (! excl_dir.isEmpty() && QString(a).contains(excl_dir)) continue;
          if (QFileInfo(a).isFile())
             result &= QFile(a).remove();
          else if (QFileInfo(a).isDir() && QDir(a).isEmpty())
              result &= QDir(a).removeRecursively();
    }
    return result;
}


bool tools::unzipDir (const QString& inPath , const QString& outPath, const QStringList& filter)
{
    if (! QFileInfo(inPath).isDir()) return false;
    QDirIterator it (inPath,  filter, QDir::Files, QDirIterator::Subdirectories);
    bool result = true;
    while (it.hasNext())
        {
            const QString s = it.next();
            QString filepath = getEmbeddedPath(s, inPath);
            filepath.chop (5);
            result &= unzip (s, outPath + filepath);

        }

    return result;
}


bool tools::unzipDir (const QString& inPath ,  const QStringList& filter)
{
    if (! QFileInfo(inPath).isDir()) return false;
    QDirIterator it (inPath,  filter, QDir::Files, QDirIterator::Subdirectories);
    bool result = true;
    while (it.hasNext())
        {
            const QString s = it.next();
            QString filepath = getEmbeddedPath(s, inPath);
            filepath.chop (5);
            result &= unzip (s, inPath + filepath);

        }

    return result;
}

bool tools::unzip (const QString& zipfilename , const QString& filename, bool del)
{
    bool res = IOControl(zipfilename, filename);
    if (! res) return false;

    QFile infile(zipfilename);
    QFile outfile(filename);
    const QString din  = QFileInfo(zipfilename).absolutePath();
    const QString dout = QFileInfo(filename).absolutePath();

    qint64 n=0;
    bool result;
    
    result = infile.open(QIODevice::ReadOnly);
    result &= outfile.open(QIODevice::WriteOnly);
    
    if (result == false) return false;
    n = infile.size();
    
    QByteArray uncompressedData = infile.readAll();
    
    int nBytesIn = uncompressedData.size();
    result &= (n == nBytesIn);
    
    QByteArray compressedData = qUncompress(uncompressedData);
    
    int nBytesOut = compressedData.size();
    
    n = outfile.write(compressedData);
    
    result &= (n == nBytesOut);
    
    if (dout == din && del == true)
    {
        infile.remove();
    }
        
    outfile.close();
    return result;
}

bool tools::unzip (const QString& zipfilename)
{
    QString tmp = zipfilename + ".tmp";
    bool res = IOControl(zipfilename, tmp);
    if (! res) return false;

    QFile infile(zipfilename);
    QFile outfile(tmp);
    
    qint64 n=0;
    bool result;
    
    result = infile.open(QIODevice::ReadOnly);
    result &= outfile.open(QIODevice::WriteOnly);
    
    if (result == false) return false;
    n = infile.size();
    
    QByteArray uncompressedData = infile.readAll();
    
    int nBytesIn = uncompressedData.size();
    result &= (n == nBytesIn);
    
    QByteArray compressedData = qUncompress(uncompressedData);
    
    int nBytesOut = compressedData.size();
    
    n = outfile.write(compressedData);
    
    result &= (n == nBytesOut);
  
    outfile.close();
    infile.remove();
    outfile.rename(zipfilename.chopped(5));
    return result;
}


bool tools::unzip (const QString& dir, const QStringList& filelist)
{
    bool result = true;
    for (auto& a: filelist) 
    {
        result &= unzip(dir.right(1) == "/" || dir.right(1) == "\\" ? dir + a : dir + "/" + a);
    }
    return result;
}

