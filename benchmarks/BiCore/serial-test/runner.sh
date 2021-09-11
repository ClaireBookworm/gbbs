export SERIAL=1
make
./BiCore_serial -s -bi 556076 -rounds 1 ../../../inputs/trec_adj > trec_serial_1
./BiCore_serial -s -bi 556076 -rounds 1 ../../../inputs/trec_adj > trec_serial_2
./BiCore_serial -s -bi 556076 -rounds 1 ../../../inputs/trec_adj > trec_serial_3

sudo shutdown
