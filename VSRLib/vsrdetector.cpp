#include <QDebug>
#include <QThread>
#include <stdio.h>

#include <ctime>

/* Following necessary as a workaround for qdatetime errors at Qt 5.0.2.
 * See for instance http://qt-project.org/forums/viewthread/22133 */
#define NOMINMAX
#define STR_LENGTH 1024

#include <QImage>
#include <QDir>

#include "windowsevent.h"
#include "vsrdetector.h"
#include "detectorfactory.h"

static PUCHAR bufferReady;
static p_u8 returnBuffer;
static ULONG validFrames;
static ULONGLONG remainingFrames;
static double invalidTemperatureValue = -50.0;

HexitecOperationMode operationMode = {AS_CONTROL_DISABLED, AS_CONTROL_DISABLED,
                                      AS_CONTROL_DISABLED, AS_CONTROL_ENABLED,

//shk78                                      AS_CONTROL_DISABLED, AS_CONTROL_ENABLED,
                                      AS_CONTROL_DISABLED, AS_CONTROL_DISABLED,

                                      AS_CONTROL_DISABLED, AS_CONTROL_DISABLED,
                                      AS_CONTROL_DISABLED, AS_CONTROL_DISABLED,
                                      AS_CONTROL_DISABLED, 0 };
HexitecSystemConfig	systemConfig = {2, 10, AS_HEXITEC_ADC_SAMPLE_FALLING_EDGE, 9};

#ifdef DETECTORLIB_EXPORTS
static void bufferCallBack(p_u8 transferBuffer, u32 frameCount)
{
   time_t now = time(0);
   //shk78 printf("*** Aspect buffer %ld received: %ld\n", transferBuffer, now); fflush(stdout);
   bufferReady = transferBuffer;
   validFrames = frameCount;
   remainingFrames -=validFrames;

   WindowsEvent *bufferReadyEvent = DetectorFactory::instance()->getBufferReadyEvent();
   WindowsEvent *showImageEvent = DetectorFactory::instance()->getShowImageEvent();

   bufferReadyEvent->SetEvent1();
   showImageEvent->SetEvent1();
}
#else
static void bufferCallBack(p_u8 transferBuffer, u32 frameCount)
{
}
#endif

VSRDetector::VSRDetector(string aspectFilename)
{
   this->aspectFilename = QString::fromStdString(aspectFilename);
   constructorInit();
}

VSRDetector::VSRDetector(QString aspectFilename, const QObject *parent)
{
   this->aspectFilename = aspectFilename;
   constructorInit(parent);
}

void VSRDetector::constructorInit(const QObject *parent)
{
   timeout = 1000;
   collectDcTime = 0;
   vsrDetectorThread = new QThread();
   vsrDetectorThread->start();
   moveToThread(vsrDetectorThread);

   framesPerBuffer = 500;
   directory = "C:";
   prefix = "VSR_";
   xRes = 0;
   yRes = 0;
   vCal = 0.5;
   uMid = 1.0;
   detCtrl = 0;
   targetTemperature = 24.0;
   hvSetPoint = 0;
   appendTimestamp = false;
   saveRaw = true;
   triggeringAvailable = false;
   triggerTimeout = 60000.0;
   readIniFile(this->aspectFilename);

   qRegisterMetaType<VSRDetector::DetectorCommand>("VSR::DetectorCommand");
   qRegisterMetaType<VSRDetector::DetectorState>("VSR::DetectorState");
   qRegisterMetaType<HANDLE>("HANDLE");


//   notifyStateEvent = new WindowsEvent(HEXITEC_NOTIFY_STATE, true);
//   notifyStateEvent = CreateEvent(NULL, FALSE, FALSE, HEXITEC_NOTIFY_STATE);

   connectUp(parent);
}

VSRDetector::~VSRDetector()
{
//   if (connected)
//   {
      terminateConnection();
//      connected = false;
//      free(imageInfoPtr);
//   }
   QThread::currentThread()->exit();
}

void VSRDetector::connectUp(const QObject *parent)
{

#ifdef DETECTORLIB_EXPORTS
   bufferReadyEvent = new WindowsEvent(HEXITEC_BUFFER_READY, true);
   bufferReadyEvent->connect1(parent, SLOT(handleBufferReady()));
   returnBufferReadyEvent = new WindowsEvent(HEXITEC_RETURN_BUFFER_READY, true);

   showImageEvent = new WindowsEvent(HEXITEC_SHOW_IMAGE, true);
   showImageEvent->connect1(parent, SLOT(handleShowImage()));

   connect(this , SIGNAL(executeReturnBufferReady(unsigned char *)), this, SLOT(handleReturnBufferReady()));
#endif
   connect(this, SIGNAL(executeGetImages()), this, SLOT(handleExecuteGetImages()));
}

