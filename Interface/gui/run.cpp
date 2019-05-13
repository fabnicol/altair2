// Copyright Cour des comptes, 2017
// Contributeur :
// Fabrice Nicol, années 2012 à 2017
// fabrice.nicol@crtc.ccomptes.fr
//
// Ce logiciel est un programme informatique servant à extraire et analyser
// les fichiers de paye produits au format spécifié par l'annexe de la
// convention-cadre nationale de dématérialisation en vigueur à compter de
// l'année 2008.
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
// //////////////////////////////////////////////////////////////////////////



/// \file run.cpp
/// \author Fabrice Nicol
/// \brief Implémentation de la classe  Altair agent de la classe  MainWindow qui construit l'interface graphique.\n
/// Cette partie de l'implémentation concerne uniquement le lancement ou l'arrêt d'applications externes ou le traitement de leur exécution
/// \note fichier à encoder en \e UTF-8

#include "altair.h"
#include "fwidgets.h"

#ifdef Q_OS_LINUX
#include <sys/mount.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/cdrom.h> // that will make sure the ioctl param is there
#include <unistd.h>

#endif

QStringList Altair::createCommandLineString (const QStringList& files)
{
    // liste des composant fonctionnels

    QVectorIterator<FAbstractWidget*> w (Abstract::abstractWidgetList);
    QStringList commandLine;

    w.toBack();
    fileCount = 0;

    while (w.hasPrevious())
        {
            FAbstractWidget* item = w.previous();
            QStringList commandLineChunk;

            // Les onglets correspondent au composant fonctionnelFListFrame de balise "XHL"

            if (! files.isEmpty() && item->getHashKey() == "XHL")
                {
                    commandLineChunk = files;
                }
            else
                {
                    if (item->getHashKey() != "dossierBulletins"
                            || v(dossierBulletins).isEmpty()
                            || v(Matricules).isFilled()
                            || v(MatriculesB).isFilled()
                            || v(MatriculesC).isFilled())

                        // Cas général : on récupère le bout de ligne de commande généré par chaque composant fonctionnel:

                        commandLineChunk = item->commandLineStringList();
                }

            // Certains fidgets génèrent une ligne de commande
            // C'est le cas pour celui de balise "exportMode" mais la ligne de doit pas être crée ici :
            // On saute ce cas :

            if (item->getHashKey() == "exportMode")
                {
                    continue;
                }

            // Collage des bouts de ligne de commande :

            if (! commandLineChunk.isEmpty() && ! commandLineChunk[0].isEmpty())
                commandLine +=  commandLineChunk;

            // Le nombre de fichiers de paye filecount est simplement égal à la taille de la liste commandLineChunk

            if (item->getHashKey() == "XHL")
                {
                    fileCount = commandLineChunk.size();

                    if (fileCount == 0) return QStringList();
                }
        }

    return commandLine;
}

bool Altair::runWorkerDistributed (bool reset)
{
    static QHashIterator<QString, QStringList> w (Hash::fileList);

    if (reset)
        w.toFront();

    if (w.hasNext())
        {
            const QString &subdir = w.next().key();
            common::exporter_identification_controle (subdir);
            runWorker (subdir);
            return true;
        }
    else return false;
}

