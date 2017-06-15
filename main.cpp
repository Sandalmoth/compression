#include <iostream>
#include <fstream>

#include "transforms.h"


using namespace std;


int main() {
  vector<char> s {'b', 'i', 'c', 'y', 'c', 'l', 'e'};
  char s2[] = "The quick brown fox jumps over the lazy dog. One fish, two fish, red fish, nuke fish.";
  int lens2 = sizeof(s2);
  double s3[] = {3.14, 33.8, 1138.007};
  int lens3 = sizeof(s3);

  for (auto c: s) cout << c;
  cout << endl;
  int z = transforms::bwt(s.begin(), s.end()) - s.begin();
  for (auto c: s) cout << c;
  cout << ' ' << z << endl;
  transforms::nd_mtf(reinterpret_cast<unsigned char *>(s.data()), s.size());
  for (auto c: s) cout << c;
  cout << ' ' << endl;
  transforms::nd_imtf(reinterpret_cast<unsigned char *>(s.data()), s.size());
  for (auto c: s) cout << c;
  cout << ' ' << endl;
  transforms::ibwt(s.begin(), s.end(), s.begin() + z);
  for (auto c: s) cout << c;
  cout << endl;

  cout << s2 << endl;
  int z2 = transforms::bwt(s2, s2+lens2-1) - s2;
  cout << s2 << ' ' << z2 << endl;
  transforms::nd_mtf(reinterpret_cast<unsigned char *>(s2), lens2 - 1);
  cout << s2 << endl;
  transforms::nd_imtf(reinterpret_cast<unsigned char *>(s2), lens2 - 1);
  cout << s2 << endl;
  transforms::ibwt(s2, s2+lens2-1, s2+z2);
  cout << s2 << endl;

  for (auto c: s3) cout << c << ' ';
  cout << endl;
  int z3 = transforms::bwt(s3, s3 + 3) - s3;
  for (auto c: s3) cout << c << ' ';
  cout << ' ' << z3 << endl;
  transforms::nd_mtf(reinterpret_cast<unsigned char *>(s3), lens3);
  for (auto c: s3) cout << c << ' ';
  cout << ' ' << endl;
  transforms::nd_imtf(reinterpret_cast<unsigned char *>(s3), lens3);
  for (auto c: s3) cout << c << ' ';
  cout << ' ' << endl;
  transforms::ibwt(s3, s3 + 3, s3 + z3);
  for (auto c: s3) cout << c << ' ';
  cout << endl;
}
