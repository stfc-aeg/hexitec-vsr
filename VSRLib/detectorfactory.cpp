#include "detectorfactory.h"
#include "windowsevent.h"
#include <Windows.h>

#include <iostream>
#include <QApplication>

using namespace std;

DetectorFactory *DetectorFactory::dfInstance = 0;

DetectorFactory::DetectorFactory(const QObject *parent)
{
   this->parent = parent;
}

DetectorFactory *DetectorFactory::instance()
{
   if (dfInstance == 0)
   {
      dfInstance = new DetectorFactory(0);
   }
   return dfInstance;
}

DetectorFactory *DetectorFactory::instance(const QObject *parent)
{
   if (dfInstance == 0)
   {
      dfInstance = new DetectorFactory(parent);
   }
   return dfInstance;
}

VSRDetector *DetectorFactory::createVSRDetector(string aspectFilename)
{
  QString aspectFilenameQ = QString::fromStdString(aspectFilename);
  vsrDetector = new VSRDetector(aspectFilenameQ);

  return vsrDetector;
}

VSRDetector *DetectorFactory::createVSRDetector(QString aspectFilename, QObject *parent)
{
   vsrDetector = new VSRDetector(aspectFilename, parent);
   imageAcquirer = new ImageAcquirer(this);
   imageAcquirer->setDetector(vsrDetector);
   bufferReadyEvent = vsrDetector->getBufferReadyEvent();
   returnBufferReadyEvent = vsrDetector->getReturnBufferReadyEvent();
   showImageEvent = vsrDetector->getShowImageEvent();
   notifyStateEvent = vsrDetector->getNotifyStateEvent();

   detectorMonitor = new DetectorMonitor(vsrDetector, NULL);

   return vsrDetector;
}

VSRDetector *DetectorFactory::getVSRDetector()
{
  return vsrDetector;
}

DetectorMonitor *DetectorFactory::getDetectorMonitor()
{
   return detectorMonitor;
}

WindowsEvent *DetectorFactory::getBufferReadyEvent()
{
   return bufferReadyEvent;
}

HANDLE *DetectorFactory::getTransferBufferReadyEvent()
{
   return transferBufferReadyEvent;
}

HANDLE *DetectorFactory::getReturnBufferReadyEvent()
{
   return returnBufferReadyEvent;
}

WindowsEvent *DetectorFactory::getShowImageEvent()
{
   return showImageEvent;

}

HANDLE *DetectorFactory::getNotifyStateEvent()
{
   return notifyStateEvent;
}

DetectorFactory::~DetectorFactory()
{
   dfInstance = 0;
}
