#include "probe.h"



void StandardComplianceProbe::getXhlCharacteristics(const QString &filename)
{
   

    decoderCompliance=xhlCharacteristics::isNonCompliant;   //default

    if (filename.isEmpty()  || audioZone > 1)
                       return;

      QString extension=filename.split(".").last();

      if (extension == "xhl")
      {

        decoderCompliance=xhlCharacteristics::isCompliant;
 
      }
}

