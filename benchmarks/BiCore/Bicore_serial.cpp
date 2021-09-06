#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>
#include <vector>
#include <queue>
#include <chrono>
#include <unordered_set>
#include <fstream>
#include <assert.h>
using namespace std;
using uintE  = unsigned int;
using intE = int;
using uintT = unsigned long;
using intT = long;
using clocker = std::chrono::high_resolution_clock;

struct Graph{
  intT n, nVal;
  intT m;
  intT* degs;
  intT* offsets;
  intT* edges;
  Graph(intT n_, intT nVal_, intT m_, intT* degs_, intT* offsets_, intT* edges_)
    : n(n_), nVal(nVal_), m(m_), degs(degs_), offsets(offsets_), edges(edges_) {}
  Graph(string file_path){
    ifstream fin(file_path);
    string s; fin>>s;
    fin >> n; fin >> m;
    offsets = new intT[n];
    degs = new intT[n];
    edges = new intT[m];
    for(intT i=0; i<n; i++) fin >> offsets[i];
    for(intT i=0; i<n-1; i++) degs[i]=offsets[i+1]-offsets[i];
    degs[n-1] = m-offsets[n-1];
    for(intT i=0; i<m; i++) fin >> edges[i];
  }
  Graph(const Graph& G)
      : n(G.n),
        nVal(G.nVal),
        m(G.m)
  {
    degs = new intT[n]; offsets = new intT[n]; edges = new intT[m];
    copy(G.degs, G.degs+n, degs); copy(G.offsets, G.offsets+n, offsets);
    copy(G.edges, G.edges+m, edges); 
  }

  Graph& operator=(const Graph& G){
    clear();
    n = G.n; nVal = G.nVal; m = G.m;
    degs = new intT[n]; offsets = new intT[n]; edges = new intT[m];
    copy(G.degs, G.degs+n, degs); copy(G.offsets, G.offsets+n, offsets);
    copy(G.edges, G.edges+m, edges); 
    return *this;
  }

  Graph(Graph&& G)
      : n(G.n),
        nVal(G.nVal),
        m(G.m),
        degs(G.degs),
        offsets(G.offsets),
        edges(G.edges)
  {
    G.n = 0; G.m = 0; G.nVal = 0;
    G.degs = nullptr; G.offsets = nullptr; G.edges = nullptr;
  }

  Graph& operator=(Graph&& G){
    clear();
    n = G.n; nVal = G.nVal; m = G.m;
    degs = G.degs; offsets = G.offsets; edges = G.edges;
    G.n = 0; G.m = 0;
    G.degs = nullptr; G.offsets = nullptr; G.edges = nullptr;
    return *this;
  }

  inline void clear() {
    delete[] degs; delete[] offsets; delete[] edges;
  }

  ~Graph(){ clear(); }

  inline Graph shrink_graph(const vector<intT>& D, intT n_a, intT n_b, intT cutoffA, intT cutoffB){
    intT nValid = n;
    intT* degs_ = new intT[n]; copy(D.begin(), D.end(), degs_);
    for(intT i = 0; i<n_a; i++){
      bool cond = D[i]<cutoffA;
      degs_[i] *= (!cond);
      nValid -= cond;
    } 
    for(intT i = n_a; i<n; i++){
      bool cond = D[i]<cutoffB;
      degs_[i] *= (!cond);
      nValid -= cond;
    }
    intT* offsets_ = new intT[n+1]; offsets_[0]=0;
    for(intT i = 1; i<=n; i++) offsets_[i] = offsets_[i-1] + degs_[i-1];
    const intT m = offsets_[n];
    intT* edges_ = new intT[m];
    uintT idx = 0;
    for(intT i = 0; i<n; i++){
      intT oldDeg = degs[i], offset = offsets_[i];
      if(degs_[i] == 0){
        idx += oldDeg;
        continue;
      }
      for(intT oj = 0; oj<oldDeg; oj++, idx++){
        intT oid = edges[idx];
        edges_[offset] = oid; 
        offset+=(degs_[oid]>0);
      }
    }
    return Graph(n, nValid, m, degs_, offsets_, edges_);
  }
};

template <class E>
struct dyn_arr {
	E* A;
	intT size;
	intT capacity;
	bool alloc;