void Altair::runWorker (const QString& subdir)
{
    QStringList args0, args1;
    QString command;
    QStringList commandLine;

    // Récupérer la liste des fichiers de paye du répertoire distribué subdir

    const QStringList &fileList = Hash::fileList.value (subdir);

    // Créer la ligne de commande pour subdir
    // le traitement du répertoire suivant sera assuré par une nouvelle itération

    commandLine = createCommandLineString (fileList);

    if (commandLine.isEmpty()) return;

    // par défaut :
    // -m : libérer la mémoire en fin d'exécution
    // -d ',' : séparateur décimal virgule
    // -s ';' : séparateur de champs point-virgule
    // -E : Générer l'échelon
    // -rank sharedir + "/rank" : fichier exporté dans ~/.local/share/applications/Altair (sharedir par défaut)
    //                            indiquant l'index de la barre de progression

    args0 <<  "-m" << "-d" << "," << "-s" << ";" << "-E" << "-rank" << sharedir + "/rank";

    // Si les bases sont directement importées du CDROM dans l'onglet sans passer une copie
    // dans le répertoire v(base) (par défaut .../Donnees/R-Altair) alors basculer en un
    // seul fil d'exécution. TODO : le faire plus proprement en manipulant processWidget.

#   ifdef Q_OS_WIN

    // on part de l'hypothèse, sous Windows, qu'il n'y a qu'une seule partition de disque dur
    if (v(XHL)[0] != 'C')
#   else
      if (v(XHL).contains ("/cdrom"))
#   endif
        {
            textAppend (PROCESSING_HTML_TAG + tr ("Importation des fichiers depuis le disque optique..."));
            args1 << "--cdrom";
        }

#   ifndef INSERT_PAGE
      args1 << "-D" << v(base) + QDir::separator() + subdir;
#   endif

    QStringList temp;

    foreach (const QString &str, fileList)
        {

            for (int i = 0; i < Hash::Siret[str].size() && i < Hash::Etablissement[str].size(); ++i)
                {
                    const QString siret = Hash::Siret[str].at (i);

                    if (Hash::Suppression[siret + " " + Hash::Etablissement[str].at (i)])
                        {
                            if (! temp.contains (siret))
                                temp << siret;
                        }
                }
        }

    if (! temp.isEmpty())
        {
            args1 << "--esiret";
            args1 << temp;
        }

    temp.clear();

    foreach (const QString &str, Hash::Employeur.values())
        {
            if (Hash::Suppression[str])
                {
                    if (! temp.contains (str))
                        temp << str;
                }
        }

    if (! temp.isEmpty())
        {
            args1 << "--eemployeur";
            args1 << temp;
        }

    temp.clear();

    foreach (const QString &str, Hash::Budget.values())
        {
            if (Hash::Suppression[str])
                {
                    if (! temp.contains (str))
                        temp << str;
                }
        }

    if (! temp.isEmpty())
        {
            args1 << "--ebudget";
            args1 << temp;
        }

    args1 << commandLine;

    textAppend (PROCESSING_HTML_TAG + tr ("Importation des bases de paye (")
                + QString::number (Altair::totalSize
                                   / (1024 * 1024))
                + tr (" Mo)..."));

    command = QString ("-m -d \",\" -s \";\" -E -rank ") + sharedir + "/rank" ;

#   ifndef INSERT_PAGE
    command  += QDir::separator() + subdir;
#   endif

    QStringListIterator i (args1);

    while (i.hasNext())
        {
            const QString str = i.next();

            if (QFileInfo (str).isFile() ||  QFileInfo (str).isDir())
                command += "\"" + str + "\" ";
            else
                command += " " + str + " ";
        }

    if (v(quiet).isFalse())
        {
            parent->consoleDialog->append (PROCESSING_HTML_TAG + tr ("Ligne de commande : ")
                                           + altairCommandStr
                                           + " "
                                           + command);
        }

    outputType = "L";
    process.setProcessChannelMode (QProcess::MergedChannels);
    process.setWorkingDirectory (common::execPath);

#ifdef DEBUG
    textAppend (PROCESSING_HTML_TAG + tr ("Démarrage dans ") + common::execPath);

#endif
    fileRank = 0;

    // Lancement
    // nécessaire pour avoir l'état réel de certains champs de contrôle comme activerConsole

    updateProject (true);

    rankFile.setFileName (sharedir + "/rank");

    if (! rankFile.exists())
        rankFile.open (QIODevice::WriteOnly);

    if (rankFile.isOpen())
        rankFile.close();

    QString path_access_cl = path_access ("lhx/cl");
    QFile f (path_access_cl);

    f.open (QFile::WriteOnly | QFile::Truncate);

    QString commandStr = (args0 << args1).join ("\n");
    f.write (commandStr.replace ('"', "")
#ifndef Q_OS_LINUX
             .toLatin1());
#else
             .toLocal8Bit());
