
echo off
echo making
for /r ./ %%f in (*.erl) do erlc +debug_info -I include -o ebin %%f
del /S /Q .\app\*.beam
copy ebin\*.* app\ebin\
pause