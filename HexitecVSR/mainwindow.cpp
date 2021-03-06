/*
 S D M Jacques 24th February 2011
*/
#include "mainwindow.h"
#include "renderarea.h"
#include "thumbviewer.h"
#include "chargesharing.h"
#include "plotter.h"
#include "parameters.h"

#include "detectorcontrolform.h"
#include "dataacquisitionform.h"

#include <QDockWidget>
#include <QMessageBox>
#include <QStatusBar>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QVector>
#include <math.h>
#include <sarray.h>
#include <QList>
#include <QPair>
#include "hxtfilereader.h"
#include "hexitecsofttrigger.h"
#include "dataacquisitionmodel.h"
#include "processingForm.h"
#include "processingbuffergenerator.h"
#include "serialport.h"
#include "badinifiledialog.h"

MainWindow::MainWindow()
{
   DetectorControlForm *detectorControlForm;
   DataAcquisitionForm *dataAcquisitionForm;
   ProcessingForm *processingForm;
   ProcessingBufferGenerator *processingBufferGenerator;
   ProcessingDefinition *processingDefinition;

   QSettings *settings = new QSettings(QSettings::UserScope, "TEDDI", "HexitecVSR");

   QString version = "HexitecVSR V1.0";
   QString nonDAQVersion = "HexitecVSR V1.0 (excluding DAQ)";

   activeDAQ = checkDAQChoice();

   if (activeDAQ)
   {
      hVOn = false;
      if (settings->contains("aspectIniFilename"))
      {

         QFileInfo fileInfo = QFileInfo(settings->value("aspectIniFilename").toString());
         if (!fileInfo.isReadable())
         {
            BadIniFileDialog *badIniFileDialog = new BadIniFileDialog();
            badIniFileDialog->setWindowTitle("aspectIniFilename: " + settings->value("aspectIniFilename").toString());
            badIniFileDialog->exec();
            exit(1);
         }
      }
      setWindowTitle(version);
      setWindowIconText(version);
   }
   else
   {
      setWindowTitle(nonDAQVersion);
      setWindowIconText(nonDAQVersion);
   }
   setWindowIcon(QIcon(":/images/HEXITEC.ico"));
   setMinimumSize(705,730);
   setUnifiedTitleAndToolBarOnMac(true);
   createMenus();

   readDir = "";
   readFilter = "";

//   saveH5 = false;

   QTabWidget *tabs = new QTabWidget(this);
   setCentralWidget(tabs);

   createApplicationOutput();

   tabs->addTab(createVisualisation(), QString("Visualisation"));

   connect(this, SIGNAL(writeMessage(QString)), ApplicationOutput::instance(), SLOT(writeMessage(QString)));
   connect(this, SIGNAL(writeWarning(QString)), ApplicationOutput::instance(), SLOT(writeWarning(QString)));
   connect(this, SIGNAL(writeError(QString)), ApplicationOutput::instance(), SLOT(writeError(QString)));

   connect(plotter, SIGNAL(renderChannel(double)), thumbViewer, SLOT(renderChannel(double)));
   connect(plotter, SIGNAL(renderSum(double, int)), thumbViewer, SLOT(renderSum(double, int)));
   connect(plotter, SIGNAL(renderSum(double, double)), thumbViewer, SLOT(renderSum(double, double)));
   connect(plotter, SIGNAL(renderChannel(double)), MainViewer::instance(), SLOT(renderChannel(double)));
   connect(plotter, SIGNAL(renderSum(double, int)), MainViewer::instance(), SLOT(renderSum(double, int)));
   connect(plotter, SIGNAL(renderSum(double, double)), MainViewer::instance(), SLOT(renderSum(double, double)));
   connect(plotter, SIGNAL(writeMessage(QString)), ApplicationOutput::instance(), SLOT(writeMessage(QString)));

   connect(MainViewer::instance()->getRenderArea(), SIGNAL(updatePlotter()), plotter, SLOT(updatePlotter()));
   connect(MainViewer::instance()->getRenderArea(), SIGNAL(updatePlotter(QVector<double>, double *, int)), plotter, SLOT(updatePlotter(QVector<double>, double *, int)));
   connect(MainViewer::instance()->getRenderArea(), SIGNAL(updatePlotter(QPoint, bool)), plotter, SLOT(updatePlotter(QPoint, bool)));
   connect(MainViewer::instance()->getRenderArea(), SIGNAL(updatePlotter(QVector <QPoint> &, bool)), plotter, SLOT(updatePlotter(QVector <QPoint> &, bool)));
   connect(MainViewer::instance()->getRenderArea(), SIGNAL(writeMessage(QString)), ApplicationOutput::instance(), SLOT(writeMessage(QString)));
   connect(plotter, SIGNAL(pixelAdded(QString)), pixelManipulationForm, SLOT(updatePixelList(QString)));
   connect(MainViewer::instance(), SIGNAL(writeMessage(QString)), ApplicationOutput::instance(), SLOT(writeMessage(QString)));
   connect(MainViewer::instance(), SIGNAL(readFiles(QStringList)), this, SLOT(readFiles(QStringList)));

   connect(thumbViewer, SIGNAL(activeSliceChanged(RenderArea *)), MainViewer::instance(), SLOT(activeSliceChanged(RenderArea *)));
   connect(thumbViewer, SIGNAL(writeMessage(QString)), ApplicationOutput::instance(), SLOT(writeMessage(QString)));

   connect(workspace, SIGNAL(removeSlice(Slice *)), this, SLOT(deleteSlice(Slice *)));
   connect(workspace, SIGNAL(initializeSlice(Slice *)), this, SLOT(initializeSlice(Slice *)));
   connect(workspace, SIGNAL(writeMessage(QString)), ApplicationOutput::instance(), SLOT(writeMessage(QString)));
   connect(workspace, SIGNAL(writeError(QString)), ApplicationOutput::instance(), SLOT(writeError(QString)));
//   connect(matlab::instance(), SIGNAL(matlabStatus(bool)), workspace, SLOT(matlabStatus(bool)));

//   connect(matlab::instance(), SIGNAL(writeMessage(QString)), ApplicationOutput::instance(), SLOT(writeMessage(QString)));
//   connect(matlab::instance(), SIGNAL(writeWarning(QString)), ApplicationOutput::instance(), SLOT(writeWarning(QString)));
//   connect(matlab::instance(), SIGNAL(writeError(QString)), ApplicationOutput::instance(), SLOT(writeError(QString)));

   if (activeDAQ)
   {
      // HEXITEC camera acquisition stuff
      activeHexitec = false;
      hexitecSoftTrigger = new HexitecSoftTrigger();
      connect(hexitecSoftTrigger, SIGNAL(writeMessage(QString)), ApplicationOutput::instance(), SLOT(writeMessage(QString)));
      connect(hexitecSoftTrigger, SIGNAL(writeWarning(QString)), ApplicationOutput::instance(), SLOT(writeWarning(QString)));
      connect(hexitecSoftTrigger, SIGNAL(writeError(QString)), ApplicationOutput::instance(), SLOT(writeError(QString)));
      connect(hexitecSoftTrigger, SIGNAL(readLastAcquiredFile(QString)), this, SLOT(readData(QString)));
      hexitecSoftTrigger->setProperty("objectName", "HEX");
      emit addObject(hexitecSoftTrigger);
   }

#ifdef NI
   if (activeDAQ)
   {
      hardTrigger = new HardTrigger();
      connect(hardTrigger, SIGNAL(writeMessage(QString)), ApplicationOutput::instance(), SLOT(writeMessage(QString)));
      connect(hardTrigger, SIGNAL(writeWarning(QString)), ApplicationOutput::instance(), SLOT(writeWarning(QString)));
      connect(hardTrigger, SIGNAL(writeError(QString)), ApplicationOutput::instance(), SLOT(writeError(QString)));
      hardTrigger->setProperty("objectName", "TTL");
      emit addObject(hardTrigger);
   }
#endif

   if (activeDAQ)
   {
      detectorControlForm = new DetectorControlForm();
      dataAcquisitionForm = new DataAcquisitionForm();
      connect(dataAcquisitionForm, SIGNAL(enableMainWindowActions()),
              this, SLOT(enableMainWindowActions()));
      connect(dataAcquisitionForm, SIGNAL(disableMainWindowActions()),
              this, SLOT(disableMainWindowActions()));
      connect(detectorControlForm, SIGNAL(enableHVActions()),
              this, SLOT(enableHVActions()));
      connect(detectorControlForm, SIGNAL(disableHVActions()),
              this, SLOT(disableHVActions()));
      connect(dataAcquisitionForm, SIGNAL(disableStopDAQAction()),
              this, SLOT(disableStopDAQAction()));
   }
//   We don't have one of these so this won't happen.
//   scriptingWidget->runInitScript();

   // The processing window needs to have been created before the data acquisition factory!
   IniFile *twoEasyIniFile;
   int rows, columns;
   if (settings->contains("hexitecVSRIniFilename"))
   {
      QString twoEasyFilename = Parameters::twoEasyIniFilename;
      twoEasyFilename = settings->value("hexitecVSRIniFilename").toString();
      twoEasyIniFile = new IniFile(twoEasyFilename);
      if ((rows = twoEasyIniFile->getInt("Processing/Rows")) == QVariant(INVALID))
      {
         rows = 160;
      }
      if ((columns = twoEasyIniFile->getInt("Processing/Columns")) == QVariant(INVALID))
      {
         columns = 160;
      }
   }
   processingDefinition = new ProcessingDefinition(rows * columns);
   processingForm = new ProcessingForm();
   processingBufferGenerator = new ProcessingBufferGenerator(processingDefinition);

   if (activeDAQ)
   {
      dataAcquisitionFactory = DataAcquisitionFactory::instance(dataAcquisitionForm, detectorControlForm,
                                                                progressForm, processingBufferGenerator, this);
//      motionControlform = new MotionControlForm();
   }

   if (activeDAQ)
   {
      HV *hv = VoltageSourceFactory::instance()->getHV();
      detectorControlForm->setHvName(hv->property("objectName").toString());
      QString daqName = dataAcquisitionFactory->getDataAcquisition()->property("objectName").toString();
      dataAcquisitionForm->setDaqName(daqName);
      QString daqModelName = dataAcquisitionFactory->getDataAcquisitionModel()->property("objectName").toString();
      dataAcquisitionForm->setDaqModelName(daqModelName);
      detectorControlForm->handleSetFingerTemperature();
      detectorControlForm->handleSetTriggerTimeout();
   }

   createStatusBar();
   tabs->addTab(processingForm->getMainWindow(), QString("Processing"));

   connect(processingForm, SIGNAL(configureSensor(int, int)),
           processingBufferGenerator, SLOT(handleConfigureSensor(int, int)));
   connect(processingForm, SIGNAL(processImages()),
           processingBufferGenerator, SLOT(handlePostProcessImages()));
   connect(processingForm, SIGNAL(configureProcessing(bool, bool, int, int, QString)),
           processingBufferGenerator, SLOT(handleConfigureProcessing(bool, bool, int, int, QString)));
   connect(processingForm, SIGNAL(configureProcessing(bool, bool, long long, long long, double, bool, QString, QString)),
           processingBufferGenerator, SLOT(handleConfigureProcessing(bool, bool, long long, long long, double, bool, QString, QString)));
   connect(processingForm, SIGNAL(configureProcessing(int, int)),
           processingBufferGenerator, SLOT(handleConfigureProcessing(int, int)));
   connect(processingBufferGenerator, SIGNAL(hxtFileWritten(unsigned short*, QString)),
           this, SLOT(readBuffer(unsigned short*, QString)));
   /// Prevent HexitecGigE/ProcessingBufferGenerator swamping with iterations of
   ///   same image while GUI busy rendering same image on display
   connect(this, SIGNAL(mainWindowBusy(bool)),
           processingBufferGenerator, SLOT(handleMainWindowBusy(bool)));
   bMainWindowBusy = false;
   ///
   connect(processingForm, SIGNAL(configureProcessing(QStringList, QString, QString)),
           processingBufferGenerator, SLOT(handleConfigureProcessing(QStringList, QString, QString)));
   connect(processingBufferGenerator, SIGNAL(processingComplete()),
           processingForm, SLOT(handleProcessingComplete()));
   connect(processingBufferGenerator, SIGNAL(imageStarted()),
           processingForm, SLOT(handleImageStarted()));
   connect(processingBufferGenerator, SIGNAL(invalidParameterFiles(bool,bool,bool)),
           processingForm, SLOT(handleInvalidParameterFiles(bool,bool,bool)));

//   connect(processingForm, SIGNAL(processImages(int, int)),
//           processingBufferGenerator, SLOT(handlePostProcessImages(int, int)));
//   connect(DetectorFactory::instance()->getVSRDetector(), SIGNAL(detectorResolutionSet(unsigned char, unsigned char)),
//           processingForm, SLOT(handleDetectorResolutionSet(unsigned char, unsigned char)));

   processingForm->initialiseProcessingForm();

   if (activeDAQ)
   {
      tabs->addTab(detectorControlForm->getMainWindow(), QString("Detector Control"));
      tabs->addTab(dataAcquisitionForm->getMainWindow(), QString("Data Acquisition"));
      connect(this, SIGNAL(startDAQ()), dataAcquisitionForm, SLOT(handleCollectImagesPressed()));
      connect(this, SIGNAL(stopDAQ()), dataAcquisitionForm, SLOT(handleAbortDAQPressed()));
      connect(this, SIGNAL(startHV()), detectorControlForm, SLOT(handleHVOn()));
      connect(this, SIGNAL(stopHV()), detectorControlForm, SLOT(handleHVOff()));
   }

   if (activeDAQ)
   {
      VSRDetector *vsrDetector = DetectorFactory::instance()->getVSRDetector();
      connect(this, SIGNAL(executeBufferReady(unsigned char*, unsigned long)), dataAcquisitionFactory->getDataAcquisition(),
              SLOT(handleBufferReady(unsigned char*, unsigned long)));
      connect(processingBufferGenerator, SIGNAL(returnBufferReady(unsigned char*, unsigned long)),
              DetectorFactory::instance()->getVSRDetector(), SLOT(handleReturnBufferReady(unsigned char*, unsigned long)));
      connect(this, SIGNAL(executeShowImage()),
              vsrDetector, SLOT(handleShowImage()));
   }
}

