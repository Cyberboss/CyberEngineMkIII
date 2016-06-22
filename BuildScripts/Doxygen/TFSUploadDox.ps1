$branch = $args[0]
$TARGETDIR = "S:/Website/dox/$branch/CyberEngineMkIII"
if(!(Test-Path -Path $TARGETDIR )){
    New-Item -ItemType directory -Path $TARGETDIR
}else{
rm -r "$TARGETDIR/*"
}
cp -r Documentation/CyberEngineMkIII/* $TARGETDIR
Add-Type -A System.IO.Compression.FileSystem
[IO.Compression.ZipFile]::CreateFromDirectory('Documentation/CyberEngineMkIII/API', 'CyberEngineMkIIIAPIDox.zip')
[IO.Compression.ZipFile]::CreateFromDirectory('Documentation/CyberEngineMkIII/WindowsDebugEngine', 'CyberEngineMkIIIWindowsDebugDox.zip')
[IO.Compression.ZipFile]::CreateFromDirectory('Documentation/CyberEngineMkIII/WindowsEngine', 'CyberEngineMkIIIWindowsDox.zip')
[IO.Compression.ZipFile]::CreateFromDirectory('Documentation/CyberEngineMkIII/OSXDebugEngine', 'CyberEngineMkIIIOSXDebugDox.zip')
[IO.Compression.ZipFile]::CreateFromDirectory('Documentation/CyberEngineMkIII/OSXEngine', 'CyberEngineMkIIIOSXDox.zip')
[IO.Compression.ZipFile]::CreateFromDirectory('Documentation/CyberEngineMkIII/LinuxDebugEngine', 'CyberEngineMkIIILinuxDebugDox.zip')
[IO.Compression.ZipFile]::CreateFromDirectory('Documentation/CyberEngineMkIII/LinuxEngine', 'CyberEngineMkIIILinuxDox.zip')