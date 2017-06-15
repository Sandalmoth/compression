#ifndef __TRANSFORMS_H__
#define __TRANSFORMS_H__

#include <vector>
#include <map>
#include <tuple>
#include <algorithm>
#include <list>

#include <iostream>

namespace transforms {

  template <typename Titer>
  Titer bwt(Titer first, Titer last) {
    typedef typename std::remove_reference<decltype(*std::declval<Titer>())>::type value_type;
    typedef std::pair<value_type, bool> value_end;

    std::vector<value_end> v;
    for (auto it = first; it != last; ++it) {
      v.emplace_back(std::make_pair(*it, false));
    }
    v.back().second = true;

    std::vector<std::vector<value_end> > rotations;
    for (size_t i = 0; i < v.size(); ++i) {
      std::rotate(v.begin(), ++v.begin(), v.end());
      rotations.emplace_back(v);
    }

    std::sort(rotations.begin(), rotations.end()
              , [](auto &a, auto &b) {
                return a < b;
              });

    std::vector<value_end> t;
    for (auto r: rotations) {
      t.emplace_back(r.back());
    }

    auto last_character = first;
    for (auto it = t.begin(); it != t.end(); ++it) {
      *first = it->first;
      if (it->second)
        last_character = first;
      ++first;
    }

    return last_character;
  }


  // Inverse Burrows-Wheeler transform
  template <typename Titer>
  void ibwt(Titer first, Titer last, Titer end) {
    typedef typename std::remove_reference<decltype(*std::declval<Titer>())>::type value_type;
    typedef std::pair<value_type, bool> value_end;
    std::vector<std::list<value_end>> matrix;

    // Build last column of matrix with end char marker
    for (auto it = first; it != last; ++it) {
      matrix.push_back(std::list<value_end>{std::make_pair(*it, (it == end))});
    }

    // complete matrix
    auto it_c = first;
    ++it_c;
    for (; it_c != last; ++it_c) {
      std::stable_sort(matrix.begin(), matrix.end()
                , [](auto &a, auto &b) {
                  return a < b;
                });

      auto mit = matrix.begin();
      auto rit = first;
      for (; rit != last; ++rit, ++mit) {
        mit->push_front(std::make_pair(*rit, (rit == end)));
      }
    }

    // copy row with ending marker
    for (auto m: matrix) {
      if (m.back().second) {
        for(auto x: m) {
          *first = x.first;
          ++first;
        }
      }
    }
  }


} // end namespace transforms


#endif
