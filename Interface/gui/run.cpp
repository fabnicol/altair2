#include "altair.h"
#include "fwidgets.h"


QStringList Altair::createCommandLineString(flags::commandLineType commandLineType)
{
    QListIterator<FAbstractWidget*> w(Abstract::abstractWidgetList);
    QStringList commandLine;
    
    w.toBack();
    
    while (w.hasPrevious())
    {
        FAbstractWidget* item=w.previous();
        
        const QStringList commandLineChunk=item->commandLineStringList();
        if (!commandLineChunk.isEmpty() && !commandLineChunk[0].isEmpty())
            commandLine +=  commandLineChunk;
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
        Q("Charger un projet !")
        processFinished(exitCode::shouldLaunchRAltairAlone);
        return;
    }
    QString path=Hash::wrapper["base"]->toQString();

    if (path.isEmpty())
    {
        QMessageBox::warning(this, "Répertoire de sortie", "Le répertoire de création des bases " + path +" n'a pas été indiqué, renseigner le dialogue des paramètres.");
        processFinished(exitCode::shouldLaunchRAltairAlone);
        return;
    }
    QDir targetDirObject(path);

    if (!targetDirObject.exists())
    {
        Q("Le répertoire " + path +" n'existe pas. Veuillez le créer manuellement par sécurité.")
        processFinished(exitCode::shouldLaunchRAltairAlone);
        return;
    }

    if (!targetDirObject.removeRecursively())
    {
        QMessageBox::information(0, QString("Supprimer le répertoire au lancement"),
                                    QString("Il n'a pas été possible de nettoyer le répertoire %1 au lancement de LHX.\nNettoyer le répertoire et relancer.").arg(QDir::toNativeSeparators(path)));
        processFinished(exitCode::shouldLaunchRAltairAlone);
        return;
    }
    else
    if (targetDirObject.mkpath(path) == false)
    {
        QMessageBox::warning(0, QString("Répertoire"), QString("Le répertoire de sortie %1 n'a pas pu être créé. Relancer après avoir réglé le problème.").arg(path), QMessageBox::Ok);
        return;
    }

    outputTextEdit->append(PROCESSING_HTML_TAG + tr("Validation du répertoire de sortie ") + path);
       
    QStringList args;
    QString command;
    
    progress->show();
    
    args <<  "-m" << createCommandLineString(flags::commandLineType::altairCommandLine);
    
    outputTextEdit->append(STATE_HTML_TAG + tr("Décodage des fichiers .xhl..."));
    outputTextEdit->append(PROCESSING_HTML_TAG + tr("Taille totale des fichiers ")+QString::number(Altair::totalSize[0]/(1024*1024)) +tr(" Mo"));
    
    command=args.join(" ");
    outputTextEdit->append(STATE_HTML_TAG + tr("Ligne de commande : ")+ altairCommandStr+ " "+command);
    
    outputType="LHX";
    
    process->setProcessChannelMode(QProcess::MergedChannels);
    process->setWorkingDirectory(execPath);
    outputTextEdit->append(PROCESSING_HTML_TAG + tr("Démarrage dans ") + execPath);
    progress->setRange(0, Hash::counter["XHL"]-1);
    progress->setInterval(1200);
    progress->setValue(0);
    progress->start(300);

    process->start(altairCommandStr,  args);

    if (process->waitForStarted())
    {
        outputTextEdit->append(PROCESSING_HTML_TAG + tr("Lancement de LHX...Veuillez patienter\n"));
        if (Hash::wrapper["ecoRAM"]->toFString().isTrue())
            outputTextEdit->append(PROCESSING_HTML_TAG + tr("En mode économe de mémoire, le lancement effectif peut être retardé de plusieurs dizaines de secondes.\n"));
    }
    else
    {
        outputTextEdit->append(PROCESSING_HTML_TAG + tr("Echec du lancement de LHX, ligne de commande ")+ altairCommandStr);
    }

}


void Altair::on_switch_to_progress_2()
{
     progress->stage_2=true;
     progress->setRange(0,100);
     progress->stop();
     progress->setValue(0);
     progress->setTarget(Hash::wrapper["base"]->toQString());
     progress->setReference(Altair::totalSize[0]/2.5);
     progress->start(700);
}

void Altair::runRAltair()
{
    outputType="R";           
    outputTextEdit->append(tr(STATE_HTML_TAG "Création du rapport R-Altair..."));
    
    outputTextEdit->append(tr(STATE_HTML_TAG "Ligne de commande : %1").arg(RAltairCommandStr));
    QDir dir=QDir::current();
    dir.setCurrent(RAltairDirStr);
    process->start(RAltairCommandStr);
}



void Altair::processFinished(exitCode code)
{
    
    switch(code)
    {
    case exitCode::exitFailure : 
        outputTextEdit->append(ERROR_HTML_TAG  +outputType + tr(": crash exit"));
        progress->stop();
        return;
        
    case exitCode::noAudioFiles :  
        outputTextEdit->append(ERROR_HTML_TAG  +outputType + tr(": Pas de fichier xhl."));
        progress->stop();
        return;
        
    default :
        outputTextEdit->append(PROCESSING_HTML_TAG  + tr(" Terminé."));
        
    }
    
    
    if (process->exitStatus() == QProcess::CrashExit) return;
    
    qint64 fsSize=1;
    
    if (outputType == "LHX")
    {
        outputTextEdit->append(PARAMETER_HTML_TAG  + tr(" Répertoire de sortie : %1").arg(v(base)));
        
        fsSize=getDirectorySize(v(base), "*.*");
        
        outputTextEdit->append(tr(PROCESSING_HTML_TAG "Taille de la base : ")+ QString::number(fsSize) + " Octets ("+ QString::number(((float)fsSize)/(1024.0*1024.0), 'f', 2)+ " Mo)");
        
        //if (v(launchRAltairAlone).isTrue())
        //  runRAltair();
    }
}


void Altair::killProcess()
{
    process->kill();
    outputTextEdit->append(PROCESSING_HTML_TAG+ outputType + tr(" en arrêt (SIGKILL)"));
    progress->stop();
}

void Altair::printMsg(qint64 new_value, const QString &str)
{
    if (process->state() != QProcess::Running)        return;
    if (new_value < 1024*1024*1024)
        outputTextEdit->append(tr(STATE_HTML_TAG) + str + QString::number(new_value) +" B ("+QString::number(new_value/(1024*1024))+ " Mo)");
    else
        outputTextEdit->append(tr(WARNING_HTML_TAG) + "La taille du projet est supérieurs à 1 Go");
}

void Altair::printBaseSize(qint64 new_value)
{
    if (new_value > 1024) printMsg(new_value, "Création de la base ...");
}
