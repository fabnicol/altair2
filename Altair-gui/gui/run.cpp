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
  
  if (Altair::totalSize[AUDIO] == 0)
      {
        if (v(launchRAltairAlone).isTrue())
        {
             runRAltair();
             return;
        }
        else
        {
            outputType="No DVD-Audio files";
            processFinished(exitCode::shouldLaunchRAltairAlone);
        }
      }
    
  QStringList args;
  QString command;

  progress->show();
  progress2->hide();

  args <<  createCommandLineString(flags::commandLineType::altairCommandLine);

  outputTextEdit->append(STATE_HTML_TAG + tr("Décodage des fichiers .xhl..."));
  outputTextEdit->append(PROCESSING_HTML_TAG + tr("Taille totale des fichiers ")+QString::number(Altair::totalSize[AUDIO]));
  
  command=args.join(" ");
  outputTextEdit->append(STATE_HTML_TAG + tr("Ligne de commande : ")+ altairCommandStr+ " "+command);

  outputType="LHX";
  
  process->setProcessChannelMode(QProcess::MergedChannels);
  
  process->start(altairCommandStr,  args);
  if (process->waitForStarted())
      outputTextEdit->append(PROCESSING_HTML_TAG + tr("Lancement de LXH..."));
  else
      outputTextEdit->append(PROCESSING_HTML_TAG + tr("Echec du lancement de LXH"));
  
  //progress->setTarget(Hash::wrapper["targetDir"]->toQString());
  //progress2->setTarget(Hash::wrapper["mkisofsPath"]->toQString());

  progress->setReference(Altair::totalSize[AUDIO]);
  progress->start(500);

}

void Altair::runRAltair()
{
            
  outputTextEdit->append(tr(STATE_HTML_TAG "Création du rapport R-Altair..."));
    
  outputTextEdit->append(tr(STATE_HTML_TAG "Ligne de commande : %1").arg(RAltairCommandStr));

  process->start(RAltairCommandStr);
  //progress->setTarget(Hash::wrapper["targetDir"]->toQString());
  //progress2->setTarget(Hash::wrapper["mkisofsPath"]->toQString());

  //progress->start(500);

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
                
  }
  

if (process->exitStatus() == QProcess::CrashExit) return;

qint64 isoSize=1, fsSize=1;

if (outputType == "XHL")
 {
    outputTextEdit->append(PROCESSING_HTML_TAG  + outputType + tr(" terminé, répertoire de sortie : %1").arg(v(targetDir)));

    fsSize=getDirectorySize(v(targetDir), "*.*");

    progress2->setReference(fsSize);

    outputTextEdit->append(tr(PROCESSING_HTML_TAG "Taille de la base : ")+ QString::number(fsSize) + " Octets ("+ QString::number(((float)fsSize)/(1024.0*1024.0*1024.0), 'f', 2)+ " GB)");
    
    if (v(launchRAltairAlone).isTrue())
        runRAltair();
    

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



void Altair::printFileSize(qint64 new_value)
{
    if (new_value > 1024) printMsg(new_value, "Processing .iso disc image...");
}

void Altair::printBurnProcess(qint64 new_value)
{
    if (new_value > 1024) printMsg(new_value, "Burning disc...");
}



