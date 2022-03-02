#ifndef MONITORDATA_H
#define MONITORDATA_H

#include <QObject>
#include <string.h>

class MonitorData : public QObject
{
   Q_OBJECT
private:
   double th[2];
   double t;
   double tdp[2];
   double rh[2];
   double ik[2];
   double tasic[4];
   bool valid;
public:
   explicit MonitorData(double *th, double t, double *tdp, double *rh, double *ik, double *tasic,
                        bool valid, QObject *parent = 0);
   double getTH(int element);
   double *getTH();
   double getT();
   double getTDP(int element);
   double *getTDP();
   double getRH(int element);
   double *getRH();
   double getIK(int element);
   double *getIK();
   double getTASIC(int element);
   double *getTASIC();
   bool getValid();
};

#endif // MONITORDATA_H
