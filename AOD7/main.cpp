#include <iostream>
#include <string>
using namespace std;
void mult_order(int *arr, int len){ // передача параметров
    int matrix_max[len][len]; //макс матрица
    int matrix_min[len][len]; //мин матрица
    for (int i = 0; i < len; i++){ // заполнение
        for (int j = 0; j < len; j++){
            matrix_min[i][j] = 0;
            matrix_max[i][j] = 0;
        }
    }
    for (int i = 1; i < len; i++){ // присваивание нового значения
        for (int j = 0; j < len - i; j++){
            int k = i + j;
            matrix_min[j][k] = INT_FAST32_MAX;
            matrix_max[j][k] = INT_FAST32_MIN;
            for (int e = j; e < k; e++){ // построение новой матрицы
                int x = matrix_min[j][e] + matrix_min[e + 1][k] + arr[k + 1]
                                                                  * arr[e + 1] * arr[j];
                if (x < matrix_min[j][k]) { // легативное сравнение списков матрицы
                    matrix_min[j][k] = x;
                    matrix_min[k][j] = e;
                }
                if(x > matrix_max[j][k]){ // то же самое только со знаком >
                    matrix_max[j][k] = x;
                    matrix_max[k][j] = e;

                }
            }
        }
    }
    cout << "Min matrix: " << matrix_min[0][len-1] << endl; // вывод мин матрицы
    cout << "Max matrix: " << matrix_max[0][len-1]; // вывод макс матрицы
}

// Разработка действий над матрицей
int main() {
    int test_array[] = {5, 10, 3, 12, 5, 50, 6}; // заполненная матрица
    int len = sizeof(test_array) / sizeof(test_array[0]); // длина образа матрицы
    mult_order(test_array, len - 1); // вызов функции
}