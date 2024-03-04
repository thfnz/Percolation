set datafile separator ','
set xlabel 'p'
set ylabel 'Probabilité que le réseau percole'
plot './resultats/evaluation.csv' with lines notitle smooth unique