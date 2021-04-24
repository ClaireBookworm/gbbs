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
#include algorithm

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

  bool mask[n];
  std::fill_n(mask, n, false);

  parallel_for(0,bipartition+1,[&](size_t i){
    if(G.get_vertex(i).out_degree()<alpha)
      mask[i]=true;
  });

  auto D =
      sequence<uintE>(n, [&](size_t i) { return G.get_vertex(i+bipartition+1).out_degree(); });

  auto em = hist_table<uintE, uintE>(std::make_tuple(UINT_E_MAX, 0), (size_t)G.m / 50);
  auto b = make_vertex_buckets(n-bipartition-1, D, increasing, num_buckets);
  // make num_buckets buckets such that each vertex i is in D[i] bucket
  timer bt;

  size_t finished = 0, rho_alpha = 0;
  while (finished != n_b) {
    bt.start();
    auto bkt = b.next_bucket();
    bt.stop();
    auto activeV = vertexSubset(n, std::move(bkt.identifiers)); // container of vertices
    finished += activeV.size();

    auto apply_f = [&](const std::tuple<uintE, uintE>& p)
        -> const std::optional<std::tuple<uintE, uintE> > {
      uintE v = std::get<0>(p), edgesRemoved = std::get<1>(p);
      uintE deg = D[v];
      if (deg > k) {
        uintE new_deg = std::max(deg - edgesRemoved, k);
        D[v] = new_deg;
        return wrap(v, b.get_bucket(new_deg));
      } // deg==k means it's effectually deleted
      return std::nullopt;
    };

    auto cond_f = [] (const uintE& u) { return true; };
    vertexSubsetData<uintE> moved = nghCount(G, active, cond_f, apply_f, em, no_dense);
    // "moved" is a wrapper storing a sequence of tuples like (id, newBucket)
    bt.start();
    b.update_buckets(moved);
    bt.stop();
    rho_alpha++;
  }
  std::cout << "### rho_alpha = " << rho_alpha << " k_{max} = " << k_max << "\n";
  debug(bt.reportTotal("bucket time"););
  return D;
}


}  // namespace gbbs