void VSRDetector::handleBufferReady()
{
   updateState(COLLECTING);
   emit executeBufferReady(bufferReady, validFrames);
}

PUCHAR VSRDetector::getBufferReady()
{
   return bufferReady;
}

ULONG VSRDetector::getValidFrames()
{
   return validFrames;
}

void VSRDetector::handleReturnBufferReady()
{
   ReturnBuffer(detectorHandle, returnBuffer);
}

void VSRDetector::handleReturnBufferReady(unsigned char *returnBuffer, unsigned long validFrames)
{
   /*
   if ((saveRaw) && (mode == CONTINUOUS))
   {
      outFile.open(pathString, std::ofstream::binary | std::ofstream::app);
      outFile.write((const char *)returnBuffer, validFrames * frameSize);
      outFile.close();
   }
*/
   ReturnBuffer(detectorHandle, returnBuffer);
   time_t now = time(0);
   //shk78 printf("*** Aspect buffer %ld returned: %ld\n", returnBuffer, now); fflush(stdout);
}

void VSRDetector::setTargetTemperature(double targetTemperature)
{
   handleSetTargetTemperature(targetTemperature);
}

void VSRDetector::setTriggeringMode(int triggeringMode)
{
   this->triggeringMode = (Triggering)triggeringMode;
}

void VSRDetector::setTtlInput(int ttlInput)
{
   this->ttlInput = (TtlInput)ttlInput;
}

int VSRDetector::getTtlInput()
{
   return ttlInput;
}

void VSRDetector::handleSetTargetTemperature(double targetTemperature)
{
    int status = -1;

    this->targetTemperature = targetTemperature;

    status = SetDAC(detectorHandle, &vCal, &uMid, &hvSetPoint, &detCtrl, &targetTemperature, timeout);
    showError("SetDAC", status);
}

void VSRDetector::setHV(double *voltage)
{
   handleSetHV(*voltage);
}

void VSRDetector::handleSetHV(double voltage)
{
    int status = -1;

    this->hvSetPoint = voltage;

    status = SetDAC(detectorHandle, &vCal, &uMid, &hvSetPoint, &detCtrl, &targetTemperature, timeout);
    showError("SetDAC", status);
}

void VSRDetector::handleAppendTimestamp(bool appendTimestamp)
{
    this->appendTimestamp = appendTimestamp;
}

void VSRDetector::setSaveRaw(bool *saveRaw)
{
   this->saveRaw = *saveRaw;
}

unsigned long long VSRDetector::getRemainingFrames()
{
   return remainingFrames;
}

void VSRDetector::handleSaveRawChanged(bool saveRaw)
{
   this->saveRaw = saveRaw;
}

void VSRDetector::handleSetTriggerTimeout(double triggerTimeout)
{
   this->triggerTimeout = triggerTimeout * 1000.0;
}

int VSRDetector::initialiseConnection()
{
   int status = initialise();
   RegisterTransferBufferReadyCallBack(detectorHandle, bufferCallBack);
   return status;
}

int VSRDetector::initialiseConnection(p_bufferCallBack bufferCallBack)
{
   int status = initialise();
   RegisterTransferBufferReadyCallBack(detectorHandle, bufferCallBack);
   return status;
}

