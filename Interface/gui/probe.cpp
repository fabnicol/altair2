#include "probe.h"



void StandardComplianceProbe::getXhlCharacteristics(const QString &filename)
{
   

    decoderCompliance=xhlCharacteristics::isNonCompliant;   //default

    if (filename.isEmpty()  || audioZone > 1)
                       return;

      QString extension=filename.split(".").last();

      if (extension[0] == 'x' && (extension[1] == 'h' || extension[1] == 'm') && extension[2] == 'l')  // xhl ou xml
      {

        decoderCompliance=xhlCharacteristics::isCompliant;
 
      }
}

