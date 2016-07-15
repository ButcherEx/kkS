@echo off
LLL:
echo "commit local....."
git commit -a -m "auto commit"


echo "push to remote....."   
git push --progress "origin" master:master

echo all done!
goto LLL
pause 

