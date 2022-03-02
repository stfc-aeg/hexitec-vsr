#ifndef DETECTORFACTORY_H
#define DETECTORFACTORY_H

/* Following necessary as a workaround for qdatetime errors at Qt 5.0.2.
 * See for instance http://qt-project.org/forums/viewthread/22133 */
#define NOMINMAX

#include "vsrdetector.h"
#include "imageacquirer.h"
#include "detectormonitor.h"

#include <string>
class DetectorFactory : public QObject
{
   Q_OBJECT
public:
   static DetectorFactory *instance();
   static DetectorFactory *instance(const QObject *parent);
   VSRDetector *createVSRDetector(QString aspectFilename, QObject *parent = 0);
   VSRDetector *createVSRDetector(string aspectFilename);
   VSRDetector *getVSRDetector();
   DetectorMonitor *getDetectorMonitor();
   HANDLE *getNotifyStateEvent();
   WindowsEvent *getBufferReadyEvent();
//   WindowsEvent *getReturnBufferReadyEvent();
   WindowsEvent *getShowImageEvent();
   HANDLE *getTransferBufferReadyEvent();
   HANDLE *getReturnBufferReadyEvent();
   HANDLE *getMonitoringDoneEvent();
   HANDLE *getTemperatureBelowDPEvent();
    ~DetectorFactory();

protected:
   explicit DetectorFactory(const QObject *parent = 0);
private:
   static DetectorFactory *dfInstance;
   ImageAcquirer *imageAcquirer;
   VSRDetector *vsrDetector;
   DetectorMonitor *detectorMonitor;
   const QObject *parent;
   WindowsEvent *bufferReadyEvent;
//   WindowsEvent *returnBufferReadyEvent;
   WindowsEvent *showImageEvent;
   HANDLE *transferBufferReadyEvent;
   HANDLE *returnBufferReadyEvent;
   HANDLE *notifyStateEvent;
};

#endif // DETECTORFACTORY_H
