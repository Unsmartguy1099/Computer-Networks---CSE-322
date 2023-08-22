#!/bin/bash
echo "hello world"


# Define a function
my_function() {
    
output_file="delivery_ratio.dat"

# Check if the output file exists, and if so, delete it
if [ -f "$output_file" ]; then
    rm "$output_file"
fi

# Loop through the input files and extract the first line
for i in {1..5}; do
    input_file="delivery_ratio$i.dat"

    # Read the first line of the input file and append it to the output file
    if [ -f "$input_file" ]; then
        head -n 1 "$input_file" >> "$output_file"
    fi
done

echo "Merged first lines of delivery_ratio files into $output_file"

#----------------------------------------


output_file="average_net_throughput.dat"

# Check if the output file exists, and if so, delete it
if [ -f "$output_file" ]; then
    rm "$output_file"
fi

# Loop through the input files and calculate averages
for i in {1..5}; do
    input_file="net_throughput$i.dat"

    if [ -f "$input_file" ]; then
        avg_column1=$(awk '{ sum += $1 } END { print sum / NR }' "$input_file")
        avg_column2=$(awk '{ sum += $2 } END { print sum / NR }' "$input_file")

        # Append averages to the output file
        echo "$avg_column1 $avg_column2" >> "$output_file"
    fi
done

echo "Averages calculated and written to $output_file"
}

cd ..

echo "nWifi"
i=0
mkdir "1905114-offline-2-Static1"
for nWifi in 20 40 60 80 100; do
    ((i++))
  ./ns3 run "scratch/1905114_static.cc --nWifi=$nWifi --number=$i --plot=1"
done
cd "1905114-offline-2-Static1"
my_function
cd ..

echo "flow"
i=0
mkdir "1905114-offline-2-Static2"
for flow in 10 20 30 40 50; do
    ((i++))
  ./ns3 run "scratch/1905114_static.cc --flow=$flow --number=$i --plot=2" 
done
cd "1905114-offline-2-Static2"
my_function
cd ..

echo "pps"
i=0
mkdir "1905114-offline-2-Static3"
for packet_rate in 100 200 300 400 500; do
  ((i++))
  ./ns3 run "scratch/1905114_static.cc --packet_rate=$packet_rate --number=$i --plot=3" 
done
cd "1905114-offline-2-Static3"
my_function
cd ..

echo "range"
i=0
mkdir "1905114-offline-2-Static4"
for range in 1 2 4 5 6; do
    ((i++))
  ./ns3 run "scratch/1905114_static.cc --range=$range -number=$i --plot=4" 
done
cd "1905114-offline-2-Static4"
my_function
cd ..

#mobile----------

echo "nWifi"
i=0
mkdir "1905114-offline-2-Mobile1"
for nWifi in 20 40 60 80 100; do
  ((i++))
  ./ns3 run "scratch/1905114_mobile.cc --nWifi=$nWifi --number=$i --plot=1"
done
cd "1905114-offline-2-Mobile1"
my_function
cd ..

echo "flow"
i=0
mkdir "1905114-offline-2-Mobile2"
for flow in 10 20 30 40 50; do
  ((i++))
  ./ns3 run "scratch/1905114_mobile.cc --flow=$flow --number=$i --plot=2" 
done
cd "1905114-offline-2-Mobile2"
my_function
cd ..

echo "pps"
i=0
mkdir "1905114-offline-2-Mobile3"
for packet_rate in 100 200 300 400 500; do
  ((i++))
  ./ns3 run "scratch/1905114_mobile.cc --packet_rate=$packet_rate --number=$i --plot=3" 
done
cd "1905114-offline-2-Mobile3"
my_function
cd ..

echo "speed"
i=0
mkdir "1905114-offline-2-Mobile4"
for speed in 5 10 15 20 25; do
  ((i++))
  ./ns3 run "scratch/1905114_mobile.cc --speed=$speed --number=$i --plot=4" 
done
cd "1905114-offline-2-Mobile4"
my_function
cd ..



mkdir plots

gnuplot -e "set terminal pngcairo enhanced font 'arial,10' fontscale 1.0 size 800,600;
            set output 'plots/Static_throughput_vs_node.png';
            set title 'throughput vs node graph';
            set xlabel 'node';
            set ylabel 'throughput';
            plot '1905114-offline-2-Static1/average_net_throughput.dat' using 1:2 with lines title 'line';"

gnuplot -e "set terminal pngcairo enhanced font 'arial,10' fontscale 1.0 size 800,600;
            set output 'plots/Static_delivery_ratio_vs_node.png';
            set title 'delivery raito graph vs node';
            set xlabel 'node';
            set ylabel 'delivery ratio';
            plot '1905114-offline-2-Static1/delivery_ratio.dat' using 1:2 with lines title 'line';"

gnuplot -e "set terminal pngcairo enhanced font 'arial,10' fontscale 1.0 size 800,600;
            set output 'plots/Mobile_throughput_vs_node.png';
            set title 'throughput vs node graph';
            set xlabel 'node';
            set ylabel 'throughput';
            plot '1905114-offline-2-Mobile1/average_net_throughput.dat' using 1:2 with lines title 'line';"

