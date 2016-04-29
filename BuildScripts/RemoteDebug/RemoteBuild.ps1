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
	$command = "cd $wd; cd ../..;cmake . -DCMAKE_BUILD_TYPE=DEBUG;make;make test ARGS=`"-V`""
	& "BuildScripts/RemoteDebug/plink.exe" "$userhost" -i "$key" "$command"
}

