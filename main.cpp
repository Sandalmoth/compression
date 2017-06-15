#include <iostream>
#include <fstream>

#include "transforms.h"


using namespace std;


int main() {
  vector<char> s {'b', 'i', 'c', 'y', 'c', 'l', 'e'};
  // char s2[] = "The quick brown fox jumps over the lazy dog. One fish, two fish, red fish, nuke fish.";
  char s2[] = "onethree";
  for (auto c: s) cout << c;
  cout << endl;
  int z = transforms::bwt(s.begin(), s.end()) - s.begin();
  cout << z << endl;
  for (auto c: s) cout << c;
  cout << endl;
  transforms::ibwt(s.begin(), s.end(), s.begin() + z);
  for (auto c: s) cout << c;
  cout << endl;
  cout << s2 << endl;
  int z2 = transforms::bwt(s2, s2+sizeof(s2)-1) - s2;
  cout << z2 << endl;
  cout << s2 << endl;
  transforms::ibwt(s2, s2+sizeof(s2)-1, s2+z2);
  cout << s2 << endl;
}
