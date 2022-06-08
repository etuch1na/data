#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;
class P{
public:
    long double x, y;
    P& operator=(const  P& right)     {  this->x = right.x; this->y = right.y; return *this;}
    P(long double x, long double y)   {  this->x = x;       this->y = y; };
};
bool operator<(const P& l, const P& r) { return l.x < r.x;}
long double dist(P a, P b)             { return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));}

/// нас интересуют положения точки на пути, а не ребра
class Path{
public:
  vector<P> path;
  int size;
  Path() { size = 0; }
  long double length(){
    long double l = 0,i=0;
    for(i=0; i<size;i++) {
      try{ if (i == size - 1) { throw i;}
        l += dist(path[i], path[i + 1]);
      }
      catch (int i) {l += dist(path[i], path[0]);}
    }
    return l;
  }
void addPoint(P& A) {
    if (size < 2)  path.push_back(A);
    else {
      path.push_back(path[0]);
      auto it = path.begin(), jt = it + 1;
      long double minimalDist = dist(*it, A) + dist(A, *(it + 1));
      it++;
      for (int i = 0; i < size - 1; i++) {
        long double possibleDist = dist(*it, A) + dist(A, *(it + 1));
        if (possibleDist < minimalDist) {minimalDist = possibleDist;jt = it + 1;}
        it++;
      }
      path.emplace(jt, A); path.pop_back();
    }size++;
  }
};
int main(){
  int n, i;
  long double x, y, minimalDist, possibleDist;
  Path path; cin >> n;
  for (i=0; i < n; i++) { cin >> x >> y; P A(x, y); path.addPoint(A); cout<<i<<endl;}
  bool flag = false;
  for (int i = 0; i < n - 2; i++)
    for(int j = i + 2; j < n; j++){
        try{
          if (j == n - 1) {throw j;}
	      minimalDist  = dist(path.path[i], path.path[i + 1]) + dist(path.path[j], path.path[j + 1]);
	      possibleDist = dist(path.path[i], path.path[j]) + dist(path.path[i + 1], path.path[j + 1]);
        }
        catch (int j) {
	      minimalDist = dist(path.path[i], path.path[i + 1]) + dist(path.path[j], path.path[0]);
	      possibleDist = dist(path.path[i], path.path[j]) + dist(path.path[i + 1], path.path[0]);
        }

      if (minimalDist > possibleDist) {
	      auto it1 = next(path.path.begin(), i + 1);
	      auto it2 = next(it1, j - i); ;
	      reverse(it1, it2);
	      flag = true;
      }
      if (flag == true){ flag = false; j = i + 2;}
    }
  cout << path.length();
}
