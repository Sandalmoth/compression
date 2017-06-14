#include <iostream>
#include <fstream>

#include "transforms.h"


using namespace std;


int main() {
  vector<char> s {'b', 'i', 'c', 'y', 'c', 'l', 'e'};
  char s2[] = "The quick brown fox jumps over the lazy dog. One fish, two fish, red fish, nuke fish.";
  for (auto c: s) cout << c;
  cout << endl;
  cout << transforms::bwt(s.begin(), s.end()) - s.begin() << endl;
  for (auto c: s) cout << c;
  cout << endl;
  cout << s2 << endl;
  cout << transforms::bwt(s2, s2+sizeof(s2)-1) - s2 << endl;
  cout << s2 << endl;
}
