export NUM_THREADS=60
make
./BiCore -s -bi 2783195 -rounds 1 ../../inputs/orkut_adj > orkut_60
./BiCore -s -bi 3201202 -rounds 1 ../../inputs/journal_adj > journal_60
./BiCore -s -bi 27665729 -rounds 1 ../../inputs/tracker_adj > tracker_60

export NUM_THREADS=30
make
./BiCore -s -bi 2783195 -rounds 1 ../../inputs/orkut_adj > orkut_30
./BiCore -s -bi 3201202 -rounds 1 ../../inputs/journal_adj > journal_30
./BiCore -s -bi 27665729 -rounds 1 ../../inputs/tracker_adj > tracker_30

export NUM_THREADS=24
make
./BiCore -s -bi 2783195 -rounds 1 ../../inputs/orkut_adj > orkut_24
./BiCore -s -bi 3201202 -rounds 1 ../../inputs/journal_adj > journal_24
./BiCore -s -bi 27665729 -rounds 1 ../../inputs/tracker_adj > tracker_24

export NUM_THREADS=16
make
./BiCore -s -bi 2783195 -rounds 1 ../../inputs/orkut_adj > orkut_16
./BiCore -s -bi 3201202 -rounds 1 ../../inputs/journal_adj > journal_16
./BiCore -s -bi 27665729 -rounds 1 ../../inputs/tracker_adj > tracker_16

export NUM_THREADS=12
make
./BiCore -s -bi 2783195 -rounds 1 ../../inputs/orkut_adj > orkut_12
./BiCore -s -bi 3201202 -rounds 1 ../../inputs/journal_adj > journal_12
./BiCore -s -bi 27665729 -rounds 1 ../../inputs/tracker_adj > tracker_12

export NUM_THREADS=8
make
./BiCore -s -bi 2783195 -rounds 1 ../../inputs/orkut_adj > orkut_8
./BiCore -s -bi 3201202 -rounds 1 ../../inputs/journal_adj > journal_8
./BiCore -s -bi 27665729 -rounds 1 ../../inputs/tracker_adj > tracker_8

export NUM_THREADS=4
make
./BiCore -s -bi 2783195 -rounds 1 ../../inputs/orkut_adj > orkut_4
./BiCore -s -bi 3201202 -rounds 1 ../../inputs/journal_adj > journal_4
./BiCore -s -bi 27665729 -rounds 1 ../../inputs/tracker_adj > tracker_4

export NUM_THREADS=2
make
./BiCore -s -bi 2783195 -rounds 1 ../../inputs/orkut_adj > orkut_2
./BiCore -s -bi 3201202 -rounds 1 ../../inputs/journal_adj > journal_2
./BiCore -s -bi 27665729 -rounds 1 ../../inputs/tracker_adj > tracker_2

export SERIAL=1
make
./BiCore -s -bi 2783195 -rounds 1 ../../inputs/orkut_adj > orkut_1
./BiCore -s -bi 3201202 -rounds 1 ../../inputs/journal_adj > journal_1
./BiCore -s -bi 27665729 -rounds 1 ../../inputs/tracker_adj > tracker_1

sudo shutdown