int VSRDetector::initialise(Triggering triggering)
{
   i32 status = -1;
   const str8 deviceDescriptor = (str8) "";
   u32 internalResult;
   char pleoraErrorCodeStr[STR_LENGTH];
   u32 pleoraErrorCodeStringLen = STR_LENGTH;
   char pleoraErrorDescription[STR_LENGTH];
   u32 pleoraErrorDescriptionLen = STR_LENGTH;
   PvDeviceSerial serialPort = PvDeviceSerialBulk0;
   u32 rxBufferSize = 2048;
   u8 useTermChar = 1;
   u8 termChar = 0x0d;
   u32 timeOut = 1000;
   FpgaFwVersion controller, vsr1, vsr2;

   updateState(INITIALISING);

   status = InitDevice(&detectorHandle, deviceDescriptor, &internalResult, pleoraErrorCodeStr,
                       &pleoraErrorCodeStringLen, pleoraErrorDescription, &pleoraErrorDescriptionLen);
   qDebug() << "(VSRLib)InitDevice returns " << status;
   showError("InitDevice", status);

   frameTime = 0;

   status = CloseSerialPort(detectorHandle);
   status = ClosePipeline(detectorHandle);
   status = CloseStream(detectorHandle);
   status = OpenSerialPort(detectorHandle, serialPort, rxBufferSize, useTermChar, termChar);
   showError("OpenSerialPort", status);

   status = OpenStream(detectorHandle);
   showError("OpenStream", status);

   status = EnableDetector(detectorHandle, timeout);
   showError("EnableDetector", status);

   u8 customerID = 0x01;
   u8 daqProjectID = 0x19;
   u8 daqVersion = 0x02;
   u8 vsrProjectID = 0x17;
   u8 vsrVersion1 = 0x03, vsrVersion2 = 0x03;
   controller = {customerID, daqProjectID, daqVersion, AS_CONTROL_DISABLED};
   vsr1 = {customerID,	vsrProjectID, vsrVersion1, AS_CONTROL_DISABLED};
   vsr2 = {customerID,	vsrProjectID, vsrVersion2, AS_CONTROL_DISABLED};
   status = CheckFirmware(detectorHandle, &controller, &vsr1, &vsr2, timeOut);
   showError("CheckFirmware", status);
   if (!status)
   {
      triggeringAvailable = true;
   }
   else
   {
      triggeringAvailable = false;
   }

   status = configureDetector();
   /* shk78 if (!status)
   {
      qDebug() << "Configure Detector - frames per buffer:" << framesPerBuffer;
      qDebug() << "Configure Detector - frameTime:" << frameTime ;
      qDebug() << "Configure Detector - collectDcTime:" << collectDcTime ;
      qDebug() << "Configure Detector - resolution:" << xRes << "x" << yRes;
   }*/

   status = SetDAC(detectorHandle, &vCal, &uMid, &hvSetPoint, &detCtrl, &targetTemperature, timeout);
   showError("SetDAC", status);
#ifdef DETECTORLIB_EXPORTS
   RegisterTransferBufferReadyCallBack(detectorHandle, bufferCallBack);
#endif
   updateState(READY);
   QString message = QString("Detector connection initialised Ok");
   emit writeMessage(message);
   emit triggeringAvailableChanged(triggeringAvailable);
//   emit detectorResolutionSet(xRes * sensorColumns, yRes * sensorRows);

   return status;
}

int VSRDetector::configure(bool triggeringSuspended)
{
   i32 status = -1;
   PvDeviceSerial serialPort = PvDeviceSerialBulk0;
   u32 rxBufferSize = 2048;
   u8 useTermChar = 1;
   u8 termChar = 0x0d;

   updateState(INITIALISING);

   status = CloseSerialPort(detectorHandle);
   status = ClosePipeline(detectorHandle);
   status = CloseStream(detectorHandle);

   status = OpenSerialPort(detectorHandle, serialPort, rxBufferSize, useTermChar, termChar);
   showError("OpenSerialPort", status);

   status = OpenStream(detectorHandle);
   showError("OpenStream", status);

   status = configureDetector(triggeringSuspended);

   updateState(READY);

   return status;
}

int VSRDetector::configureDetector(bool triggeringSuspended)
{
   i32 status = -1;
   u32 timeOut = 1000;

   if (triggeringSuspended)
   {
      triggerConfigMode = Triggering::NO_TRIGGERING;
   }
   else
   {
      triggerConfigMode = triggeringMode;
   }
   switch (triggerConfigMode)
   {
      case Triggering::NO_TRIGGERING:
         status = ConfigureDetector(detectorHandle, &sensorConfig, &operationMode, &systemConfig,
                                    &xRes, &yRes, &frameTime, &collectDcTime, timeOut);
         /*shk78 status = ConfigureDetectorWithTrigger(detectorHandle, &sensorConfig, &operationMode, &systemConfig,
                                    &xRes, &yRes, &frameTime, &collectDcTime, timeOut, AS_CONTROL_DISABLED, AS_CONTROL_DISABLED);*/
         showError( "ConfigureDetector", status);
         break;
      case Triggering::STANDARD_TRIGGERING:
         status = ConfigureDetectorWithTrigger(detectorHandle, &sensorConfig, &operationMode, &systemConfig,
                                               &xRes, &yRes, &frameTime, &collectDcTime, timeOut, AS_CONTROL_DISABLED, AS_CONTROL_ENABLED);
         showError("ConfigureDetectorWithTrigger", status);
         break;
      case Triggering::SYNCHRONISED_TRIGGERING:
         status = ConfigureDetectorWithTrigger(detectorHandle, &sensorConfig, &operationMode, &systemConfig,
                                               &xRes, &yRes, &frameTime, &collectDcTime, timeOut, AS_CONTROL_ENABLED, AS_CONTROL_ENABLED);
         showError("ConfigureDetectorWithTrigger", status);
         break;
      default:
         triggeringMode = Triggering::INVALID_TRIGGERING;
         break;
   }

   status = setImageFormat(xRes, yRes);
   status = CreatePipeline(detectorHandle, 512, 100, framesPerBuffer);
   showError( "CreatePipeline", status);

   return status;
}

