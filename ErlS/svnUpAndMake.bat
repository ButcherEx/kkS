
echo off
echo making
for /r src %%f in (*.erl) do erlc -I include -o ebin %%f

pause