MainWindow::~MainWindow()
{
   if (activeDAQ)
   {
      delete dataAcquisitionFactory;
   }
}

QMainWindow *MainWindow::createVisualisation()
{
   visualisation = new QMainWindow();

   // MainViewer and ThumbViewer are children of visualisation
   createMainViewer();
   createProgressViewer();
   createThumbViewer();

   // Temporarily plotter and workspace are placed inside another tabs widget which is attached to visualisation.
   QTabWidget *subTabs = new QTabWidget(visualisation);
   createPlotter();
   createWorkSpace();
   subTabs->addTab(plotter->getMainWindow(), QString("Plotter"));
   subTabs->addTab(workspace->getMainWindow(), QString("Workspace"));

   createPixelManipulation();
   QDockWidget *dock = new QDockWidget(tr("Plotter and Workspace"), visualisation);
   dock->setAllowedAreas(Qt::RightDockWidgetArea);

   QWidget *newWidget = new QWidget(this);
   QGridLayout *layout = new QGridLayout;
   layout->setColumnStretch(0, 1);
   layout->setColumnStretch(1, 0);
   layout->addWidget(subTabs, 0, 0);
   layout->addWidget(pixelManipulationForm, 0, 1);
   newWidget->setLayout(layout);
   dock->setWidget(newWidget);
   newWidget->setParent(dock);
   visualisation->addDockWidget(Qt::RightDockWidgetArea, dock);
   viewMenu->addAction(dock->toggleViewAction());

   return visualisation;
}

