reset
set border 3 lw 2
set style fill solid
set xtics 1 nomirror
set ytics nomirror
set  boxwidth 0.25
set xlabel "Threshold τ"
set ylabel "P(τ)"
plot "thresh1.txt" u ($1-0.25):2 w boxes lc rgbcolor "#FF0000" title "Strategy 1", "thresh2.txt" u ($1-0.00):2 w boxes lc rgbcolor "#eebb00" title "Strategy 2", "thresh3.txt" u ($1+0.25):2 w boxes lc rgbcolor "#0000FF" title "Strategy 3"

