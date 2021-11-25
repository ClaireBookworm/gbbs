unset SERIAL
export NUM_THREADS=60
make
./BiCore -s -bi 120491 -rounds 3 ../../inputs/epinion_adj > epinion_60

export NUM_THREADS=30
make
./BiCore -s -bi 120491 -rounds 3 ../../inputs/epinion_adj > epinion_30

export NUM_THREADS=24
make
./BiCore -s -bi 120491 -rounds 3 ../../inputs/epinion_adj > epinion_24

export NUM_THREADS=16
make
./BiCore -s -bi 120491 -rounds 3 ../../inputs/epinion_adj > epinion_16

export NUM_THREADS=12
make
./BiCore -s -bi 120491 -rounds 3 ../../inputs/epinion_adj > epinion_12

export NUM_THREADS=8
make
./BiCore -s -bi 120491 -rounds 3 ../../inputs/epinion_adj > epinion_8

export NUM_THREADS=4
make
./BiCore -s -bi 120491 -rounds 3 ../../inputs/epinion_adj > epinion_4

export NUM_THREADS=2
make
./BiCore -s -bi 120491 -rounds 3 ../../inputs/epinion_adj > epinion_2

export SERIAL=1
make
./BiCore -s -bi 120491 -rounds 3 ../../inputs/epinion_adj > epinion_1

unset SERIAL
export NUM_THREADS=60
make
./BiCore -s -bi 3201202 -rounds 3 ../../inputs/journal_adj > journal_60

export NUM_THREADS=30
make
./BiCore -s -bi 3201202 -rounds 3 ../../inputs/journal_adj > journal_30

export NUM_THREADS=24
make
./BiCore -s -bi 3201202 -rounds 3 ../../inputs/journal_adj > journal_24

export NUM_THREADS=16
make
./BiCore -s -bi 3201202 -rounds 3 ../../inputs/journal_adj > journal_16

export NUM_THREADS=12
make
./BiCore -s -bi 3201202 -rounds 3 ../../inputs/journal_adj > journal_12

export NUM_THREADS=8
make
./BiCore -s -bi 3201202 -rounds 3 ../../inputs/journal_adj > journal_8

export NUM_THREADS=4
make
./BiCore -s -bi 3201202 -rounds 3 ../../inputs/journal_adj > journal_4

export NUM_THREADS=2
make
./BiCore -s -bi 3201202 -rounds 3 ../../inputs/journal_adj > journal_2

export SERIAL=1
make
./BiCore -s -bi 3201202 -rounds 3 ../../inputs/journal_adj > journal_1

sudo shutdown