#endif

    f.close();
    f.setPermissions (QFileDevice::ReadOwner | QFileDevice::ReadUser
                      | QFileDevice::ReadGroup
                      | QFileDevice::ReadOther
                      | QFileDevice::WriteOwner
                      | QFileDevice::WriteUser
                      | QFileDevice::WriteGroup
                      | QFileDevice::WriteOther);

    process.start (altairCommandStr,  QStringList() << "-f" << path_access_cl);

    if (process.waitForStarted())
        {
            if (v(memoryUse) != "Intensive")
                textAppend (PROCESSING_HTML_TAG
                            + tr ("Préallocation des ressources...\n"));
            else
                textAppend (PROCESSING_HTML_TAG
                            + tr ("Analyse des bases de paye...Veuillez patienter\n"));


            if (v(activerConsole).isTrue())
                {
                    parent->consoleDialog->insertHtml (QString ("<br>" PROCESSING_HTML_TAG " ")
                                                       + ((outputType == "L") ?
                                                          " Décodage des bases " :
                                                          " Analyse des données ")
                                                       + "...<br>");

                    parent->consoleDialog->moveCursor (QTextCursor::End);

                    connect (&process, &QProcess::readyReadStandardOutput,
                             [&]
                    {

                        if (v(limitConsoleOutput).isTrue())
                            {
                                if (parent->getConsoleCounter() > MAXIMUM_CONSOLE_OUTPUT)
                                    return;
                            }

                        if (v(activerConsole).isFalse())
                            return;

                        if (outputType[0] == 'L')
                            {
                                parent->feedLHXConsoleWithHtml();
                            }
                        else parent->feedRConsoleWithHtml();  // add counter
                    });

                }
            else
                {
                    QTimer *timer = new QTimer (this);
                    connect (timer, &QTimer::timeout, [&] { readRankSignal();});
                    connect (&process, SIGNAL (finished (int)), timer, SLOT (stop()));
                    timer->start (500);
                }

            emit (setProgressBar (0, fileCount == 1 ? 2 : fileCount));
        }
    else
        {
            textAppend (PROCESSING_HTML_TAG
                        + tr ("Echec du lancement de LHX, ligne de commande ")
                        + altairCommandStr);
        }
}



