rmdir /s /q "ipch"
rmdir /s /q "Debug"
rmdir /s /q "Release"

rmdir /s /q .\Client\Include\Debug
rmdir /s /q .\Client\Include\ipch
rmdir /s /q .\Client\Include\Release
rmdir /s /q .\Client\Include\x64

rmdir /s /q .\Engine\Resources\Include\Debug
rmdir /s /q .\Engine\System\Include\Debug
rmdir /s /q .\Engine\Utility\Include\Debug

del /f /q .\Client\Bin\Debug\Client.exe
del /f /q .\Client\Bin\Debug\Client.ilk
del /f /q .\Client\Bin\Debug\Client.pdb
del /f /q .\Client\Bin\Release\Client.exe
del /f /q .\Client\Bin\Release\Client.ilk
del /f /q .\Client\Bin\Release\Client.pdb
del /f /q Frame97_1.VC.db
