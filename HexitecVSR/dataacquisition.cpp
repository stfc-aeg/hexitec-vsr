#include "objectreserver.h"
#include "dataacquisition.h"
#include "detectorfilename.h"
#include "detectorfactory.h"
#include "voltageSourceFactory.h"
#include "math.h"

#include <QDebug>
#include <QMessageBox>
#include <QDateTime>

DataAcquisition *DataAcquisition::daqInstance = 0;

DataAcquisition::DataAcquisition(QObject *parent) :
   QThread(parent)
{
   collecting = false;
   triggered = false;
   biasRefreshing = false;
   monitoring = false;
   configuring = false;
   biasOn = false;
   biasRefreshRequired = false;
   setAbort(false);
   vsrDetector = DetectorFactory::instance()->getVSRDetector();
   detectorState = VSRDetector::IDLE;
   hv = VoltageSourceFactory::instance()->getHV();
   biasPriority = hv->getBiasPriority();
   tdp[0] = 0.0;
   tdp[1] = 0.0;
   currentImageNumber = 0;
   daqStatus = DataAcquisitionStatus();
   qRegisterMetaType<DataAcquisitionStatus>("DataAcquisitionStatus");
   qRegisterMetaType<DataAcquisitionDefinition>("DataAcquisitionDefinition");
   qRegisterMetaType<VSRDetector::DetectorCommand>("VSRDetector::DetectorCommand");
   qRegisterMetaType<VSRDetector::DetectorState>("VSRDetector::DetectorState");
   busy = false;
   rdaql.append(this);
}

void DataAcquisition::configureTriggering(int triggeringMode)
{
   mode = VSRDetector::RECONFIGURE;
   vsrDetector->setTriggeringMode(triggeringMode);
}

void DataAcquisition::configureBasicCollection()

{
   dataAcquisitionModel = DataAcquisitionModel::getInstance();
   dataAcquisitionDefinition = dataAcquisitionModel->getDataAcquisitionDefinition();
   vsrDetector->setTimestampOn(false);
   vsrDetector->setDataAcquisitionDuration(1000);
   mode = VSRDetector::VSR_DEFAULT;
   vsrDetector->setMode(mode);
}

void DataAcquisition::configureDataCollection()
{
   dataAcquisitionModel = DataAcquisitionModel::getInstance();
   QList<QObject *> rdaqml = dataAcquisitionModel->getReserveList();


   Reservation modelReservation = ObjectReserver::instance()->reserveForGUI(rdaqml);
   reservation = reservation.add(modelReservation);

   if (reservation.getReserved().isEmpty())
   {
      qDebug() <<"configureDataCollection Could not reserve all objects, message = " << reservation.getMessage();
   }
   else
   {
      dataAcquisitionDefinition = dataAcquisitionModel->getDataAcquisitionDefinition();
      vsrDetector->setTimestampOn(dataAcquisitionDefinition->getDataFilename()->getTimestampOn());
      vsrDetector->setDirectory(dataAcquisitionDefinition->getDataFilename()->getDirectory());
      vsrDetector->setPrefix(dataAcquisitionDefinition->getDataFilename()->getPrefix());
      vsrDetector->setDataAcquisitionDuration(dataAcquisitionDefinition->getDuration());
      if (dataAcquisitionDefinition->getOffsets())
      {
         vsrDetector->enableDarks();
      }
      else
      {
         vsrDetector->disableDarks();
      }

      setAbort(false);
      nRepeat = 1;
      appendRepeatCount = false;


      if (biasOn && biasPriority)
      {
         splitDataCollections = ceil(((double) dataAcquisitionDefinition->getDuration()) / ((double )hv->getBiasRefreshInterval()));
      }
      else
      {
         splitDataCollections = 1;
      }

      if ((nRepeat = dataAcquisitionDefinition->getRepeatCount()) > 1)
      {
         appendRepeatCount = true;
      }

      mode = VSRDetector::CONTINUOUS;
      vsrDetector->setMode(mode);
      currentImageNumber = 0;

      daqStatus.setDaqImages(splitDataCollections * nRepeat);
      daqStatus.setCurrentImage(0);
      daqStatus.setPercentage(0);
      changeDAQStatus(DataAcquisitionStatus::ACQUIRING_DATA,
                      daqStatus.getMinorStatus());
   }
}

