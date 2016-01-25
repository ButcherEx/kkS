@echo off

del /S /Q .\app\*.beam
rebar compile
copy ebin\*.* app\ebin\
pause