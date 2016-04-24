set terminal pngcairo size 700,700 enhanced font 'Verdana,10'
set output 'last.png'
set key off; 
plot for [i=1:400] ''.i.'.txt' using 1:2
