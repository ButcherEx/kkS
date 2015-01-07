@echo off
mkdir dbglog
set tm1=%time:~0,2%
set tm2=%time:~3,2%
set tm3=%time:~6,2%
set tm4=%time:~0,8%
set da1=%date:~0,4%
set da2=%date:~5,2%
set da3=%date:~8,2%
set log=log\pingpong.%da1%-%da2%-%da3%-%tm1%.log

ping -n 5 127.0.0.1 > nul
echo "============DEBUG============" >> %log%
echo "Start 1s:1c" >> %log%
echo "Start 1s:1c"
start "Svr1"  /NORMAL /MIN bst_d.exe --isserver 1
start "Cli1"  /NORMAL /MIN  bst_d.exe

ping -n 102 127.0.0.1 > nul
echo "Start 1s:2c" >> %log%
echo "Start 1s:2c"
start "Svr1"  /NORMAL /MIN bst_d.exe --isserver 1
start "Cli1"  /NORMAL /MIN  bst_d.exe
start "Cli2"  /NORMAL /MIN  bst_d.exe

ping -n 102 127.0.0.1 > nul
echo "Start 1s:4c" >> %log%
echo "Start 1s:4c"
start "Svr1"  /NORMAL /MIN bst_d.exe --isserver 1
start "Cli1"  /NORMAL /MIN  bst_d.exe
start "Cli2"  /NORMAL /MIN  bst_d.exe
start "Cli3"  /NORMAL /MIN  bst_d.exe
start "Cli4"  /NORMAL /MIN  bst_d.exe

ping -n 102 127.0.0.1 > nul
echo "Start 1s:6c" >> %log%
echo "Start 1s:6c"
start "Svr1"  /NORMAL /MIN bst_d.exe --isserver 1
start "Cli1"  /NORMAL /MIN  bst_d.exe
start "Cli2"  /NORMAL /MIN  bst_d.exe
start "Cli3"  /NORMAL /MIN  bst_d.exe
start "Cli4"  /NORMAL /MIN  bst_d.exe
start "Cli5"  /NORMAL /MIN  bst_d.exe
start "Cli6"  /NORMAL /MIN  bst_d.exe

ping -n 102 127.0.0.1 > nul
echo "Start 1s:8c" >> %log%
echo "Start 1s:8c"
start "Svr1"  /NORMAL /MIN bst_d.exe --isserver 1
start "Cli1"  /NORMAL /MIN  bst_d.exe
start "Cli2"  /NORMAL /MIN  bst_d.exe
start "Cli3"  /NORMAL /MIN  bst_d.exe
start "Cli4"  /NORMAL /MIN  bst_d.exe
start "Cli5"  /NORMAL /MIN  bst_d.exe
start "Cli6"  /NORMAL /MIN  bst_d.exe
start "Cli7"  /NORMAL /MIN  bst_d.exe
start "Cli8"  /NORMAL /MIN  bst_d.exe

ping -n 102 127.0.0.1 > nul
echo "auto-debug-end" >> %log%

echo "....End...."