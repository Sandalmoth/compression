#ifndef __TRANSFORMS_H__
#define __TRANSFORMS_H__


#include <vector>
#include <tuple>
#include <algorithm>
#include <list>


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


  // Non-dictionary building mtf-transform working with bytes.
  // Assumes dictionary that is simply [0, 1, 2, .., 255]
  // So allows encoding an arbitrary string of bytes
  void nd_mtf(unsigned char *v, int c) {
    // while it is non-dictionary building, in the sense that it doesn't construct a dict from data
    // we still need the dictionary
    std::list<unsigned char> dict;
    for (int i = 0x0; i < 0x100; ++i)
      dict.emplace_back(i);

    for (int i = 0; i < c; ++i) {
      int pos = 0;
      auto it = dict.begin();
      for (; *it != *v; ++it, ++pos)
        ;
      *v = pos;
      dict.push_front(*it);
      dict.erase(it);
      ++v;
    }
  }

  void nd_imtf(unsigned char*v, int c) {
    std::list<unsigned char> dict;
    for (int i = 0x0; i < 0x100; ++i)
      dict.emplace_back(i);

    for (int i = 0; i < c; ++i) {
      unsigned char pos = 0;
      auto it = dict.begin();
      for (; pos < *v; ++it, ++pos)
        ;
      *v = *it;
      dict.push_front(*it);
      dict.erase(it);
      ++v;
    }
  }

} // end namespace transforms


#endif
