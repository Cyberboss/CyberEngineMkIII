if ($env:CONFIGURATION -eq "Debug"){
	$coverageFilePath = Resolve-Path -path "TestResults\*\*.coverage"
 
	$coverageFilePath = $coverageFilePath.ToString()
 
	if(Test-Path .\coverage.coveragexml){ rm .\coverage.coveragexml }
 
	& "C:\Program Files (x86)\Microsoft Visual Studio 14.0\Team Tools\Dynamic Code Coverage Tools\CodeCoverage.exe" analyze /output:coverage.coveragexml "$coverageFilePath"
 
	$coveralls = "coveralls.net/tools/csmacnz.coveralls.exe"
	& $coveralls --repoToken $env:COVERALLS_REPO_TOKEN --dynamiccodecoverage -i coverage.coveragexml --useRelativePaths
}