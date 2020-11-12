#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;


/*
Объявляем структуру Ребро, которой будем пользоваться для построения графа и дерева
*/
struct edge{
    int x, y; //вершины
    edge(){}
    edge(int a, int b){
        x = a;
        y = b;
    }
};

int main() {
    int n, m;
    cin >> n >> m;

    vector <edge> graph (m); //ребра исходного графа
    vector <edge> tree; //ребра дерева
    vector <int> variety (n); //множество вершин

    /*
    variety[n] = m - вершина n принадлежит множеству m.
    Следующий цикл определяет для каждой вершины свое множество
    */
    for (int i = 0; i < n; i++){
        variety[i] = i;
    }

    /*
    Заполняем ребра исходного графа значениями из стандартного ввода
    */
    for (int i = 0; i < m; i++){
        int a, b;
        cin >> a >> b;
        graph[i].x = a;
        graph[i].y = b;
    }

    /*
    Проверяем вершины каждого ребра.
    Если вершины не принадлежат одному и тому же множеству,
    но такое ребро добавляем в наше дерево, а вершины помещаем в одно множество
    */
    for (int i = 0; i < m; i++){
        int a = graph[i].x;
        int b = graph[i].y;
        if (variety[a] != variety[b]){
            tree.push_back(graph[i]);
            int old_variety = variety[b], new_variety = variety[a];
            for (int j = 0; j < n; j++){
                if (variety[j] == old_variety){
                    variety[j] = new_variety;
                }
            }



        }
    }


    for(int i = 0; i < n - 1; i++){ //по принципу построения дерева, оно содержит n-1 ребро
        cout << tree[i].x << " " << tree[i].y;
        if (i != n-2){
            cout << endl;
        }
    }
    return 0;
}