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

template <class Graph>
inline sequence<uintE> BiCore(Graph& G, size_t num_buckets = 16, size_t bipartition = 2) {
  const size_t n = G.n;
  auto D =
      sequence<uintE>(n, [&](size_t i) { return G.get_vertex(i).out_degree(); });

  auto em = hist_table<uintE, uintE>(std::make_tuple(UINT_E_MAX, 0), (size_t)G.m / 50);
  auto b = make_vertex_buckets(n, D, increasing, num_buckets);
  timer bt;

  size_t finished = 0, rho = 0, k_max = 0;
  while (finished != n) {
    bt.start();
    auto bkt = b.next_bucket();
    bt.stop();
    auto active = vertexSubset(n, std::move(bkt.identifiers));
    uintE k = bkt.id;
    finished += active.size();
    k_max = std::max(k_max, bkt.id);

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
    rho++;
  }
  std::cout << "### rho = " << rho << " k_{max} = " << k_max << "\n";
  debug(bt.reportTotal("bucket time"););
  return D;
}


}  // namespace gbbs
