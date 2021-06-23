<<<<<<< HEAD
unset NUM_THREADS
unset SERIAL

export NUM_THREADS=60 &&
make &&
./BiCore -s -bi 395978 -rounds 10 ../../inputs/flickr_adj
export NUM_THREADS=30 &&
make &&
./BiCore -s -bi 395978 -rounds 10 ../../inputs/flickr_adj
=======
export NUM_THREADS=60 &&
make &&
./BiCore -s -bi 781264 -rounds 3 ../../inputs/reuter_adj
export NUM_THREADS=30 &&
make &&
./BiCore -s -bi 781264 -rounds 3 ../../inputs/reuter_adj
>>>>>>> f1968d38deea70cbacc391ec6fb08999298c6e7b
export NUM_THREADS=16 &&
make &&
./BiCore -s -bi 781264 -rounds 3 ../../inputs/reuter_adj
export NUM_THREADS=8 &&
make &&
./BiCore -s -bi 781264 -rounds 3 ../../inputs/reuter_adj
export NUM_THREADS=4 &&
make &&
./BiCore -s -bi 781264 -rounds 3 ../../inputs/reuter_adj
export NUM_THREADS=2 &&
make &&
./BiCore -s -bi 781264 -rounds 3 ../../inputs/reuter_adj
unset NUM_THREADS &&
export SERIAL=1 &&
make &&
./BiCore -s -bi 781264 -rounds 3 ../../inputs/reuter_adj


unset NUM_THREADS
unset SERIAL

export NUM_THREADS=60 &&
make &&
./BiCore -s -bi 367 -rounds 10 ../../inputs/sqwiki_adj
export NUM_THREADS=30 &&
make &&
./BiCore -s -bi 367 -rounds 10 ../../inputs/sqwiki_adj
export NUM_THREADS=16 &&
make &&
./BiCore -s -bi 367 -rounds 10 ../../inputs/sqwiki_adj
export NUM_THREADS=8 &&
make &&
./BiCore -s -bi 367 -rounds 10 ../../inputs/sqwiki_adj
export NUM_THREADS=4 &&
make &&
./BiCore -s -bi 367 -rounds 10 ../../inputs/sqwiki_adj
export NUM_THREADS=2 &&
make &&
./BiCore -s -bi 367 -rounds 10 ../../inputs/sqwiki_adj
unset NUM_THREADS &&
export SERIAL=1 &&
make &&
<<<<<<< HEAD
./BiCore -s -bi 367 -rounds 10 ../../inputs/sqwiki_adj
=======
./BiCore -s -bi 16725 -rounds 10 ../../inputs/arXiv_adj
>>>>>>> f1968d38deea70cbacc391ec6fb08999298c6e7b


unset NUM_THREADS
unset SERIAL

export NUM_THREADS=60 &&
make &&
./BiCore -s -bi 255 -rounds 10 ../../inputs/tgwiki_adj
export NUM_THREADS=30 &&
make &&
<<<<<<< HEAD
./BiCore -s -bi 255 -rounds 10 ../../inputs/tgwiki_adj
export NUM_THREADS=16 &&
make &&
./BiCore -s -bi 255 -rounds 10 ../../inputs/tgwiki_adj
export NUM_THREADS=8 &&
make &&
./BiCore -s -bi 255 -rounds 10 ../../inputs/tgwiki_adj
export NUM_THREADS=4 &&
make &&
./BiCore -s -bi 255 -rounds 10 ../../inputs/tgwiki_adj
export NUM_THREADS=2 &&
make &&
./BiCore -s -bi 255 -rounds 10 ../../inputs/tgwiki_adj
unset NUM_THREADS &&
export SERIAL=1 &&
make &&
./BiCore -s -bi 255 -rounds 10 ../../inputs/tgwiki_adj


unset NUM_THREADS
unset SERIAL

