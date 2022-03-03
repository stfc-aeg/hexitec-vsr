cd %1
mc -u -U %2.mc
rc -r %2.rc
rc -r %2_ver.rc
md %3
link /MACHINE:%4 -dll -noentry -out:%3\%2.dll %2_ver.res %2.res