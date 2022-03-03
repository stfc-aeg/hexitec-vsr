# How to generate the vsr.dll needed by HexitecVSR.exe

Navigate to: HexitecVSR\VSR_DLL_1.0.0.1\VSR
Open: VSR.sln
# Once loaded, ensure correct Solution Configuration and Solution Platform selected
#	to generate debug DLL: Debug, x64.
#	To generate release DLL: Release, x64.
Build Solution (F7)

# The DLL will be found in: 

Release -> HexitecVSR\VSR_DLL_1.0.0.1\VSR\x64\Release\VSR.dll
Debug -> HexitecVSR\VSR_DLL_1.0.0.1\VSR\x64\Debug\VSR.dll

Copy the relevant DLL to the folder of the relevant HexitecVSR.exe executable

HexitecVSR\HexitecVSR\release
or
HexitecVSR\HexitecVSR\debug

# Instructions prepared by Christian Angelsen on 2022/03/02
# DLL generation tested using:

Microsoft Visual Studio Professional 2017
Windows SDK Version 10.0.17763.0

# NOTE: There is also a VSR.DLL labelled version 1.0.0.2 available elsewhere. It is not known what the differences come down to though.