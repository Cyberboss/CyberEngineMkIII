$scriptDir = split-path -parent $MyInvocation.MyCommand.Definition
cd ../..
$TARGETDIR = 'Documentation/CyberEngineMkIII'
if(!(Test-Path -Path $TARGETDIR )){
    New-Item -ItemType directory -Path $TARGETDIR
}
& "$scriptDir\doxygen.exe" "BuildScripts/Doxygen/CyberEngineMkIIIAPIDoxy.cfg"
& "$scriptDir\doxygen.exe" "BuildScripts/Doxygen/CyberEngineMkIIIDoxy.cfg"
cd BuildScripts/Doxygen