void MainWindow::readFiles()
{
   const char *df = "Data Files (*.hxt)";
//   const char *sf = "Script Files (*.js)";
//   const char *mf = "Matlab Files (*.m)";

   QString filter = tr(df) + ";;" /*+ tr(sf) + ";;" *//*+ tr(mf)*/;
   QStringList files = QFileDialog::getOpenFileNames(this,
                                                     tr("Select Files for Reading"),
                                                     readDir,
                                                     filter,
                                                     &readFilter);

   // NB Do not remove this duplication of variables - the documentation for
   // QFileDialog says you should iterate over a COPY of the returned list.
   QStringList fileNameList = files;

   if (!fileNameList.isEmpty())
   {
      // Save readDir for next call
      readDir = QFileInfo(fileNameList.at(0)).absoluteDir().canonicalPath();

      // Pass the whole list to Slice::readFileNameList.
      // Note that we cannot process the files one by one because we might be constructing
      // a slice from a whole set of files.
      for (int i = 0; i < fileNameList.size(); i++)
      {
         QString suffix = QFileInfo(fileNameList[i]).suffix();
         if (suffix != "js" && suffix != "m")
         {
            break;
         }
      }

      QVector<Slice *> slices = Slice::readFileNameList(fileNameList);

      for (int i = 0; i < slices.size(); i++)
      {
         if (slices.at(i) != NULL)
         {
            initializeSlice(slices.at(i));
         }
      }
   }
}

