export NUM_THREADS=60
make
./BiCore -s -bi 556076 -rounds 1 ../../inputs/trec_adj > trec_60

export NUM_THREADS=30
make
./BiCore -s -bi 556076 -rounds 1 ../../inputs/trec_adj > trec_30

export NUM_THREADS=24
make
./BiCore -s -bi 556076 -rounds 1 ../../inputs/trec_adj > trec_24

export NUM_THREADS=16
make
./BiCore -s -bi 556076 -rounds 1 ../../inputs/trec_adj > trec_16

export NUM_THREADS=12
make
./BiCore -s -bi 556076 -rounds 1 ../../inputs/trec_adj > trec_12

export NUM_THREADS=8
make
./BiCore -s -bi 556076 -rounds 1 ../../inputs/trec_adj > trec_8

export NUM_THREADS=4
make
./BiCore -s -bi 556076 -rounds 1 ../../inputs/trec_adj > trec_4

export NUM_THREADS=2
make
./BiCore -s -bi 556076 -rounds 1 ../../inputs/trec_adj > trec_2

export SERIAL=1
make
./BiCore -s -bi 556076 -rounds 1 ../../inputs/trec_adj > trec_1

sudo shutdown