	dyn_arr(intT s) : size(0), capacity(s), alloc(true) { A = new E[s]; }

	void del() {
		// free allocated memory and set allocated to false
		if (alloc) {
			delete[] A;
			alloc = false;
		}
    }
	void clear() {
		size = 0;
	}
	inline void resize(intT new_size) {
		if (new_size + size > capacity) {
			intT new_capacity = max(2 * (new_size + size), (intT)2000); // k dyn array min bucket size = 2000
			E* nA = new E[new_capacity];
      		copy(A, A+size, nA);
			if (alloc) {
				delete[] A; // clear for new
			}
			A = nA; // new array
			capacity = new_capacity;
			alloc = true;
		}
	}
	inline void push_back(E val) {
      	A[size] = val;
      	size++;
    }
};

struct Buckets{
	vector<intT>* bkts;
	vector<intT>& degs;
	intT ahead, curPos;
	intT n,curDeg;
	Buckets(vector<intT>& degs_, intT startPos, intT endPos)
	 : degs(degs_), curPos(0)
	{
		intT maxDeg = 0;
		intT minDeg = numeric_limits<intT>::max();
		for(intT i=startPos; i<endPos; i++) {
			maxDeg = max(degs[i], maxDeg);
			if(degs[i]>0) minDeg = min(degs[i], minDeg);
		}
		bkts = new vector<intT>[maxDeg+1];
		ahead = 0;
		for(intT i=startPos; i<endPos; i++)
			if(degs[i]>0){bkts[degs[i]].push_back(i); ahead++;}
		curDeg = minDeg;
		n = maxDeg;
	}

	inline void update(intT idx, intT newDeg){ bkts[newDeg].push_back(idx); }

	inline intT next_vtx(){
		if(curPos >= bkts[curDeg].size()){
			curDeg++;
			curPos = 0;
			while(curDeg <= n && bkts[curDeg].size() == 0) curDeg++;
		}
		vector<intT>& bkt = bkts[curDeg];
		while(curPos < bkt.size() && degs[bkt[curPos]] != curDeg) curPos++;
		if(curPos < bkt.size()) {ahead--; return bkt[curPos++];}
		else return next_vtx();
	}

	inline vector<intT> next_bucket(){
		vector<intT> filterBkt; filterBkt.reserve(bkts[curDeg].size());
		while(filterBkt.size()==0){
			while(curDeg <= n && bkts[curDeg].size() == 0) curDeg++;
			vector<intT>& bkt = bkts[curDeg];
			for(intT i = bkt.size()-1; true; i--){
				if(degs[bkt[i]] == curDeg) filterBkt.push_back(bkt[i]);
				bkt.pop_back();
				if(i==0) break;
			}
		}
		ahead -= filterBkt.size();	
		return filterBkt;
	}

	inline bool empty(){
		return ahead == 0;
	}
};

inline pair<double, double> PeelFixA(Graph& G, vector<intT>& Deg, intT alpha, intT n_a, intT n_b);
inline pair<double, double> PeelFixB(Graph& G, vector<intT>& Deg, intT beta, intT n_a, intT n_b);

inline void BiCore_serial(Graph &G, intT bipartition, intT delta)
{
	cout << "starting" << endl;
	const intT n = G.n;					// # of vertices
	const intT n_a = bipartition + 1;		// number of vertices in first partition
	const intT n_b = n - bipartition - 1; // number of vertices in second partition
	double pqt = 0;
	double pt = 0;
	vector<intT> DA(n);
	for(intT i=0; i<n; i++) DA[i] = G.degs[i];
	vector<intT> DB = DA;
	cout<<"finished preprocessing"<<endl;
	for(intT core = 1; core<=delta; core++){
		cout<<"running PeelFixA core "<<core<<endl;
		auto ret = PeelFixA(G, DA, core, n_a, n_b);
		pqt += ret.first;
		pt += ret.second;
	}
	for(intT core = 1; core<=delta; core++){
		cout<<"running PeelFixB core "<<core<<endl;
		auto ret = PeelFixB(G, DB, core, n_a, n_b);
		pqt += ret.first;
		pt += ret.second;
	}
	cout<<"pq time "<<pqt<<endl;
	cout<<"pt time "<<pt<<endl;
	//mkt.reportTotal("make_graph time");
}