int VSRDetector::terminateConnection()
{
   LONG status = -1;
   u32 timeOut = 1000;

   status = ClosePipeline(detectorHandle);
   showError("ClosePipeline", status);

   status = CloseStream(detectorHandle);
   showError("CloseStream", status);

   status = DisableDetector(detectorHandle, timeOut);
   showError("DisableDetector", status);

   status = CloseSerialPort(detectorHandle);
   showError("CloseSerialPort", status);

   status = ExitDevice(detectorHandle);
   showError("ExitDevice", status);

   updateState(IDLE);

   return status;
}

int VSRDetector::getDetectorValues(double *rh, double *th, double *tasic, double *tdac, double *ntcTemperature, double *hvCurrent)
{
    HexitecVsrEnvironmentValues envValuesVsr1, envValuesVsr2;
    HexitecControllerOperatingValues opValuesController;
    HexitecVsrOperatingValues opValuesVsr1, opValuesVsr2;
    i32 status = -1;
    u8 t1, t2, t3;

    status = ReadEnvironmentValues(detectorHandle, &envValuesVsr1, &envValuesVsr2, (p_dbl) ntcTemperature, (p_dbl) tdac, timeout);
    showError("ReadEnvironmentValues", status);

    status = ReadOperatingValues(detectorHandle, &opValuesController, &opValuesVsr1, &opValuesVsr2, timeout);
    showError("ReadOperatingValues", status);

    status = GetTriggerState(detectorHandle, &t1, &t2, &t3, timeout);
    showError("GetTriggerState", status);

    if (*ntcTemperature < invalidTemperatureValue)
    {
       status = ReadEnvironmentValues(detectorHandle, &envValuesVsr1, &envValuesVsr2, (p_dbl) ntcTemperature, (p_dbl) tdac, timeout);
       showError("ReadEnvironmentValues", status);
    }

    rh[0] = envValuesVsr1.humidity;
    rh[1] = envValuesVsr2.humidity;
    th[0] = envValuesVsr1.ambientTemperature;
    th[1] = envValuesVsr2.ambientTemperature;
    tasic[0] = envValuesVsr1.asicTemperature1;
    tasic[1] = envValuesVsr1.asicTemperature2;
    tasic[2] = envValuesVsr2.asicTemperature1;
    tasic[3] = envValuesVsr2.asicTemperature2;
    hvCurrent[0] = opValuesVsr1.hvOut;
    hvCurrent[1] = opValuesVsr2.hvOut;

    return status;
}

int VSRDetector::setImageFormat(unsigned long xResolution, unsigned long yResolution)
{
   LONG status = -1;

   const LPSTR pArg2 = LPSTR("Mono16");
   const LPSTR pArg7 = LPSTR("Two");
   const LPSTR pArg8 = LPSTR("Off");

   status = SetFrameFormatControl(detectorHandle, pArg2, xResolution, yResolution, 0, 0, pArg7, pArg8);
   showError("SetFrameFormatControl", status);
   frameSize = xResolution * yResolution * 2;

   return status;
}

void VSRDetector::setCommand(DetectorCommand command)
{
   this->command = command;
}

void VSRDetector::collectImage()
{
   setMode(CONTINUOUS);
   getImages(0, 0);
}

void VSRDetector::handleExecuteCommand(VSRDetector::DetectorCommand command, int ival1, int ival2)
{
   int status;

   if (command == INITIALISE)
   {
      if ((status = initialiseConnection()) == 0)
      {
         emit writeMessage("Detector connection initialised Ok");
      }
      else
      {
         emit writeError("Failed to initialise detector connection, status = " + QString::number(status));
      }
   }
   if (command == CONFIGURE)
   {
      if ((status = configure(ival1)) == 0)
      {
         emit writeMessage("Detector configured Ok");
      }
      else
      {
         emit writeError("Failed to configure detector, status = " + QString::number(status));
      }
   }
   if (command == RESTART)
   {
      restartImages(ival1);
   }

   if (command == COLLECT)
   {
      getImages(ival1, ival2);
   }
   else if (command == COLLECT_OFFSETS)
   {

   }

   else if (command == ABORT)
   {
      abort(false);
//      abort(true);
   }
   else if (command == CLOSE)
   {
      if ((status = terminateConnection()) == 0)
      {
         emit writeMessage("Detector connection terminated Ok");
      }
      else
      {
         emit writeError("(VSRLib/vsrdet)Failed to terminate detector connection, status = " + QString::number(status));
      }
   }

   else if (command == KILL)
   {
      abort(false);
      terminateConnection();
      vsrDetectorThread->exit();
      vsrDetectorThread->terminate();
   }
   else if (command == STATE)
   {
      updateState((DetectorState) ival1);
   }
}

