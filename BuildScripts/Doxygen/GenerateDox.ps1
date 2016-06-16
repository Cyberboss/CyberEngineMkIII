param (
	[bool]$all = $true,
	[switch]$debug,
	[switch]$api,
    [switch]$windows,
    [switch]$osx,
    [switch]$linux
 )
$scriptPath = "C:/Program Files/doxygen/bin/doxygen.exe"
cd ../..
$TARGETDIR = 'Documentation/CyberEngineMkIII'
if(!(Test-Path -Path $TARGETDIR )){
    New-Item -ItemType directory -Path $TARGETDIR
}
if($all -or $api){
	&$scriptPath "BuildScripts/Doxygen/CyberEngineMkIIIAPIDoxy.cfg"
}
if($all -or ($debug -and $windows)){
	$(cat "BuildScripts/Doxygen/CyberEngineMkIIIDoxy.cfg" ; echo "EXPAND_AS_DEFINED = DEBUG TARGET_OS_WINDOWS" ; echo "OUTPUT_DIRECTORY = Documentation/CyberEngineMkIII/WindowsDebugEngine"; echo "EXCLUDE_PATTERNS = CYBPosix* CYBLinux* CYBOSX*" ) | &$scriptPath -
}
if($all -or ($debug -and $osx)){
	$(cat "BuildScripts/Doxygen/CyberEngineMkIIIDoxy.cfg" ; echo "EXPAND_AS_DEFINED = DEBUG TARGET_OS_MAC" ; echo "OUTPUT_DIRECTORY = Documentation/CyberEngineMkIII/OSXDebugEngine"; echo "EXCLUDE_PATTERNS = CYBWin32* CYBLinux*" ) | &$scriptPath -
}
if($all -or ($debug -and $linux)){
	$(cat "BuildScripts/Doxygen/CyberEngineMkIIIDoxy.cfg" ; echo "EXPAND_AS_DEFINED = DEBUG TARGET_OS_LINUX" ; echo "OUTPUT_DIRECTORY = Documentation/CyberEngineMkIII/LinuxDebugEngine"; echo "EXCLUDE_PATTERNS = CYBWin32* CYBOSX*" ) | &$scriptPath -
}
if($all -or (-Not($debug) -and $windows)){
	$(cat "BuildScripts/Doxygen/CyberEngineMkIIIDoxy.cfg" ; echo "EXPAND_AS_DEFINED = TARGET_OS_WINDOWS" ; echo "OUTPUT_DIRECTORY = Documentation/CyberEngineMkIII/WindowsEngine"; echo "EXCLUDE_PATTERNS = CYBPosix* CYBLinux* CYBOSX*" ) | &$scriptPath -
}
if($all -or (-Not($debug) -and $osx)){
	$(cat "BuildScripts/Doxygen/CyberEngineMkIIIDoxy.cfg" ; echo "EXPAND_AS_DEFINED = TARGET_OS_MAC" ; echo "OUTPUT_DIRECTORY = Documentation/CyberEngineMkIII/OXSEngine"; echo "EXCLUDE_PATTERNS = CYBWin32* CYBLinux*" ) | &$scriptPath -
}
if($all -or (-Not($debug) -and $linux)){
	$(cat "BuildScripts/Doxygen/CyberEngineMkIIIDoxy.cfg" ; echo "EXPAND_AS_DEFINED = TARGET_OS_LINUX" ; echo "OUTPUT_DIRECTORY = Documentation/CyberEngineMkIII/LinuxEngine"; echo "EXCLUDE_PATTERNS = CYBWin32* CYBOSX*" ) | &$scriptPath -
}
cd BuildScripts/Doxygen