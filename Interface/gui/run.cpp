#include "altair.h"
#include "fwidgets.h"


QStringList Altair::createCommandLineString()
{
    QListIterator<FAbstractWidget*> w(Abstract::abstractWidgetList);
    QStringList commandLine;
    
    w.toBack();
    
    while (w.hasPrevious())
    {
        FAbstractWidget* item=w.previous();
        QStringList commandLineChunk=item->commandLineStringList();
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
        QMessageBox::warning(nullptr, "Projet", "Charger un projet !");
        processFinished(exitCode::shouldLaunchRAltairAlone);
        return;
    }
    QString path=Hash::wrapper["base"]->toQString();

    if (path.isEmpty())
    {
        Warning0( "Répertoire de sortie", "Le répertoire de création des bases " + path +" n'a pas été indiqué, renseigner le dialogue des paramètres.");
        processFinished(exitCode::shouldLaunchRAltairAlone);
        return;
    }
    QDir targetDirObject(path);

    if (!targetDirObject.exists())
    {
        QMessageBox::critical(nullptr, "Répertoire des bases", "Le répertoire " + path +" n'existe pas. Veuillez le créer manuellement par sécurité.");
        processFinished(exitCode::shouldLaunchRAltairAlone);
        return;
    }

    if (!targetDirObject.removeRecursively())
    {
        Warning0(QString("Supprimer le répertoire au lancement"),
                 QString("Il n'a pas été possible de nettoyer le répertoire %1 au lancement de LHX.\nNettoyer le répertoire et relancer.").arg(QDir::toNativeSeparators(path)));
        processFinished(exitCode::shouldLaunchRAltairAlone);
        return;
    }
    else
    if (targetDirObject.mkpath(path) == false)
    {
        Warning0(QString("Répertoire"), QString("Le répertoire de sortie %1 n'a pas pu être créé. Relancer après avoir réglé le problème.").arg(path));
        return;
    }

    outputTextEdit->append(PROCESSING_HTML_TAG + tr("Validation du répertoire de sortie ") + path);
       
    QStringList args;
    QString command;
    
    progress->show();
    
    args <<  "-m" << createCommandLineString();
    
    outputTextEdit->append(STATE_HTML_TAG + tr("Décodage des fichiers .xhl..."));
    outputTextEdit->append(PROCESSING_HTML_TAG + tr("Taille totale des fichiers ")+QString::number(Altair::totalSize[0]/(1024*1024)) +tr(" Mo"));
    
    command=args.join(" ");
    outputTextEdit->append(STATE_HTML_TAG + tr("Ligne de commande : ")+ altairCommandStr+ " "+command);
    
    outputType="L";
    
    process->setProcessChannelMode(QProcess::MergedChannels);
    process->setWorkingDirectory(execPath);
    outputTextEdit->append(PROCESSING_HTML_TAG + tr("Démarrage dans ") + execPath);
    progress->setRange(0, Hash::counter["XHL"]-1);
#ifdef DEBUG
    outputTextEdit->append(PROCESSING_HTML_TAG + tr("Amplitude de la barre de progression : ") + QString::number(Hash::counter["XHL"]));
#endif
    fileRank=0;
    progress->rewind();
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


void Altair::runRAltair()
{

    outputTextEdit->append(tr(STATE_HTML_TAG "Création du rapport d'analyse des données..."));
    QDir dir=QDir::current();
    dir.setCurrent(RAltairDirStr);
    process->setWorkingDirectory(RAltairDirStr);
    process->setProcessChannelMode(QProcess::MergedChannels);
#ifdef MINIMAL
    outputType="R";
    outputTextEdit->append(tr(STATE_HTML_TAG "Ligne de commande : %1").arg(RAltairCommandStr + " " + RAltairDirStr + QDir::separator() + "rapport_msword.R"));
    progress->rewind();
    process->start(RAltairCommandStr + " " + RAltairDirStr + QDir::separator() + "rapport_msword.R");
    if (process->waitForStarted())
    {
         outputTextEdit->append(tr(STATE_HTML_TAG \
                    "Lancement du traitement des données ...Veuillez patienter.<br>\
                       Vous pouvez suivre l'exécution du traitement dans la console<br>(Configurer > Configurer l'interface > Afficher les messages)."));
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "Echec du traitement des données. Recommencer en mode avancé ou en mode expert.", "Fermer");
    }

#else

    outputTextEdit->append(tr(STATE_HTML_TAG "Ligne de commande : %1").arg(RAltairCommandStr));
    process->start(RAltairCommandStr);
#endif
}

void Altair::processFinished(exitCode code)
{
    
    switch(code)
    {
    case exitCode::exitFailure : 
        outputTextEdit->append(ERROR_HTML_TAG + QString((outputType == "L") ? " Décodage des bases " : " Analyse des données ") + tr(": plantage de l'application' ."));
        progress->stop();
        return;
        
    case exitCode::noAudioFiles :  
        outputTextEdit->append(ERROR_HTML_TAG  + QString((outputType == "L") ? " Décodage des bases " : " Analyse des données ") + tr(": Pas de fichier xhl."));
        progress->stop();
        return;
        
    default :
        outputTextEdit->append(PROCESSING_HTML_TAG  + tr(" Terminé."));
        
    }
    
    
    if (process->exitStatus() == QProcess::CrashExit) return;
    
    qint64 fsSize=1;
    
    if (outputType == "L")
    {
        outputTextEdit->append(PARAMETER_HTML_TAG  + tr(" Répertoire de sortie : %1").arg(v(base)));
        
        fsSize=getDirectorySize(v(base), "*.*");
        
        outputTextEdit->append(tr(PROCESSING_HTML_TAG "Taille de la base : ")+ QString::number(fsSize) + " Octets ("+ QString::number(((float)fsSize)/(1024.0*1024.0), 'f', 2)+ " Mo)");

    }
}


void Altair::killProcess()
{
    process->kill();
    outputTextEdit->append(PROCESSING_HTML_TAG+ QString((outputType == "L") ? " Décodage des bases " : " Analyse des données ") + tr(" en arrêt (SIGKILL)"));
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
