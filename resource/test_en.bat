@echo off

REM Test for -c option
echo Test for -c option:
echo Test 1, expect output 20:
sudoku.exe -c 20
echo Test 2, input incorrect parameter 1000001, expect output:
sudoku.exe -c 1000001

REM Test for -s option
echo Test for -s option:
echo Test 1, expect output the number of successfully parsed sudoku puzzles from file game.txt:
sudoku.exe -s game.txt
echo Test 2, input nonexistent file name game2.txt, expect error message:
sudoku.exe -s game2.txt

REM Test for -n option
echo Test for -n option:
echo Test 1, expect output 1000, generating difficult sudoku puzzles:
sudoku.exe -n 1000 -m 3 -r 40~50
echo Test 2, input incorrect parameter 10001, expect output:
sudoku.exe -n 10001

REM Test for -m and -r options
echo Test for -m and -r options:
echo Test 1, expect output 1000, generating easy sudoku puzzles:
sudoku.exe -n 1000 -m 1
echo Test 2, input incorrect parameter 4, expect output:
sudoku.exe -n 1000 -m 4
echo Test 3, input incorrect parameter 60~70, expect output:
sudoku.exe -n 1000 -m 2 -r 60~70

REM Test for -u option
echo Test for -u option:
echo Test 1, expect output 1000, generating unique solution sudoku puzzles:
sudoku.exe -n 1000 -u

pause
