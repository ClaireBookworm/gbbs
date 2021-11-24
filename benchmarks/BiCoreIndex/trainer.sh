unset SERIAL
unset NUM_THREADS
make
./BiCoreIndex -s -bi 833080 -rounds 3 ../../inputs/dui_adj > dui_60_idx.txt

export NUM_THREADS=30
make
./BiCoreIndex -s -bi 833080 -rounds 3 ../../inputs/dui_adj > dui_30_idx.txt

export NUM_THREADS=24
make
./BiCoreIndex -s -bi 833080 -rounds 3 ../../inputs/dui_adj > dui_24_idx.txt

export NUM_THREADS=16
make
./BiCoreIndex -s -bi 833080 -rounds 3 ../../inputs/dui_adj > dui_16_idx.txt

export NUM_THREADS=12
make
./BiCoreIndex -s -bi 833080 -rounds 3 ../../inputs/dui_adj > dui_12_idx.txt

export NUM_THREADS=8
make
./BiCoreIndex -s -bi 833080 -rounds 3 ../../inputs/dui_adj > dui_8_idx.txt

export NUM_THREADS=4
make
./BiCoreIndex -s -bi 833080 -rounds 3 ../../inputs/dui_adj > dui_4_idx.txt

export NUM_THREADS=2
make
./BiCoreIndex -s -bi 833080 -rounds 3 ../../inputs/dui_adj > dui_2_idx.txt

export SERIAL=1
make
./BiCoreIndex -s -bi 833080 -rounds 3 ../../inputs/dui_adj > dui_1_idx.txt


unset SERIAL
unset NUM_THREADS
make
./BiCoreIndex -s -bi 781264 -rounds 3 ../../inputs/reuter_adj > reuter_60_idx.txt

export NUM_THREADS=30
make
./BiCoreIndex -s -bi 781264 -rounds 3 ../../inputs/reuter_adj > reuter_30_idx.txt

export NUM_THREADS=24
make
./BiCoreIndex -s -bi 781264 -rounds 3 ../../inputs/reuter_adj > reuter_24_idx.txt

export NUM_THREADS=16
make
./BiCoreIndex -s -bi 781264 -rounds 3 ../../inputs/reuter_adj > reuter_16_idx.txt

export NUM_THREADS=12
make
./BiCoreIndex -s -bi 781264 -rounds 3 ../../inputs/reuter_adj > reuter_12_idx.txt

export NUM_THREADS=8
make
./BiCoreIndex -s -bi 781264 -rounds 3 ../../inputs/reuter_adj > reuter_8_idx.txt

export NUM_THREADS=4
make
./BiCoreIndex -s -bi 781264 -rounds 3 ../../inputs/reuter_adj > reuter_4_idx.txt

export NUM_THREADS=2
make
./BiCoreIndex -s -bi 781264 -rounds 3 ../../inputs/reuter_adj > reuter_2_idx.txt

export SERIAL=1
make
./BiCoreIndex -s -bi 781264 -rounds 3 ../../inputs/reuter_adj > reuter_1_idx.txt

sudo shutdown
