export NUM_THREADS=16 &&
make &&
./BiCore -s -bi 395978 -rounds 10 ../../inputs/flickr_adj
export NUM_THREADS=8 &&
make &&
./BiCore -s -bi 395978 -rounds 10 ../../inputs/flickr_adj
export NUM_THREADS=4 &&
make &&
./BiCore -s -bi 395978 -rounds 10 ../../inputs/flickr_adj
export NUM_THREADS=2 &&
make &&
./BiCore -s -bi 395978 -rounds 10 ../../inputs/flickr_adj
unset NUM_THREADS &&
export SERIAL=1 &&
make &&
./BiCore -s -bi 395978 -rounds 10 ../../inputs/flickr_adj

unset SERIAL &&
unset NUM_THREADS &&
make &&
./BiCore -s -bi 556076 -rounds 10 ../../inputs/trec_adj

export NUM_THREADS=30 &&
make &&
./BiCore -s -bi 556076 -rounds 10 ../../inputs/trec_adj
export NUM_THREADS=16 &&
make &&
./BiCore -s -bi 556076 -rounds 10 ../../inputs/trec_adj
export NUM_THREADS=8 &&
make &&
./BiCore -s -bi 556076 -rounds 10 ../../inputs/trec_adj
export NUM_THREADS=4 &&
make &&
./BiCore -s -bi 556076 -rounds 10 ../../inputs/trec_adj
export NUM_THREADS=2 &&
make &&
./BiCore -s -bi 556076 -rounds 10 ../../inputs/trec_adj

unset NUM_THREADS &&
export SERIAL=1 &&
make &&
./BiCore -s -bi 556076 -rounds 10 ../../inputs/trec_adj

