def ComShrDecom(U, V, E):
	delta = max_unicore(U+V, E)
	for a in range(1, delta+1):
		peelByB(U, V, E, a)
	for b in range(1, delta+1):
		peelByA(U, V, E, b)


def histogram(tracker):
	parallel filter(tracker, 0)  # filter out empty elements
	parallel sort(tracker)
	hist = parallel freqCount(tracker)
	return hist


def prefixSums(x):
	for d in range(0, (lg n)):
		parallel for i in range((2**d), n-1):
			newx[i] = x[i-2**d] + x[i]
		x = newx


def max_unicore(V, E):
	degbuckets = ParallelBucketArray(V)
	# bucketqueue datastructure: a dynamic array of buckets
	# where each bucket (implemented as dynamic arrays) stores vertices of a certain deg
	max_deg = 0
	# store exp search
	while exponentialSearch(degbuckets) is not None:
		cur_bucket = exponentialSearch(degbuckets)
		# each bucket stores the deg it corresponds to
		max_deg = max(max_deg, cur_bucket.deg)
		while cur_bucket is not None:  # need a wrapper because new vertices could be moved to cur_bucket
			nextLayerTracker = []

			parallel for i, v in enumerate(cur_bucket):
				indices[i] = deg(v)
      indices = parallel prefix_sum(indices)

			parallel for i,v in enumerate(cur_bucket):
				parallel for j,u in enumerate(E[v]) if u is not removed:	
          nextLayerTracker[indices[i]+j] = u
				set v as removed
      
      nextLayerTracker = parallel filter(nextLayerTracker,removed)

      freqs,nextLayerTracker = histogram(nextLayerTracker)

      indices,nextLayerTracker = aggregate(nextLayerTracker,deg)
      parallel for i in indices:
        deg_u = deg(nextLayerTracker[indices[i]])
        degbuckets[deg_u].removeAll(nextLayerTracker[indices[i] : indices[i+1]-1])
      
			parallel for i,freq,u in enumerate(freqs,nextLayerTracker):
				deg(u)-=freq
				if deg(u)<=cur_bucket.deg:
          filterAddCur[i] = True
				else:
          filterAddNew[i] = True
      
      trackerAddCur = filter(nextLayerTracker,filterAddCur)
      trackerAddNew = filter(nextLayerTracker,filterAddNew)

      indices,trackerAddCur = aggregate(trackerAddCur,deg)
      parallel for i in indices:
        cur_bucket_new.addAll(trackerAddCur[indices[i] : indices[i+1]-1])
        
      indices,trackerAddNew = aggregate(trackerAddNew,deg)
      parallel for i in indices:
        deg_u = deg(trackerAddNew[indices[i]])
        degbuckets[deg_u].addAll(trackerAddNew[indices[i] : indices[i+1]-1])

			cur_bucket = cur_bucket_new
	return max_deg

def peelByA(U, V, E, a):
	# u correspond to a; v correspond to b
	# we need Bmax(a, u) and Amax(b, v)
	# U = set of vertices u 
	parallel for u in U:
		if deg(u)<a:
			set u as removed
			update E[u]
	bbuckets = ParallelBucketArray(V)
	while exponentialSearch(bbuckets) is not None:
		vbucket = exponentialSearch(bbuckets)
		cur_b = vbucket.deg
		while vbucket is not None:
			uTracker = []
			vTracker = []

      parallel for i,v in enumerate(vbucket):
        indices[i] = deg(v)
			indices = parallel prefix_sum(indices)
			parallel for i,v in enumerate(vbucket):
        set v as removed
				parallel for bi in range(1, cur_b+1):
					if Amax(bi,v)<a:
						Amax(bi,v)=a
				parallel for j,u in enumerate(E[v]) if not removed:
					uTracker[indices[i]+j] = u
      
      uTracker = parallel filter(uTracker,None)
      # some empty positions exist because u is already removed

			parallel for i,u,freq in enumerate(histogram(uTracker)):
				deg(u)-=freq
        if deg(u)<a:
          filterMap[i]=True
          Bmax(a,u)=cur_b
					set u as removed

      uTracker = parallel filter(uTracker,filterMap)
      parallel for i,u in enumerate(uTracker):
        indices[i] = deg(u)

      indices = parallel prefix_sum(indices)
      parallel for i,u in enumerate(uTracker):
			  parallel for j,v in enumerate(E[u]) if v is not removed:
				  vTracker[indices[i]+j] = v

			vTracker = parallel filter(vTracker,None)
      freqs,vTracker = histogram(vTracker)
      
      indices,vTrackerRemove = aggregate(vTracker,deg) 
      # do aggregation over degree; return sorted tracker and an array indicating the starts of vertices of a certain deg
      parallel for i in range(indices):
        deg_v = deg(vTrackerRemove[indices[i]])
        bbuckets[deg_v].removeAll(vTrackerRemove[indices[i] : indices[i+1]-1])

      parallel for i,freq,v in enumerate(freq,vTracker):
        deg(v)-=freq
        if deg(v)<=cur_b:
          filterAddCur[i]=True
        else:
          filterAddNew[i]=True
      
      vTrackerAddNew = parallel filter(vTracker,filterAddNew)
      vTrackerAddCur = parallel filter(vTracker,filterAddCur)

      indices,vTrackerAddNew = aggregate(vTrackerAddNew,deg)
      parallel for i in range(indices):
        deg_v = deg(vTrackerAddNew[indices[i]])
        bbuckets[deg(v)].addAll(vTrackerAddNew[indices[i] : indices[i+1]-1])
      
      indices,vTrackerAddCur = aggregate(vTrackerAddCur,deg)
      parallel for i in range(indices):
        vbucket_new.addAll(vTrackerAddCur[indices[i] : indices[i+1]-1])
				
			vbucket = vbucket_new
						
def peelByB(U, V, E, b):
	reverse peelByA

def checkInterval(arr):
  # this checks in O(1) span whether the interval contains a nonempty bucket
  hasNext = False
  parallel for bucket in arr:
    if bucket is not None:
      compare_and_swap(hasNext,True)
  return hasNext

def exponentialSearch (curPos, degbuckets, max_deg):
  n = 1
  while n <= max_deg:
    # we check the interval [curPos+2^(i-1)+1,curPos+2^i]
    start = curPos+n//2+1
    end = curPos+n
    if checkInterval(degbuckets[start : end+1]):
      return parallel reduce_min(degbuckets[start : end+1]) 
    n *= 2