/* This method does all the connecting of a new slice to its environment:
  connects signals, sets as active slice, adds it to scripting, adds to main viewer and thumb viewer.
  */
void MainWindow::initializeSlice(Slice *slice, int sliceNumber)
{
   // This connect enables the new Slice to create another slice from scripting and emit this signal
   // to get to this point - see Slice::times().
   if (sliceNumber < 0)
   {
      connect(slice, SIGNAL(initializeSlice(Slice*)), this, SLOT(initializeSlice(Slice*)));
      DataModel::instance()->setActiveSlice(slice);
      emit addObject(slice);

      MainViewer::instance()->showNewActiveSlice();
      thumbViewer->addSlice(slice);
   }
   else
   {
      DataModel::instance()->setActiveSlice(slice);
      MainViewer::instance()->showNewActiveSlice();
   }
   update();
}

void MainWindow::handleStartDAQ()
{
   disableMainWindowActions();
   emit startDAQ();
}

void MainWindow::handleStopDAQ()
{
   emit stopDAQ();
   enableMainWindowActions();
}

void MainWindow::handleStartHV()
{
   hVOn = true;
   emit startHV();
   enableHVActions();
}

void MainWindow::handleStopHV()
{
   hVOn = false;
   emit stopHV();
   enableHVActions();
}