DataAcquisition::~DataAcquisition()
{
   hv->off();
   emit executeCommand(VSRDetector::KILL, 0, 0);
   /* Stops crash on application exit.
      Increased following registered callback use.
      Increased following introduction of DAQ status and crashing post triggered DAQ. */
   Sleep(3000);
}

bool DataAcquisition::isBusy()
{
   return busy;
}

char *DataAcquisition::getStatus()
{
   int major = daqStatus.getMajorStatus();
   int minor = daqStatus.getMinorStatus();
   char *status = NULL;
   sprintf(status,"%d.%d", major, minor);
   return status;//daqStatus.getMajorStatus() + "." + daqStatus.getMinorStatus();
}

void DataAcquisition::run()
{
   try
   {
   if (mode == VSRDetector::VSR_DEFAULT)
   {
      performVSRDefaultDataCollection();
      // TODO : would emiting this to DataAcquisition be better for thread safety
      changeDAQStatus(DataAcquisitionStatus::IDLE,
                      daqStatus.getMinorStatus());
   }
   else if (mode == VSRDetector::CONTINUOUS)
   {
      if (dataAcquisitionDefinition->isTriggering())
      {
         performContinuousDataCollection(true);
      }
      else
      {
         performContinuousDataCollection();
      }
      // TODO : would emiting this to DataAcquisition be better for thread safety
      changeDAQStatus(daqStatus.getMajorStatus(),
                      DataAcquisitionStatus::DONE);
      changeDAQStatus(DataAcquisitionStatus::IDLE,
                      DataAcquisitionStatus::READY);
   }
   else if (mode == VSRDetector::RECONFIGURE)
   {
      performTriggeringConfigure();
   }
/*   else if (mode == VSRDetector::FIXED)
   {
      performFixedDataCollection();
      // TODO : would emiting this to DataAcquisition be better for thread safety
      changeDAQStatus(DataAcquisitionStatus::IDLE,
                      daqStatus.getMinorStatus());
   }
   */
   }
   catch (DetectorException &ex)
   {
//         emit writeError(ex.getMessage());
      qDebug() << "DetectorException caught: ";
      qDebug() << "Message: " << ex.getMessage();
   }
}

void DataAcquisition::setDirectory(int repeatCount)
{
   QString *dir = new QString(dataAcquisitionDefinition->getDataFilename()->getDirectory());

   if (appendRepeatCount)
   {
      dir->append("/" + dataAcquisitionDefinition->getDataFilename()->getPrefix() + QString().sprintf("repeat_%03d", repeatCount));
   }

   vsrDetector->setDirectory(*dir);
   delete dir;
}

void DataAcquisition::performTriggeringConfigure()
{
   configuring = true;
   emit storeBiasSettings();
   emit disableBiasRefresh();
   emit disableMonitoring();

   emit executeCommand(VSRDetector::CONFIGURE, 0, 0);
   waitForConfiguringDone();

   emit restoreBiasSettings();
   emit enableMonitoring();

}

void DataAcquisition::performContinuousDataCollection(bool triggering)
{
//   int nDaq;
   int repeatCount;
   int nDaqOverall = 0;

   emit storeBiasSettings();
   emit disableBiasRefresh();
   emit disableMonitoring();
   dataAcquisitionModel = DataAcquisitionModel::getInstance();

   for (repeatCount = 0; repeatCount < nRepeat; repeatCount++)
   {
      totalFramesAcquired = 0;
      emit appendTimestamp(true);
      setDirectory(repeatCount);
      emit imageStarting(dataAcquisitionModel->getDaqCollectionDuration()/1000, repeatCount, nRepeat);  //TODO: 20180618, no connected slot
      performMonitorEnvironmentalValues();

      if (biasPriority)
      {
         nDaqOverall = doSplitDataCollections(nDaqOverall, repeatCount, triggering);
      }
      else
      {
         nDaqOverall = doLowPriorityBiasDataCollections(nDaqOverall);
      }

      emit imageComplete(totalFramesAcquired);
      emit imageComplete((long long)totalFramesAcquired);

      // Break the outer loop too.
      if (abortRequired())
         break;

      if (repeatPauseRequired(repeatCount))
      {
         pauseDataAcquisition();
         
         if (abortRequired())
            break;
         changeDAQStatus(daqStatus.getMajorStatus(),
                         DataAcquisitionStatus::COLLECTING);
      }
   }

   daqStatus.setCurrentImage(++nDaqOverall);
   emit dataAcquisitionStatusChanged(daqStatus);

   vsrDetector->setDataAcquisitionDuration(dataAcquisitionDefinition->getDuration());
   emit restoreBiasSettings();
   emit enableMonitoring();
}

