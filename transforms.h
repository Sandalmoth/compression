#ifndef __TRANSFORMS_H__
#define __TRANSFORMS_H__

#include <vector>
#include <map>
#include <tuple>
#include <algorithm>
#include <list>

#include <iostream>

namespace transforms {

  // Burrows-Wheeler transform
  template <typename Titer>
  Titer bwt(Titer first, Titer last) {

    typedef typename std::remove_reference<decltype(*std::declval<Titer>())>::type value_type;
    typedef std::tuple<value_type, value_type, bool> rotation;

    // We really only care about the first character for sorting and the last for the bwt
    // We also need to remember where the last character is
    std::vector<rotation> rotations;

    // Build all rotations
    auto fc = first;
    auto lc = last;

    --lc;
    rotations.emplace_back(rotation(*fc, *lc, true));
    fc = last;
    --fc;

    for (; fc != first; --fc) {
      --lc;
      rotations.emplace_back(rotation(*fc, *lc, false));
    }

    std::sort(rotations.begin(), rotations.end()
         , [](auto &a, auto &b) {
           return std::get<0>(a) < std::get<0>(b);
         });

    // Update the given list
    for (auto r: rotations) {
      (*first) = std::get<1>(r);
      ++first;
    }

    // Find location of ending character.
    auto it = rotations.end();
    --it;
    for (; !std::get<2>(*it); --it, --last)
      ;
    return --last;
  }


  // Inverse Burrows-Wheeler transform
  template <typename Titer>
  void ibwt(Titer first, Titer last, Titer end) {
    typedef typename std::remove_reference<decltype(*std::declval<Titer>())>::type value_type;
    typedef std::pair<bool, std::list<value_type>> full_rotation;
    std::vector<full_rotation> matrix;

    // Build last column of matrix with end char marker
    for (auto it = first; it != last; ++it) {
      matrix.push_back(std::make_pair(false, std::list<value_type>{*it}));
      if (it == end) {
        matrix.back().first = true;
      }
    }

    // complete matrix
    auto it_c = first;
    ++it_c;
    for (; it_c != last; ++it_c) {
      for (auto m: matrix) {
        for (auto x: m.second) {
          std::cout << x << ' ';
        }
        if (m.first) std::cout << '*';
        std::cout << std::endl;
      }
      std::sort(matrix.begin(), matrix.end()
                , [](auto &a, auto &b) {
                  auto ait = a.second.begin();
                  auto bit = b.second.begin();
                  for (; ait != a.second.end(); ++ait, ++bit) {
                    if (*ait < *bit) {
                      return true;
                    }
                    if ((*ait == *bit) && (*ait = --a.second.end())) {
                      if (a.first)
                        return true;
                    }
                  }
                  return false;
                });
      auto mit = matrix.begin();
      auto rit = first;
      for (; rit != last; ++rit, ++mit) {
        mit->second.push_front(*rit);
      }
    }

    // copy row with ending marker
    for (auto m: matrix) {
      if (m.first) {
        for(auto x: m.second) {
          *first = x;
          ++first;
        }
      }
    }
  }


} // end namespace transforms


#endif
