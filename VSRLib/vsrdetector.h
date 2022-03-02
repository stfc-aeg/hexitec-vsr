#ifndef VSRDETECTOR_H
#define VSRDETECTOR_H
#define NOMINMAX

#include <QObject>
#include <QPixmap>
#include <fstream>
#include <QDateTime>
#include <string>
#include <QString>

#include "detectorexception.h"
#include "inifile.h"
#include "windowsevent.h"
#include <iostream>
#include "VSR.h"

#define HEXITEC_BUFFER_READY TEXT("Hexitec_BufferReady")
#define HEXITEC_TRANSFER_BUFFER_READY TEXT("Hexitec_TransferBufferReady")
#define HEXITEC_RETURN_BUFFER_READY TEXT("Hexitec_ReturnBufferReady")
#define HEXITEC_STOP_DAQ TEXT("Hexitec_StopDAQ")
#define HEXITEC_SHOW_IMAGE TEXT("Hexitec_ShowImage")
#define HEXITEC_NOTIFY_STATE TEXT("Hexitec_NotifyState")

using namespace std;

class VSRDetector : public QObject
{
   Q_OBJECT

public:
/* If change add a Mode be sure to add to modes variable. */
   enum Mode {CONTINUOUS, VSR_DEFAULT, RECONFIGURE, INVALID_MODE};
/**********************************************************/
   enum Triggering {NO_TRIGGERING, STANDARD_TRIGGERING, SYNCHRONISED_TRIGGERING, INVALID_TRIGGERING};
   enum TtlInput{INPUT2, INPUT3, INVALID_TTLINPUT};
   enum DetectorState {IDLE, READY, INITIALISING, INITIALISED, WAITING_DARK, OFFSETS, OFFSETS_PREP, COLLECTING_PREP, COLLECTING, WAITING_TRIGGER, TRIGGERING_STOPPED};
   enum DetectorCommand {INITIALISE, CONFIGURE, COLLECT, COLLECT_OFFSETS, TRIGGER, ABORT, CLOSE, KILL, STATE, RESTART};
   VSRDetector(string aspectFilename);
   int initialiseConnection(p_bufferCallBack bufferCallBack);
   void setDataDirectory(string *directory);
   void setDataPrefix(string *prefix);
   ULONGLONG setDataAcquisitionDuration(double *imageAcquisitionDuration);
   string getDataDirectory();
   void collectOffsets();
   void setHV(double *voltage);
   void setSaveRaw(bool *saveRaw);
   unsigned long long getRemainingFrames();
   DetectorState getState();
 
   VSRDetector(QString aspectFilename, const QObject *parent = 0);
   ~VSRDetector();

   Q_INVOKABLE int initialiseConnection();
   Q_INVOKABLE int terminateConnection();
   Q_INVOKABLE int getDetectorValues(double *rh, double *th, double *tasic, double *tdac, double *ntcTemperature, double *hvCurrent);
   Q_INVOKABLE void getImages(int count, int ndaq);
   Q_INVOKABLE void restartImages(bool startOfImage);
   Q_INVOKABLE void enableDarks();
   Q_INVOKABLE void disableDarks();
   Q_INVOKABLE Mode getMode();

   void setCommand(DetectorCommand command);
   void setMode(Mode mode);
   void setBufferReadyEvent();
   QString getDirectory();
   QString getPrefix();
   void setDataPrefix(string  prefix);
   string getDataPrefix();
   bool getTimestampOn();
   void setTimestampOn(bool timestampOn);
   void setDirectory(QString directory);
   void setPrefix(QString prefix);
   ULONGLONG setDataAcquisitionDuration(double imageAcquisitionDuration);
   void setTargetTemperature(double targetTemperature);
   void setHV(double voltage);
   void setSaveRaw(bool saveRaw);
   void setTriggeringMode(int triggeringMode);
   void setTtlInput(int ttlInput);
   int getTtlInput();
   void collectImage();
   void acquireImages(bool startOfImage = true);
   int getLoggingInterval();
   void beginMonitoring();
   bool getTriggeringAvailable();

   WindowsEvent *getBufferReadyEvent();
//   WindowsEvent *getReturnBufferReadyEvent();
   WindowsEvent *getShowImageEvent();
   HANDLE *getTransferBufferReadyEvent();
   HANDLE *getReturnBufferReadyEvent();
   HANDLE *getNotifyStateEvent();

