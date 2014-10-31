#ifndef PROBE_H
#define PROBE_H
#include "common.h"

class StandardComplianceProbe : public QObject
{
    Q_OBJECT

private:

    QStringList xhlFormatList ={"xhl"};

    uint audioZone;

    void getXhlCharacteristics(const QString &filename);

    void checkStandardCompliance();
    enum class xhlCharacteristics   { isCompliant, isNonCompliant};
    xhlCharacteristics decoderCompliance;
    int nAgent=0;
    int char2=0;
    int char3=0;

 public:
    StandardComplianceProbe(QString &filename, uint zone=0)
    {
        audioZone=zone;
        getXhlCharacteristics(filename);
    }
    bool isStandardCompliant();
    QString getnAgent() {return QString::number(nAgent);}
    QString getChar2() {return QString::number(char2);}
    QString getChar3() {return QString::number(char3);}

};


#endif // PROBE_H
