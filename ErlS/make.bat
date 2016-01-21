@echo off

del /S /Q .\app\*.beam

rebar compile

xcopy /T /E .\svr\ebin\*.* .\app\ebin\
xcopy .\\deps\\log4erl\\ebin\*.beam .\\app\\ebin\\