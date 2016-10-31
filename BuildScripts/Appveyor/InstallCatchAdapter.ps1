Write-Host "Downloading VSCatchAdapter v0.1.0..."
appveyor DownloadFile  https://github.com/CyberbossJHCB/VSCatchAdapter/releases/download/0.1.0/Release.zip
Write-Host "Unzipping..."
&"7z.exe" e Release.zip

&"C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE\CommonExtensions\Microsoft\TestWindow\vstest.console.exe" /logger:Appveyor /Platform:x64 /TestAdapterPath:"./" /inIsolation /Enablecodecoverage Output/bin/CyberEngineMkIIITester.exe