@echo off

pushd "%~dp0"

echo Copie des fichiers...
md "%ProgramFiles%\APPC"
copy "Susi4.dll" "C:\Program Files\APPC\"
copy "APPC.exe" "C:\Program Files\APPC\"
copy "icon.ico" "C:\Program Files\APPC\"
copy "VC_redist.x64.exe" "C:\Program Files\APPC\"
echo Copie de la police...
copy "SEVEN SEGMENT.TTF" "C:\Windows\Fonts\"
regedit /s Data\font.reg
FontReg.exe
echo Installation des fonctions necessaires...
cd "%ProgramFiles%\APPC\"
VC_redist.x64.exe
cd "%~dp0"
if %ERRORLEVEL% NEQ 0 ( 
   echo PROBLEME D'INSTALLATION - LE PROGRAMME NECESSITE VC REDIST POUR FONCTIONNER
   )
echo. 
del "C:\Program Files\APPC\VC_redist.x64.exe"
echo Creation d'un raccourci sur le bureau...
set SCRIPT="%TEMP%\%RANDOM%-%RANDOM%-%RANDOM%-%RANDOM%.vbs"
echo Set oWS = WScript.CreateObject("WScript.Shell") >> %SCRIPT%
echo sLinkFile = "%USERPROFILE%\Desktop\APPC.lnk" >> %SCRIPT%
echo Set oLink = oWS.CreateShortcut(sLinkFile) >> %SCRIPT%
echo oLink.IconLocation = "C:\Program Files\APPC\icon.ico" >> %SCRIPT%
echo oLink.TargetPath = "C:\Program Files\APPC\APPC.exe" >> %SCRIPT%
echo oLink.WorkingDirectory = "C:\Program Files\APPC\" >> %SCRIPT%
echo oLink.Save >> %SCRIPT%
cscript /nologo %SCRIPT%
del %SCRIPT%

echo.
echo installation terminee !
echo.
echo ATTENTION : Le programme necessite un redemarrage du PC pour fonctionner. C'est a vous de le redemarrer

pause