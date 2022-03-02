#ifndef GENREALPIXELPROCESSOR_H
#define GENREALPIXELPROCESSOR_H

#include <cstdint>
#include <string>
#include <unordered_map>

#define DETS 4
#define ROWS 80
#define COLUMNS 80
#define DETPOINTS 6400
#define VSRPOINTS 25600

using namespace std;

class GeneralPixelProcessor
{

public:
   static uint16_t pixelMap[VSRPOINTS];
   static bool pixelMapInitialised;
   static uint32_t frameSize;
   static uint16_t dets[DETS][DETPOINTS];
   static uint16_t input[ROWS][COLUMNS];
   static uint16_t rotated2d[ROWS][COLUMNS];

public:
   GeneralPixelProcessor();
   void initialiseEnergyCalibration(double *gradientValue, double *interceptValue);
   void setFrameSize(int frameSize);
   void setEnergyCalibration(bool energyCalibration);
   void setLastFrameCorrection(bool lastFrameCorrection);
   void setGradientValue(double *gradientValue);
   void setInterceptValue(double *gradientValue);
   double *getGradientValue();
   double *getInterceptValue();
   /// Need to change PixelProcessorFrame::lastRe_ordered size, but as member not part of parent,
   ///  and PixelProcessorFrame object only known at program execution, we need a virtual function here
   virtual void resetLastRe_orderedSize();
   ///

   uint16_t *processFrame(unordered_map<int, double>**pixelRawValMapPtr,
                          uint16_t *frame);
   virtual uint16_t *processFrame(unordered_map<int, double>**pixelRawValMapPtr,
                                  uint16_t *frame, uint16_t thresholdValue);
   virtual uint16_t *processFrame(unordered_map<int, double>**pixelRawValMapPtr,
                                  uint16_t *frame, uint16_t *thresholdPerPixel);

   virtual uint16_t *processFrame(uint16_t *frame,
              unordered_map<int, double>**pixelEnergyMapPtr);
   virtual uint16_t *processFrame(uint16_t *frame, uint16_t thresholdValue,
				  unordered_map<int, double>**pixelEnergyMapPtr);
   virtual uint16_t *processFrame(uint16_t *frame, uint16_t *thresholdPerPixel,
				  unordered_map<int, double>**pixelEnergyMapPtr);

   uint16_t *processRe_orderFrame(unordered_map<int, double>**pixelRawValMapPtr, uint16_t *frame);
   virtual uint16_t *processRe_orderFrame(unordered_map<int, double>**pixelRawValMapPtr, uint16_t *frame, uint16_t thresholdValue);
   virtual uint16_t *processRe_orderFrame(unordered_map<int, double>**pixelRawValMapPtr, uint16_t *frame, uint16_t *thresholdPerPixel);

   uint16_t *processRe_orderFrame(uint16_t *frame, 
				  unordered_map<int, double>**pixelEnergyMapPtr);
   virtual uint16_t *processRe_orderFrame(uint16_t *frame, uint16_t thresholdValue,
                                                  unordered_map<int, double>**pixelEnergyMapPtr);
   virtual uint16_t *processRe_orderFrame(uint16_t *frame, uint16_t *thresholdPerPixel,
              unordered_map<int, double>**pixelEnergyMapPtr);

protected:
   void initialisePixelMap();
   void rotate180(uint16_t *arr);
   bool energyCalibration;
   bool lastFrameCorrection;
   bool chargedSharingCorrection;
   double *gradientValue;
   double *interceptValue;
   uint16_t *lastFrame;

};

#endif // GENREALPIXELPROCESSOR_H