void MainWindow::deleteActiveSlice()
{
   deleteSlice(DataModel::instance()->getActiveSlice());
}

void MainWindow::deleteSlice(Slice *slice)
{
   if (slice != NULL)
   {
      emit writeMessage("Volume size was: " + QString::number(DataModel::instance()->numberOfSlices()));
      emit writeMessage("...deleting slice " + slice->objectName());
      thumbViewer->deleteSlice(slice);
      //DataModel::instance()->deleteSlice(slice);
      emit writeMessage("Volume size is now: " + QString::number(DataModel::instance()->numberOfSlices()));
      update();
   }
}

void MainWindow::deleteAllSlices()
{
   for (int i = 0 ; i < DataModel::instance()->numberOfSlices(); ++i)
   {
   }
}

void MainWindow::deleteFirstSlice()
{
//    deleteSlice(DataModel::instance()->sliceAt(0));               // Causes a crash in a very specific case?
     thumbViewer->deleteSlice( DataModel::instance()->sliceAt(0));  // Also causes a crash, in a very specific use case
}

void MainWindow::deleteExcessSlices()
{
    //int numberOfSlices = DataModel::instance()->numberOfSlices();
    //emit writeMessage("MainWindow::deleteExcessSlices numberOfSlices: " + QString::number(numberOfSlices));   // Debug info

    // Keep removing slices until we have less than 10 remaining
    while (DataModel::instance()->numberOfSlices() > 9)
    {
        deleteFirstSlice();
        emit writeMessage("MainWindow Removed the oldest Slice! (There were 10+ already)");
    }
}


void MainWindow::about()
{
   QMessageBox::about(this, tr("About This App"),
                      tr(//"<img src=:/images/Hexitec_Logo10.png>",
                         "The HEXITEC VSR software is designed to control and readout the data from the HEXITEC detector. "
                         " The software will collect, process and display the hyperspectral data collected from HEXITEC. "
                         " <p> This work was designed and funded by the Science & Technology Facilities Council, UK "
                         "and builds upon the work conducted throughout the HEXITEC collaboration through the EPSRC funded "
                         "grants (EP/D048737/1 and EP/H046577/1). </p>"
                         "<p>For more information please contact Matt Wilson - matt.wilson@stfc.ac.uk.</p>"));
}

