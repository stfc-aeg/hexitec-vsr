#include "detectormonitor.h"

#include <QDateTime>
#include <cmath>

DetectorMonitor::DetectorMonitor(VSRDetector *vsrDetector, QObject *parent) :
   QObject(parent)
{
   monitoringEnabled = false;
   //VSR this->loggingInterval = csrDetector->getLoggingInterval();
   monitorCount = this->loggingInterval;
   logfileWriter = NULL;
   readTAsic = false;
   temperatureInRange = false;
   a = 6.112;
   b = 17.67;
   c = 243.5;
   timer = new QTimer(this);
   connect(timer, SIGNAL(timeout()), this, SLOT(monitor()));
   this->vsrDetector = vsrDetector;
   monitoringDoneEvent = CreateEvent(NULL, FALSE, FALSE, HEXITEC_MONITORING_DONE);
   temperatureBelowDPEvent = CreateEvent(NULL, FALSE, FALSE, HEXITEC_TEMPERATURE_BELOWDP);
}

DetectorMonitor::~DetectorMonitor()
{
}

int DetectorMonitor::stop()
{
   int status = 0;

   return status;
}

int DetectorMonitor::getLoggingInterval()
{
   return loggingInterval;
}

int DetectorMonitor::start()
{
   int status = 0;

   timer->start(1000);

   return status;
}
/*
void DetectorMonitor::monitor()
{
   if (monitoringEnabled)
   {
      monitorEnvironmentalValues();
   }
}
*/
MonitorData *DetectorMonitor::monitor(bool external)
{ 
   if (monitoringEnabled)
   {
      monitorEnvironmentalValues(external);
   }

   return monitorData;
}

void DetectorMonitor::executeMonitorEnvironmentalValues()
{
   /* Bias refreshing is turned off when the detector performs other operations
    * e.g. data collection. When a long DAQ occurs the Envirnmental parameters
    * need to be checked before a bias refresh is performed.
    */

   if (!monitoringEnabled)
   {
      monitorEnvironmentalValues();
   }
}


void DetectorMonitor::monitorEnvironmentalValues(bool external)
{
   try
   {
      read();
      if (external)
      {
         monitorData = new MonitorData(th, t, tdp, rh, ik, tasic, true);
         SetEvent(monitoringDoneEvent);
      }
      else
      {
         emit updateMonitorData(new MonitorData(th, t, tdp, rh, ik, tasic, valid));
	      emit monitoringDone();
      }

      if (logfileWriter != NULL)
      {
         if (monitorCount == loggingInterval)
         {
            logfileWriter->append(QDateTime::currentDateTime().toString("yyMMdd_hhmmss") + " " +
                                  QString::number(t, 'f', 1) + " " +
                                  QString::number(th[0], 'f', 1) + " " +
                                  QString::number(th[1], 'f', 1) + " " +
                                  QString::number(tdp[0], 'f', 1) + " " +
                                  QString::number(tdp[1], 'f', 1) + " " +
                                  QString::number(tasic[0], 'f', 1) + " " +
                                  QString::number(tasic[1], 'f', 1) + " " +
                                  QString::number(tasic[2], 'f', 1) + " " +
                                  QString::number(tasic[3], 'f', 1) + " " +
                                  QString::number(tdac, 'f', 1) + " " +
                                  QString::number(rh[0], 'f', 1) + " " +
                                  QString::number(rh[1], 'f', 1));
            monitorCount = 0;
         }
         monitorCount++;
      }

      if (t < tdp[0] || t < tdp[1])
      {
         if (temperatureInRange)
         {
            emit temperatureBelowDP();
            SetEvent(temperatureBelowDPEvent);
            temperatureInRange = false;
         }
      }
      else if (!temperatureInRange)
      {
         emit temperatureAboveDP();
         temperatureInRange = true;
      }
   }
   catch (DetectorException &ex)
   {
      printf("!!! Caught detector exception\n !!!"); fflush(stdout);
      emit updateMonitorData(new MonitorData(th, t, tdp, rh, ik, tasic, false));
      emit writeError(ex.getMessage());
   }
}

void DetectorMonitor::read()
{
   int status = -1;

   status = vsrDetector->getDetectorValues(rh, th, tasic, &tdac, &t, ik);
   /*shk78 rh[0] = 19.8; rh[1] = 19.9;
   th[0] = 35.4; th[1] = 35.5;
   tasic[0] = 41.0; tasic[1] = 41.1; tasic[2] = 41.2; tasic[3] = 41.3;
   tdac = 19;
   t = 32.0;
   ik[0] = -0.2; ik[1] = -3.9;
   status = 0;*/

   if(!status)
   {
      valid = true;
      calcTDP();
   }
   else
   {
      valid = false;
   }
}

void DetectorMonitor::calcTDP()
{
  gamma = log(rh[0]/100) + ((b*th[0]) / (c+th[0]));
  tdp[0] = c * gamma / (b - gamma);
  gamma = log(rh[1]/100) + ((b*th[1]) / (c+th[1]));
  tdp[1] = c * gamma / (b - gamma);
}

void DetectorMonitor::enableMonitoring()
{
   monitoringEnabled = true;
}

void DetectorMonitor::disableMonitoring()
{
   monitoringEnabled = false;
}

void DetectorMonitor::handleWriteError(QString message)
{
   if (logfileWriter != NULL)
   {
      logfileWriter->append(QDateTime::currentDateTime().toString("yyMMdd_hhmmss"));
      logfileWriter->append(message);
   }
   //shk78 emit writeError(message);
}

void DetectorMonitor::handleWriteMessage(QString message)
{
   emit writeMessage(message);
}

void DetectorMonitor::handleBiasRefreshing()
{
   readTAsic = true;
}

void DetectorMonitor::receiveState(VSRDetector::DetectorState detectorState)
{
   if (detectorState == VSRDetector::INITIALISED)
   {
      readTAsic = true;
      read();
   }
}

void DetectorMonitor::createLogFile(DetectorFilename *logFilename)
{
   QString filename;

   if (logfileWriter != NULL)
   {
      delete logfileWriter;
   }

   if (logFilename == NULL)
   {
      logfileWriter = NULL;
      monitorCount = loggingInterval;
   }
   else
   {
      filename.append(logFilename->getPrefix());
      logfileWriter = new FileWriter(logFilename->getDirectory(), filename, logFilename->getTimestampOn(), true);
      if (logfileWriter != NULL)
      {
         logfileWriter->append("Timestamp : Finger Temp : Housing Temp : Dew Point : Detector Temp : DAC Temp : Relative Humidity");
      }
   }

}

HANDLE *DetectorMonitor::getMonitoringDoneEvent()
{
   return &monitoringDoneEvent;
}

HANDLE *DetectorMonitor::getTemperatureBelowDPEvent()
{
   return &temperatureBelowDPEvent;
}