int DataAcquisition::doSplitDataCollections(int nDaqOverall, int repeatCount, bool triggering/*, int ttlInput*/)
{
   bool suspendTriggering = false;
   bool configureRequired = false;

   for (nDaq = 0; nDaq < splitDataCollections ; nDaq++)
   {
      setDataAcquisitionTime(nDaq);
      triggered = false;

      if (nDaq > 0)
      {
         emit appendTimestamp(false);
         collecting = true;
         emit executeCommand(VSRDetector::COLLECT,
                             dataAcquisitionDefinition->getRepeatCount(), nDaqOverall);
      }
      else
      {
         collecting = true;
         emit executeCommand(VSRDetector::COLLECT,
                             dataAcquisitionDefinition->getRepeatCount(), nDaqOverall);
         if (triggering && (splitDataCollections > 1))
         {
            suspendTriggering = true;
         }
      }

      nDaqOverall++;
      waitForCollectingDone();

      daqStatus.setCurrentImage(nDaqOverall);
      if (abortRequired())
      {
         break;
      }

      if (nDaq < (splitDataCollections - 1) ||
          !repeatPauseRequired(repeatCount))
      {
         performMonitorEnvironmentalValues();
         performSingleBiasRefresh();
         if (abortRequired())
         {
            break;
         }
      }
      if (suspendTriggering)
      {
         configuring = true;
         emit executeCommand(VSRDetector::CONFIGURE, 1, 0);
         waitForConfiguringDone();
         configureRequired = true;
         suspendTriggering = false;
      }
   }

   if (configureRequired && (!abortRequired()))
   {
      configuring = true;
      emit executeCommand(VSRDetector::CONFIGURE, 0, 0);
      waitForConfiguringDone();
   }

   return nDaqOverall;
}

int DataAcquisition::doLowPriorityBiasDataCollections(int nDaqOverall)
{
   unsigned long long remainingFrames = -1;
   totalImageFrames = -1;
   bool startOfImage = true;

   dataCollectionTime = dataAcquisitionDefinition->getDuration();
   collecting = true;
   nDaqOverall++;
   emit appendTimestamp(true);

   remainingFrames = vsrDetector->getRemainingFrames();
   totalImageFrames = remainingFrames;

   while (remainingFrames != 0)
   {
      if (!startOfImage)
      {
         emit appendTimestamp(false);
      }
      hv->setReadyForRefresh(false);
      collecting = true;
      emit executeCommand(VSRDetector::RESTART, startOfImage, 0);
      waitForCollectingDone();
      if (abortRequired())
      {
         break;
      }
      remainingFrames = vsrDetector->getRemainingFrames();

      if (remainingFrames != 0)
      {
         performMonitorEnvironmentalValues();
         biasRefreshing = true;  // NEWLY ADDED
         emit executeSingleBiasRefresh();
         waitForBiasRefreshDone();
      }
      startOfImage = false;
      sleep(0.1);
   }

   daqStatus.setCurrentImage(nDaqOverall);

   return nDaqOverall;
}

void DataAcquisition::performVSRDefaultDataCollection()
{
   dataAcquisitionModel = DataAcquisitionModel::getInstance();
   dataAcquisitionDefinition = dataAcquisitionModel->getDataAcquisitionDefinition();

   emit storeBiasSettings();
   emit disableBiasRefresh();
   emit disableMonitoring();
//   waitForMonitoringDone();

   collecting = true;
   emit executeCommand(VSRDetector::COLLECT, dataAcquisitionDefinition->getFixedImageCount(), 1);
   waitForCollectingDone();
   collecting = false;
   emit restoreBiasSettings();
   emit enableMonitoring();
}

bool DataAcquisition::repeatPauseRequired(int repeatCount)
{
   bool repeatPauseRequiredFlag = false;

   if ((repeatCount < nRepeat - 1) &&
       (dataAcquisitionDefinition->getRepeatInterval() > 0))
   {
      repeatPauseRequiredFlag = true;
   }

   return repeatPauseRequiredFlag;
}

