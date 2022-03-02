#include "imageprocessorhandler.h"
#include <QDebug>
#include <QThread>
ImageProcessorHandler::ImageProcessorHandler(ImageProcessor *imageProcessor) :
   QThread()
{
   this->imageProcessor = imageProcessor;
   start();
}

void ImageProcessorHandler::run()
{
   time_t now = time(0);
   imageProcessor->setImageInProgress(true);
   imageProcessor->handleProcess();

   //shk78 qDebug()<< "ImageProcessorHandler::run()ENDING!!! threadId: "<< QThread::currentThreadId();
   now = time(0);
   delete imageProcessor;
   emit processingComplete();
}
