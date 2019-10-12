# most done in ~10 min
# luck2fullbig takes longer

gcc -o3 battlesim.c -lm -o battlesim.ce
gcc -o3 luck2fullbig.c -lm -o luck2fullbig.ce
gcc -o3 dl1.c -lm -o dl1.ce
gcc -o3 socal-log-test.c -lm -o socal-log-test.ce
./battlesim.ce > b.out &
./luck2fullbig.ce > l.out &
./dl1.ce > d.out &
./socal-log-test.ce > s.out &
# s.out gets big (70M)
