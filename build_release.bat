Rem Batch file that will build hexitec processing, DetectorLib, VSRLib, and HexitecVSR.exe
Rem NOTE: HexitecGigE.exe has been commented out, not needed for the 2x2 Camera software?
Rem Note2: VSR.dll (when required) is built separately through Microsoft Visual Studio

del HxtProcessingLib\release\HxtProcessingLib.lib

cd\develop\projects\HexitecVSR\HxtProcessingLib
C:\Qt\Tools\QtCreator\bin\jom.exe -f Makefile.Release clean
C:\Qt\Tools\QtCreator\bin\jom.exe -f Makefile.Release 

echo "___________________________________________________________________________"

cd\develop\projects\HexitecVSR\

del DetectorLib\release\DetectorLib.lib

cd\develop\projects\HexitecVSR\DetectorLib
C:\Qt\Tools\QtCreator\bin\jom.exe -f Makefile.Release clean
C:\Qt\Tools\QtCreator\bin\jom.exe -f Makefile.Release 

echo "___________________________________________________________________________"

cd\develop\projects\HexitecVSR\

del VSRLib\release\VSRLib.lib

cd\develop\projects\HexitecVSR\VSRLib
C:\Qt\Tools\QtCreator\bin\jom.exe -f Makefile.Release clean
C:\Qt\Tools\QtCreator\bin\jom.exe -f Makefile.Release 

echo "___________________________________________________________________________"

cd\develop\projects\HexitecVSR\

del HexitecVSR\release\HexitecVSR.exe

cd\develop\projects\HexitecVSR\HexitecVSR
C:\Qt\Tools\QtCreator\bin\jom.exe -f Makefile.Release clean
C:\Qt\Tools\QtCreator\bin\jom.exe -f Makefile.Release 

cd\develop\projects\HexitecVSR\

:: del HexitecGigE\release\HexitecGigE.exe

:: cd\develop\projects\HexitecVSR\HexitecGigE
:: C:\Qt\Tools\QtCreator\bin\jom.exe -f Makefile.Release clean
:: C:\Qt\Tools\QtCreator\bin\jom.exe -f Makefile.Release 

:: cd\develop\projects\HexitecVSR\

echo "_______________________________________"
echo "HxtProcessingLib?"
dir HxtProcessingLib\release\HxtProcessingLib.lib"
echo "_______________________________________"
echo "DetectorLib?"
dir DetectorLib\release\DetectorLib.lib"
echo "_______________________________________"
echo "VSRLib?"
dir VSRLib\release\VSRLib.lib"
:: echo "_______________________________________"
:: echo "HexitecGigE?"
:: dir HexitecGigE\release\HexitecGigE.exe
echo "_______________________________________"
echo "HexitecVSR?"
dir HexitecVSR\release\HexitecVSR.exe