export NUM_THREADS=60 &&
make &&
./BiCore -s -bi 89354 -rounds 10 ../../inputs/writer_adj
export NUM_THREADS=30 &&
make &&
./BiCore -s -bi 89354 -rounds 10 ../../inputs/writer_adj
export NUM_THREADS=16 &&
make &&
./BiCore -s -bi 89354 -rounds 10 ../../inputs/writer_adj
export NUM_THREADS=8 &&
make &&
./BiCore -s -bi 89354 -rounds 10 ../../inputs/writer_adj
export NUM_THREADS=4 &&
make &&
./BiCore -s -bi 89354 -rounds 10 ../../inputs/writer_adj
export NUM_THREADS=2 &&
make &&
./BiCore -s -bi 89354 -rounds 10 ../../inputs/writer_adj
unset NUM_THREADS &&
export SERIAL=1 &&
make &&
./BiCore -s -bi 89354 -rounds 10 ../../inputs/writer_adj


unset NUM_THREADS
unset SERIAL

export NUM_THREADS=60 &&
make &&
./BiCore -s -bi 168267 -rounds 10 ../../inputs/record_adj
export NUM_THREADS=30 &&
make &&
./BiCore -s -bi 168267 -rounds 10 ../../inputs/record_adj
export NUM_THREADS=16 &&
make &&
./BiCore -s -bi 168267 -rounds 10 ../../inputs/record_adj
export NUM_THREADS=8 &&
make &&
./BiCore -s -bi 168267 -rounds 10 ../../inputs/record_adj
export NUM_THREADS=4 &&
make &&
./BiCore -s -bi 168267 -rounds 10 ../../inputs/record_adj
export NUM_THREADS=2 &&
make &&
./BiCore -s -bi 168267 -rounds 10 ../../inputs/record_adj
unset NUM_THREADS &&
export SERIAL=1 &&
make &&
./BiCore -s -bi 168267 -rounds 10 ../../inputs/record_adj


unset NUM_THREADS
unset SERIAL

export NUM_THREADS=60 &&
make &&
./BiCore -s -bi 56518 -rounds 5 ../../inputs/github_adj
export NUM_THREADS=30 &&
make &&
./BiCore -s -bi 56518 -rounds 5 ../../inputs/github_adj
export NUM_THREADS=16 &&
make &&
./BiCore -s -bi 56518 -rounds 5 ../../inputs/github_adj
export NUM_THREADS=8 &&
make &&
./BiCore -s -bi 56518 -rounds 5 ../../inputs/github_adj
export NUM_THREADS=4 &&
make &&
./BiCore -s -bi 56518 -rounds 5 ../../inputs/github_adj
export NUM_THREADS=2 &&
make &&
./BiCore -s -bi 56518 -rounds 5 ../../inputs/github_adj
unset NUM_THREADS &&
export SERIAL=1 &&
make &&
./BiCore -s -bi 56518 -rounds 5 ../../inputs/github_adj


unset NUM_THREADS
unset SERIAL

export NUM_THREADS=60 &&
make &&
./BiCore -s -bi 56518 -rounds 5 ../../inputs/github_adj
export NUM_THREADS=30 &&
make &&
./BiCore -s -bi 56518 -rounds 5 ../../inputs/github_adj
export NUM_THREADS=16 &&
make &&
./BiCore -s -bi 56518 -rounds 5 ../../inputs/github_adj
export NUM_THREADS=8 &&
make &&
./BiCore -s -bi 56518 -rounds 5 ../../inputs/github_adj
export NUM_THREADS=4 &&
make &&
./BiCore -s -bi 56518 -rounds 5 ../../inputs/github_adj
export NUM_THREADS=2 &&
make &&
./BiCore -s -bi 56518 -rounds 5 ../../inputs/github_adj
unset NUM_THREADS &&
export SERIAL=1 &&
make &&
./BiCore -s -bi 56518 -rounds 5 ../../inputs/github_adj


unset NUM_THREADS
unset SERIAL