void MainWindow::createMenus()
{
   QToolBar *fileToolBar = addToolBar(tr("File"));

   if (activeDAQ)
   {
      startDAQAct = new QAction(QIcon(":/images/startDAQ.png"), tr(""),this);
      stopDAQAct = new QAction(QIcon(":/images/stopDAQ.png"), tr(""),this);
      startDAQAct->setText("Start DAQ");
      stopDAQAct->setText("Stop DAQ");
      startDAQAct->setDisabled(true);
      stopDAQAct->setDisabled(true);
      fileToolBar->addAction(startDAQAct);
      fileToolBar->addAction(stopDAQAct);
      connect(startDAQAct, SIGNAL(triggered()), this, SLOT(handleStartDAQ()));
      connect(stopDAQAct, SIGNAL(triggered()), this, SLOT(handleStopDAQ()));

      startHVAct = new QAction(QIcon(":/images/VbiasOn.png"), tr(""),this);
      stopHVAct = new QAction(QIcon(":/images/VbiasOff.png"), tr(""),this);
      startHVAct->setText("Turn HV On (start bias refreshing)");
      stopHVAct->setText("Turn HV Off (stop bias refreshing)");
      startHVAct->setDisabled(true);
      stopHVAct->setDisabled(true);
      fileToolBar->addAction(startHVAct);
      fileToolBar->addAction(stopHVAct);
      connect(startHVAct, SIGNAL(triggered()), this, SLOT(handleStartHV()));
      connect(stopHVAct, SIGNAL(triggered()), this, SLOT(handleStopHV()));
   }

   QAction *deleteSliceAct = new QAction(QIcon(":/images/removeImage.png"), tr(""),this);
   QAction *readAction = new QAction(QIcon(":/images/ReadImage.png"), tr("&Load data or scripts..."), this);
   QAction *quitAct = new QAction(tr("&Quit"), this);
   QAction *aboutAct = new QAction(tr("&About"), this);

   readAction->setShortcuts(QKeySequence::New);
   quitAct->setShortcuts(QKeySequence::Quit);

   deleteSliceAct->setText(tr("Clear active image"));
   readAction->setStatusTip(tr("Load data or scripts"));
   quitAct->setStatusTip(tr("Quit the application"));
   aboutAct->setStatusTip(tr("Show the application's About box"));

   connect(deleteSliceAct, SIGNAL(triggered()), this, SLOT(deleteActiveSlice()));
   connect(readAction, SIGNAL(triggered()), this, SLOT(readFiles()));
   connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));
   connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

   /*QMenu **/fileMenu = menuBar()->addMenu(tr("&File"));
   viewMenu = menuBar()->addMenu(tr("&View"));
   QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

   // Must make fileMenu a class number
   // this->fileMenu
   fileMenu->addAction(readAction);
   fileMenu->addAction(deleteSliceAct);
   fileMenu->addSeparator();
   fileMenu->addAction(quitAct);

   menuBar()->addSeparator();
   helpMenu->addAction(aboutAct);

   fileToolBar->addAction(deleteSliceAct);
   fileToolBar->addAction(readAction);

//   QAction *exportActiveSliceToMatlab = new QAction(QIcon(":/images/exportToMatlab.png"), tr(""),this);
//   exportActiveSliceToMatlab->setText(tr("Export active slice to Matlab"));
//   exportActiveSliceToMatlab->setEnabled(false);
//   fileToolBar->addAction(exportActiveSliceToMatlab);
//   connect(exportActiveSliceToMatlab, SIGNAL(triggered()), this, SLOT(sendActiveSliceToMatlab()));
//   connect(matlab::instance(), SIGNAL(matlabStatus(bool)), exportActiveSliceToMatlab, SLOT(setEnabled(bool)));

//   QAction *importActiveSliceFromMatlab = new QAction(QIcon(":/images/importFromMatlab.png"), tr(""),this);
//   importActiveSliceFromMatlab->setEnabled(false);
//   importActiveSliceFromMatlab->setText(tr("Import active slice from Matlab"));
//   fileToolBar->addAction(importActiveSliceFromMatlab);
//   connect(importActiveSliceFromMatlab, SIGNAL(triggered()), this, SLOT(getActiveSliceFromMatlab()));
//   connect(matlab::instance(), SIGNAL(matlabStatus(bool)), importActiveSliceFromMatlab, SLOT(setEnabled(bool)));
}

void MainWindow::createStatusBar()
{
   statusBar()->showMessage(tr("Ready"));
}

/* The renderArea is now inside MainViewer.
   The toolBar created for the RenderArea is displayed on the MainWindow so we need
   to get that out too - cf. PlotterWindow which keeps its own toolbar.
   */
void MainWindow::createMainViewer()
{
   visualisation->setCentralWidget(MainViewer::instance());
   addToolBar(Qt::TopToolBarArea, MainViewer::instance()->createToolbar());
}

