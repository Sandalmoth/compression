#include <iostream>
#include <fstream>

#include "huff.h"
#include "transforms.h"


using namespace std;


int main() {
  vector<char> s {'b', 'i', 'c', 'y', 'c', 'l', 'e'};
  char s2[] = "The quick brown fox jumps over the lazy dog. One fish, two fish, red fish, nuke fish.";
  // char s2[] = "a dead dad ceded a bad babe a beaded abaca bed";
  int lens2 = sizeof(s2);
  double s3[] = {3.14, 33.8, 1138.007};
  int lens3 = sizeof(s3);

  // for (auto c: s) cout << c;
  // cout << endl;
  // int z = transforms::bwt(s.begin(), s.end()) - s.begin();
  // for (auto c: s) cout << c;
  // cout << ' ' << z << endl;
  // transforms::nd_mtf(reinterpret_cast<unsigned char *>(s.data()), s.size());
  // for (auto c: s) cout << c;
  // cout << ' ' << endl;
  // unsigned char *comp;
  // int os;
  // auto hmap = huff::encode(reinterpret_cast<unsigned char *>(s.data()), s.size(), comp, os);
  // auto sdecomp = huff::decode(comp, s.size(), os, hmap);
  // cout << "decompressed ";
  // for (auto c: sdecomp) cout << c;
  // cout << ' ' << endl;
  // transforms::nd_imtf(reinterpret_cast<unsigned char *>(s.data()), s.size());
  // for (auto c: s) cout << c;
  // cout << ' ' << endl;
  // transforms::ibwt(s.begin(), s.end(), s.begin() + z);
  // for (auto c: s) cout << c;
  cout << endl;

  cout << s2 << endl;
  int z2 = transforms::bwt(s2, s2+lens2-1) - s2;
  cout << s2 << ' ' << z2 << endl;
  transforms::nd_mtf(reinterpret_cast<unsigned char *>(s2), lens2 - 1);
  cout << s2 << endl;
  for (auto x: s2) cout << static_cast<int>(x) << ' ';
  cout << lens2 << endl;
  unsigned char *comp2;
  int os2;
  auto hmap2 = huff::encode(reinterpret_cast<unsigned char *>(s2), lens2 - 1, comp2, os2);
  cout << "  compressed: ";
  for (int i = 0; i < os2; ++i) cout << comp2[i];
  cout << " -- " << os2 << " bytes" << endl;
  cout << "  compressed: ";
  for (int i = 0; i < os2; ++i) cout << static_cast<int>(comp2[i]) << ' ';
  cout << " -- " << os2 << " bytes" << endl;
  auto sd2 = huff::decode(comp2, os2, lens2-1, hmap2);
  cout << "decompressed: ";
  for (auto x: sd2) cout << x;
  cout << sd2.size() << " bytes" << endl;
  cout << "decompressed: ";
  for (auto x: sd2) cout << static_cast<int>(x) << ' ';
  cout << sd2.size() << " bytes" << endl;
  // free(comp2);
  sd2.push_back('\0'); // make it null terminated (like the original)
  for (int i = 0; i < lens2 - 1; ++i) {
    cout << static_cast<int>(s2[i]) << ' ' << static_cast<int>(sd2[i]) << endl;
    s2[i] = *((char *)((void *)&sd2[i]));
  }
  transforms::nd_imtf(reinterpret_cast<unsigned char *>(s2), lens2 - 1);
  cout << s2 << endl;
  transforms::ibwt(s2, s2+lens2-1, s2+z2);
  cout << s2 << endl;

//   for (auto c: s3) cout << c << ' ';
//   cout << endl;
//   int z3 = transforms::bwt(s3, s3 + 3) - s3;
//   for (auto c: s3) cout << c << ' ';
//   cout << ' ' << z3 << endl;
//   transforms::nd_mtf(reinterpret_cast<unsigned char *>(s3), lens3);
//   for (auto c: s3) cout << c << ' ';
//   cout << ' ' << endl;
//   unsigned char *comp3;
//   int os3;
//   huff::encode(reinterpret_cast<unsigned char *>(s3), lens3, comp3, os3);
//   transforms::nd_imtf(reinterpret_cast<unsigned char *>(s3), lens3);
//   for (auto c: s3) cout << c << ' ';
//   cout << ' ' << endl;
//   transforms::ibwt(s3, s3 + 3, s3 + z3);
//   for (auto c: s3) cout << c << ' ';
//   cout << endl;
}
