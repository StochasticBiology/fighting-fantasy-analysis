awk '{if($15 == 1 && $6 > $5+0.01) {n[$16]++; total++;}}END{for(i=1;i<=12;i++) print i, n[i]/total;}' heuristic.txt > thresh1.txt
awk '{if($15 == 2 && $7 > $5+0.01) {n[$16]++; total++;}}END{for(i=1;i<=12;i++) print i, n[i]/total;}' heuristic.txt > thresh2.txt
awk '{if($15 == 3 && $8 > $5+0.01) {n[$16]++; total++;}}END{for(i=1;i<=12;i++) print i, n[i]/total;}' heuristic.txt > thresh3.txt
