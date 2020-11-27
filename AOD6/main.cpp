#include <iostream>
#include <vector>
#include <list>
#include <map>

using namespace std;
//создание главного узла и основных переменных дерева
class Node
{
public:
    int count;
    char symbol;
    Node *left;
    Node *right;

    Node() { }

    Node(char __symbol, int __count)
    {
        symbol = __symbol;
        count = __count;
    }

    Node(Node *l, Node *r) // создание родителя для послед. построения дерева(главный узел)
    {
        symbol = 0;
        left = l;
        right = r;
        count = l->count + r->count;
    }

    static void Print(Node *root, int depth = 0) // передача значения от родителя(главного узла)
    {
        if (!root) return;

        if (root->symbol) //вывод детей
        {
            for (int i = 0; i < depth; i++)
                cout << ".";
            cout << root->symbol << endl;
        }
        else depth++; //если проверку не прошло то идет вторая проверка
        Print(root->left, depth);
        Print(root->right, depth);
    }
};

void BuildTable(Node *root, vector<bool> &code, map<char, vector<bool>> &table) // сдвиг влево и вправо
{
    if (root->left)
    {
        code.push_back(0); // сдвиг влево узла (элемента)
        BuildTable(root->left, code, table);
    }

    if (root->right)
    {
        code.push_back(1); // сдвиг вправо узла (элемента)
        BuildTable(root->right, code, table);
    }

    if (root->symbol) // передача остаточных данных
        table[root->symbol] = code;
    if (code.size())
        code.pop_back();
}

bool SortNode(const Node *a, const Node *b) //передача новых детей в дерево
{
    return a->count < b->count;
}

string Decode(string &str, map<vector<bool>, char> &table) // перевод узла в случае сдвига
{
    string out = "";
    vector<bool> code;
    for (int i = 0; i < str.length(); i++)
    {
        code.push_back(str[i] == '0' ? false : true);
        if (table[code])
        {
            out += table[code];
            code.clear();
        }
    }
    return out;
}

int main() //ввод данных
{
    string raw = "SSSPPORTTTT";
    map<char, int> symbols;

    for (int i = 0; i < raw.length(); i++) // считываем длину элемента
        symbols[raw[i]]++;

    list<Node*> trees;
    map<char, int>::iterator itr;
    for (itr = symbols.begin(); itr != symbols.end(); itr++) // начало и конец строки
    {
        Node *p = new Node(itr->first, itr->second); // ключ;  само значение
        trees.push_back(p);
    }

    while (trees.size() != 1) // передача элементов в методы
    {
        trees.sort(SortNode);

        Node *l = trees.front();
        trees.pop_front();
        Node *r = trees.front();
        trees.pop_front();

        Node *parent = new Node(l, r);
        trees.push_back(parent);
    }

    Node *root = trees.front();
    root->Print(root);

    vector<bool> code; // буффер
    map<char, vector<bool> > table;
    BuildTable(root, code, table); // генерация ключа

    // вывод
    // вывод ключа
    for (itr = symbols.begin(); itr != symbols.end(); itr++)
    {
        cout << itr->first << " - ";
        for (int j = 0; j < table[itr->first].size(); j++)
            cout << table[itr->first][j];
        cout << endl;
    }

    string out = "";
    // вывод закодированного значения
    for (int i = 0; i < raw.length(); i++)
        for (int j = 0; j < table[raw[i]].size(); j++)
        {
            out += table[raw[i]][j] + '0';
            cout << table[raw[i]][j];
        }
    cout << endl;
    cout << out.c_str() << endl;


    // вывод декодирования
    map<vector<bool>, char> ftable;
    for (auto i = table.begin(); i != table.end(); i++)
        ftable[i->second] = i->first;
    cout << Decode(out, ftable).c_str() << endl;


    while (true);
}