void VSRDetector::getImages(int count, int ndaq)
{
   this->count = count;

   if (mode == CONTINUOUS && appendTimestamp)
   {
     setGetImageParams();
   }

   if (ndaq == 0 && offsetsOn)
   {
      updateState(OFFSETS_PREP);
      emit prepareForOffsets();
   }
   else
   {
      if ( !QDir(directory).exists())
      {
         QDir().mkpath(directory);
      }
      handleReducedDataCollection();
   }
}

void VSRDetector::setGetImageParams()
{
   QString path;

   path = directory + "/" + prefix;

   if (timestampOn)
   {
      path += "_";
      path += QDateTime::currentDateTime().toString("yyMMdd_hhmmss");
   }

   path.replace(QString("/"), QString("\\"));
   sprintf_s(processingFilename, "%s", path.toUtf8().data());
   emit imageStarted(processingFilename);
   sprintf_s(pathString, "%s.bin", processingFilename);

   if ( !QDir(directory).exists())
   {
      QDir().mkpath(directory);
   }

   imgCntAverage = 1;
}

WindowsEvent *VSRDetector::getBufferReadyEvent()
{
   return bufferReadyEvent;
}

HANDLE *VSRDetector::getTransferBufferReadyEvent()
{
   return &transferBufferReadyEvent;
}

HANDLE *VSRDetector::getReturnBufferReadyEvent()
{
   return &returnBufferReadyEvent;
}

WindowsEvent *VSRDetector::getShowImageEvent()
{
   return showImageEvent;
}

HANDLE *VSRDetector::getNotifyStateEvent()
{
   return &notifyStateEvent;
}

void VSRDetector::handleReducedDataCollection()
{
   emit executeGetImages();
}

void VSRDetector::restartImages(bool startOfImage)
{
//   qDebug() << "VSRDetector::restartImages() called, startOfImage = " << startOfImage;
   if (mode == CONTINUOUS && appendTimestamp)
   {
     setGetImageParams();
   }

   if (startOfImage && offsetsOn)
   {
      updateState(OFFSETS_PREP);
      emit prepareForOffsets();
   }

   handleExecuteGetImages(startOfImage);
//   emit executeAcquireImages(startOfImage);
}

void VSRDetector::handleExecuteGetImages(bool startOfImage)
{
   if ((triggerConfigMode == STANDARD_TRIGGERING) || (triggerConfigMode == SYNCHRONISED_TRIGGERING))
   {
      updateState(WAITING_TRIGGER);
   }
   else
   {
      updateState(COLLECTING);
   }
   emit executeAcquireImages(startOfImage);
}

void VSRDetector::handleExecuteOffsets()
{
   if (offsetsOn)
   {
      updateState(OFFSETS);
//      collectOffsets();
      collectOffsetValues();
      updateState(COLLECTING_PREP);
      emit prepareForDataCollection();
   }
   else
   {
      updateState(READY);
   }
}
void VSRDetector::collectOffsets()
{
   updateState(OFFSETS);
   collectOffsetValues();
   updateState(READY);

}

LONG VSRDetector::collectOffsetValues()
{
   int status = -1;

   status = CollectOffsetValues(detectorHandle, 1000, collectDcTime);								// make sure to have stable operating conditions (high voltage, temperature, x-ray turned off)
   showError("CollectOffsetValues", status);

   return 0;
}

void VSRDetector::acquireImages(bool startOfImage)
{
   int status = -1;
   u64 framesAcquired = 0;
   double durationSeconds = dataAcquisitionDuration/1000.0;
   u32 frameCount = (durationSeconds/frameTime) + 0.5;
   u32 frameTimeout = (ULONG)(frameTime * 2500.0);

   if (startOfImage)
   {
      remainingFrames = frameCount;
      totalFramesAcquired = 0;
   }
   else
   {
      qDebug() <<"Restart collection, remainingFrames = " << remainingFrames;
      frameCount = remainingFrames;
   }

   if( frameTimeout < 100 )
   {
      frameTimeout = 100;
   }

   if (triggerConfigMode == NO_TRIGGERING)
   {
      status = SetFrameTimeOut(detectorHandle, frameTimeout);

      time_t now = time(0);
      //shk78 printf("=== Acquiring Frames: %ld\n", now); fflush(stdout);
      status = AcquireFrames(detectorHandle, frameCount, &framesAcquired, frameTimeout);
      now = time(0);
      //shk78 printf("=== Acquired Frames: %ld\n", now); fflush(stdout);

      char asErrorMessage[STR_LENGTH] = {0};
      status = GetAsErrorMsg(status, asErrorMessage, STR_LENGTH);
      printf("Aspect error message: %s", asErrorMessage);

      //shk78 Causes Crash showError("AcquireFrames", status);
      showErrorNew((const str8) "AcquireFrames");
///SHOULD THIS BE ADDED???
      if (mode == CONTINUOUS)
      {
//         qDebug() << "IN SHOULD THIS BE ADDED??? section!!!";
         //emit imageComplete(framesAcquired);
      }
   }
   else
   {
      status = SetTriggeredFrameCount(detectorHandle, frameCount, frameTimeout);
      showError("SetTriggeredFrameCount", status);
      try
      {
         status = SetFrameTimeOut(detectorHandle, frameTimeout);
         status = AcquireFrames(detectorHandle, frameCount, &framesAcquired, triggerTimeout);
         showError("AcquireFrames Triggered", status);
      }
      catch (DetectorException &ex)
      {
         QString message = "Triggered collection terminated: " + ex.getMessage();
         qDebug() << "Triggered collection aborted - NO TRIGGER";
         emit writeError(message);
         remainingFrames = 0;
         emit cancelDataCollection();
      }
      qDebug() << "Collect triggered image over!";
   }

   totalFramesAcquired += framesAcquired;

   if (mode == CONTINUOUS && remainingFrames == 0)
   {
      emit imageComplete(totalFramesAcquired);
   }
   updateState(READY);
}

