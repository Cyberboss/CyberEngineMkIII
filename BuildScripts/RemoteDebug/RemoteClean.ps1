# $(RemoteUserName) $(RemoteHostName) $(PrivateKey) $(RemoteWorkingDirectory)
 param (
    [string]$username = $(throw "-username is required."),
    [string]$server = $(throw "-server is required."),
    [string]$key = $(throw "-key is required."),
    [string]$wd = $(throw "-wd is required.")
 )
$userhost = "$username@$server"
$paththing = "$wd/../../"
$command = "mkdir -p $wd; cd $paththing;rm -rf *; echo Working Directory cleared"
& "../../BuildScripts/RemoteDebug/plink.exe" $userhost -i "$key" "$command"
& "../ModifiedSources/bin/Remote/ModifiedSources.exe" --clear