int main(int argc, char** argv){
	// first argument should be bipartition, second should be kcore, third should be file path
	ios_base::sync_with_stdio(0);
	assert(argc == 4);
	intT bi = stol(string(argv[1]));
	intT delta = stol(string(argv[2]));
	string file_path(argv[3]);
	cout<<"reading graph from "<<file_path<<endl;
	Graph G(file_path);
	auto st = clocker::now();
	BiCore_serial(G, bi, delta);
	auto et = clocker::now();
	cout<<"Total runtime: "<<chrono::duration_cast<chrono::nanoseconds>(et-st).count()/1000000000.0<<endl;
}

inline pair<double, double> PeelFixA(Graph& G, vector<intT>& Deg, intT alpha, intT n_a, intT n_b)
{
	const intT n = n_a + n_b;
	intT rho_alpha = 0, max_beta = 0;
	intT vtxCount = n;
	bool firstMove = true;

	vector<intT> uDel;
	for (intT i = 0; i < n_a; i++)
		if (Deg[i] == alpha-1) uDel.push_back(i);
	// (alpha,0)
	// peels all vertices in U which are < alpha, and repeatedly peels vertices in V which has deg == 0
	while (uDel.size()>0)
	{
		vector<intT> newUDel;
		for(intT const& ui : uDel){
			intT deg_ui = G.degs[ui];
			intT offset_ui = G.offsets[ui];
			for(intT i = 0; i<deg_ui; i++){
				intT vi = G.edges[i+offset_ui];
				if(Deg[vi]-- ==1){
					intT deg_vi = G.degs[vi];
					intT offset_vi = G.offsets[vi];
					for(intT j = 0; j<deg_vi; j++){
						intT uii = G.edges[j+offset_vi]; 
						if(Deg[uii]-- == alpha) newUDel.push_back(uii);
					}
				}
			}
		}
		uDel = move(newUDel);
	}
	vector<intT> D = Deg;
	// for(intT i=0; i<n_a; i++) if(D[i]<alpha) vtxCount--;
	// for(intT i=n_a; i<n; i++) if(D[i]<1) vtxCount--;
	// Graph new_G;
	// if(vtxCount*1.1 < G.nValid){
	// 	firstMove=false;
	// 	new_G = shrink_graph(st,G, D, n_a, n_b, alpha, 1);
	// 	G = new_G;
	// 	cout<<"compact at start "<<vtxCount<<endl;
	// } else 
	// 	new_G = move(G);

	Buckets bbuckets(D, n_a, n);
	intT iter = 0;
	vector<intT> tracker(n, 0);
	dyn_arr<intT> changeVtx(16);
	while (!bbuckets.empty())
	{
		iter++;
		vector<intT> bkt = bbuckets.next_bucket();
		if(bbuckets.curDeg > max_beta){
			// if(vtxCount*3.5 < new_G.nValid && new_G.nValid*10 > n){
			// 	if(firstMove){
			// 		firstMove = false;
			// 		G = move(new_G);
			// 		new_G = shrink_graph(st,G, D, n_a, n_b, alpha, max_beta+1);
			// 	}else
			// 		new_G = shrink_graph(st,new_G, D, n_a, n_b, alpha, max_beta+1);
			// 	cout<<"compact "<<vtxCount<<" "<<new_G.nValid<<endl;
			// }
			max_beta = bbuckets.curDeg;
		}
		for(auto const& vi : bkt){
			//vtxCount--;
			intT deg_vi = G.degs[vi];
			intT offset_vi = G.offsets[vi];
			for(intT i = 0; i<deg_vi; i++){
				auto ui = G.edges[i+offset_vi];
				if(D[ui]-- == alpha){
					//vtxCount--;
					intT deg_ui = G.degs[ui];
					intT offset_ui = G.offsets[ui];
					for(intT j = 0; j<deg_ui; j++){
						auto vii = G.edges[j+offset_ui]; 
						changeVtx.resize(1);
						changeVtx.A[changeVtx.size] = vii;
						intT curIter = tracker[vii];
						bool cond = (D[vii]-- > max_beta) & (curIter!=iter);
						changeVtx.size += cond;
						tracker[vii] += (iter - curIter)*cond;
					}
				}
			}
		}
		for(intT i = 0; i<changeVtx.size; i++){
			intT vii = changeVtx.A[i];
			D[vii] = max(D[vii], max_beta);
			bbuckets.update(vii, D[vii]);
		}
		changeVtx.clear();
		rho_alpha++;
	}
	cout<<rho_alpha << " "<<max_beta<<endl;
	//if(firstMove) G = move(new_G);
	return make_pair(0, 0);
}