void DataAcquisition::performFixedDataCollection()
{
   dataAcquisitionModel = DataAcquisitionModel::getInstance();
   dataAcquisitionDefinition = dataAcquisitionModel->getDataAcquisitionDefinition();
   emit storeBiasSettings();
   emit disableBiasRefresh();

   collecting = true;
   emit executeCommand(VSRDetector::COLLECT, dataAcquisitionDefinition->getFixedImageCount(), 1);

   waitForCollectingDone();
   collecting = false;

   emit restoreBiasSettings();
}

void DataAcquisition::setDataAcquisitionTime(int nDaq)
{
   double biasRefreshDataCollectionTime = hv->getBiasRefreshInterval();
   double finalDataCollectionTime = dataAcquisitionDefinition->getDuration() - (biasRefreshDataCollectionTime * ((double) (splitDataCollections - 1)));
//   double durationSeconds;

   dataCollectionTime = biasRefreshDataCollectionTime;

   if (nDaq == (splitDataCollections - 1))
   {
      dataCollectionTime = finalDataCollectionTime;
   }

   if (nDaq != 0)
   {
//      dataCollectionTime -= vsrDetector->getTimeError();
   }

   totalImageFrames = vsrDetector->setDataAcquisitionDuration(dataCollectionTime);
}

void DataAcquisition::performMonitorEnvironmentalValues()
{
   monitoring = true;
   emit executeMonitorEnvironmentalValues();
   waitForMonitoringDone();
}

void DataAcquisition::waitForMonitoringDone()
{
   while (monitoring)
      sleep(0.1);
}

void DataAcquisition::performSingleBiasRefresh()
{
   if (biasOn)
   {
      changeDAQStatus(daqStatus.getMajorStatus(), DataAcquisitionStatus::BIAS_REFRESHING);
      biasRefreshing = true;
      emit executeSingleBiasRefresh();
      waitForBiasRefreshDone();
      emit disableBiasRefresh();
   }
}

void DataAcquisition::pauseDataAcquisition()
{
   int pauseDuration = 0;

   if ((pauseDuration = dataAcquisitionDefinition->getRepeatInterval()) > 0)
   {
      changeDAQStatus(daqStatus.getMajorStatus(), DataAcquisitionStatus::PAUSED);
      for (int i = 0; i < pauseDuration; i+=10)
      {
         Sleep(10);

         if (abortRequired())
         {
            break;
         }
      }
      performSingleBiasRefresh();
   }
}

DataAcquisition *DataAcquisition::instance()
{
   if (daqInstance == 0)
   {
      daqInstance = new DataAcquisition();
   }
   return daqInstance;
}

int DataAcquisition::waitForBiasRefreshDone()
{
   int status = 0;

   changeDAQStatus(daqStatus.getMajorStatus(), DataAcquisitionStatus::BIAS_REFRESHING);
   while (biasRefreshing)
   {
      sleep(0.1);
   }

   return status;
}

int DataAcquisition::waitForTrigger()
{
   int status = 0;

   while (!triggered)
      sleep(0.1);

   return status;

}

int DataAcquisition::waitForConfiguringDone()
{
   int status = 0;

   while (configuring)
      sleep(0.1);

   return status;
}

int DataAcquisition::waitForCollectingDone()
{
   int status = 0;
   int elapsed = 0;
   int percentage = 0;
   unsigned long long remainingFrames;

   while (collecting)
   {
      sleep(1);
      if (mode == VSRDetector::CONTINUOUS)
      {
         elapsed++;
         if (daqStatus.getMinorStatus() == DataAcquisitionStatus::OFFSETS)
         {
            percentage = (100000.0 * (double) elapsed / dataCollectionTime) + 0.5;
         }
         else
         {
            remainingFrames = vsrDetector->getRemainingFrames();
            percentage = ((double)totalImageFrames - (double)remainingFrames) / (double)totalImageFrames * 100.0;
            if (percentage > 0)
            {
               changeDAQStatus(daqStatus.getMajorStatus(), DataAcquisitionStatus::COLLECTING);
            }
         }

         if (percentage > 100)
         {
            percentage = 100;
         }
         daqStatus.setPercentage(percentage);
         emit dataAcquisitionStatusChanged(daqStatus);
      }
   }

   if (mode == VSRDetector::VSR_DEFAULT)
   {
      percentage = 100;
      daqStatus.setPercentage(percentage);
      emit dataAcquisitionStatusChanged(daqStatus);
   }
   else if ((remainingFrames = vsrDetector->getRemainingFrames()) != 0)
   {
      percentage = ((double)totalImageFrames - (double)remainingFrames) / (double)totalImageFrames * 100.0;
   }
   else
   {
      percentage = 100;
   }
   return status;
}

