#!/bin/bash
echo "hello world"

cd ..
i=1
echo "$i"
for nWifi in 20 40 60 80 100; do
  ./ns3 run "scratch/1905114_static.cc --nWifi=$nWifi --number=$i --plot=1"
done
((i++))
echo "flow"
for flow in 10 20 30 40 50; do
  ./ns3 run scratch/1905114_static.cc "--flow=$flow --number=$i --plot=2" 
done
((i++))
echo "pps"
for packet_rate in 100 200 300 400 500; do
  ./ns3 run "scratch/1905114_static.cc --packet_rate=$packet_rate --number=$i --plot=3" 
done
((i++))
for range in 1 2 4 5 ; do
  ./ns3 run "scratch/1905114_static.cc --range=$range -number=$i --plot=4" 
done
((i++))

i=0
echo "$i"
for nWifi in 20 40 60 80 100; do
    ((i++))
  ./ns3 run "scratch/1905114_mobile.cc --nWifi=$nWifi --number=$i --plot=1"
done
echo "flow"
for flow in 10 20 30 40 50; do
    ((i++))
  ./ns3 run "scratch/1905114_mobile.cc --flow=$flow --number=$i --plot=2" 
done
echo "pps"
for packet_rate in 100 200 300 400 500; do
    ((i++))
  ./ns3 run "scratch/1905114_mobile.cc --packet_rate=$packet_rate --number=$i --plot=3" 
done
for range in 1 2 4 5 ; do
    ((i++))
  ./ns3 run "scratch/1905114_mobile.cc --range=$range --number=$i --plot=4" 
done

#static

gnuplot -e "set terminal pngcairo enhanced font 'arial,10' fontscale 1.0 size 800,600;
            set output 'scratch/output.png';
            set title 'node vs throughput graph';
            set xlabel 'ns';
            set ylabel 'delivery ratio';
            plot '1905114-offline-2-Mobile/delivery_ratio.dat' using 1:2 with lines title 'line';"

gnuplot -e "set terminal pngcairo enhanced font 'arial,10' fontscale 1.0 size 800,600;
            set output 'scratch/output.png';
            set title 'node vs throughput graph';
            set xlabel 'ns';
            set ylabel 'delivery ratio';
            plot '1905114-offline-2-Mobile/delivery_ratio.dat' using 1:2 with lines title 'line';"

gnuplot -e "set terminal pngcairo enhanced font 'arial,10' fontscale 1.0 size 800,600;
            set output 'scratch/output.png';
            set title 'node vs throughput graph';
            set xlabel 'ns';
            set ylabel 'delivery ratio';
            plot '1905114-offline-2-Mobile/delivery_ratio.dat' using 1:2 with lines title 'line';"

gnuplot -e "set terminal pngcairo enhanced font 'arial,10' fontscale 1.0 size 800,600;
            set output 'scratch/output.png';
            set title 'node vs throughput graph';
            set xlabel 'ns';
            set ylabel 'delivery ratio';
            plot '1905114-offline-2-Mobile/delivery_ratio.dat' using 1:2 with lines title 'line';"

gnuplot -e "set terminal pngcairo enhanced font 'arial,10' fontscale 1.0 size 800,600;
            set output 'scratch/output.png';
            set title 'node vs throughput graph';
            set xlabel 'ns';
            set ylabel 'delivery ratio';
            plot '1905114-offline-2-Mobile/delivery_ratio.dat' using 1:2 with lines title 'line';"

gnuplot -e "set terminal pngcairo enhanced font 'arial,10' fontscale 1.0 size 800,600;
            set output 'scratch/output.png';
            set title 'node vs throughput graph';
            set xlabel 'ns';
            set ylabel 'delivery ratio';
            plot '1905114-offline-2-Mobile/delivery_ratio.dat' using 1:2 with lines title 'line';"

gnuplot -e "set terminal pngcairo enhanced font 'arial,10' fontscale 1.0 size 800,600;
            set output 'scratch/output.png';
            set title 'node vs throughput graph';
            set xlabel 'ns';
            set ylabel 'delivery ratio';
            plot '1905114-offline-2-Mobile/delivery_ratio.dat' using 1:2 with lines title 'line';"

gnuplot -e "set terminal pngcairo enhanced font 'arial,10' fontscale 1.0 size 800,600;
            set output 'scratch/output.png';
            set title 'node vs throughput graph';
            set xlabel 'ns';
            set ylabel 'delivery ratio';
            plot '1905114-offline-2-Mobile/delivery_ratio.dat' using 1:2 with lines title 'line';"



#mobile

gnuplot -e "set terminal pngcairo enhanced font 'arial,10' fontscale 1.0 size 800,600;
            set output 'scratch/output.png';
            set title 'node vs throughput graph';
            set xlabel 'ns';
            set ylabel 'delivery ratio';
            plot '1905114-offline-2-Mobile/delivery_ratio.dat' using 1:2 with lines title 'line';"

gnuplot -e "set terminal pngcairo enhanced font 'arial,10' fontscale 1.0 size 800,600;
            set output 'scratch/output.png';
            set title 'node vs throughput graph';
            set xlabel 'ns';
            set ylabel 'delivery ratio';
            plot '1905114-offline-2-Mobile/delivery_ratio.dat' using 1:2 with lines title 'line';"

gnuplot -e "set terminal pngcairo enhanced font 'arial,10' fontscale 1.0 size 800,600;
            set output 'scratch/output.png';
            set title 'node vs throughput graph';
            set xlabel 'ns';
            set ylabel 'delivery ratio';
            plot '1905114-offline-2-Mobile/delivery_ratio.dat' using 1:2 with lines title 'line';"

gnuplot -e "set terminal pngcairo enhanced font 'arial,10' fontscale 1.0 size 800,600;
            set output 'scratch/output.png';
            set title 'node vs throughput graph';
            set xlabel 'ns';
            set ylabel 'delivery ratio';
            plot '1905114-offline-2-Mobile/delivery_ratio.dat' using 1:2 with lines title 'line';"

gnuplot -e "set terminal pngcairo enhanced font 'arial,10' fontscale 1.0 size 800,600;
            set output 'scratch/output.png';
            set title 'node vs throughput graph';
            set xlabel 'ns';
            set ylabel 'delivery ratio';
            plot '1905114-offline-2-Mobile/delivery_ratio.dat' using 1:2 with lines title 'line';"

gnuplot -e "set terminal pngcairo enhanced font 'arial,10' fontscale 1.0 size 800,600;
            set output 'scratch/output.png';
            set title 'node vs throughput graph';
            set xlabel 'ns';
            set ylabel 'delivery ratio';
            plot '1905114-offline-2-Mobile/delivery_ratio.dat' using 1:2 with lines title 'line';"

gnuplot -e "set terminal pngcairo enhanced font 'arial,10' fontscale 1.0 size 800,600;
            set output 'scratch/output.png';
            set title 'node vs throughput graph';
            set xlabel 'ns';
            set ylabel 'delivery ratio';
            plot '1905114-offline-2-Mobile/delivery_ratio.dat' using 1:2 with lines title 'line';"

gnuplot -e "set terminal pngcairo enhanced font 'arial,10' fontscale 1.0 size 800,600;
            set output 'scratch/output.png';
            set title 'node vs throughput graph';
            set xlabel 'ns';
            set ylabel 'delivery ratio';
            plot '1905114-offline-2-Mobile/delivery_ratio.dat' using 1:2 with lines title 'line';"