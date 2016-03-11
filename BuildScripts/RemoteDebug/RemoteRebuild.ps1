# $(RemoteUserName) $(RemoteHostName) $(PrivateKey) $(RemoteWorkingDirectory)
 param (
    [string]$username = $(throw "-username is required."),
    [string]$server = $(throw "-server is required."),
    [string]$key = $(throw "-key is required."),
    [string]$wd = $(throw "-wd is required.")
 )
& "../../BuildScripts/RemoteDebug/RemoteClean.ps1" -server $server -username $username -key $key -wd $wd
& "../../BuildScripts/RemoteDebug/RemoteBuild.ps1" -server $server -username $username -key $key -wd $wd