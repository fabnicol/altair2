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
    }
    
    QString path=Hash::wrapper["base"]->toQString();
    QDir targetDirObject(path);
    
    if (! targetDirObject.exists() && ! targetDirObject.mkdir(path))
    {
        QMessageBox::critical(this, "Erreur", "Impossible de trouver le répertoire " + path);
        return;
    }
    else
        outputTextEdit->append(PROCESSING_HTML_TAG + tr("Validation du répertoire de sortie ") + path);    
       
    QStringList args;
    QString command;
    
    progress->show();
    
    args <<  createCommandLineString(flags::commandLineType::altairCommandLine);
    
    outputTextEdit->append(STATE_HTML_TAG + tr("Décodage des fichiers .xhl..."));
    outputTextEdit->append(PROCESSING_HTML_TAG + tr("Taille totale des fichiers ")+QString::number(Altair::totalSize[AUDIO]));
    
    command=args.join(" ");
    outputTextEdit->append(STATE_HTML_TAG + tr("Ligne de commande : ")+ altairCommandStr+ " "+command);
    
    outputType="LHX";
    
    process->setProcessChannelMode(QProcess::MergedChannels);
    
    process->start(altairCommandStr,  args);
    if (process->waitForStarted())
    {
        outputTextEdit->append(PROCESSING_HTML_TAG + tr("Lancement de LHX...Veuillez patienter\n"));
        if (Hash::wrapper["ecoRAM"]->toFString().isTrue())
            outputTextEdit->append(PROCESSING_HTML_TAG + tr("En mode économe de mémoire, le lancement effectif peut être retardé de plusieurs dizaines de secondes.\n"));
    }
    else
        outputTextEdit->append(PROCESSING_HTML_TAG + tr("Echec du lancement de LHX"));
    
    
    
   progress->getBar()->setRange(0, Hash::counter["XHL"]-1);
   progress->start(700);
   
}


void Altair::on_switch_to_progress_2()
{
     progress->stage_2=true;
     progress->getBar()->setRange(0,100);
     progress->stop();
     progress->getBar()->setValue(0);
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
    outputTextEdit->append(PROCESSING_HTML_TAG+ outputType + tr(" was killed (SIGKILL)"));
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
