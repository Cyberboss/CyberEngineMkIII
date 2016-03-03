$scriptDir = split-path -parent $MyInvocation.MyCommand.Definition
cd ../..
$TARGETDIR = 'Documentation\CyberEngineMkIII'
if(!(Test-Path -Path $TARGETDIR )){
    New-Item -ItemType directory -Path $TARGETDIR
}
& "$scriptDir\doxygen.exe" "Documentation/CyberEngineMkIIIAPIDoxy.cfg"
& "$scriptDir\doxygen.exe" "Documentation/CyberEngineMkIIIDoxy.cfg"
cd BuildScripts/Doxygen