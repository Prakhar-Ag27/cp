#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

bool isPossible(vector<int> &v, int k, int ele) {
  int count = 0;
  for (int i = 1; i <= v.size() - k; i++) {
    if (v[i] == ele && v[i] != v[i + k])
      count++;
  }
  return count;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  std::cout << "TESTING!!" << "\n";
  return 0;
}
