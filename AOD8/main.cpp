//listing 1
// Search for a route.
#include <iostream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

struct FlightInfo {
    string from;
    string to;
    int distance;
    bool skip;

    FlightInfo() {
        from = "";
        to = "";
        distance = 0;
        skip = false;
    }

    FlightInfo(string f, string t, int d) {
        from = f;
        to = t;
        distance = d;
        skip = false;
    }
};

// Находим соединения, используя поиск в глубину.
class Search {
    // Этот вектор содержит информацию о полете.
    vector<FlightInfo> flights;

    // Этот стек используется для поиска с возвратом.
    stack<FlightInfo> btStack;

// Если есть рейс от и до,
    // сохраняем расстояние полета в dist.
    // Возвращаем истину, если рейс существует и,
    // иначе ложь.
    bool match(string from, string to, int &dist);

// Получено из, найти любое соединение.
    // Возвращаем истину, если соединение найдено,
    // и false в противном случае.
    bool find(string from, FlightInfo &f);

public:

    // Поместите рейсы в базу данных.
    void addflight(string from, string to, int dist) {
        flights.push_back(FlightInfo(from, to, dist));
    }

    // Показать маршрут и общее расстояние.
    void route();

    // Определите, есть ли маршрут между от и до.
    void findroute(string from, string to);

    // Вернуть true, если маршрут был найден.
    bool routefound() {
        return !btStack.empty();
    }
    //=================================================//
    void resetSkip(FlightInfo);
    //=====================================================//
};

// Показать маршрут и общее расстояние.
void Search::route()
{
    stack<FlightInfo> rev;
    int dist = 0;
    FlightInfo f;

    // Переверните стек, чтобы отобразить маршрут.
    while(!btStack.empty()) {
        f = btStack.top();
        rev.push(f);
        btStack.pop();
    }

    // Отобразите маршрут.
    while(!rev.empty()) {
        f = rev.top();
        rev.pop();
        cout << f.from << " to ";
        dist += f.distance;
    }

    cout << f.to << endl;
    cout << "Distance is " << dist << endl;
}

// Если есть рейс от и до,
// сохраняем расстояние полета в dist.
// Возвращаем истину, если рейс существует и,
// иначе ложь.
bool Search::match(string from, string to, int &dist)
{
    for(unsigned i=0; i < flights.size(); i++) {
        if(flights[i].from == from &&
           flights[i].to == to && !flights[i].skip)
        {
            flights[i].skip = true; // предотвращение
            dist = flights[i].distance;
            return true;
        }
    }

    return false; // не найдено
}

// Получено из, найти любое соединение.
// Возвращаем истину, если соединение найдено,
// и false в противном случае.
bool Search::find(string from, FlightInfo &f)
{
    for(unsigned i=0; i < flights.size(); i++) {
        if(flights[i].from == from && !flights[i].skip) {
            f = flights[i];
            flights[i].skip = true; // предотвращение

            return true;
        }
    }

    return false;
}

// Сбросьте поля пропуска в векторе полетов.
void Search::resetSkip(FlightInfo f) {
    for(unsigned i=0; i < flights.size(); i++)
        if(flights[i].from == f.from &&
           flights[i].to == f.to)
            flights[i].skip = false;
}

void Search::findroute(string from, string to)
{
    int dist;
    FlightInfo f;

    // Этот стек нужен для поиска в ширину.
    stack<FlightInfo> resetStck;

    // Посмотрите, если в пункте назначения.
    if(match(from, to, dist)) {
        btStack.push(FlightInfo(from, to, dist));
        return;
    }

// Далее следует первая часть в ширину
    // модификация. Проверяет все стыковочные рейсы
    // из указанного узла.
    while(find(from, f)) {
        resetStck.push(f);
        if(match(f.to, to, dist)) {
            resetStck.push(FlightInfo(f));
            btStack.push(FlightInfo(from, f.to, f.distance));
            btStack.push(FlightInfo(f.to, to, dist));
            return;
        }
    }

// Следующий код сбрасывает поля пропуска, установленные
    // предшествующий циклу while. Это тоже часть
    // модификация в ширину.
    while(!resetStck.empty()) {
        resetSkip(resetStck.top());
        resetStck.pop();
    }

    // Попробуйте другое подключение.
    if(find(from, f)) {
        btStack.push(FlightInfo(from, to, f.distance));
        findroute(f.to, to);
    }
    else if(!btStack.empty()) {
        // Вернитесь назад и попробуйте другое соединение.
        f = btStack.top();
        btStack.pop();
        findroute(f.from, f.to);
    }
}
//____________________________________________________________________________________//
int main() {
    char to[40], from[40];
    Search ob;

    // Добавить рейсы в базу данных.
    ob.addflight("New York", "Chicago", 900);
    ob.addflight("Chicago", "Denver", 1000);
    ob.addflight("New York", "Toronto", 500);
    ob.addflight("New York", "Denver", 1800);
    ob.addflight("Toronto", "Calgary", 1700);
    ob.addflight("Toronto", "Los Angeles", 2500);
    ob.addflight("Toronto", "Chicago", 500);
    ob.addflight("Denver", "Urbana", 1000);
    ob.addflight("Denver", "Houston", 1000);
    ob.addflight("Houston", "Los Angeles", 1500);
    ob.addflight("Denver", "Los Angeles", 1000);

    // Получение города отправления и назначения.
    cout << "From? ";

    cin.getline(from, 40);
    cout << "To? ";

    cin.getline(to, 40);

    // Посмотрите, есть ли маршрут между от и до.
    ob.findroute(from, to);

    // Если есть маршрут, покажите его.
    if(ob.routefound())
        ob.route();

    return 0;
}