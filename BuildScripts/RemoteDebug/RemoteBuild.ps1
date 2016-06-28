# $(RemoteUserName) $(RemoteHostName) $(PrivateKey) $(RemoteWorkingDirectory)
 param (
    [string]$username = $(throw "-username is required."),
    [string]$server = $(throw "-server is required."),
    [string]$key = $(throw "-key is required."),
    [string]$wd = $(throw "-wd is required.")
 )
cd ../..
& "Utilities/ModifiedSources/bin/Remote/ModifiedSources.exe"
if($LastExitCode -eq 0){
	$userhost = "$username@$server"
	$paththing = $userhost + ":" + "$wd/../../"
	& "BuildScripts/RemoteDebug/plink.exe" "$userhost" -i "$key" "mkdir -p $wd"
	& "BuildScripts/RemoteDebug/pscp.exe" -i "$key" -r GDBSync/* "$paththing"
	rm -r GDBSync
	$command = "cd $wd; cd ../..;cmake . -DCMAKE_BUILD_TYPE=DEBUG -DCOTIRE_MINIMUM_NUMBER_OF_TARGET_SOURCES=1;dos2unix Tests/llvm-gcov.sh;chmod +x Tests/llvm-gcov.sh;dos2unix BuildScripts/GenerateTestXMLs.sh;chmod +x BuildScripts/GenerateTestXMLs.sh;find . -iname `"*.gcda`" -exec rm -rf {} \;rm -rf Testing;make && ctest -T Test --output-on-failure; BuildScripts/GenerateTestXMLs.sh genhtml"
	& "BuildScripts/RemoteDebug/plink.exe" "$userhost" -i "$key" "$command"
}