gnuplot -e "set terminal pngcairo enhanced font 'arial,10' fontscale 1.0 size 800,600;
            set output 'plots/Mobile_delivery_ratio_vs_node.png';
            set title 'delivery raito graph vs node';
            set xlabel 'node';
            set ylabel 'delivery ratio';
            plot '1905114-offline-2-Mobile1/delivery_ratio.dat' using 1:2 with lines title 'line';"

gnuplot -e "set terminal pngcairo enhanced font 'arial,10' fontscale 1.0 size 800,600;
            set output 'plots/Static_throughput_vs_flow.png';
            set title 'throughput vs flow graph';
            set xlabel 'flow';
            set ylabel 'throughput';
            plot '1905114-offline-2-Static2/average_net_throughput.dat' using 1:2 with lines title 'line';"

gnuplot -e "set terminal pngcairo enhanced font 'arial,10' fontscale 1.0 size 800,600;
            set output 'plots/Static_delivery_ratio_vs_flow.dat.png';
            set title 'deliveryratio vs flow graph';
            set xlabel 'flow';
            set ylabel 'delivery ratio.dat';
            plot '1905114-offline-2-Static2/delivery_ratio.dat' using 1:2 with lines title 'line';"

gnuplot -e "set terminal pngcairo enhanced font 'arial,10' fontscale 1.0 size 800,600;
            set output 'plots/Mobile_throughput_vs_flow.png';
            set title 'throughput vs flow graph';
            set xlabel 'flow';
            set ylabel 'throughput';
            plot '1905114-offline-2-Mobile2/average_net_throughput.dat' using 1:2 with lines title 'line';"

gnuplot -e "set terminal pngcairo enhanced font 'arial,10' fontscale 1.0 size 800,600;
            set output 'plots/Mobile_delivery_ratio_vs_flow.dat.png';
            set title 'deliveryratio vs flow graph';
            set xlabel 'flow';
            set ylabel 'delivery ratio.dat';
            plot '1905114-offline-2-Mobile2/delivery_ratio.dat' using 1:2 with lines title 'line';"

gnuplot -e "set terminal pngcairo enhanced font 'arial,10' fontscale 1.0 size 800,600;
            set output 'plots/Static_throughput_vs_pps.png';
            set title 'throughput vs pps graph';
            set xlabel 'pps';
            set ylabel 'throughput';
            plot '1905114-offline-2-Static3/average_net_throughput.dat' using 1:2 with lines title 'line';"

gnuplot -e "set terminal pngcairo enhanced font 'arial,10' fontscale 1.0 size 800,600;
            set output 'plots/Static_delivery_ratio_vs_pps.dat.png';
            set title 'deliveryratio vs pps graph';
            set xlabel 'pps';
            set ylabel 'delivery ratio.dat';
            plot '1905114-offline-2-Static3/delivery_ratio.dat' using 1:2 with lines title 'line';"

gnuplot -e "set terminal pngcairo enhanced font 'arial,10' fontscale 1.0 size 800,600;
            set output 'plots/Mobile_throughput_vs_pps.png';
            set title 'throughput vs pps graph';
            set xlabel 'pps';
            set ylabel 'throughput';
            plot '1905114-offline-2-Mobile3/average_net_throughput.dat' using 1:2 with lines title 'line';"

gnuplot -e "set terminal pngcairo enhanced font 'arial,10' fontscale 1.0 size 800,600;
            set output 'plots/Mobile_delivery_ratio_vs_pps.dat.png';
            set title 'deliveryratio vs pps graph';
            set xlabel 'pps';
            set ylabel 'delivery ratio.dat';
            plot '1905114-offline-2-Mobile3/delivery_ratio.dat' using 1:2 with lines title 'line';"

gnuplot -e "set terminal pngcairo enhanced font 'arial,10' fontscale 1.0 size 800,600;
            set output 'plots/Static_throughput_vs_range.png';
            set title 'throughput vs range graph';
            set xlabel 'range';
            set ylabel 'throughput';
            plot '1905114-offline-2-Static4/average_net_throughput.dat' using 1:2 with lines title 'line';"

gnuplot -e "set terminal pngcairo enhanced font 'arial,10' fontscale 1.0 size 800,600;
            set output 'plots/Static_delivery_ratio_vs_range.dat.png';
            set title 'deliveryratio vs range graph';
            set xlabel 'range';
            set ylabel 'delivery ratio.dat';
            plot '1905114-offline-2-Static4/delivery_ratio.dat' using 1:2 with lines title 'line';"

gnuplot -e "set terminal pngcairo enhanced font 'arial,10' fontscale 1.0 size 800,600;
            set output 'plots/Static_throughput_vs_speed.png';
            set title 'throughput vs speed graph';
            set xlabel 'speed';
            set ylabel 'throughput';
            plot '1905114-offline-2-Mobile4/average_net_throughput.dat' using 1:2 with lines title 'line';"

gnuplot -e "set terminal pngcairo enhanced font 'arial,10' fontscale 1.0 size 800,600;
            set output 'plots/Mobile_delivery_ratio_vs_speed.dat.png';
            set title 'deliveryratio vs speed graph';
            set xlabel 'speed';
            set ylabel 'delivery ratio.dat';
            plot '1905114-offline-2-Mobile4/delivery_ratio.dat' using 1:2 with lines title 'line';"

            