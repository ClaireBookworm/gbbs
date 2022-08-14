unset NUM_THREADS
unset SERIAL




for((s=30; s<=60; s*=2)); do
	echo ${s}
	export NUM_THREADS=${s}
	make
	./BiCoreIndex -s -bi 395978 -rounds 3 ../../inputs/flickr_adj > flickr_${s}_idx.txt
	./BiCoreIndex -s -bi 120491 -rounds 3 ../../inputs/epinions_adj > epinions_${s}_idx.txt
	./BiCoreIndex -s -bi 781264 -rounds 3 ../../inputs/reuters_adj > reuters_${s}_idx.txt
	./BiCoreIndex -s -bi 556076 -rounds 3 ../../inputs/trec_adj > trec_${s}_idx.txt
	./BiCoreIndex -s -bi 3201202 -rounds 3 ../../inputs/livejournal_adj > livejournal_${s}_idx.txt
	./BiCoreIndex -s -bi 27665729 -rounds 3 ../../inputs/trackers_adj > trackers_${s}_idx.txt
	./BiCoreIndex -s -bi 2783195 -rounds 3 ../../inputs/orkut_adj > orkut_${s}_idx.txt
done

for((s=1; s<=24; s*=2)); do
	echo ${s}
	export NUM_THREADS=${s}
	make
	./BiCoreIndex -s -bi 395978 -rounds 3 ../../inputs/flickr_adj > flickr_${s}_idx.txt
	./BiCoreIndex -s -bi 120491 -rounds 3 ../../inputs/epinions_adj > epinions_${s}_idx.txt
	./BiCoreIndex -s -bi 781264 -rounds 3 ../../inputs/reuters_adj > reuters_${s}_idx.txt
	./BiCoreIndex -s -bi 556076 -rounds 3 ../../inputs/trec_adj > trec_${s}_idx.txt
	./BiCoreIndex -s -bi 3201202 -rounds 3 ../../inputs/livejournal_adj > livejournal_${s}_idx.txt
	./BiCoreIndex -s -bi 27665729 -rounds 3 ../../inputs/trackers_adj > trackers_${s}_idx.txt
	./BiCoreIndex -s -bi 2783195 -rounds 3 ../../inputs/orkut_adj > orkut_${s}_idx.txt
done


sudo shutdown
