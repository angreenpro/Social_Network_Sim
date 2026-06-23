@echo off
echo Đang biên dịch dự án Social Network Sim bằng g++...

echo.
echo [1/4] Biên dịch chương trình chính (social_net.exe)...
g++ -std=c++17 -I include src/User.cpp src/Graph.cpp src/CSVHandler.cpp src/Algorithms.cpp src/Menu.cpp src/Visualizer.cpp src/main.cpp -o social_net.exe

echo [2/4] Biên dịch test_graph.exe...
g++ -std=c++17 -I include src/User.cpp src/Graph.cpp src/CSVHandler.cpp src/Algorithms.cpp src/Menu.cpp src/Visualizer.cpp tests/test_graph.cpp -o test_graph.exe

echo [3/4] Biên dịch test_algorithms.exe...
g++ -std=c++17 -I include src/User.cpp src/Graph.cpp src/CSVHandler.cpp src/Algorithms.cpp src/Menu.cpp src/Visualizer.cpp tests/test_algorithms.cpp -o test_algorithms.exe

echo [4/4] Biên dịch test_performance.exe...
g++ -std=c++17 -I include src/User.cpp src/Graph.cpp src/CSVHandler.cpp src/Algorithms.cpp src/Menu.cpp src/Visualizer.cpp tests/test_performance.cpp -o test_performance.exe

echo.
echo ========================================================
echo Biên dịch hoàn tất!
echo Ban co the chay cac file .exe truc tiep:
echo   - .\social_net.exe
echo   - .\test_graph.exe
echo   - .\test_algorithms.exe
echo   - .\test_performance.exe
echo ========================================================
pause
