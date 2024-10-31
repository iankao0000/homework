set terminal pngcairo size 1280,960 enhanced font 'Verdana,12'
set output 'memory_mountain.png'


set title "記憶體山 : Core i7-9750H"
set xlabel "Stride (x8 bytes)"
set ylabel "Size (bytes)"
set zlabel "讀取吞吐量 (MB/s)"
set xyplane at 0

# 設置 z 軸範圍和刻度
set zrange [0:*]
set ztics 10000  # 每隔 10000 的吞吐量顯示一個標線，可根據需要調整

# 打開 z 軸的網格線
set grid ztics lw 2 lc rgb "black"  # 增加線寬 (lw) 為 2，顏色為黑色

set xtics ("s1" 1, "s3" 3, "s5" 5, "s7" 7, "s9" 9, "s11" 11, "s13" 13, "s15" 15)
set ytics ("128m" 1, "64m" 2, "32m" 3, "16m" 4, "8m" 5, "4m" 6, "2m" 7, "1m" 8, "512k" 9, "256k" 10, "128k" 11, "64k" 12, "32k" 13, "16k" 14)

set pm3d at s
set view 60, 45  # 測試一個更基礎的視角

splot "data.dat" matrix using 1:2:3 with pm3d

