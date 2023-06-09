set terminal pngcairo enhanced font 'Verdana,12'
set output 'autovectores.png'

set title 'Autovectores'
set xlabel 'x'
set ylabel 'phi(x)'

set key outside

plot 'datos.dat' using 1:2 every ::0::100 with lines title 'E1', \
     'datos.dat' using 1:2 every ::102::202 with lines title 'E2', \
     'datos.dat' using 1:2 every ::204::304 with lines title 'E3', \
     'datos.dat' using 1:2 every ::306::406 with lines title 'E4', \
     'datos.dat' using 1:2 every ::408::508 with lines title 'E5'