inline pair<double, double> PeelFixB(Graph& G, vector<intT>& Deg, intT beta, intT n_a, intT n_b)
{
	const intT n = n_a + n_b;
	intT rho_beta = 0, max_alpha = 0;
	intT vtxCount = n;
	bool firstMove = true;

	vector<intT> vDel;
	for (intT i = n_a; i < n; i++)
		if (Deg[i] == beta-1) vDel.push_back(i);

	while (vDel.size()>0)
	{
		vector<intT> newVDel;
		for(intT const& vi : vDel){
			intT deg_vi = G.degs[vi];
			intT offset_vi = G.offsets[vi];
			for(intT i = 0; i<deg_vi; i++){
				auto ui = G.edges[offset_vi+i];
				if(Deg[ui]-- ==1){
					intT deg_ui = G.degs[ui];
					intT offset_ui = G.offsets[ui];
					for(intT j = 0; j<deg_ui; j++){
						intT vii = G.edges[offset_ui+j]; 
						if(Deg[vii]-- ==beta) newVDel.push_back(vii);
					}
				}
			}
		}
		vDel = move(newVDel);
	}
	vector<intT> D = Deg;
	// for(intT i=0; i<n_a; i++) if(D[i]<1) vtxCount--;
	// for(intT i=n_a; i<n; i++) if(D[i]<beta) vtxCount--;
	// Graph new_G;
	// cout<<"initial count "<<vtxCount<<" "<<G.nValid<<endl;
	// if(vtxCount*1.1 < G.nValid){
	// 	firstMove=false;
	// 	new_G = shrink_graph(st, G, D, n_a, n_b, 1, beta);
	// 	G = new_G;
	// 	cout<<"compact at start "<<vtxCount<<endl;
	// }else 
	// 	new_G = move(G);

	Buckets abuckets(D, 0, n_a);
	intT iter = 0;
	vector<intT> tracker(n, 0);
	dyn_arr<intT> changeVtx(16);
	while (!abuckets.empty())
	{
		iter++;
		vector<intT> bkt = abuckets.next_bucket();
		if(abuckets.curDeg > max_alpha){
			// if(vtxCount*3.5 < new_G.nValid && new_G.nValid*10 > n){
			// 	if(firstMove){
			// 		firstMove = false;
			// 		G = move(new_G);
			// 		new_G = shrink_graph(st,G, D, n_a, n_b, max_alpha+1, beta);
			// 	}else
			// 		new_G = shrink_graph(st,new_G, D, n_a, n_b, max_alpha+1, beta);
			// 	cout<<"compact "<<vtxCount<<endl;
			// }
			max_alpha = abuckets.curDeg;
		}
		for(auto const& ui : bkt){
			//vtxCount--;
			intT deg_ui = G.degs[ui];
			intT offset_ui = G.offsets[ui];
			for(intT i = 0; i<deg_ui; i++){
				auto vi = G.edges[i+offset_ui];
				if(D[vi]-- ==beta){
					//vtxCount--;
					intT deg_vi = G.degs[vi];
					intT offset_vi = G.offsets[vi];
					for(intT j = 0; j<deg_vi; j++){
						auto uii = G.edges[j+offset_vi]; 
						changeVtx.resize(1);
						changeVtx.A[changeVtx.size] = uii;
						intT curIter = tracker[uii];
						bool cond = (D[uii]-- > max_alpha) & (curIter!=iter);
						changeVtx.size += cond;
						tracker[uii] += (iter - curIter)*cond;
					}
				}
			}
		}
		for(intT i = 0; i<changeVtx.size; i++){
			intT uii = changeVtx.A[i];
			D[uii] = max(D[uii], max_alpha);
			abuckets.update(uii,D[uii]);
		}
		changeVtx.clear();
		rho_beta++;
	}
	cout<<rho_beta << " "<<max_alpha<<endl;
	//if(firstMove) G = move(new_G);
	return make_pair(0, 0);
}

