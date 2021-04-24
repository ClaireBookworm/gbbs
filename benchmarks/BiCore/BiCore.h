// This code is part of the project "Theoretically Efficient Parallel Graph
// Algorithms Can Be Fast and Scalable", presented at Symposium on Parallelism
// in Algorithms and Architectures, 2018.
// Copyright (c) 2018 Laxman Dhulipala, Guy Blelloch, and Julian Shun
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all  copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include "gbbs/gbbs.h"
#include "gbbs/julienne.h"

namespace gbbs {

// bipartition gives the last vertex id in first partition
template <class Graph>
inline void BiCore(Graph& G, size_t num_buckets=16, size_t = bipartition){

}

template <class Graph>
inline void PeelFixA(Graph& G, size_t alpha, size_t num_buckets = 16, size_t bipartition = 2) {

  const size_t n = G.n;
  const size_t n_b = n-bipartition-1;
  const size_t n_a = bipartition+1;

  auto em = hist_table<uintE, uintE>(std::make_tuple(UINT_E_MAX, 0), (size_t)G.m / 50);

  mask = sequence<bool>(n, [&](size_t i){
    return G.get_vertex(i).out_degree()<alpha;
  });

  uDel = vertexSubset(n,mask)
  auto cond_f = [] (const uintE& u) { return D[u]>0; };
  auto clearZeroV = [&](const std::tuple<uintE, uintE>& p)
      -> const std::optional<std::tuple<uintE, uintE> > {
    uintE v = std::get<0>(p), edgesRemoved = std::get<1>(p);
    uintE deg = D[v];
    uintE new_deg = deg - edgesRemoved;
    D[v] = new_deg;
    if(new_deg==0)
      return wrap(v, pbbslib::empty);
    return std::nullopt;
  };

  auto clearU = [&](const std::tuple<uintE, uintE>& p)
      -> const std::optional<std::tuple<uintE, uintE> > {
    uintE u = std::get<0>(p), edgesRemoved = std::get<1>(p);
    uintE deg = D[u];
    uintE new_deg = deg - edgesRemoved;
    D[u] = new_deg;
    if(new_deg<alpha)
      return wrap(u, pbbslib::empty);
    return std::nullopt;
  };

  auto getVBuckets = [&](const std::tuple<uintE, uintE>& p)
      -> const std::optional<std::tuple<uintE, uintE> > {
    uintE v = std::get<0>(p), edgesRemoved = std::get<1>(p);
    uintE deg = D[v];
    if (deg > max_beta) {
      uintE new_deg = std::max(deg - edgesRemoved, max_beta);
      D[v] = new_deg;
      return wrap(v, b.get_bucket(new_deg));
    } // deg==k means it's effectually deleted and traversed on this round
    return std::nullopt;
  };



  // peels all vertices in U which are < alpha, and repeatedly peels vertices in V which has deg == 0
  while(!uDel.isEmpty()){
    vertexSubset vDel = nghCount(G, uDel, cond_f, clearZeroV, em, no_dense);
    uDel = nghCount(G, vDel, cond_f, clearU, em, no_dense);
  }

  auto D =
      sequence<uintE>(n_b, [&](size_t i) {
        return G.get_vertex(i+bipartition+1).out_degree();
      });

  auto b = make_vertex_buckets(n_b, D, increasing, num_buckets);
  // make num_buckets buckets such that each vertex i is in D[i] bucket
  // note this i value is not real i value; realI = i+bipartition+1
  timer bt;

  size_t finished = 0, rho_alpha = 0, max_beta = 0;
  while (finished != n_b) {
    bt.start();
    auto vbkt = b.next_bucket();
    bt.stop();
    max_beta = std::max(max_beta,vbkt.id);

    if(vbkt.id==0)
      continue;

    auto activeV = vertexSubset(n, std::move(vbkt.identifiers)); // container of vertices
    finished += activeV.size();

    vertexSubset deleteU = nghCount(G, activeV, cond_f, clearU, em, no_dense);
    // "deleteU" is a wrapper storing a sequence id of deleted vertices in U

    vertexSubset movedV = nghCount(G, deleteU, cond_f, getVBuckets, em, no_dense)
    // "movedV" is a wrapper storing a sequence of tuples like (id, newBucket)

    bt.start();
    b.update_buckets(movedV);
    bt.stop();
    rho_alpha++;
  }
  std::cout << "### rho_alpha = " << rho_alpha << " beta_{max} = " << max_beta << "\n";
  debug(bt.reportTotal("bucket time"););
  return D;
}


}  // namespace gbbs
