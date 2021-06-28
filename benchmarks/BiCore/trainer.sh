unset NUM_THREADS
unset SERIAL

export NUM_THREADS=60 &&
make &&
./BiCore -s -bi 89354 -rounds 10 ../../inputs/writer_adj > writer_60.out > writer_60.error
export NUM_THREADS=30 &&
make &&
./BiCore -s -bi 89354 -rounds 10 ../../inputs/writer_adj > writer_30.out > writer_30.error
export NUM_THREADS=16 &&
make &&
./BiCore -s -bi 89354 -rounds 10 ../../inputs/writer_adj > writer_16.out > writer_16.error
export NUM_THREADS=8 &&
make &&
./BiCore -s -bi 89354 -rounds 10 ../../inputs/writer_adj > writer_8.out > writer_8.error
export NUM_THREADS=4 &&
make &&
./BiCore -s -bi 89354 -rounds 10 ../../inputs/writer_adj > writer_4.out > writer_4.error
export NUM_THREADS=2 &&
make &&
./BiCore -s -bi 89354 -rounds 10 ../../inputs/writer_adj > writer_2.out > writer_2.error
unset NUM_THREADS &&
export SERIAL=1 &&
make &&
./BiCore -s -bi 89354 -rounds 10 ../../inputs/writer_adj > writer_1.out > writer_1.error


unset NUM_THREADS
unset SERIAL

export NUM_THREADS=60 &&
make &&
./BiCore -s -bi 168267 -rounds 10 ../../inputs/record_adj > record_60.out > record_60.error
export NUM_THREADS=30 &&
make &&
./BiCore -s -bi 168267 -rounds 10 ../../inputs/record_adj > record_30.out > record_30.error
export NUM_THREADS=16 &&
make &&
./BiCore -s -bi 168267 -rounds 10 ../../inputs/record_adj > record_16.out > record_16.error
export NUM_THREADS=8 &&
make &&
./BiCore -s -bi 168267 -rounds 10 ../../inputs/record_adj > record_8.out > record_8.error
export NUM_THREADS=4 &&
make &&
./BiCore -s -bi 168267 -rounds 10 ../../inputs/record_adj > record_4.out > record_4.error
export NUM_THREADS=2 &&
make &&
./BiCore -s -bi 168267 -rounds 10 ../../inputs/record_adj > record_2.out > record_2.error
unset NUM_THREADS &&
export SERIAL=1 &&
make &&
./BiCore -s -bi 168267 -rounds 10 ../../inputs/record_adj > record_1.out > record_1.error

unset NUM_THREADS
unset SERIAL

export NUM_THREADS=60 &&
make &&
./BiCore -s -bi 56518 -rounds 5 ../../inputs/github_adj > github_60.out > github_60.error
export NUM_THREADS=30 &&
make &&
./BiCore -s -bi 56518 -rounds 5 ../../inputs/github_adj > github_30.out > github_30.error
export NUM_THREADS=16 &&
make &&
./BiCore -s -bi 56518 -rounds 5 ../../inputs/github_adj > github_16.out > github_16.error
export NUM_THREADS=8 &&
make &&
./BiCore -s -bi 56518 -rounds 5 ../../inputs/github_adj > github_8.out > github_8.error
export NUM_THREADS=4 &&
make &&
./BiCore -s -bi 56518 -rounds 5 ../../inputs/github_adj > github_4.out > github_4.error
export NUM_THREADS=2 &&
make &&
./BiCore -s -bi 56518 -rounds 5 ../../inputs/github_adj > github_2.out > github_2.error
unset NUM_THREADS &&
export SERIAL=1 &&
make &&
./BiCore -s -bi 56518 -rounds 5 ../../inputs/github_adj > github_1.out > github_1.error


unset NUM_THREADS
unset SERIAL

export NUM_THREADS=60 &&
make &&
./BiCore -s -bi 590111 -rounds 5 ../../inputs/country_adj > country_60.out > country_60.error
export NUM_THREADS=30 &&
make &&
./BiCore -s -bi 590111 -rounds 5 ../../inputs/country_adj > country_30.out > country_30.error
export NUM_THREADS=16 &&
make &&
./BiCore -s -bi 590111 -rounds 5 ../../inputs/country_adj > country_16.out > country_16.error
export NUM_THREADS=8 &&
make &&
./BiCore -s -bi 590111 -rounds 5 ../../inputs/country_adj > country_8.out > country_8.error
export NUM_THREADS=4 &&
make &&
./BiCore -s -bi 590111 -rounds 5 ../../inputs/country_adj > country_4.out > country_4.error
export NUM_THREADS=2 &&
make &&
./BiCore -s -bi 590111 -rounds 5 ../../inputs/country_adj > country_2.out > country_2.error
unset NUM_THREADS
export SERIAL=1 &&
make &&
./BiCore -s -bi 590111 -rounds 5 ../../inputs/country_adj > country_1.out > country_1.error

unset SERIAL
unset NUM_THREADS
export NUM_THREADS=60 &&
make &&
./BiCore -s -bi 105277 -rounds 5 ../../inputs/book_adj > book_60.out > book_60.error
export NUM_THREADS=30 &&
make &&
./BiCore -s -bi 105277 -rounds 5 ../../inputs/book_adj > book_30.out > book_30.error
export NUM_THREADS=16 &&
make &&
./BiCore -s -bi 105277 -rounds 5 ../../inputs/book_adj > book_16.out > book_16.error
export NUM_THREADS=8 &&
make &&
./BiCore -s -bi 105277 -rounds 5 ../../inputs/book_adj > book_8.out > book_8.error
export NUM_THREADS=4 &&
make &&
./BiCore -s -bi 105277 -rounds 5 ../../inputs/book_adj > book_4.out > book_4.error
export NUM_THREADS=2 &&
make &&
./BiCore -s -bi 105277 -rounds 5 ../../inputs/book_adj > book_2.out > book_2.error
unset NUM_THREADS &&
export SERIAL=1 &&
make &&
./BiCore -s -bi 105277 -rounds 5 ../../inputs/book_adj > book_1.out > book_1.error


unset NUM_THREADS
unset SERIAL

export NUM_THREADS=60 &&
make &&
./BiCore -s -bi 35961 -rounds 5 ../../inputs/wikiquote_adj > wikiquote_60.out > wikiquote_60.error
export NUM_THREADS=30 &&
make &&
./BiCore -s -bi 35961 -rounds 5 ../../inputs/wikiquote_adj > wikiquote_30.out > wikiquote_30.error
export NUM_THREADS=16 &&
make &&
./BiCore -s -bi 35961 -rounds 5 ../../inputs/wikiquote_adj > wikiquote_16.out > wikiquote_16.error
export NUM_THREADS=8 &&
make &&
./BiCore -s -bi 35961 -rounds 5 ../../inputs/wikiquote_adj > wikiquote_8.out > wikiquote_8.error
export NUM_THREADS=4 &&
make &&
./BiCore -s -bi 35961 -rounds 5 ../../inputs/wikiquote_adj > wikiquote_4.out > wikiquote_4.error
export NUM_THREADS=2 &&
make &&
./BiCore -s -bi 35961 -rounds 5 ../../inputs/wikiquote_adj > wikiquote_2.out > wikiquote_2.error
unset NUM_THREADS &&
export SERIAL=1 &&
make &&
./BiCore -s -bi 35961 -rounds 5 ../../inputs/wikiquote_adj > wikiquote_1.out > wikiquote_1.error


unset NUM_THREADS
unset SERIAL
