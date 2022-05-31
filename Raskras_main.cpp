#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;
//------------------------------------------------------------------------------------------------------
//   1. Покрасьте первую вершину в первый цвет.
//   2. Сделайте следующее для оставшихся вершин V-1.
//   а) Рассмотрим вершину, выбранную в данный момент, и покрасим ее в цвет с
//     наименьшим номером, который не использовался ни в одной из ранее
//     окрашенных смежных с ней вершин. Если все ранее использовавшиеся цвета
//    появляются на вершинах, смежных с v, назначьте ей новый цвет.
//------------------------------------------------------------------------------------------------------
int main() {
    int vertex, edge;
    cin>>vertex>>edge;

    vector<pair<int, int>> edges(edge);
    for (int i = 0; i <edge; i++) { cin >> edges[i].first >> edges[i].second;}
    vector<int> color(vertex, 0);
    color[0] = 0; ///хочу первый цвет 0
    for (int i = 0; i < vertex; i++){// цвет i-ой вершины.
        set<int> colors;
        //хотим все ребра рассмотреть, но нужна двухстороняя проверка на смежность
        for(int j = 0; j < edge; j++) {
            // если вершина j-ого ребра смежная i-той вершине, цвет вершины обязан быть другим для i-ой вершины
            if (edges[j].first == i)       {colors.insert(color[edges[j].second]);}
            else if (edges[j].second == i) {colors.insert(color[edges[j].first]) ;}
        }
        //// множество цветов вершин, смежных i-ой прогнали
        //// определяем максимум цветов, необходимый для раскраски графа
        for (int j = 0; j < vertex; j++) {				// с нуля цвета рассматриваем, потом +1 сделаем
            //-------------------------------------------
            /// Проверка на использование цвета, занят ли?
            auto search = colors.find(j);
            if (search != colors.end()) {}
            else { color[i] = j; break;}
        }
    }
    sort(color.begin(), color.end());
    cout <<"Result:"<< *(color.end() - 1); ///+1 потому что индексацию с нуля сделали
}
