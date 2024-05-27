@echo on
taskkill /IM FileTransport_Client.exe /F
taskkill /IM TSpeedTest_Host.exe /F
taskkill /IM TSpeedTest_Client.exe /F
pause