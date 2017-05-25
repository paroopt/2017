#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

int numOfUnique1(vector<int> const & v)
{
  auto s = set<int>(begin(v), end(v));
  return s.size();
}

int numOfUnique2(vector<int> v)
{
  sort(begin(v), end(v));
  v.erase(unique(begin(v), end(v)), end(v));
  return v.size();
}

int main()
{
  vector<int> v{1,2, 3, 2, 1};
  auto r1 = numOfUnique1(v);
  auto r2 = numOfUnique2(v);

  std::cout << r1 << " " << r2 << "\n";
} 
