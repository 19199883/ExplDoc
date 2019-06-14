@echo on
@echo 复制windows交易API文件
copy ..\..\lib\debug\EESTraderApi.dll "trade\C++\win32_debug\" /Y
copy ..\..\lib\debug\EESTraderApi.lib "trade\C++\win32_debug\" /Y

copy ..\..\lib\release\EESTraderApi.dll "trade\C++\win32_release\" /Y
copy ..\..\lib\release\EESTraderApi.lib "trade\C++\win32_release\" /Y
@echo 复制windows行情API文件
copy ..\..\lib\debug\EESQuoteApi.dll "quote\C++\win32_debug\" /Y
copy ..\..\lib\debug\EESQuoteApi.lib "quote\C++\win32_debug\" /Y

copy ..\..\lib\release\EESQuoteApi.dll "quote\C++\win32_release\" /Y
copy ..\..\lib\release\EESQuoteApi.lib "quote\C++\win32_release\" /Y

@echo 复制windows CTP API文件


rem @echo 复制linux交易API文件
rem copy ..\..\lib\debug\libEESTraderApi.so "trade\C++\linux64_debug\" /Y
rem @echo 复制linux行情API文件
rem copy ..\..\lib\debug\libEESQuoteApi.so "quote\C++\linux64_debug\" /Y

pause