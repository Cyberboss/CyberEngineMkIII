Write-Host "Downloading VSCatchAdapter v0.1.0..."
appveyor DownloadFile  https://github.com/CyberbossJHCB/VSCatchAdapter/releases/download/0.1.0/Release.zip
Write-Host "Unzipping..."
&"7z.exe" e Release.zip
ls