void MainWindow::createPixelManipulation()
{
   pixelManipulationForm = new PixelManipulationForm(visualisation);
   connect (pixelManipulationForm, SIGNAL(pixelAdditionChanged(bool)),
            plotter, SLOT(handlePixelAdditionChanged(bool)));
}

void MainWindow::createPlotter()
{
   plotter = new Plotter(visualisation);
}

void MainWindow::createThumbViewer()
{
   thumbViewer = new ThumbViewer(visualisation);

   QDockWidget *dock = new QDockWidget(tr("ThumbViewer"), visualisation);
   dock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea | Qt::RightDockWidgetArea);
   dock->setMinimumHeight(175);
   visualisation->addDockWidget(Qt::BottomDockWidgetArea, dock);
   viewMenu->addAction(dock->toggleViewAction());
   QMainWindow *thumbWindow = thumbViewer->getMainWindow();
   dock->setWidget(thumbWindow) ;
   thumbWindow->setParent(dock) ;
}

void MainWindow::createProgressViewer()
{
   progressForm = new ProgressForm(visualisation);

   QDockWidget *dock = new QDockWidget(tr("ProgressViewer"), visualisation);
   dock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea | Qt::LeftDockWidgetArea);
   dock->setMinimumHeight(175);
//   dock->setMaximumWidth(120);
   visualisation->addDockWidget(Qt::BottomDockWidgetArea, dock);
//   viewMenu->addAction(dock->toggleViewAction());
   QMainWindow *progressWindow = progressForm->getMainWindow();
   dock->setWidget(progressWindow) ;
   progressWindow->setParent(dock) ;
}

void MainWindow::createWorkSpace()
{
   workspace = new Workspace();
}

void MainWindow::createApplicationOutput()
{
   QDockWidget *dock = new QDockWidget(tr("Application Output"), this);
   dock->setAllowedAreas(Qt::BottomDockWidgetArea);
   dock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);
   addDockWidget(Qt::BottomDockWidgetArea, dock);
   dock->setWidget(ApplicationOutput::instance());
   viewMenu->addAction(dock->toggleViewAction());
}

void MainWindow::externalChargeShare()
{
   chargeSharingInstance = new ChargeSharing(0);
   connect(chargeSharingInstance, SIGNAL(writeMessage(QString)), ApplicationOutput::instance(), SLOT(writeMessage(QString)));
   connect(chargeSharingInstance, SIGNAL(readData(QString)), this, SLOT(readData(QString)));
   chargeSharingInstance->exec();
}


void MainWindow::handleSpectrumFile(QString fileName)
{
    /*qDebug() << "MainWindow received spectrum file: " << fileName*/;
}

void MainWindow::save()
{
   hexitecSoftTrigger->acquire();
}

/* This is a slot and may be used from scripts.
  */
void MainWindow::readData(QString fileName)
{
   readFiles(QStringList(fileName));
}

void MainWindow::readFiles(QStringList files)
{
   // Drag and Drop operations seem to result in files being in any old order so sort them first in
   // case they are a set of numbered files representing a single slice.
   files.sort();

   QVector<Slice *> slices = Slice::readFileNameList(files);

   for (int i = 0; i < slices.size(); i++)
   {
      if (slices.at(i) != NULL)
      {
         initializeSlice(slices.at(i));
      }
   }
}

void MainWindow::readBuffer(unsigned short* buffer, QString fileName)
{
   if (busyMutex.tryLock(100))
   {
      bMainWindowBusy = true;
      busyMutex.unlock();
      emit mainWindowBusy(bMainWindowBusy);

   }
   else
   {
      emit writeError(QString(" *** ERROR: MainWindow couldn't lock busyMutex to signal it's BUSY!"));
      return;
   }

   int sliceNumber = -1;
   /*qDebug() << QThread::currentThreadId() << " MainWind Updating file:" <<  fileName
            << " buffer: " << &buffer;*/

   MainViewer::instance()->getRenderArea()->setMouseEnabled(false);
   Slice *slice = Slice::readFileBuffer(buffer, fileName);
   sliceNumber = slice->getSliceToReplace();
   initializeSlice(slice, sliceNumber);
   /*qDebug() << QThread::currentThreadId() << " MainWind now finished";*/

   if (busyMutex.tryLock(100))
   {
      bMainWindowBusy = false;
      busyMutex.unlock();
      emit mainWindowBusy(bMainWindowBusy);
   }
   else
   {
      emit writeError(QString(" *** ERROR: MainWindow couldn't lock busyMutex to signal it's IDLE!"));
   }
}

