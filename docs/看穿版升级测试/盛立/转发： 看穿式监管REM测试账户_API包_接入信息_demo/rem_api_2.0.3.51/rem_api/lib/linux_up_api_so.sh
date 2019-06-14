#!/bin/bash
set -x
echo cp linux trade API files
cp ../../lib/debug/libEESTraderApi.so "trade/C++/linux64_debug/" -f
echo cp linuxquote API files
cp ../../lib/debug/libEESQuoteApi.so "quote/C++/linux64_debug/" -f

cp ../../lib/debug/EESTraderApi.dll "trade/C++/win32_debug/" -f
cp ../../lib/debug/EESTraderApi.lib "trade/C++/win32_debug/" -f

cp ../../lib/Release/EESTraderApi.dll "trade/C++/win32_release/" -f
cp ../../lib/Release/EESTraderApi.lib "trade/C++/win32_release/" -f

cp ../../lib/debug/EESQuoteApi.dll "quote/C++/win32_debug/" -f
cp ../../lib/debug/EESQuoteApi.lib "quote/C++/win32_debug/" -f

cp ../../lib/Release/EESQuoteApi.dll "quote/C++/win32_release/" -f
cp ../../lib/Release/EESQuoteApi.lib "quote/C++/win32_release/" -f
