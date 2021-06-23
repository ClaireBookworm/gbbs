export NUM_THREADS=60 &&
make &&
./BiCore -s -bi 781264 -rounds 3 ../../inputs/reuter_adj
export NUM_THREADS=30 &&
make &&
./BiCore -s -bi 781264 -rounds 3 ../../inputs/reuter_adj
export NUM_THREADS=16 &&
make &&
./BiCore -s -bi 781264 -rounds 3 ../../inputs/reuter_adj
export NUM_THREADS=8 &&
make &&
./BiCore -s -bi 781264 -rounds 3 ../../inputs/reuter_adj
export NUM_THREADS=4 &&
make &&
./BiCore -s -bi 781264 -rounds 3 ../../inputs/reuter_adj
export NUM_THREADS=2 &&
make &&
./BiCore -s -bi 781264 -rounds 3 ../../inputs/reuter_adj
unset NUM_THREADS &&
export SERIAL=1 &&
make &&
./BiCore -s -bi 781264 -rounds 3 ../../inputs/reuter_adj

unset SERIAL &&
unset NUM_THREADS &&
make &&
./BiCore -s -bi 16725 -rounds 10 ../../inputs/arXiv_adj

export NUM_THREADS=30 &&
make &&
./BiCore -s -bi 16725 -rounds 10 ../../inputs/arXiv_adj
export NUM_THREADS=16 &&
make &&
./BiCore -s -bi 16725 -rounds 10 ../../inputs/arXiv_adj
export NUM_THREADS=8 &&
make &&
./BiCore -s -bi 16725 -rounds 10 ../../inputs/arXiv_adj
export NUM_THREADS=4 &&
make &&
./BiCore -s -bi 16725 -rounds 10 ../../inputs/arXiv_adj
export NUM_THREADS=2 &&
make &&
./BiCore -s -bi 16725 -rounds 10 ../../inputs/arXiv_adj
unset NUM_THREADS &&
export SERIAL=1 &&
make &&
./BiCore -s -bi 16725 -rounds 10 ../../inputs/arXiv_adj

unset SERIAL &&
unset NUM_THREADS &&
make &&
./BiCore -s -bi 1953084 -rounds 5 ../../inputs/dblp_adj

export NUM_THREADS=30 &&
make &&
./BiCore -s -bi 1953084 -rounds 5 ../../inputs/dblp_adj
export NUM_THREADS=16 &&
make &&
./BiCore -s -bi 1953084 -rounds 5 ../../inputs/dblp_adj
export NUM_THREADS=8 &&
make &&
./BiCore -s -bi 1953084 -rounds 5 ../../inputs/dblp_adj
export NUM_THREADS=4 &&
make &&
./BiCore -s -bi 1953084 -rounds 5 ../../inputs/dblp_adj
export NUM_THREADS=2 &&
make &&
./BiCore -s -bi 1953084 -rounds 5 ../../inputs/dblp_adj

unset NUM_THREADS &&
export SERIAL=1 &&
make &&
./BiCore -s -bi 1953084 -rounds 5 ../../inputs/dblp_adj
