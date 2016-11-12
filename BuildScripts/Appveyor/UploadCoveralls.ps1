$coverageFilePath = Resolve-Path -path "TestResults\*\*.coverage"
$coveralls = Resolve-Path -path "packages\*\tools\csmacnz.coveralls.exe.coverage"
 
$coverageFilePath = $coverageFilePath.ToString()
$coveralls = $coveralls.ToString()
 
if(Test-Path .\coverage.coveragexml){ rm .\coverage.coveragexml }
 
& "C:\Program Files (x86)\Microsoft Visual Studio 14.0\Team Tools\Dynamic Code Coverage Tools\CodeCoverage.exe" analyze /output:coverage.coveragexml "$coverageFilePath"
 
& $coveralls --dynamiccodecoverage -i coverage.coveragexml --useRelativePaths