void DataAcquisition::handleSetFingerTemperature(double temperature)
{
   if (temperature < tdp[0] || temperature < tdp[1])
   {
      QMessageBox msgBox;
      msgBox.setText(QString("Cannot set a Finger Temperature lower than dew point\n") +
                     QString("Dew points currently " +
                             QString::number(tdp[0], 'f', 1) +
                             " " +
                             QString::number(tdp[1], 'f', 1)));
      msgBox.setIcon(QMessageBox::Warning);
      msgBox.exec();
   }
   else
   {
     emit setTargetTemperature(temperature);
   }
}

void DataAcquisition::handleMonitorData(MonitorData *md)
{
   if (md->getValid())
   {
      memcpy(tdp, md->getTDP(), sizeof(tdp));
   }
   else
   {
      qDebug() << "Monitoring Failed!";
   }

}

void DataAcquisition::changeDAQStatus(DataAcquisitionStatus::MajorStatus majorStatus,
                                      DataAcquisitionStatus::MinorStatus minorStatus)
{
   if ((majorStatus == DataAcquisitionStatus::IDLE) && (minorStatus == DataAcquisitionStatus::READY))
   {
      busy = false;
      ObjectReserver::instance()->release(reservation.getReserved(), "GUIReserver");
   }
   daqStatus.setMajorStatus(majorStatus);
   daqStatus.setMinorStatus(minorStatus);
   emit dataAcquisitionStatusChanged(daqStatus);
}

void DataAcquisition::receiveState(VSRDetector::DetectorState detectorState)
{
   this->detectorState = detectorState;
   busy = true;

   switch (detectorState)
   {
   case VSRDetector::IDLE:
         changeDAQStatus(daqStatus.getMajorStatus(),
                         DataAcquisitionStatus::NOT_INITIALIZED);
      break;
   case VSRDetector::READY:
      if (daqStatus.getMajorStatus() == DataAcquisitionStatus::INITIALISING)
      {
         daqStatus.setMajorStatus(DataAcquisitionStatus::IDLE);
      }
      changeDAQStatus(daqStatus.getMajorStatus(),
                      DataAcquisitionStatus::READY);
      if (collecting)
      {
         collecting = false;
      }
      if (configuring)
      {
         configuring = false;
      }
      break;
   case VSRDetector::INITIALISING:
      changeDAQStatus(DataAcquisitionStatus::INITIALISING,
                      DataAcquisitionStatus::ACTIVE);
      break;
   case VSRDetector::INITIALISED:
      changeDAQStatus(DataAcquisitionStatus::INITIALISING,
                      DataAcquisitionStatus::DONE);
      emit enableMonitoring();
      emit enableBiasRefresh();
      break;
   case VSRDetector::WAITING_DARK:
      changeDAQStatus(daqStatus.getMajorStatus(),
                      DataAcquisitionStatus::WAITING_DARK);
      break;
   case VSRDetector::OFFSETS:
      changeDAQStatus(daqStatus.getMajorStatus(),
                      DataAcquisitionStatus::OFFSETS);
      break;
   case VSRDetector::OFFSETS_PREP:
      changeDAQStatus(daqStatus.getMajorStatus(),
                      DataAcquisitionStatus::OFFSETS_PREP);
      break;
   case VSRDetector::COLLECTING_PREP:
      changeDAQStatus(daqStatus.getMajorStatus(),
                      DataAcquisitionStatus::COLLECTING_PREP);
      break;
   case VSRDetector::COLLECTING:
      changeDAQStatus(DataAcquisitionStatus::ACQUIRING_DATA,
                      DataAcquisitionStatus::COLLECTING);
      break;
   case VSRDetector::WAITING_TRIGGER:
      changeDAQStatus(DataAcquisitionStatus::ACQUIRING_DATA,
                      DataAcquisitionStatus::WAITING_TRIGGER);
         break;
   }
}

void DataAcquisition::handleTriggeringSelectionChanged(int triggeringMode)
{
   reservation = ObjectReserver::instance()->reserveForGUI(rdaql);
   if (reservation.getReserved().isEmpty())
   {
      qDebug() << "handleintTriggeringSelectionChanged Could not reserve all objects, message = " << reservation.getMessage();
   }
   else
   {
      qDebug() << "handleTriggeringSelectionChanged all objects reserved, configuring triggering";
      configureTriggering(triggeringMode);
//      try
//      {
         start();
//      }
//      catch (DetectorException &ex)
//      {
//         emit writeError(ex.getMessage());
//         qDebug() << "DetectorException caught: " << ex.getMessage();
//      }

   }
}

