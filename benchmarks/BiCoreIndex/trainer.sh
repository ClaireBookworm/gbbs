unset NUM_THREADS
unset SERIAL
export NUM_THREADS=60
make
./BiCoreIndex -s -bi 781264 -rounds 3 ../../inputs/reuter_adj > reuter_60

export NUM_THREADS=30
make
./BiCoreIndex -s -bi 781264 -rounds 3 ../../inputs/reuter_adj > reuter_30

export NUM_THREADS=24
make
./BiCoreIndex -s -bi 781264 -rounds 3 ../../inputs/reuter_adj > reuter_24

export NUM_THREADS=16
make
./BiCoreIndex -s -bi 781264 -rounds 3 ../../inputs/reuter_adj > reuter_16

export NUM_THREADS=12
make
./BiCoreIndex -s -bi 781264 -rounds 3 ../../inputs/reuter_adj > reuter_12

export NUM_THREADS=8
make
./BiCoreIndex -s -bi 781264 -rounds 3 ../../inputs/reuter_adj > reuter_8

export NUM_THREADS=4
make
./BiCoreIndex -s -bi 781264 -rounds 3 ../../inputs/reuter_adj > reuter_4

export NUM_THREADS=2
make
./BiCoreIndex -s -bi 781264 -rounds 3 ../../inputs/reuter_adj > reuter_2

export SERIAL=1
make
./BiCoreIndex -s -bi 781264 -rounds 3 ../../inputs/reuter_adj > reuter_1

sudo shutdown