void Altair::run()
{

    updateProject (true);

    // crucial otherwise some dynamic settings in the option dialog
    // may not get through to command line

    if (Altair::totalSize == 0)
        {
            processFinished (exitCode::shouldLaunchRAltairAlone);
            return;
        }

    if (Hash::wrapper.isEmpty() || Hash::wrapper["XHL"] == nullptr
            || Hash::wrapper["XHL"]->isEmpty() ||  Hash::wrapper["XHL"]->at (0).isEmpty()
            || Hash::wrapper["XHL"]->at (0).at (0).isEmpty())
        {
            QMessageBox::warning (nullptr, "Projet", "Charger un projet !");
            processFinished (exitCode::shouldLaunchRAltairAlone);
            return;
        }

    QString path {v(base)};

    if (path.isEmpty())
        {
            Warning ("Répertoire de sortie", "Le répertoire de création des bases " + path +
                     " n'a pas été indiqué, renseigner le dialogue des paramètres.");

            processFinished (exitCode::shouldLaunchRAltairAlone);
            return;
        }

    QDir targetDirObject (path);

    if (! targetDirObject.exists() && ! targetDirObject.mkpath (path))
        {
            QMessageBox::critical (nullptr, "Répertoire des bases", "Le répertoire " + path +
                                   " n'a pas pu être créé. Veuillez le faire manuellement.");

            processFinished (exitCode::shouldLaunchRAltairAlone);
            return;
        }

    const QStringList& files = targetDirObject.entryList (QDir::Files
                               | QDir::Dirs
                               | QDir::NoDotAndDotDot);

    if (v(genererTable).isTrue() && ! files.isEmpty() && v(exportMode) != "Cumulative" && v(exportMode) != "Distributive+")
        {
            if (QMessageBox::Cancel
                    == QMessageBox::warning (this, QString ("Attention"),
                                             tr ("Vous allez supprimer les fichiers créés par le précédent traitement.\n"),
                                             QMessageBox::Ok | QMessageBox::Cancel))
                {
                    processFinished (exitCode::shouldLaunchRAltairAlone);
                    return;
                }

            for (const QString& file : files)
                {
                    const QString filepath = path + "/" + file;

                    if (QFileInfo (filepath).isFile())
                        {
                            QFile::remove (filepath);
                        }
                    else
                        {
                            QDir (filepath).removeRecursively();
                        }
                }
        }

    Hash::fileList.clear();

    if (v(exportMode).left (12) == "Distributive")
        {
            const QString cdROM = cdRomMounted();

            for (int j = 0; j < Hash::wrapper["XHL"]->size() - 3; ++j)
                {
                    const QStringList &q = Hash::wrapper["XHL"]->at (j);

                    for (const QString &s : q)
                        {
                            QString d = s.section ("xhl/", 1, 1, QString::SectionSkipEmpty);
                            bool hasSubDir;
#            ifdef Q_OS_WIN

                            hasSubDir = d.count ('/') > 0;
                            d = d.section ('/', 0, 0, QString::SectionSkipEmpty);

#            else

                            if (username == "fab")
                                {
                                    hasSubDir = d.count ('/') > 0;
                                    d = d.section ('/', 0, 0, QString::SectionSkipEmpty);
                                }
                            else
                                {
                                    hasSubDir = d.count ('/') > 1;
                                    d = d.section ('/', 1, 1, QString::SectionSkipEmpty);
                                }

#            endif

                            if (d.isEmpty()  && ! cdROM.isEmpty())
                                {
                                    d = s.section (cdROM, 0, 0, QString::SectionSkipEmpty);
                                    hasSubDir = d.count ('/') > 0;

                                    d = d.section ('/', 0, 0, QString::SectionSkipEmpty);
                                    rootDir = cdROM;
                                }
                            else
                                rootDir = userdatadir;

                            const QString &outdir = v(base) + QDir::separator() + d;

                            if (hasSubDir && ! d.isEmpty() && ! QFileInfo (outdir).isDir())
                                {
                                    QDir().mkpath (outdir);
                                }

                            Hash::fileList[hasSubDir ? d : ""] << s;
                        }
                }

            if (! Hash::fileList.empty())
                {
                    runWorkerDistributed (true);
                }
        }
    else
        {
            common::exporter_identification_controle();
            runWorker();
        }
}

void Altair::runRAltair()
{
    textAppend (tr (STATE_HTML_TAG "Création du rapport d'analyse des données..."));

    process.setWorkingDirectory (path_access ("Tests/Exemple"));

    process.setProcessChannelMode (QProcess::MergedChannels);

    // ne pas utiliser isFalse() car la valeur peut être non-spécifiée au lancement
    QDir outputDir = QDir (common::path_access (DONNEES_SORTIE));

    outputDir.remove ("altaïr.pdf");
    outputDir.remove ("altaïr.odt");
    outputDir.remove ("altaïr.docx");

    if (parent->dialog && ! parent->dialog->extraTab->budgetCheckBox->isChecked()) QDir(common::path_access (DONNEES_XHL)).remove("paye_budget.csv");
             
    if (! v(enchainerRapports).isTrue())
        {
            process.start (RAltairCommandStr, QStringList() << path_access ("altaïr.Rproj"));
            return;
        }

    outputType = "R";
    emit (setProgressBar (0, 100));
    QString  path_access_rapport;

    if (v(rapportType) == "WORD et ODT")
        {
            path_access_rapport = path_access (SCRIPT_DIR "rapport_msword.R");
        }
    else if (v(rapportType) == "PDF")
        {
            path_access_rapport = path_access (SCRIPT_DIR "rapport_pdf.R");
        }
    else if (v(rapportType) == "WORD, ODT et PDF")
        {
            path_access_rapport = path_access (SCRIPT_DIR "rapport_msword_et_pdf.R");
        }

#ifdef Q_OS_WIN
    RAltairDirStr = path_access ("R/bin/x64");
    RAltairCommandStr = RAltairDirStr + QDir::separator() + "Rscript" + QString (systemSuffix);
#else

    bool global_R = QFileInfo ("/usr/bin/Rscript").exists();
    bool local_R = QFileInfo ("/usr/local/bin/Rscript").exists();

    if (local_R)
        RAltairCommandStr = "/usr/local/bin/Rscript";
    else if (global_R)
        RAltairCommandStr = "/usr/bin/Rscript";
    else
        {
            Q ("Rscript n'est pas installé. La génération automatique du rapport sans interface RStudio n'est pas possible");
            return;
        }

#endif
    process.setWorkingDirectory (path_access (""));
    QDir::setCurrent (path_access (""));
    process.start (RAltairCommandStr + " " + path_access_rapport);

    if (process.waitForStarted())
        {
            textAppend (RAltairCommandStr + " " + path_access_rapport);
            textAppend (tr (STATE_HTML_TAG \
                            "Lancement du traitement des données ...Veuillez patienter.<br>"
                            "Vous pouvez suivre l'exécution du traitement dans la console<br>"
                            "(Configurer > Configurer l'interface > Afficher les messages)."));
        }
    else
        {
            QMessageBox::critical (this,
                                   "Erreur",
                                   "Echec du traitement des données."
                                   "Recommencer en mode avancé ou en mode expert.",
                                   "Fermer");
        }


#ifdef DEBUG
    textAppend (tr (STATE_HTML_TAG "Ligne de commande : %1").arg (RAltairCommandStr));
#endif
}

