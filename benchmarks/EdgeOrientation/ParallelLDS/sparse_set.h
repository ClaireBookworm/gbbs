// This code is part of the Problem Based Benchmark Suite (PBBS)
// Copyright (c) 2010-2016 Guy Blelloch and the PBBS team
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights (to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#pragma once

#include <tuple>
#include <cassert>

#include "gbbs/bridge.h"

namespace gbbs {

template <class K>
class sparse_set {
 public:

  static constexpr K kEmptyKey = std::numeric_limits<K>::max();
  static constexpr K kTombstone = std::numeric_limits<K>::max() - 1;
  static constexpr double kSpaceMult = 1.25;

  uintE mask;  // table.size() - 1
  uintE elms_in_table;
  parlay::sequence<K> table;  // table.size() is a power of two

  size_t size() const {
    return table.size();
  }

  size_t num_elms() const {
    return elms_in_table;
  }

  static void clearA(K* A, size_t n, K k) {
    parallel_for(0, n, [&] (size_t i) { A[i] = k; });
  }

  inline size_t hashToRange(size_t h) const { return h & mask; }
  inline size_t firstIndex(K& k) const { return hashToRange(parlay::hash32(k)); }
  inline size_t incrementIndex(size_t h) const { return hashToRange(h + 1); }

  // Updates elms_in_table by the number of incoming elements (guaranteed not to be
  // in the table).
  void resize(size_t incoming) {
    size_t total = elms_in_table + incoming;
    // std::cout << "total = " << total << " n_elms = " << elms_in_table << " incoming = " << incoming << std::endl;
    if (total * kSpaceMult >= table.size()) {
      size_t new_size = (1 << parlay::log2_up((size_t)(kSpaceMult * total) + 1));
      // std::cout << "new_size = " << new_size << std::endl;
      auto new_table = parlay::sequence<K>(new_size, kEmptyKey);
      auto old_table = std::move(table);
      table = std::move(new_table);
      mask = table.size() - 1;
      // std::cout << "old_table_size = " << old_table.size() << std::endl;
      parallel_for(0, old_table.size(), [&] (size_t i) {
        if (old_table[i] != kEmptyKey) {
          insert(old_table[i]);
        }
      });
    }
    elms_in_table += incoming;
  }

  void resize_down(size_t removed) {
    size_t total = elms_in_table - removed;
    if (total * kSpaceMult <= table.size() / 2) {
      size_t new_size = (1 << parlay::log2_up((size_t)(kSpaceMult * total)));
      auto new_table = parlay::sequence<K>(new_size, kEmptyKey);
      auto old_table = std::move(table);
      table = std::move(new_table);
      mask = table.size() - 1;
      parallel_for(0, old_table.size(), [&] (size_t i) {
        if (old_table[i] != kEmptyKey) {
          insert(old_table[i]);
        }
      });
    }
    elms_in_table -= removed;
  }

  sparse_set() : mask(0), elms_in_table(0) {}

  // Size is the maximum number of values the hash table will hold.
  // Overfilling the table could put it into an infinite loop.
  sparse_set(size_t _m, long inp_space_mult=-1) {
    double space_mult = 1.1;
    if (inp_space_mult != -1) space_mult = inp_space_mult;
    auto m = (size_t)1 << parlay::log2_up((size_t)(space_mult * _m) + 1);
    mask = m - 1;
    table = parlay::sequence<K>(m, kEmptyKey);
  }

  // Pre-condition: k must be present in T.
  inline size_t idx(K k) const {
    size_t h = firstIndex(k);
    while (true) {
      if (table[h] == k) {
        return h;
      }
      h = incrementIndex(h);
    }
  }

  // Can be called concurrently. Must ensure that the caller has called resize
  // before concurrent insertions to update num_elms appropriately.
  bool insert(K k) {
    size_t h = firstIndex(k);
    while (true) {
      auto read = table[h];
      if (read == kEmptyKey || read == kTombstone) {
        if (pbbslib::CAS(&table[h], read, k)) {
          return true;
        }
      }
      if (table[h] == k) {
        return false;
      }
      h = incrementIndex(h);
    }
    return false;
  }

  bool remove(K k) {
    size_t h = firstIndex(k);
    while (true) {
      auto read = table[h];
      if (table[h] == kEmptyKey) {
        return false;
      }
      if (table[h] == k) {
        bool succ = pbbslib::CAS(&table[h], k, kTombstone);
        return succ;
      }
      h = incrementIndex(h);
    }
    return false;
  }

  // Requires that no element in s is currently in the structure (s has been
  // prefiltered using contains()).
  template <class Seq>
  void append(Seq& s) {
    resize(s.size());  // resize updates num_elms.
    parallel_for(0, s.size(), [&] (size_t i) {
      insert(s[i]);
    });
  }

  bool contains(K k) const {
    if (table.size() > 0) {
      size_t h = firstIndex(k);
      while (true) {
        if (table[h] == k) {
          return true;
        } else if (table[h] == kEmptyKey) {
          return false;
        }
        h = incrementIndex(h);
      }
    }
    return false;
  }

  sequence<K> entries() const {
    auto pred = [&](const K& k) { return k != kEmptyKey; };
    return parlay::filter(parlay::make_slice(table), pred);
  }

  void clear() {
    parallel_for(0, table.size(), [&] (size_t i) { table[i] = kEmptyKey; });
  }
};

}  // namespace gbbs