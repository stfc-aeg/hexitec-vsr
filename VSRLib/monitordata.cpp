#include "monitordata.h"

MonitorData::MonitorData(double *th, double t, double *tdp, double *rh, double *ik, double *tasic,
                         bool valid, QObject *parent) :
   QObject(parent)
{
   memcpy(this->th, th, sizeof(this->th));
   this->t = t;
   memcpy(this->tdp, tdp, sizeof(this->tdp));
   memcpy(this->rh, rh, sizeof(this->rh));
   memcpy(this->ik, ik, sizeof(this->ik));
   memcpy(this->tasic, tasic, sizeof(this->tasic));
   this->valid = valid;
}

double MonitorData::getTH(int element)
{
   return th[element];
}

double *MonitorData::getTH()
{
   return th;
}

double MonitorData::getT()
{
   return t;
}

double MonitorData::getTDP(int element)
{
   return tdp[element];
}

double *MonitorData::getTDP()
{
   return tdp;
}

double MonitorData::getRH(int element)
{
   return rh[element];
}

double *MonitorData::getRH()
{
   return rh;
}

double MonitorData::getIK(int element)
{
   return ik[element];
}

double *MonitorData::getIK()
{
   return ik;
}

double MonitorData::getTASIC(int element)
{
   return tasic[element];
}

double *MonitorData::getTASIC()
{
   return tasic;
}

bool MonitorData::getValid()
{
   return valid;
}
