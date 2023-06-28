@echo off

REM 测试-c参数  
echo 测试-c参数:  
echo 测试1，期望输出20:  
sudoku.exe -c 20  
echo 测试2，输入错误参数1000001，期望输出:  
sudoku.exe -c 1000001  

REM 测试-s参数  
echo 测试-s参数:  
echo 测试1，要求从文件game.txt中读取并解析数独的答案，期望输出成功解析的数量:  
sudoku.exe -s game.txt  
echo 测试2，输入不存在的文件名game2.txt，期望输出错误信息:  
sudoku.exe -s game2.txt  

REM 测试-n参数  
echo 测试-n参数:  
echo 测试1，期望输出1000，生成困难的数独:  
sudoku.exe -n 1000 -m 3 -r 40~50  
echo 测试2，输入错误参数10001，期望输出:  
sudoku.exe -n 10001  

REM 测试-m和-r参数  
echo 测试-m、-r参数:  
echo 测试1，期望输出1000，生成简单的数独:  
sudoku.exe -n 1000 -m 1  
echo 测试2，输入错误参数4，期望输出:  
sudoku.exe -n 1000 -m 4  
echo 测试3，输入错误参数60~70，期望输出:  
sudoku.exe -n 1000 -m 2 -r 60~70  

REM 测试-u参数  
echo 测试-u参数:  
echo 测试1，期望输出1000，生成解唯一的数独:  
sudoku.exe -n 1000 -u  

pause  
