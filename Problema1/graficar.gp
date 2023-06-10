set terminal pngcairo enhanced size 1024, 1024
set output 'grafico.png
set style data lines
set xlabel 'Eje x [m]'
set ylabel 'Eje y [m]'
set polar
set grid polar

 plot 'misil.dat' using 4:2 ti 'Trayectoria Misil', 6371000 ti 'Tierra', 'proyectil.dat' using 4:2 ti 'Proyectil Misil'
