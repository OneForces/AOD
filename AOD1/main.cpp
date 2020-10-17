#include "iostream"

using namespace std;

const int n = 10;

int rankNum(int number, int razr) // функция для поиска разряда числа
{
    while(razr > 1)
    {
        number /= 10;
        razr--;
    }
    return number % 10;
}

void sort_razr(int dop_mas[n][n], int mas[n], int razr) // функция получения элементов
{
    int mas_col[n] = {0}; // двумерный массив для очередей 10x10
    int temp = 0;

    for(int i = 0; i < n; i++) {
        int a = rankNum(mas[i], razr); // получение количества разрядов
        dop_mas[mas_col[a]][a] = mas[i];
        mas_col[a]++;
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < mas_col[i]; j++) {
            mas[temp] = dop_mas[j][i];
            temp++;
        }
    }
}

int main()
{

    int razr;
    int mas[n] = {623, 543, 120, 935, 423, 610, 732, 126, 784, 648}; // массив
    int dop_mas[n][n]; // двумерный для очередей
    for(razr = 1; razr < 10; razr++) // отправление массива в функцию для сортировки
        sort_razr(dop_mas, mas, razr);
    for(int i = 0; i < n; i++)
        cout << mas[i] << endl; //вывод отсортированного массива
    return 0;
}

