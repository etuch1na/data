#include <iostream>
#include <queue>
using namespace std;
// решение через удельный вес  -- наше всё
class RS{
public:
    double price, weight;
    RS& operator=(const  RS& right){
        this->price = right.price;
        this->weight = right.weight;
        return *this;
    }
    RS(double price, double weight) {this->price = price;this->weight = weight;}
};
auto COMPARE = [](RS& l, RS& r){return l.price / l.weight < r.price / r.weight;};
int main(){
    double weight, price, K;
    priority_queue<RS, vector<RS>, decltype(COMPARE)> knapsack{ COMPARE };
    int n;
    cin >> n >> K;
    for (int j = 0; j < n; j++) {cin >> price >> weight; knapsack.push(RS(price, weight));}
    double wt = 0, pr = 0;
    for (int j = 0; j < n; j++) {
        RS d = knapsack.top();
        if (wt + d.weight <= K) { wt = wt + d.weight; pr = pr + d.price;}
        knapsack.pop();
    }
    cout<<"ans:"<<pr<<endl;
}

