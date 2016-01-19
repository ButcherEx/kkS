
echo off
echo making
for /r src %%f in (*.erl) do erlc +debug_info -I include -o ebin %%f

pause