int VSRDetector::getLoggingInterval()
{
   return loggingInterval;
}

void VSRDetector::beginMonitoring()
{
   emit enableMonitoring();
}

bool VSRDetector::getTriggeringAvailable()
{
   return triggeringAvailable;
}

void VSRDetector::handleStop()
{
   updateState(READY);
   emit writeMessage("VSR DAQ finished.");
}

void VSRDetector::abort(bool restart)
{
   if (restart)
   {
      StopAcquisition(detectorHandle);
      //shk78 qDebug() << "Acquisition STOPPED - WITH restart";
   }
   else
   {
      StopAcquisition(detectorHandle);
      //shk78 qDebug() << "Acquisition STOPPED - NO restart";
      updateState(READY);
   }
}

void VSRDetector::updateState(DetectorState state)
{
   this->state = state;
   emit notifyState(state);
//   SetEvent(notifyStateEvent);
}

VSRDetector::DetectorState VSRDetector::getState()
{
   return state;
}

void VSRDetector::setMode(Mode mode)
{
   this->mode = mode;
}

ULONGLONG VSRDetector::setDataAcquisitionDuration(double *dataAcquisitionDuration)
{
   double durationSeconds = *dataAcquisitionDuration/1000.0;
   remainingFrames = (durationSeconds/frameTime) + 0.5;

   this->dataAcquisitionDuration = *dataAcquisitionDuration;

   return remainingFrames;
}

ULONGLONG VSRDetector::setDataAcquisitionDuration(double dataAcquisitionDuration)
{
   double durationSeconds = dataAcquisitionDuration/1000.0;
   remainingFrames = (durationSeconds/frameTime) + 0.5;

   this->dataAcquisitionDuration = dataAcquisitionDuration;

   return remainingFrames;
}

void VSRDetector::setDataDirectory(string  *directory)
{
   this->directory = QString::fromStdString(*directory);
}

void VSRDetector::setDirectory(QString directory)
{
   this->directory = directory;
}

string VSRDetector::getDataDirectory()
{
   return this->directory.toStdString();
}

QString VSRDetector::getDirectory()
{
   return this->directory;
}

void VSRDetector::setDataPrefix(string *prefix)
{
   this->prefix = QString::fromStdString(*prefix);
}

void VSRDetector::setPrefix(QString prefix)
{
   this->prefix = prefix;
}

string VSRDetector::getDataPrefix()
{
   return prefix.toStdString();
}

QString VSRDetector::getPrefix()
{
   return prefix;
}

void VSRDetector::setTimestampOn(bool timestampOn)
{
   this->timestampOn = timestampOn;
}

bool VSRDetector::getTimestampOn()
{
   return timestampOn;
}

VSRDetector::Mode VSRDetector::getMode()
{
   return mode;
}

void VSRDetector::enableDarks()
{
   offsetsOn = true;
}

void VSRDetector::disableDarks()
{
   offsetsOn = false;
}

void VSRDetector::imageDestToPixmap()
{
   QVector<QRgb> colorTable;

   for (int i = 0; i < 256; i++)
   {
      colorTable.push_back(QColor(i, i, i).rgb());
   }

   if (getBufferReady() != NULL)
   {
      charImageDest = getImage(0); // TO DO : Free the memory malloc'd
      QImage image(charImageDest, xRes, yRes, QImage::Format_Indexed8);

      image.setColorTable(colorTable);

      imagePixmap = QPixmap::fromImage(image);
      free(imageDest);
      emit imageAcquired(imagePixmap);

   }
}

