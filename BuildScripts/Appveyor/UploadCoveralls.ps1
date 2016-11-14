if ($env:CONFIGURATION -eq "Debug"){
	$coverageFilePath = Resolve-Path -path "TestResults\*\*.coverage"
 
	$coverageFilePath = $coverageFilePath.ToString()

	Write-Host "Running CodeCoverage.exe..."
	&"C:\Program Files (x86)\Microsoft Visual Studio 14.0\Team Tools\Dynamic Code Coverage Tools\CodeCoverage.exe" analyze /output:coverage.coveragexml "$coverageFilePath"

	Write-Host "Downloading PathCapitalizationCorrector v0.1.4..."
	appveyor DownloadFile https://github.com/Cyberboss/PathCapitalizationCorrector/releases/download/0.1.4/PathCapitalizationCorrector.exe

	Write-Host "Fixing Window's terrible case ignorance..."
	&"./PathCapitalizationCorrector.exe" coverage.coveragexml

	$coveralls = "coveralls.net/tools/csmacnz.coveralls.exe"
	if (-Not $env:APPVEYOR_PULL_REQUEST_NUMBER) {
		& $coveralls --serviceName appveyor-ci --dynamiccodecoverage -i coverage.coveragexml --useRelativePaths  -commitId $env:APPVEYOR_REPO_COMMIT --commitBranch $env:APPVEYOR_REPO_BRANCH --commitAuthor $env:APPVEYOR_REPO_COMMIT_AUTHOR --commitEmail $env:APPVEYOR_REPO_COMMIT_AUTHOR_EMAIL --commitMessage $env:APPVEYOR_REPO_COMMIT_MESSAGE --jobId $env:APPVEYOR_JOB_ID
	}
	else{
		& $coveralls --serviceName appveyor-ci --pullRequest $env:APPVEYOR_PULL_REQUEST_NUMBER --dynamiccodecoverage -i coverage.coveragexml --useRelativePaths --commitId $env:APPVEYOR_REPO_COMMIT --commitBranch $env:APPVEYOR_REPO_BRANCH --commitAuthor $env:APPVEYOR_REPO_COMMIT_AUTHOR --commitEmail $env:APPVEYOR_REPO_COMMIT_AUTHOR_EMAIL --commitMessage $env:APPVEYOR_REPO_COMMIT_MESSAGE --jobId $env:APPVEYOR_JOB_ID
	}
}