
unset SERIAL
unset NUM_THREADS
make
./BiCoreIndex -s -bi 27665729 -rounds 3 ../../inputs/tracker_adj > tracker_60_idx.txt

export NUM_THREADS=30
make
./BiCoreIndex -s -bi 27665729 -rounds 3 ../../inputs/tracker_adj > tracker_30_idx.txt

export NUM_THREADS=24
make
./BiCoreIndex -s -bi 27665729 -rounds 3 ../../inputs/tracker_adj > tracker_24_idx.txt

export NUM_THREADS=16
make
./BiCoreIndex -s -bi 27665729 -rounds 3 ../../inputs/tracker_adj > tracker_16_idx.txt

export NUM_THREADS=12
make
./BiCoreIndex -s -bi 27665729 -rounds 3 ../../inputs/tracker_adj > tracker_12_idx.txt

export NUM_THREADS=8
make
./BiCoreIndex -s -bi 27665729 -rounds 3 ../../inputs/tracker_adj > tracker_8_idx.txt

export NUM_THREADS=4
make
./BiCoreIndex -s -bi 27665729 -rounds 3 ../../inputs/tracker_adj > tracker_4_idx.txt

export NUM_THREADS=2
make
./BiCoreIndex -s -bi 27665729 -rounds 1 ../../inputs/tracker_adj > tracker_2_idx.txt

export SERIAL=1
make
./BiCoreIndex -s -bi 27665729 -rounds 1 ../../inputs/tracker_adj > tracker_1_idx.txt



unset SERIAL
unset NUM_THREADS
make
./BiCoreIndex -s -bi 120491 -rounds 3 ../../inputs/epinion_adj > epinion_60_idx.txt

export NUM_THREADS=30
make
./BiCoreIndex -s -bi 120491 -rounds 3 ../../inputs/epinion_adj > epinion_30_idx.txt

export NUM_THREADS=24
make
./BiCoreIndex -s -bi 120491 -rounds 3 ../../inputs/epinion_adj > epinion_24_idx.txt

export NUM_THREADS=16
make
./BiCoreIndex -s -bi 120491 -rounds 3 ../../inputs/epinion_adj > epinion_16_idx.txt

export NUM_THREADS=12
make
./BiCoreIndex -s -bi 120491 -rounds 3 ../../inputs/epinion_adj > epinion_12_idx.txt

export NUM_THREADS=8
make
./BiCoreIndex -s -bi 120491 -rounds 3 ../../inputs/epinion_adj > epinion_8_idx.txt

export NUM_THREADS=4
make
./BiCoreIndex -s -bi 120491 -rounds 3 ../../inputs/epinion_adj > epinion_4_idx.txt

export NUM_THREADS=2
make
./BiCoreIndex -s -bi 120491 -rounds 3 ../../inputs/epinion_adj > epinion_2_idx.txt

export SERIAL=1
make
./BiCoreIndex -s -bi 120491 -rounds 3 ../../inputs/epinion_adj > epinion_1_idx.txt



unset SERIAL
unset NUM_THREADS
make
./BiCoreIndex -s -bi 3201202 -rounds 3 ../../inputs/journal_adj > journal_60_idx.txt

export NUM_THREADS=30
make
./BiCoreIndex -s -bi 3201202 -rounds 3 ../../inputs/journal_adj > journal_30_idx.txt

export NUM_THREADS=24
make
./BiCoreIndex -s -bi 3201202 -rounds 3 ../../inputs/journal_adj > journal_24_idx.txt

export NUM_THREADS=16
make
./BiCoreIndex -s -bi 3201202 -rounds 3 ../../inputs/journal_adj > journal_16_idx.txt

export NUM_THREADS=12
make
./BiCoreIndex -s -bi 3201202 -rounds 3 ../../inputs/journal_adj > journal_12_idx.txt

export NUM_THREADS=8
make
./BiCoreIndex -s -bi 3201202 -rounds 3 ../../inputs/journal_adj > journal_8_idx.txt

export NUM_THREADS=4
make
./BiCoreIndex -s -bi 3201202 -rounds 3 ../../inputs/journal_adj > journal_4_idx.txt

export NUM_THREADS=2
make
./BiCoreIndex -s -bi 3201202 -rounds 1 ../../inputs/journal_adj > journal_2_idx.txt

export SERIAL=1
make
./BiCoreIndex -s -bi 3201202 -rounds 1 ../../inputs/journal_adj > journal_1_idx.txt


sudo shutdown
