Rem Batch file that will build hexitec processing, DetectorLib, VSRLib, and HexitecVSR.exe
Rem NOTE: HexitecGigE.exe has been commented out, not needed for the 2x2 Camera software?
Rem Note2: VSR.dll (when required) is built separately through Microsoft Visual Studio

del HxtProcessingLib\debug\HxtProcessingLib.lib

cd\develop\projects\HexitecVSR\HxtProcessingLib
C:\Qt\Tools\QtCreator\bin\jom.exe -f Makefile.Debug clean
C:\Qt\Tools\QtCreator\bin\jom.exe -f Makefile.Debug 

echo "___________________________________________________________________________"

cd\develop\projects\HexitecVSR\

del DetectorLib\debug\DetectorLib.lib

cd\develop\projects\HexitecVSR\DetectorLib
C:\Qt\Tools\QtCreator\bin\jom.exe -f Makefile.Debug clean
C:\Qt\Tools\QtCreator\bin\jom.exe -f Makefile.Debug 

echo "___________________________________________________________________________"

cd\develop\projects\HexitecVSR\

del VSRLib\debug\VSRLib.lib

cd\develop\projects\HexitecVSR\VSRLib
C:\Qt\Tools\QtCreator\bin\jom.exe -f Makefile.Debug clean
C:\Qt\Tools\QtCreator\bin\jom.exe -f Makefile.Debug 

echo "___________________________________________________________________________"

cd\develop\projects\HexitecVSR\

del HexitecVSR\debug\HexitecVSR.exe

cd\develop\projects\HexitecVSR\HexitecVSR
C:\Qt\Tools\QtCreator\bin\jom.exe -f Makefile.Debug clean
C:\Qt\Tools\QtCreator\bin\jom.exe -f Makefile.Debug 

cd\develop\projects\HexitecVSR\

:: del HexitecGigE\debug\HexitecGigE.exe

:: cd\develop\projects\HexitecVSR\HexitecGigE
:: C:\Qt\Tools\QtCreator\bin\jom.exe -f Makefile.Debug clean
:: C:\Qt\Tools\QtCreator\bin\jom.exe -f Makefile.Debug 

:: cd\develop\projects\HexitecVSR\

echo "_______________________________________"
echo "HxtProcessingLib?"
dir HxtProcessingLib\debug\HxtProcessingLib.lib"
echo "_______________________________________"
echo "DetectorLib?"
dir DetectorLib\debug\DetectorLib.lib"
echo "_______________________________________"
echo "VSRLib?"
dir VSRLib\debug\VSRLib.lib"
:: echo "_______________________________________"
:: echo "HexitecGigE?"
:: dir HexitecGigE\debug\HexitecGigE.exe
echo "_______________________________________"
echo "HexitecVSR?"
dir HexitecVSR\debug\HexitecVSR.exe

