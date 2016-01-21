@echo off

del /S /Q .\app\*.beam
copy ebin\*.* app\ebin\
