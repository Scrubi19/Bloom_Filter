#!/usr/bin/gnuplot -persist

set terminal png size 1000,600 linewidth 4 font "Verdana,14"
set termoption dash
set output "graphic.png"
set title "Экспериментальные результаты опыта"

set grid x y
set key left top
unset log
set xlabel "Количество добавленных элементов"
set ylabel "Процент ошибок поиска"
set xrange [2500 : 100000]
set yrange [0 : 100]

plot "exp.txt" using 1:2 with lines linecolor 7 title "BloomFilter n = 10000(p = 10%)"