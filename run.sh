# most done in ~10 min

gcc -o3 battlesim.c -lm -o battlesim.ce
#gcc -o3 heuristic.c -lm -o heuristic.ce
gcc -o3 simpleluck.c -lm -o simpleluck.ce
gcc -o3 optimal-strategies.c -lm -o optimal-strategies.ce
./battlesim.ce > b.out 
#./heuristic.ce > h.out &
./simpleluck.ce > sl.out 
./optimal-strategies.ce 0 > os0.out 
./optimal-strategies.ce 1 > os1.out 
# these .out files get big (70M)

R CMD BATCH rtree.R

