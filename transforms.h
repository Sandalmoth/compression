#ifndef __TRANSFORMS_H__
#define __TRANSFORMS_H__

#include <vector>
#include <map>
#include <tuple>
#include <algorithm>

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
    typedef std::tuple<value_type, Titer, bool> movement;

  }


} // end namespace transforms


#endif