   static PUCHAR getBufferReady();
   static ULONG getValidFrames();
   void abort(bool restart = false);

signals:
   void notifyState(VSRDetector::DetectorState state);
   void writeError(QString message);
   void writeMessage(QString message);
   void executeCommand(VSRDetector::DetectorCommand, int, int);
   void executeGetImages();
   void executeReturnBufferReady(unsigned char * transferBuffer);
   void executeBufferReady(unsigned char * transferBuffer, unsigned long validFrames);
   void notifyStop();
   void imageAcquired(QPixmap data);
   void imageStarted(char *path, int frameSize);
   void imageStarted(char *path);
   void imageComplete(unsigned long long framesAcquired);
   void executeAcquireImages(bool startOfImage = true);
   void prepareForOffsets();
   void prepareForDataCollection();
   void enableMonitoring();
   void triggeringAvailableChanged(bool triggeringAvailable);
   void cancelDataCollection();
   void detectorResolutionSet(int totalXRes, int totalYRes);

public slots:
   void handleShowImage();
   void handleExecuteCommand(VSRDetector::DetectorCommand command, int ival1, int ival2 = 1);
   void handleExecuteGetImages(bool startOfImage = true);
   void handleStop();
   void handleReducedDataCollection();
   void handleExecuteOffsets();
   void handleBufferReady();
   void handleReturnBufferReady();
   void handleReturnBufferReady(unsigned char *returnBuffer, unsigned long validFrames);
   void handleSetTargetTemperature(double targetTemperature);
   void handleSetHV(double voltage);
   void handleAppendTimestamp(bool appendTimestamp);
   void handleSaveRawChanged(bool saveRaw);
//   void handleTriggeringSelectionChanged(int triggering);
   void handleSetTriggerTimeout(double triggerTimeout);

private:
   QThread *vsrDetectorThread;
   DetectorState state;
   unsigned int xResolution;
   unsigned int yResolution;
   int frameSize;
   std::ofstream outFile;
   bool appendTimestamp;
   bool saveRaw;
   QString errorMessage;
   bool triggeringAvailable;
   Triggering triggeringMode;
   TtlInput ttlInput;
   double triggerTimeout;
   Triggering triggerConfigMode;


   WindowsEvent *bufferReadyEvent;
//   WindowsEvent *returnBufferReadyEvent;
   WindowsEvent *showImageEvent;
   HANDLE transferBufferReadyEvent;
   HANDLE returnBufferReadyEvent;
   HANDLE notifyStateEvent;
   const QObject *parent;
   Mode mode;
   bool offsetsOn;
   bool triggered;
   double hvSetPoint;
   double targetTemperature;
   double vCal;
   double uMid;
   double detCtrl;

   bool timestampOn;
   int count;
   QString directory;
   QString prefix;
   char pathString[1024];
   char processingFilename[1024];
   // Data acquisition time in mSecs
   double dataAcquisitionDuration;
   u32 xRes;
   int sensorRows;
   u32 yRes;
   int sensorColumns;

   ULONGLONG totalFramesAcquired;
   u32 collectDcTime;
   double frameTime;
   int imgCntAverage;
   ULONG timeout;
   short *imageDest;
   int *summedImageDest;
   unsigned char *charImageDest;
   QPixmap imagePixmap;
   unsigned long framesPerBuffer;
   int loggingInterval;
   QString aspectFilename;
   QString aspectFilenameQ;
   IniFile *iniFile;
   HexitecSensorConfig sensorConfig;
   HexitecSetupRegister rowSetupRegister;
   HexitecSetupRegister columnSetupRegister;
   aS_Pleora_NTx * detectorHandle;
   DetectorCommand command;

   void constructorInit(const QObject *parent = 0);
   int initialise(Triggering triggering = Triggering::NO_TRIGGERING);
   void connectUp(const QObject *parent);
   LONG readIniFile(QString aspectFilename);
   HexitecSetupRegister initSetupRegister(QString type);
   void configCharacters2Bytes(std::string configCharacters, unsigned char *result);
   int setImageFormat(unsigned long xResolution, unsigned long yResolution);
   void updateState(DetectorState state);
   void imageDestToPixmap();
   unsigned char *getImage(int imageNumber);
   void run();
   void showError(const LPCSTR context, long asError);
   void showErrorNew(const str8 context);
   LONG collectOffsetValues();
   void setGetImageParams();
   int configure(bool triggeringSuspended);
   int configureDetector(bool triggeringSuspended = false);
};

#endif // VSRDETECTOR_H
