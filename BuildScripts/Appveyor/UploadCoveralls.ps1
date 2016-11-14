if ($env:CONFIGURATION -eq "Debug"){
	$coverageFilePath = Resolve-Path -path "TestResults\*\*.coverage"
 
	$coverageFilePath = $coverageFilePath.ToString()
 
	if(Test-Path .\coverage.coveragexml){ rm .\coverage.coveragexml }
 
	& "C:\Program Files (x86)\Microsoft Visual Studio 14.0\Team Tools\Dynamic Code Coverage Tools\CodeCoverage.exe" analyze /output:coverage.coveragexml "$coverageFilePath"
 
	$coveralls = "coveralls.net/tools/csmacnz.coveralls.exe"
	if (-Not $env:APPVEYOR_PULL_REQUEST_NUMBER) {
		& $coveralls --serviceName "appveyor-ci" --dynamiccodecoverage -i coverage.coveragexml --useRelativePaths
	}
	else{
		& $coveralls --serviceName "appveyor-ci" --pullRequest $env:APPVEYOR_PULL_REQUEST_NUMBER --dynamiccodecoverage -i coverage.coveragexml --useRelativePaths
	}
}