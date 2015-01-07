@echo off
mkdir log
set tm1=%time:~0,2%
set tm2=%time:~3,2%
set tm3=%time:~6,2%
set tm4=%time:~0,8%
set da1=%date:~0,4%
set da2=%date:~5,2%
set da3=%date:~8,2%

set log=log\pingpong.%da1%-%da2%-%da3%-%tm1%.log

ping -n 1 127.0.0.1 > nul
echo "============RELEASE============" >> %log%
echo "Start 1s:1c" >> %log%
echo "Start 1s:1c"
start "Svr1"  /NORMAL /MIN bst.exe --isserver
start "Cli1"  /NORMAL /MIN  bst.exe

ping -n 52 127.0.0.1 > nul
echo "Start 1s:2c" >> %log%
echo "Start 1s:2c"
start "Svr1"  /NORMAL /MIN bst.exe --isserver
start "Cli1"  /NORMAL /MIN  bst.exe
start "Cli2"  /NORMAL /MIN  bst.exe

ping -n 52 127.0.0.1 > nul
echo "Start 1s:4c" >> %log%
echo "Start 1s:4c"
start "Svr1"  /NORMAL /MIN bst.exe --isserver
start "Cli1"  /NORMAL /MIN  bst.exe
start "Cli2"  /NORMAL /MIN  bst.exe
start "Cli3"  /NORMAL /MIN  bst.exe
start "Cli4"  /NORMAL /MIN  bst.exe

ping -n 52 127.0.0.1 > nul
echo "Start 1s:6c" >> %log%
echo "Start 1s:6c"
start "Svr1"  /NORMAL /MIN bst.exe --isserver
start "Cli1"  /NORMAL /MIN  bst.exe
start "Cli2"  /NORMAL /MIN  bst.exe
start "Cli3"  /NORMAL /MIN  bst.exe
start "Cli4"  /NORMAL /MIN  bst.exe
start "Cli5"  /NORMAL /MIN  bst.exe
start "Cli6"  /NORMAL /MIN  bst.exe

ping -n 52 127.0.0.1 > nul
echo "Start 1s:8c" >> %log%
echo "Start 1s:8c"
start "Svr1"  /NORMAL /MIN bst.exe --isserver
start "Cli1"  /NORMAL /MIN  bst.exe
start "Cli2"  /NORMAL /MIN  bst.exe
start "Cli3"  /NORMAL /MIN  bst.exe
start "Cli4"  /NORMAL /MIN  bst.exe
start "Cli5"  /NORMAL /MIN  bst.exe
start "Cli6"  /NORMAL /MIN  bst.exe
start "Cli7"  /NORMAL /MIN  bst.exe
start "Cli8"  /NORMAL /MIN  bst.exe

ping -n 105 127.0.0.1 > nul
echo "auto-realease-end" >> %log%

echo "....End...."