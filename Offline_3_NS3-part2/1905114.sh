#!/bin/bash
cd ..

#if [ 1 == 2 ]; then
#fi

for bottleNechDataRate in {1..300}; do
  ./ns3 run "scratch/1905114.cc --bottleNechDataRate=$bottleNechDataRate --type=1"
done

for errorRate in 100 1000 10000 100000 1000000; do
  ./ns3 run "scratch/1905114.cc --errorRate=$errorRate --type=2 --bottleNechDataRate=50"
done

algo0="algorithm 1"
algo1="algorithm 2"

gnuplot -persist <<-EOFMarker
    set terminal png size 640,480
    set output "scratch/throughput_vs_bottleneck.png"
    plot "scratch/throughput_vs_bottleneck_algo1.dat" using 1:2 ls 1 title '$algo0' with linespoints ,\
         "scratch/throughput_vs_bottleneck_algo2.dat" using 1:2 ls 2 title '$algo1' with linespoints 
EOFMarker

gnuplot -persist <<-EOFMarker
    set terminal png size 640,480
    set output "scratch/throughput_vs_packetloss.png"
    plot "scratch/throughput_vs_pcktloss_algo1.dat" using 1:2 ls 1 title '$algo0' with linespoints ,\
         "scratch/throughput_vs_bpcktloss_algo2.dat" using 1:2 ls 2 title '$algo1' with linespoints 
EOFMarker

gnuplot -persist <<-EOFMarker
    set terminal png size 640,480
    set output "scratch/CongestionWindow_vs_Time.png"
    plot "scratch/CongestionWindow_vs_Time_algo1.dat" using 1:2 ls 1 title '$algo0' with linespoints ,\
         "scratch/CongestionWindow_vs_Time_algo2.dat" using 1:2 ls 2 title '$algo1' with linespoints 
EOFMarker
