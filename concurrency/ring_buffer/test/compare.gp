# compare.gp

set terminal pngcairo size 800,600      # 設定輸出為 PNG 圖片
set output "ringbuffer.png"             # 設定輸出的檔案名稱

set title "ring buffer speed"           # 圖表標題
set xlabel "Number of messages (x128bytes)"  # X 軸標籤
set ylabel "time(us)"                 # Y 軸標籤

set grid                               # 顯示網格
set key left top                       # 設定圖例的位置

# 設定線條樣式
set style line 1 lc rgb "green" lw 2 pt 7 ps 1.5
set style line 2 lc rgb "purple" lw 2 pt 6 ps 1.5

# 使用兩個檔案的數據繪製折線圖
plot "output_dyn" using 1:2 with linespoints linestyle 1 title "malloc", \
     "output_mmap" using 1:2 with linespoints linestyle 2 title "mmap"

