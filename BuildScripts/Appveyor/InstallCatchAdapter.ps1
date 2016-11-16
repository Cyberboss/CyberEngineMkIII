Write-Host "Downloading VSCatchAdapter v0.1.0..."
appveyor DownloadFile https://github.com/CyberbossJHCB/VSCatchAdapter/releases/download/0.1.0/VSCatchAdapter.vsix
Write-Host "Installing VSIX..."
&"C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE\VSIXInstaller.exe" /q /a VSCatchAdapter.vsix