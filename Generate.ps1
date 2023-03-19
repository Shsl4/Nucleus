$CMakeExists = Get-Command cmake.exe -errorAction SilentlyContinue
$CMakeDirName = ".cmake"

if(!$CMakeExists) {

    Write-Error -Message "Could not find cmake.exe. Make sure it is installed."
    Exit

}

New-Item -Path $CMakeDirName -ItemType "directory" -Force | Out-Null
$CMakeDir = Get-Item $CMakeDirName -Force
$CMakeDir.attributes = 'Hidden'

Set-Location -Path $CMakeDirName

cmake .. -G "Visual Studio 17 2022"

Set-location ..

$Location = Get-Location
$WScriptObj = New-Object -ComObject ("WScript.Shell")
$Shortcut = $WscriptObj.CreateShortcut("$Location\Nucleus.lnk")
$Shortcut.TargetPath = "$Location\$CMakeDirName\Nucleus.sln"
$Shortcut.Save()