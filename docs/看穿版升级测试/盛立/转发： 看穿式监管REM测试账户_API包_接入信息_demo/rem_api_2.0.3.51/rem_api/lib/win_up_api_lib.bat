@echo on
@echo ����windows����API�ļ�
copy ..\..\lib\debug\EESTraderApi.dll "trade\C++\win32_debug\" /Y
copy ..\..\lib\debug\EESTraderApi.lib "trade\C++\win32_debug\" /Y

copy ..\..\lib\release\EESTraderApi.dll "trade\C++\win32_release\" /Y
copy ..\..\lib\release\EESTraderApi.lib "trade\C++\win32_release\" /Y
@echo ����windows����API�ļ�
copy ..\..\lib\debug\EESQuoteApi.dll "quote\C++\win32_debug\" /Y
copy ..\..\lib\debug\EESQuoteApi.lib "quote\C++\win32_debug\" /Y

copy ..\..\lib\release\EESQuoteApi.dll "quote\C++\win32_release\" /Y
copy ..\..\lib\release\EESQuoteApi.lib "quote\C++\win32_release\" /Y

@echo ����windows CTP API�ļ�


rem @echo ����linux����API�ļ�
rem copy ..\..\lib\debug\libEESTraderApi.so "trade\C++\linux64_debug\" /Y
rem @echo ����linux����API�ļ�
rem copy ..\..\lib\debug\libEESQuoteApi.so "quote\C++\linux64_debug\" /Y

pause