void DataAcquisition::handleTtlInputSelectionChanged(int ttlInput)
{
   vsrDetector->setTtlInput(ttlInput);
}

void DataAcquisition::collectReducedImages()
{
   handleCollectReducedImages();
}

void DataAcquisition::handleCollectReducedImages()
{
   reservation = ObjectReserver::instance()->reserveForGUI(rdaql);
   if (reservation.getReserved().isEmpty())
   {
      qDebug() << "handleCollectReducedImages Could not reserve all objects, message = " << reservation.getMessage();
   }
   else
   {
      // shk78 qDebug() << "handleCollectReducedImages all objects reserved, configuring DAQ";
      configureDataCollection();

      start();
   }
}

void DataAcquisition::handleCollectFixedImages()
{
   configureBasicCollection();
   start();
}

void DataAcquisition::handleBufferReady(unsigned char *transferBuffer, unsigned long validFrames)
{
   if (mode != VSRDetector::VSR_DEFAULT)
   {
      emit transferBufferReady(transferBuffer, validFrames);
      triggered = true;

   }
   ///  HexitecVSR Addition:
   /// Must provide a set of motorPositions also,
   /// or bufferQueue fills up while motorQueue remains empty..
}

/*
void DataAcquisition::handleImageStarted(char *path, int frameSize)
{
   qDebug() << "DataAcquisition::handleImageStarted(), path = " << path;
//   hxtProcessor->pushRawFileName(path, frameSize);
//   hxtProcessor->pushMotorPositions(&motorPositions); /// Provide motorPositions together with buffer instead
}
*/
void DataAcquisition::handleImageComplete(unsigned long long framesAcquired)
{
   totalFramesAcquired += framesAcquired;
}

void DataAcquisition::handleInitialiseDetector()
{
   vsrDetector->initialiseConnection();
   emit enableMonitoring();
   biasOn = false;
}

void DataAcquisition::prepareForBiasRefresh()
{
   vsrDetector->abort(true);
   hv->setReadyForRefresh(true);
}

void DataAcquisition::handleExecuteOffsets()
{
   emit executeOffsets();
}

void DataAcquisition::handleCancelOffsets()
{
   abort = true;
   collecting = false;
   emit executeCommand(VSRDetector::STATE, VSRDetector::READY, 0);
   // Following 2 line necessary for when soft trigger cancelled.
   vsrDetector->setDataAcquisitionDuration(dataAcquisitionDefinition->getDuration());
   emit restoreBiasSettings();
}

void DataAcquisition::handleExecuteReducedDataCollection()
{
   emit executeReducedDataCollection();
}

void DataAcquisition::handleCancelReducedDataCollection()
{
   abort = true;
   collecting = false;
   emit executeCommand(VSRDetector::STATE, VSRDetector::READY, 0);
   // Following 2 line necessary for when soft trigger cancelled.
   vsrDetector->setDataAcquisitionDuration(dataAcquisitionDefinition->getDuration());
   emit restoreBiasSettings();
}

void DataAcquisition::handleBiasRefreshing()
{
   storedMinorStatus = daqStatus.getMinorStatus();
   changeDAQStatus(daqStatus.getMajorStatus(),
                   DataAcquisitionStatus::BIAS_REFRESHING);
   biasRefreshing = true;
}

void DataAcquisition::handleBiasRefreshed(QString time, bool restartMonitoring)
{
   changeDAQStatus(daqStatus.getMajorStatus(),
                   storedMinorStatus);
   biasRefreshing = false;
   if (restartMonitoring)
   {
      emit enableMonitoring();
   }
}

void DataAcquisition::handleMonitored()
{
   monitoring = false;
}

void DataAcquisition::handleBiasState(bool biasOn)
{
   this->biasOn= biasOn;
}

void DataAcquisition::handleAbortDAQ()
{
//   setAbort(true);
//   vsrDetector->abort(true);
   setAbort(true);
   vsrDetector->abort(false);
}

void DataAcquisition::setAbort(bool abort)
{
   this->abort = abort;
}

bool DataAcquisition::abortRequired()
{
   return abort;
}
