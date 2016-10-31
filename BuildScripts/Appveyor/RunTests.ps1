# run tests
&"vstest.console" /logger:Appveyor /UseVsixExtensions:true Output/bin/CyberEngineMkIIITester.exe /xml cyb-test-results.xml

# upload results to AppVeyor
$wc = New-Object 'System.Net.WebClient'
$wc.UploadFile("https://ci.appveyor.com/api/testresults/xunit/$($env:APPVEYOR_JOB_ID)", (Resolve-Path cyb-test-results.xml))