void Altair::processFinished (exitCode code)
{
    switch (code)
        {
        case exitCode::exitFailure :
            textAppend (ERROR_HTML_TAG + QString ((outputType == "L") ?
                                                  " Décodage des bases " :
                                                  " Analyse des données ")
                        + tr (": plantage de l'application."));

            textAppend (ERROR_HTML_TAG + QString ("Ligne de commande : ") + RAltairCommandStr + " " + path_access ("altaïr.Rproj"));
            return;



        default :
            textAppend (PROCESSING_HTML_TAG  + tr (" Terminé."));
        }

    if (process.exitStatus() == QProcess::CrashExit) return;

    qint64 fsSize = 1;

    if (outputType == "L")
        {
            textAppend (PARAMETER_HTML_TAG
                        + tr (" Répertoire de sortie : %1").arg (v(base)));

            fsSize = getDirectorySize (v(base), "*.*");

            textAppend (tr (STATE_HTML_TAG "Taille de la base : ")
                        + QString::number (fsSize)
                        + " Octets ("
                        + QString::number (static_cast<double>(fsSize) / (1024.0 * 1024.0), 'f', 2)
                        + " Mo)");

            bool res = false;

            if (v(exportMode).left (12) == "Distributive")
                {
                    res = runWorkerDistributed (false);
                }

            if (! res &&  v(enchainerRapports).isTrue())
                runRAltair();
        }
}


void Altair::killProcess()
{
    if (project->use_threads && process.state() != QProcess::Running)
        {
            textAppend (PROCESSING_HTML_TAG
                        "Arrêt de l'importation des données du disque optique.");

            // Terminer les fils d'exécution s'il y en a

            for (QThread *t :  project->thread)
                {
                    if (t)
                        {
                            t->requestInterruption();
                            t->quit();
                            t->wait();
                            delete (t);
                        }
                }

            emit (project->terminated());
            closeProject();
            setProgressBar (0);
            emit (hideProgressBar());
            return;
        }

    process.kill();
    textAppend (PROCESSING_HTML_TAG
                + QString ((outputType == "L") ?
                           " Décodage des bases " :
                           " Analyse des données ")
                + tr (" en arrêt (SIGKILL)"));
    rankFile.close();
}

void Altair::printMsg (qint64 new_value, const QString &str)
{
    if (process.state() != QProcess::Running)        return;

    if (new_value < 1024 * 1024 * 1024)
        textAppend (tr (STATE_HTML_TAG) + str
                    + QString::number (new_value)
                    + " B (" + QString::number (new_value / (1024 * 1024)) + " Mo)");
    else
        textAppend (tr (WARNING_HTML_TAG)
                    + "La taille du projet est supérieurs à  1 Go");
}

void Altair::printBaseSize (qint64 new_value)
{
    if (new_value > 1024) printMsg (new_value, "Création de la base ...");
}
