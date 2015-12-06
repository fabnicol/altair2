#include "altair.h"
#include "fwidgets.h"

/* fichier à encoder en UTF-8 */

QStringList Altair::createCommandLineString()
{
    QListIterator<FAbstractWidget*> w(Abstract::abstractWidgetList);
    QStringList commandLine;

    w.toBack();
    fileCount = 0;
    while (w.hasPrevious())
    {
        FAbstractWidget* item = w.previous();
        QStringList commandLineChunk = item->commandLineStringList();
        if (!commandLineChunk.isEmpty() && !commandLineChunk[0].isEmpty())
            commandLine +=  commandLineChunk;
        if (item->getHashKey() == "XHL") fileCount = commandLineChunk.size();
    }

    return commandLine;
}

void Altair::run()
{
    updateProject(true);   // crucial otherwise some dynamic settings in the option dialog
    //may not get through to command line

    if (Altair::totalSize[0] == 0)
    {
        processFinished(exitCode::shouldLaunchRAltairAlone);
        return;
    }
    if (Hash::wrapper["XHL"]->at(0).at(0).isEmpty())
    {
        QMessageBox::warning(nullptr, "Projet", "Charger un projet !");
        processFinished(exitCode::shouldLaunchRAltairAlone);
        return;
    }

    QString path=v(base);

    if (path.isEmpty())
    {
        Warning0( "Répertoire de sortie", "Le répertoire de création des bases " + path +" n'a pas été indiqué, renseigner le dialogue des paramètres.");
        processFinished(exitCode::shouldLaunchRAltairAlone);
        return;
    }
    QDir targetDirObject(path);

    if (! targetDirObject.exists() && ! targetDirObject.mkpath(path))
    {
        QMessageBox::critical(nullptr, "Répertoire des bases", "Le répertoire " + path +" n'a pas pu être créé. Veuillez le faire manuellement.");
        processFinished(exitCode::shouldLaunchRAltairAlone);
        return;
    }

    const QStringList& files = targetDirObject.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

    if (! files.isEmpty())
    {
        if (files.first() != "Bulletins.csv" && files.first() != "Table.csv")
        {
            if (QMessageBox::Cancel
                  == QMessageBox::warning(this, QString("Attention"),
                                                tr("Vous allez supprimer le répertoire %1\npour regénérer les bases. ").arg(QDir::toNativeSeparators(path)),
                                                QMessageBox::Ok|QMessageBox::Cancel))
                {

                    processFinished(exitCode::shouldLaunchRAltairAlone);
                    return;
                }
        }

        if (!targetDirObject.removeRecursively())
        {
            Warning0(QString("Supprimer le répertoire au lancement"),
                     QString("Il n'a pas été possible de nettoyer le répertoire %1 au lancement de LHX.\nNettoyer le répertoire et relancer.").arg(QDir::toNativeSeparators(path)));
            processFinished(exitCode::shouldLaunchRAltairAlone);
            return;
        }

        if (targetDirObject.mkpath(path) == false)
        {
            Warning0(QString("Répertoire"), QString("Le répertoire de sortie %1 n'a pas pu être créé. Relancer après avoir réglé le problème.").arg(path));
            return;
        }
    }

#ifdef DEBUG
    outputTextEdit->append(PROCESSING_HTML_TAG + tr("Validation du répertoire de sortie ") + path);
#endif

    QStringList args0, args1;
    QString command;

    args0 <<  "-m" << "-d" << "," << "-s" << ";" << "-rank" << sharedir + "/rank";
    if (v(conserverEtab).isTrue())
           args0 << "-S";

    args1 << createCommandLineString();

    outputTextEdit->append(PROCESSING_HTML_TAG + tr("Importation des bases de paye (")+QString::number(Altair::totalSize[0]/(1024*1024)) +tr(" Mo)..."));

    command = QString("-m -d \",\" -s \";\" -rank ") + sharedir + "/rank" ;

    QStringListIterator i(args1);
    while (i.hasNext())
    {
        const QString str = i.next();
        if (QFileInfo(str).isFile() ||  QFileInfo(str).isDir())
            command += "\""+str+"\" ";
        else
            command += " "+str+" ";
    }

    if (v(quiet).isFalse())
    {
        parent->consoleDialog->append(PROCESSING_HTML_TAG + tr("Ligne de commande : ")+ altairCommandStr+ " " + command);
    }

    outputType="L";

    process.setProcessChannelMode(QProcess::MergedChannels);
    process.setWorkingDirectory(common::execPath);

#ifdef DEBUG
    outputTextEdit->append(PROCESSING_HTML_TAG + tr("Démarrage dans ") + common::execPath);

#endif
    fileRank=0;

    /* Lancement */

    /* nécessaire pour avoir l'état réel de certains champs de contrôle comme activerConsole */
    updateProject(true);

    rankFile.setFileName(sharedir + "/rank");
    stateFile.setFileName(sharedir + "/processing");

    if (! rankFile.exists())
        rankFile.open(QIODevice::WriteOnly);

    if (rankFile.isOpen())
        rankFile.close();

    QString path_access_cl = path_access("lhx/cl");
    QFile f(path_access_cl);
    f.open(QFile::WriteOnly|QFile::Truncate);
    f.write((args0 << args1).join("\n").replace('"',"").toLatin1());
    f.close();
    //process.start(altairCommandStr,  args0 << args1);
    process.start(altairCommandStr,  QStringList() << "-f" << path_access_cl);

    if (process.waitForStarted())
    {
        if (v(ecoRAM).isTrue())
            outputTextEdit->append(PROCESSING_HTML_TAG + tr("Préallocation des ressources...\n"));
        else
            outputTextEdit->append(PROCESSING_HTML_TAG + tr("Analyse des bases de paye...Veuillez patienter\n"));


        if (v(activerConsole).isTrue())
        {
            parent->consoleDialog->insertHtml(QString("<br>" PROCESSING_HTML_TAG " ") + ((outputType == "L") ? " Décodage des bases " : " Analyse des données ") +"...<br>");
            parent->consoleDialog->moveCursor(QTextCursor::End);

            connect(&process, &QProcess::readyReadStandardOutput, [&] {

                if (v(limitConsoleOutput).isTrue())
                {
                    if (parent->getConsoleCounter() > MAXIMUM_CONSOLE_OUTPUT) return;
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

            QTimer *timer = new QTimer(this);
            connect(timer, &QTimer::timeout, [&] { readRankSignal();});
            connect(&process, SIGNAL(finished(int)), timer, SLOT(stop()));
            timer->start(500);
        }



        emit(setProgressBar(0, fileCount));

    }
    else
    {
        outputTextEdit->append(PROCESSING_HTML_TAG + tr("Echec du lancement de LHX, ligne de commande ")+ altairCommandStr);
    }

}


void Altair::runRAltair()
{

    outputTextEdit->append(tr(STATE_HTML_TAG "Création du rapport d'analyse des données..."));

    process.setWorkingDirectory(path_access("Tests/Exemple"));

    process.setProcessChannelMode(QProcess::MergedChannels);
#ifdef MINIMAL
    outputType="R";
    emit(setProgressBar(0, 100));
    QString  path_access_rapport_msword = path_access("Tests/Exemple/rapport_msword.R");
    process.start(RAltairCommandStr + " " + path_access_rapport_msword);
    if (process.waitForStarted())
    {
        outputTextEdit->append(RAltairCommandStr + " " + path_access_rapport_msword);
        outputTextEdit->append(tr(STATE_HTML_TAG \
                    "Lancement du traitement des données ...Veuillez patienter.<br>\
                       Vous pouvez suivre l'exécution du traitement dans la console<br>(Configurer > Configurer l'interface > Afficher les messages)."));
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "Echec du traitement des données. Recommencer en mode avancé ou en mode expert.", "Fermer");
    }

#else
  #ifdef DEBUG
    outputTextEdit->append(tr(STATE_HTML_TAG "Ligne de commande : %1").arg(RAltairCommandStr));
   #endif
    process.start(RAltairCommandStr, QStringList() << path_access("altaïr.Rproj"));
#endif
}

void Altair::processFinished(exitCode code)
{
    switch(code)
    {
    case exitCode::exitFailure :
        outputTextEdit->append(ERROR_HTML_TAG + QString((outputType == "L") ? " Décodage des bases " : " Analyse des données ") + tr(": plantage de l'application."));
        return;

    case exitCode::noAudioFiles :
        outputTextEdit->append(ERROR_HTML_TAG  + QString((outputType == "L") ? " Décodage des bases " : " Analyse des données ") + tr(": Pas de fichier xhl."));
        progress->stop();
        return;

    default :
        outputTextEdit->append(PROCESSING_HTML_TAG  + tr(" Terminé."));

    }


    if (process.exitStatus() == QProcess::CrashExit) return;

    qint64 fsSize=1;

    if (outputType == "L")
    {
        outputTextEdit->append(PARAMETER_HTML_TAG  + tr(" Répertoire de sortie : %1").arg(v(base)));

        fsSize=getDirectorySize(v(base), "*.*");

        outputTextEdit->append(tr(STATE_HTML_TAG "Taille de la base : ")+ QString::number(fsSize) + " Octets ("+ QString::number(((float)fsSize)/(1024.0*1024.0), 'f', 2)+ " Mo)");

    }
}


void Altair::killProcess()
{
    process.kill();
    outputTextEdit->append(PROCESSING_HTML_TAG+ QString((outputType == "L") ? " Décodage des bases " : " Analyse des données ") + tr(" en arrêt (SIGKILL)"));
    rankFile.close();
}

void Altair::printMsg(qint64 new_value, const QString &str)
{
    if (process.state() != QProcess::Running)        return;
    if (new_value < 1024*1024*1024)
        outputTextEdit->append(tr(STATE_HTML_TAG) + str + QString::number(new_value) +" B ("+QString::number(new_value/(1024*1024))+ " Mo)");
    else
        outputTextEdit->append(tr(WARNING_HTML_TAG) + "La taille du projet est supérieurs à  1 Go");
}

void Altair::printBaseSize(qint64 new_value)
{
    if (new_value > 1024) printMsg(new_value, "Création de la base ...");
}
