#ifndef IMAGEACQUIRER_H
#define IMAGEACQUIRER_H

#include <QObject>
#include "vsrdetector.h"

class ImageAcquirer : public QObject
{
   Q_OBJECT
public:
   ImageAcquirer(QObject *parent = 0);
   ~ImageAcquirer();
   void setDetector(VSRDetector *detector);

private:
   QThread *imageAcquirerThread;
   VSRDetector *detector;
signals:

public slots:
   void handleExecuteAcquireImages(bool startOfImage = true);
};

#endif // IMAGEACQUIRER_H
