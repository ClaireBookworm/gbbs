unset SERIAL
unset NUM_THREADS
make
./BiCoreIndex -s -bi 556076 -rounds 3 ../../inputs/trec_adj > trec_60_idx.txt

export NUM_THREADS=30
make
./BiCoreIndex -s -bi 556076 -rounds 3 ../../inputs/trec_adj > trec_30_idx.txt

export NUM_THREADS=24
make
./BiCoreIndex -s -bi 556076 -rounds 3 ../../inputs/trec_adj > trec_24_idx.txt

export NUM_THREADS=16
make
./BiCoreIndex -s -bi 556076 -rounds 3 ../../inputs/trec_adj > trec_16_idx.txt

export NUM_THREADS=12
make
./BiCoreIndex -s -bi 556076 -rounds 3 ../../inputs/trec_adj > trec_12_idx.txt

export NUM_THREADS=8
make
./BiCoreIndex -s -bi 556076 -rounds 3 ../../inputs/trec_adj > trec_8_idx.txt

export NUM_THREADS=4
make
./BiCoreIndex -s -bi 556076 -rounds 3 ../../inputs/trec_adj > trec_4_idx.txt

export NUM_THREADS=2
make
./BiCoreIndex -s -bi 556076 -rounds 3 ../../inputs/trec_adj > trec_2_idx.txt

export SERIAL=1
make
./BiCoreIndex -s -bi 556076 -rounds 3 ../../inputs/trec_adj > trec_1_idx.txt


unset SERIAL
unset NUM_THREADS
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



unset SERIAL
unset NUM_THREADS
make
./BiCoreIndex -s -bi 2783195 -rounds 3 ../../inputs/orkut_adj > orkut_60_idx.txt

export NUM_THREADS=30
make
./BiCoreIndex -s -bi 2783195 -rounds 3 ../../inputs/orkut_adj > orkut_30_idx.txt

export NUM_THREADS=24
make
./BiCoreIndex -s -bi 2783195 -rounds 3 ../../inputs/orkut_adj > orkut_24_idx.txt

export NUM_THREADS=16
make
./BiCoreIndex -s -bi 2783195 -rounds 3 ../../inputs/orkut_adj > orkut_16_idx.txt

export NUM_THREADS=12
make
./BiCoreIndex -s -bi 2783195 -rounds 3 ../../inputs/orkut_adj > orkut_12_idx.txt

export NUM_THREADS=8
make
./BiCoreIndex -s -bi 2783195 -rounds 3 ../../inputs/orkut_adj > orkut_8_idx.txt

export NUM_THREADS=4
make
./BiCoreIndex -s -bi 2783195 -rounds 3 ../../inputs/orkut_adj > orkut_4_idx.txt

export NUM_THREADS=2
make
./BiCoreIndex -s -bi 2783195 -rounds 3 ../../inputs/orkut_adj > orkut_2_idx.txt

export SERIAL=1
make
./BiCoreIndex -s -bi 2783195 -rounds 3 ../../inputs/orkut_adj > orkut_1_idx.txt



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
./BiCoreIndex -s -bi 27665729 -rounds 3 ../../inputs/tracker_adj > tracker_2_idx.txt

export SERIAL=1
make
./BiCoreIndex -s -bi 27665729 -rounds 3 ../../inputs/tracker_adj > tracker_1_idx.txt



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
./BiCoreIndex -s -bi 3201202 -rounds 3 ../../inputs/journal_adj > journal_2_idx.txt

export SERIAL=1
make
./BiCoreIndex -s -bi 3201202 -rounds 3 ../../inputs/journal_adj > journal_1_idx.txt


sudo shutdown
