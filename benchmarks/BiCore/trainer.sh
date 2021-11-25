unset SERIAL
export NUM_THREADS=60
make
./BiCore -s -bi 27665729 -rounds 3 ../../inputs/tracker_adj > tracker_60

export NUM_THREADS=30
make
./BiCore -s -bi 27665729 -rounds 3 ../../inputs/tracker_adj > tracker_30

export NUM_THREADS=24
make
./BiCore -s -bi 27665729 -rounds 3 ../../inputs/tracker_adj > tracker_24

export NUM_THREADS=16
make
./BiCore -s -bi 27665729 -rounds 3 ../../inputs/tracker_adj > tracker_16

export NUM_THREADS=12
make
./BiCore -s -bi 27665729 -rounds 3 ../../inputs/tracker_adj > tracker_12

export NUM_THREADS=8
make
./BiCore -s -bi 27665729 -rounds 3 ../../inputs/tracker_adj > tracker_8

export NUM_THREADS=4
make
./BiCore -s -bi 27665729 -rounds 1 ../../inputs/tracker_adj > tracker_4

export NUM_THREADS=2
make
./BiCore -s -bi 27665729 -rounds 1 ../../inputs/tracker_adj > tracker_2

export SERIAL=1
make
./BiCore -s -bi 27665729 -rounds 1 ../../inputs/tracker_adj > tracker_1


unset SERIAL
export NUM_THREADS=60
make
./BiCore -s -bi 395978 -rounds 3 ../../inputs/flickr_adj > flickr_60

export NUM_THREADS=30
make
./BiCore -s -bi 395978 -rounds 3 ../../inputs/flickr_adj > flickr_30

export NUM_THREADS=24
make
./BiCore -s -bi 395978 -rounds 3 ../../inputs/flickr_adj > flickr_24

export NUM_THREADS=16
make
./BiCore -s -bi 395978 -rounds 3 ../../inputs/flickr_adj > flickr_16

export NUM_THREADS=12
make
./BiCore -s -bi 395978 -rounds 3 ../../inputs/flickr_adj > flickr_12

export NUM_THREADS=8
make
./BiCore -s -bi 395978 -rounds 3 ../../inputs/flickr_adj > flickr_8

export NUM_THREADS=4
make
./BiCore -s -bi 395978 -rounds 3 ../../inputs/flickr_adj > flickr_4

export NUM_THREADS=2
make
./BiCore -s -bi 395978 -rounds 3 ../../inputs/flickr_adj > flickr_2

export SERIAL=1
make
./BiCore -s -bi 395978 -rounds 3 ../../inputs/flickr_adj > flickr_1

unset SERIAL
export NUM_THREADS=60
make
./BiCore -s -bi 781264 -rounds 3 ../../inputs/reuter_adj > reuter_60

export NUM_THREADS=30
make
./BiCore -s -bi 781264 -rounds 3 ../../inputs/reuter_adj > reuter_30

export NUM_THREADS=24
make
./BiCore -s -bi 781264 -rounds 3 ../../inputs/reuter_adj > reuter_24

export NUM_THREADS=16
make
./BiCore -s -bi 781264 -rounds 3 ../../inputs/reuter_adj > reuter_16

export NUM_THREADS=12
make
./BiCore -s -bi 781264 -rounds 3 ../../inputs/reuter_adj > reuter_12

export NUM_THREADS=8
make
./BiCore -s -bi 781264 -rounds 3 ../../inputs/reuter_adj > reuter_8

export NUM_THREADS=4
make
./BiCore -s -bi 781264 -rounds 1 ../../inputs/reuter_adj > reuter_4

export NUM_THREADS=2
make
./BiCore -s -bi 781264 -rounds 1 ../../inputs/reuter_adj > reuter_2

export SERIAL=1
make
./BiCore -s -bi 781264 -rounds 1 ../../inputs/reuter_adj > reuter_1

unset SERIAL
export NUM_THREADS=60
make
./BiCore -s -bi 2783195 -rounds 3 ../../inputs/orkut_adj > orkut_60

export NUM_THREADS=30
make
./BiCore -s -bi 2783195 -rounds 3 ../../inputs/orkut_adj > orkut_30

export NUM_THREADS=24
make
./BiCore -s -bi 2783195 -rounds 3 ../../inputs/orkut_adj > orkut_24

export NUM_THREADS=16
make
./BiCore -s -bi 2783195 -rounds 3 ../../inputs/orkut_adj > orkut_16

export NUM_THREADS=12
make
./BiCore -s -bi 2783195 -rounds 3 ../../inputs/orkut_adj > orkut_12

export NUM_THREADS=8
make
./BiCore -s -bi 2783195 -rounds 3 ../../inputs/orkut_adj > orkut_8

export NUM_THREADS=4
make
./BiCore -s -bi 2783195 -rounds 1 ../../inputs/orkut_adj > orkut_4

export NUM_THREADS=2
make
./BiCore -s -bi 2783195 -rounds 1 ../../inputs/orkut_adj > orkut_2

export SERIAL=1
make
./BiCore -s -bi 2783195 -rounds 1 ../../inputs/orkut_adj > orkut_1

sudo shutdown
