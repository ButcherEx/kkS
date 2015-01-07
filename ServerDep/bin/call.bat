
@echo off
echo "Start release auto test!"
call auto.bat

ping -n 10 127.0.0.1 > nul
echo "Start debug auto test!"
call auto_d.bat