void MainWindow::writeCsv(QString fileName, QVector<double> col0, double *col1, int numberOfBins)
{
   stringstream outputText(stringstream::out);

   for (/*unsigned */int i = 0; i < numberOfBins; i++)
   {
      outputText << col0[i] << "," << col1[i] << std::endl;
   }

   fileName.replace(".hxt", ".csv");
   QByteArray ba = fileName.toLatin1();
   const char *fileNameChars = ba.data();
   outFile.open(fileNameChars, std::ofstream::trunc);
   outFile.write(outputText.str().c_str(), outputText.str().length());
   outFile.close();

}

//void MainWindow::writeH5(QString fileName)
//{
//   QString program = "Translator.exe";
//   QStringList arguments;
//   arguments << fileName << "h5";

//   QProcess *translateProcess = new QProcess();
//   translateProcess->execute(program, arguments);
//}

void MainWindow::closeEvent(QCloseEvent *event)
{
//    if ( bHexitechProcessingBusy )
//    {
//        QMessageBox::StandardButton ret;
//        ret = QMessageBox::warning(this, tr("Application"),
//                     tr("The hexitech thread is still processing in the background.\n"
//                        "Quitting now will abort that processing.\n"
//                        "Do you want to abort?"),
//                     QMessageBox::Abort | QMessageBox::Cancel);
//        if (ret == QMessageBox::Abort)
//        {
//            // Quit
//            event->accept();
//        }
//        else if (ret == QMessageBox::Cancel)
//        {
//            // Don't quit
//            event->ignore();
//        }
//    }
}

bool MainWindow::checkDAQChoice()
{
   bool activeDAQ = false;

   QSettings *settings = new QSettings(QSettings::UserScope, "TEDDI", "HexitecVSR");

   if (settings->contains("hexitecVSRIniFilename"))
   {

      QFileInfo fileInfo = QFileInfo(settings->value("hexitecVSRIniFilename").toString());
      if (!fileInfo.isReadable())
      {
         BadIniFileDialog *badIniFileDialog = new BadIniFileDialog();
         badIniFileDialog->setWindowTitle("hexitecVSRIniFilename: " + settings->value("hexitecVSRIniFilename").toString());
         badIniFileDialog->exec();
         exit(1);
      }
   }

   if (settings->contains("DataAcquisition"))
   {
      if (settings->value("DataAcquisition").toString() == "On")
      {
         activeDAQ = true;
      }
      else
      {
         activeDAQ = false;
         emit writeWarning("Data Acquisition is disabled in this session");
      }
   }

   return activeDAQ;
}

void MainWindow::handleBufferReady()
{
//   qDebug() << "!!!!!!!!!!!!!!!!!!!!!!MainWindow::handleBufferReady() " << VSRDetector::getBufferReady();
   emit executeBufferReady(VSRDetector::getBufferReady(), VSRDetector::getValidFrames());
}

void MainWindow::handleShowImage()
{
   emit executeShowImage();
}

//void MainWindow::handleProcessingComplete(QString fileName)
//{
//   if (saveH5)
//   {
//      writeH5(fileName);
//   }
//}

//void MainWindow::handleSaveH5Changed(bool saveH5)
//{
//   this->saveH5 = saveH5;
//}

void MainWindow::enableMainWindowActions()
{
   startDAQAct->setEnabled(true);
   stopDAQAct->setDisabled(true);
}

void MainWindow::enableHVActions()
{
   if (hVOn)
   {
      stopHVAct->setEnabled(true);
      startHVAct->setDisabled(true);
   }
   else
   {
      startHVAct->setEnabled(true);
      stopHVAct->setDisabled(true);
   }
}

void MainWindow::disableMainWindowActions()
{
   startDAQAct->setDisabled(true);
   stopDAQAct->setEnabled(true);
}

void MainWindow::disableStopDAQAction()
{
   stopDAQAct->setEnabled(false);
}

void MainWindow::disableHVActions()
{
   startHVAct->setDisabled(true);
   stopHVAct->setDisabled(true);
}