export NUM_THREADS=60 &&
make &&
./BiCore -s -bi 590111 -rounds 5 ../../inputs/country_adj
export NUM_THREADS=30 &&
make &&
./BiCore -s -bi 590111 -rounds 5 ../../inputs/country_adj
export NUM_THREADS=16 &&
make &&
./BiCore -s -bi 590111 -rounds 5 ../../inputs/country_adj
export NUM_THREADS=8 &&
make &&
./BiCore -s -bi 590111 -rounds 5 ../../inputs/country_adj
export NUM_THREADS=4 &&
make &&
./BiCore -s -bi 590111 -rounds 5 ../../inputs/country_adj
export NUM_THREADS=2 &&
make &&
./BiCore -s -bi 590111 -rounds 5 ../../inputs/country_adj
unset NUM_THREADS &&
export SERIAL=1 &&
make &&
./BiCore -s -bi 590111 -rounds 5 ../../inputs/country_adj


unset NUM_THREADS
unset SERIAL

export NUM_THREADS=60 &&
make &&
./BiCore -s -bi 105277 -rounds 5 ../../inputs/book_adj
export NUM_THREADS=30 &&
make &&
./BiCore -s -bi 105277 -rounds 5 ../../inputs/book_adj
export NUM_THREADS=16 &&
make &&
./BiCore -s -bi 105277 -rounds 5 ../../inputs/book_adj
export NUM_THREADS=8 &&
make &&
./BiCore -s -bi 105277 -rounds 5 ../../inputs/book_adj
export NUM_THREADS=4 &&
make &&
./BiCore -s -bi 105277 -rounds 5 ../../inputs/book_adj
export NUM_THREADS=2 &&
make &&
./BiCore -s -bi 105277 -rounds 5 ../../inputs/book_adj
unset NUM_THREADS &&
export SERIAL=1 &&
make &&
./BiCore -s -bi 105277 -rounds 5 ../../inputs/book_adj


unset NUM_THREADS
unset SERIAL

export NUM_THREADS=60 &&
make &&
./BiCore -s -bi 35961 -rounds 5 ../../inputs/wikiquote_adj
export NUM_THREADS=30 &&
make &&
./BiCore -s -bi 35961 -rounds 5 ../../inputs/wikiquote_adj
export NUM_THREADS=16 &&
make &&
./BiCore -s -bi 35961 -rounds 5 ../../inputs/wikiquote_adj
export NUM_THREADS=8 &&
make &&
./BiCore -s -bi 35961 -rounds 5 ../../inputs/wikiquote_adj
export NUM_THREADS=4 &&
make &&
./BiCore -s -bi 35961 -rounds 5 ../../inputs/wikiquote_adj
export NUM_THREADS=2 &&
make &&
./BiCore -s -bi 35961 -rounds 5 ../../inputs/wikiquote_adj
unset NUM_THREADS &&
export SERIAL=1 &&
make &&
./BiCore -s -bi 35961 -rounds 5 ../../inputs/wikiquote_adj


unset NUM_THREADS
unset SERIAL

export NUM_THREADS=60 &&
make &&
./BiCore -s -bi 299751 -rounds 3 ../../inputs/nytime_adj
export NUM_THREADS=30 &&
make &&
./BiCore -s -bi 299751 -rounds 3 ../../inputs/nytime_adj
export NUM_THREADS=16 &&
make &&
./BiCore -s -bi 299751 -rounds 3 ../../inputs/nytime_adj
export NUM_THREADS=8 &&
make &&
./BiCore -s -bi 299751 -rounds 3 ../../inputs/nytime_adj
export NUM_THREADS=4 &&
make &&
./BiCore -s -bi 299751 -rounds 3 ../../inputs/nytime_adj
export NUM_THREADS=2 &&
make &&
./BiCore -s -bi 299751 -rounds 3 ../../inputs/nytime_adj
unset NUM_THREADS &&
export SERIAL=1 &&
make &&
./BiCore -s -bi 299751 -rounds 3 ../../inputs/nytime_adj
