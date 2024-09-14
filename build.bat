REM g++ -Wall -std=c++14 -I.\include .\src\*.cpp -o .\build\ms_windows\Joins.exe -Llib -lsqlite3.dll
g++ -o .\build\ms_windows\Joins.exe .\src\*.cpp -Llib -lsqlite3 -I.\include
