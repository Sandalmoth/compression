#ifndef __HUFF_H__
#define __HUFF_H__


#include <vector>
#include <map>
#include <queue>
#include <cstdlib>

#include <iostream>


// useful debug printing
std::string toBinary(int n)
{
  std::string r;
  while(n!=0) {r=(n%2==0 ?"0":"1")+r; n/=2;}
  return r;
}


template <typename T>
std::vector<T> reverse(const std::vector<T> &v) {
  std::vector<T> r(v.rbegin(), v.rend());
  return r;
}


namespace huff {


  // Consider less c-like implementation
  // but well, at least it uses nullptr :)
  struct Node {
    Node *parent = nullptr;
    Node *child0 = nullptr;
    Node *child1 = nullptr;
    double weight;
    unsigned char symbol;
  };

  class nodecomp {
  public:
    bool operator()(Node *a, Node *b) {
      return a->weight > b->weight;
      // fuck me i though priority queue picked the lowest element first.
      // so dumb.
    }
  };


  std::map<unsigned char, std::vector<bool>> encode(unsigned char *v, int c, unsigned char *&out, int &outsize) {
    // count occurrences of bytes.
    std::map<unsigned char, int> counts;
    for (int i = 0; i < c; ++i) {
      auto it = counts.find(v[i]);
      if (it != counts.end()) {
        ++it->second;
      } else {
        counts[v[i]] = 1;
      }
    }
    for (auto p: counts) {
      std::cout << p.first << '\t' << p.second << std::endl;
    }

    // Create leaf nodes and put them into a priority queue
    std::priority_queue<Node *, std::vector<Node *>, nodecomp> leaves;
    std::vector<Node> nodes; // circumvent construction/destruction messing with my pointers yo
    nodes.reserve(2*c); // also, when we add new parents later, we need extra space (thus the 2)
    // otherwise, reallocations would fuck shit up.
    for (auto x: counts) {
      Node n;
      n.weight = static_cast<double>(x.second) / static_cast<double>(c);
      n.symbol = x.first;
      nodes.push_back(n);
      leaves.push(&nodes.back());
    }

    // Now assemble a binary tree
    while (leaves.size() > 1) {
      auto n1 = leaves.top();
      leaves.pop();
      auto n2 = leaves.top();
      leaves.pop();
      Node p;
      p.weight = n1->weight + n2->weight;
      // order here is probably important
      p.child1 = n1;
      p.child0 = n2;
      nodes.push_back(p);
      leaves.push(&nodes.back());
      n1->parent = &nodes.back();
      n2->parent = &nodes.back();
    }

    for (auto &l: nodes) {
      std::cout << &l - &(*nodes.begin()) << ' ' << l.parent - &(*nodes.begin()) << ' ' << l.weight << std::endl;
    }

    // presumably, at this point there is one item in leaves, and it's the top node.
    std::cout << "There are some items in leaves: " << leaves.size() << std::endl;

    std::map<unsigned char, std::vector<bool>> huffs; // huffs: an excellent word.
    // the vector<bool> representation seems stupid, but eh
    // at least it's space efficient internally lol
    for (auto &n: nodes) {
      // this if identifies leaves that have a symbol
      // though maybe looking for the symbol makes more sense?
      // not that it really matters.
      if (n.child0 == nullptr) {
        auto pn = &n; // previous node
        auto cn = n.parent; // node we just moved into; so current node
        std::vector<bool> huff;
        while (true) {
          cn = pn->parent;
          bool y = (cn->child1 == pn); // is this the node in child 1? then true (i.e. 1) else 0;
          huff.push_back(y); // i could std::move here i suppose. I wonder if that's faster?
          // probably emplace with the expression. I'm sure the compiler works it out anyway though :P
          pn = cn;
          if (cn->parent == nullptr)
            break;
        }
        huff = reverse(huff);
        huffs[n.symbol] = huff;
      }
    }
    // print 'em
    for (auto &h: huffs) {
      std::cout << h.first << '\t';
      for (auto x: h.second) {
        std::cout << x << ' ';
      }
      std::cout << std::endl;
    }

    // calculate number of bytes needed to store code
    int n_bits = 0;
    for (auto &x: counts) {
      n_bits += x.second * huffs[x.first].size();
    }
    std::cout << n_bits << std::endl;
    int n_bytes = n_bits/8 + 1;
    std::cout << n_bytes << std::endl;

    // allocate bytes, and store it.
    outsize = n_bytes;
    int bitpos = 0; // position in byte
    unsigned char w = 0; // workbyte
    int outpos = 0;
    out = reinterpret_cast<unsigned char*>(malloc(outsize)); // who cares about checking for NULL
    for (int i = 0; i <= c; ++i) {
      // grab the huffman code;
      auto h = huffs[v[i]];
      for (auto b: h) {
        w = w << 1; // god i hope i get the endianness right
        // also, if your system has the wrong endianness, too bad >:D
        // is that even a thing that can happen?
        w = w | b;
        ++bitpos;
        // std::cout << toBinary(w) << std::endl;
        if (bitpos == 8) {
          out[outpos] = w;
          ++outpos;
          w = 0;
          bitpos = 0;
        }
      }
    }
    out[outpos] = w;
    // who the fuck knows if this works or not. we'll see tomorrow when I make the decoder.
    // my primary concern is that the huffman code making is wrong.
    // Tomorrow me: I had to reverse the huffman codes. now they seem correct.
    return huffs;
  }


  std::vector<unsigned char> decode(unsigned char *data, int length, int n_symbols, std::map<unsigned char, std::vector<bool>> huffs) {
    // we need a map from huffman codes to bytes
    std::map<std::vector<bool>, unsigned char> ihuffs;
    for (auto h: huffs) {
      ihuffs[h.second] = h.first;
    }

    std::vector<unsigned char> decomp;

    // read off input, one bit at a time, while looking for huffman codes
    std::vector<bool> wh; // working huff
    int k = 0; // how many symbols have we uncovered
    for (int i = 0; i < length; ++i) {
      unsigned char c = data[i];
      std::cout << "workchar: " << toBinary(static_cast<int>(c)) << '\t' << static_cast<int>(c) << std::endl;
      for (int j = 0; j < 8; ++j) {
        // mask off first bit
        bool bit = (0b10000000 & c);
        wh.push_back(bit);
        c = c << 1;
        // do we have a huffman code?
        auto it = ihuffs.find(wh);
        if (it != ihuffs.end()) {
          std::cout << "found: ";
          for (auto zz: wh) {
            std::cout << (zz ? '1' : '0');
          }
          std::cout << std::endl;
          decomp.push_back(it->second);
          ++k;
          if (k > n_symbols) {
            std::cout << k << '=' << n_symbols << " ALL SYMBOLS ENCOUNTERED" << std::endl;
            // thinly veiled goto incoming:
            j = 8;
            i = length;
            // hehe
          }
          wh.clear();
        }
      }
    }

    for (auto x: decomp) std::cout << x << ' ';
    std::cout << std::endl;
    return decomp; // what the hell warning level all. why doesnt' it stop me from not returning anything
    // oops. wall is not active. :/
    // even still. why does it compile without a return
  }


} // end namespace huff


#endif
