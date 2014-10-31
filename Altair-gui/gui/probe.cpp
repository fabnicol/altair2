#include "probe.h"


void StandardComplianceProbe::checkStandardCompliance()
{

//sampleSize=audioFileFormat.sampleSize();
//sampleRate=audioFileFormat.sampleRate();
//channelCount=audioFileFormat.channelCount();


//if (((channelCount ==0) || ( sampleRate == 0) || (sampleSize == 0)) ||
//    (channelCount > 6) ||
//    ((sampleSize != 16) && (sampleSize != 24)))
//  {
//    decoderCompliance=isNonCompliant;
//    return;
//  }

//switch (audioFileFormat.sampleRate())
//{
//case 96000:
//case 48000: decoderCompliance=isStrictlyDVDVideoCompliant;
//                      break;
//case 44100:
//case 88200:
//case 176400:
//case 192000:
//                         decoderCompliance = isStrictlyDVDAudioCompliant;
//                         break;
//default:

//  decoderCompliance=isNonCompliant;
//}

return;
}

bool StandardComplianceProbe::isStandardCompliant()
{
//    if (audioZone == VIDEO)
//       return (decoderCompliance == isStrictlyDVDVideoCompliant) ;

//    return (decoderCompliance == isStrictlyDVDAudioCompliant ||  decoderCompliance == isStrictlyDVDVideoCompliant);
}

void StandardComplianceProbe::getXhlCharacteristics(const QString &filename)
{
    QString sox_guessed_codec=QString();

    decoderCompliance=xhlCharacteristics::isNonCompliant;   //default

    if (filename.isEmpty()  || audioZone > 1)
                       return;

      QString extension=filename.split(".").last();

      if (extension == "xhl")
      {

        decoderCompliance=xhlCharacteristics::isCompliant;
      //checkStandardCompliance();
      }
}

