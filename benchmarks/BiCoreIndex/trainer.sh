unset NUM_THREADS
unset SERIAL
export NUM_THREADS=60
make
./BiCoreIndex -s -bi 395978 -rounds 3 ../../inputs/flickr_adj > flickr_60_idx.txt

export NUM_THREADS=30
make
./BiCoreIndex -s -bi 395978 -rounds 3 ../../inputs/flickr_adj > flickr_30_idx.txt

export NUM_THREADS=24
make
./BiCoreIndex -s -bi 395978 -rounds 3 ../../inputs/flickr_adj > flickr_24_idx.txt

export NUM_THREADS=16
make
./BiCoreIndex -s -bi 395978 -rounds 3 ../../inputs/flickr_adj > flickr_16_idx.txt

export NUM_THREADS=12
make
./BiCoreIndex -s -bi 395978 -rounds 3 ../../inputs/flickr_adj > flickr_12_idx.txt

export NUM_THREADS=8
make
./BiCoreIndex -s -bi 395978 -rounds 3 ../../inputs/flickr_adj > flickr_8_idx.txt

export NUM_THREADS=4
make
./BiCoreIndex -s -bi 395978 -rounds 3 ../../inputs/flickr_adj > flickr_4_idx.txt

export NUM_THREADS=2
make
./BiCoreIndex -s -bi 395978 -rounds 3 ../../inputs/flickr_adj > flickr_2_idx.txt

export SERIAL=1
make
./BiCoreIndex -s -bi 395978 -rounds 3 ../../inputs/flickr_adj > flickr_1_idx.txt

sudo shutdown