unsigned char *VSRDetector::getImage(int imageNumber)
{
   int imageSize = xRes * yRes;
   int segmentSize = xRes * yRes * 2;
   unsigned char *buffer;
   unsigned char *image;
   short current, min = 32767, max = -32768, range;


   buffer = getBufferReady();
   imageDest = (short *) malloc(imageSize * sizeof(short));
   image = (unsigned char *) malloc(imageSize * sizeof(unsigned char));
   memcpy(imageDest, buffer, segmentSize);
   returnBuffer = buffer;
   emit executeReturnBufferReady(returnBuffer);

   for (int i = 0; i < imageSize; i++)
   {
      current = imageDest[i];
      if (current > max)
      {
         max = current;
      }
      if (current < min )
      {
         min = current;
      }
   }
   range = max - min;

   switch (range)
   {
   case 0:
      for (int i = 0; i < imageSize; i++)
      {
         image[i] = 0;
      }
      break;
   default:
      for (int i = 0; i < imageSize; i++)
      {
         image[i] = ((imageDest[i] - min) * 255) / range;
      }
      break;
   }

   return image;
}

void VSRDetector::handleShowImage()
{
   if (mode == VSR_DEFAULT)
   {
      imageDestToPixmap();
   }
}

LONG VSRDetector::readIniFile(QString aspectFilename)
{
   LONG status = -1;
   int loggingInterval = 1;
   double vCal;

   status = 0;
   iniFile = new IniFile(aspectFilename);
   framesPerBuffer = iniFile->getInt("Control-Settings/Frames Per Buffer");

   if ((loggingInterval = iniFile->getInt("Control-Settings/Logging Interval")) != QVariant(INVALID))
   {
      this->loggingInterval = loggingInterval;
   }
   else
   {
      this->loggingInterval = 1;
   }

   if ((vCal = iniFile->getDouble("Control-Settings/VCAL")) != QVariant(INVALID))
   {
      this->vCal = vCal;
   }

   sensorConfig.Gain = (HexitecGain)iniFile->getInt("Control-Settings/Gain");
   sensorConfig.Row_S1.size2 = (u16) iniFile->getInt("Control-Settings/Row -> S1");
   sensorConfig.S1_Sph = (u8) iniFile->getInt("Control-Settings/S1 -> Sph");
   sensorConfig.Sph_S2 = (u8) iniFile->getInt("Control-Settings/Sph -> S2");
   sensorConfig.Vcal2_Vcal1.size2 = (u16) iniFile->getInt("Control-Settings/VCAL2 -> VCAL1");
   sensorConfig.WaitClockCol = 1;
   sensorConfig.WaitClockRow = 8;

   HexitecSetupRegister RC = {
     {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
     {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
     {0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00} };
   HexitecSensorSetupRegister Sensor = {RC, RC};
   HexitecVsrSetupRegister vsr = {Sensor, Sensor};
   sensorConfig.SetupRegVsr1 = vsr;
   sensorConfig.SetupRegVsr2 = vsr;

   return status;
}

HexitecSetupRegister VSRDetector::initSetupRegister(QString type)
{
   HexitecSetupRegister setupRegister;
   QString iniFileKey = "Sensor-Config/";
   QString endPart;
   std::string configCharacters;

   if (type == "Column")
   {
      endPart = "Channel";
   }
   else
   {
      endPart = "Block";
   }

   iniFileKey += type;

   configCharacters = (iniFile->getString(iniFileKey + "En_1st" + endPart)).toStdString();
   configCharacters += (iniFile->getString(iniFileKey + "En_2nd" + endPart)).toStdString();
   configCharacters += (iniFile->getString(iniFileKey + "En_3rd" + endPart)).toStdString();
   configCharacters += (iniFile->getString(iniFileKey + "En_4th" + endPart)).toStdString();

   configCharacters2Bytes(configCharacters, setupRegister.ReadEn);

   configCharacters = (iniFile->getString(iniFileKey + "Pwr1st" + endPart)).toStdString();
   configCharacters += (iniFile->getString(iniFileKey + "Pwr2nd" + endPart)).toStdString();
   configCharacters += (iniFile->getString(iniFileKey + "Pwr3rd" + endPart)).toStdString();
   configCharacters += (iniFile->getString(iniFileKey + "Pwr4th" + endPart)).toStdString();

   configCharacters2Bytes(configCharacters, setupRegister.PowerEn);

   configCharacters = (iniFile->getString(iniFileKey + "Cal1st" + endPart)).toStdString();
   configCharacters += (iniFile->getString(iniFileKey + "Cal2nd" + endPart)).toStdString();
   configCharacters += (iniFile->getString(iniFileKey + "Cal3rd" + endPart)).toStdString();
   configCharacters += (iniFile->getString(iniFileKey + "Cal4th" + endPart)).toStdString();

   configCharacters2Bytes(configCharacters, setupRegister.CalEn);

   return setupRegister;
}

void VSRDetector::configCharacters2Bytes(std::string configCharacters, unsigned char *result)
{
   unsigned char zero = '0';

   for (int i = 0; i < AS_HEXITEC_SETUP_REGISTER_SIZE; i++)
   {
      result[i] = 0;
      for (int j = 0; j < 8; j++)
      {
         result[i] = result[i] << 1;
         result[i] |= ((configCharacters[i * 8 + j]) - zero);
      }
   }
}

void VSRDetector::showErrorNew(const str8 context)
{
#define NEW_STR_LENGTH 1024
  i32 NEW_asError = 0;
  i8 NEW_asErrorMessage[NEW_STR_LENGTH] = { 0 };
  i8 NEW_sysErrorMessage[NEW_STR_LENGTH] = { 0 };
  u32 NEW_pleoraErrorCode;
  i8 NEW_pleoraErrorCodeStr[STR_LENGTH] = { 0 };
  i8 NEW_pleoraErrorDescription[STR_LENGTH] = { 0 };

  i32 result = 1;
  i32 sysError = 0;
  u32 pleoraErrorCodeStrLen = STR_LENGTH;
  u32 pleoraErrorDescriptionLen = STR_LENGTH;

  sysError = GetAsErrorMsg(NEW_asError, NEW_asErrorMessage, NEW_STR_LENGTH);

  if (sysError)
  {
    sysError = GetSystemErrorMsg(sysError, NEW_sysErrorMessage, NEW_STR_LENGTH);
    qDebug() << context << ":System Error ("
             << sysError << "," << NEW_sysErrorMessage
             << " occured trying to get aSpect error message for aSpect error: " << NEW_asError;
    return;
  }
  else if (NEW_asError)
  {
    if (context != "InitDevice")
    {
      result = GetLastResult(detectorHandle, &NEW_pleoraErrorCode, NEW_pleoraErrorCodeStr, &pleoraErrorCodeStrLen, NEW_pleoraErrorDescription, &pleoraErrorDescriptionLen);
    }
    else
    {
      result = 0;
    }
  }

  if (!result)
  {
    printf("\n%s\n\taSpect Result: %s", context, NEW_asErrorMessage);
    qDebug() << "Pleora Result Code: " << NEW_pleoraErrorCode;
    qDebug() << "Pleora Result Code String: " << NEW_pleoraErrorCodeStr;
    qDebug() << "Pleora Result Description: " << NEW_pleoraErrorDescription;
  }
}

void VSRDetector::showError(const LPCSTR context, long asError)
{
  i32 status;
  i32 result = 1;
  i32 sysError = 0;
  u32 pleoraErrorCodeStrLen = STR_LENGTH;
  u32 pleoraErrorDescriptionLen = STR_LENGTH;
  char asErrorMessage[STR_LENGTH] = {0};
  char sysErrorMessage[STR_LENGTH] = {0};
  u32 pleoraErrorCode = 0;
  char pleoraErrorCodeStr[STR_LENGTH] = {0};
  char pleoraErrorDescription[STR_LENGTH] = {0};

  status = GetAsErrorMsg(asError, asErrorMessage, STR_LENGTH);

  if (status)
  {
    qDebug() << "System Error";
    status = GetSystemErrorMsg(sysError, sysErrorMessage, STR_LENGTH);
    qDebug() << "\n%s\n\tSystem Error" << context << sysError << "occured trying to get aSpect error message for aSpect error: " << sysErrorMessage << asError;
    return;
  }
  else if (asError)
  {
    qDebug() << "Aspect Error";
    if (context != "InitDevice")
    {
      result = GetLastResult(detectorHandle, &pleoraErrorCode, pleoraErrorCodeStr, &pleoraErrorCodeStrLen, pleoraErrorDescription, &pleoraErrorDescriptionLen);
    }
    else
    {
      result = 0;
    }
  }

  if (!result)
  {
    qDebug() << "Pleora Error";

    errorMessage = "aSpect Result: ";

    errorMessage.append(QString::fromLatin1((char *)context));
    errorMessage.append(QString::fromLatin1((char *)asErrorMessage));

    errorMessage.append("Pleora Result Code: ");
    errorMessage.append(QString::number(pleoraErrorCode));
    errorMessage.append(", Pleora Result Code String: ");
    errorMessage.append(QString::fromLatin1((char *)pleoraErrorCodeStr));

    qDebug() << "aSpect Result: " << context << asErrorMessage;
    qDebug() << "Pleora Result Code:" << pleoraErrorCode;
    qDebug() << "Pleora Result Code String:" << pleoraErrorCodeStr;
    qDebug() << "Pleora Result Description:" << pleoraErrorDescription;

    throw DetectorException(errorMessage);
  }
}
