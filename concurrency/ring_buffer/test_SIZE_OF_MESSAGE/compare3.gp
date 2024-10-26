# compare3.gp

set terminal pngcairo size 800,600      # 設定輸出為 PNG 圖片
set output "ringbuffer3.png"             # 設定輸出的檔案名稱

set title "ring buffer speed"           # 圖表標題
set xlabel "Number of messages (x128bytes)"  # X 軸標籤
set ylabel "time(us)"                 # Y 軸標籤

set grid                               # 顯示網格
set key left top                       # 設定圖例的位置

# 設定線條樣式
set style line 1 lc rgb "green" lw 2 pt 3 ps 1.0
set style line 2 lc rgb "purple" lw 2 pt 4 ps 1.0
set style line 3 lc rgb "orange" lw 2 pt 3 ps 1.0
set style line 3 lc rgb "blue" lw 2 pt 5 ps 1.0

# 使用兩個檔案的數據繪製折線圖
plot \
     "output_size_1" using 1:2 with linespoints linestyle 1 title "mmap with msg size 1", \
     "output_size_10" using 1:2 with linespoints linestyle 2 title "mmap with msg size 10", \
     "output_size_100" using 1:2 with linespoints linestyle 3 title "mmap with msg size 100", \
     "output_size_500" using 1:2 with linespoints linestyle 4 title "mmap with msg size 500"
