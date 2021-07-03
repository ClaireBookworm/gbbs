make
./BiCore -s -bi 2783195 -rounds 3 ../../inputs/orkut_adj > orkut_60.out > orkut_60.error

export NUM_THREADS=30
make
./BiCore -s -bi 2783195 -rounds 3 ../../inputs/orkut_adj > orkut_30.out > orkut_30.error

export NUM_THREADS=24
make
./BiCore -s -bi 2783195 -rounds 3 ../../inputs/orkut_adj > orkut_24.out > orkut_24.error

unset NUM_THREADS
export SERIAL=1
make
./BiCore -s -bi 2783195 -rounds 1 ../../inputs/orkut_adj > orkut_1.out > orkut_1.error

sudo shutdown
