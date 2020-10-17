#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<memory.h>

using namespace std;

// класс дерева
class BlAndReTree {
    int sum;
    bool usl;
    int SumLists;
    struct NodeTree { NodeTree* ch1, * ch2; int value; bool red; }; // структура узла ch1,ch2 - приемники
    NodeTree* tree_root;					// главный корень дерева
    int nodes_count;					// число узлов дерева
private:
    void BalanceAddNode(NodeTree**);		// балансировка новового узла
    bool DelBalanceLeft(NodeTree**);		// левая балансировка удаления
    bool DelBalanceRight(NodeTree**);		// правая балансировка удаления
    NodeTree* NewNode(int value);		// Объявление нового корня
    void DelNode(NodeTree*);				// удаление корня
    void Clear(NodeTree*);				// удаление дерева (рекурсивная часть)
    bool AddNode(int, NodeTree**);			// добавление узла(рекурсивная часть)
    bool GetMineObject(NodeTree**, NodeTree**);	// найти и убрать максимальный узел части дерева
    NodeTree* RotLeft(NodeTree*);		// вращение влево
    NodeTree* RotRight(NodeTree*);		// вращение вправо
    bool RemoveObject(NodeTree**, int);// удаление в рекурсии в привате
    void Returne(NodeTree* node);// Рассчет среднего арифметического всех узлов в дереве
public: // отладочная часть
    int Returne();
    int ReturnSum();
    enum check_code { error_balance, error_struct, ok };	// код ошибки
    void Show();						// вывод дерева
    check_code Check();					// проверка дерева
    //bool TreeWalk(bool*, int);			// обход дерева и сверка значений с массивом
private:
    void Show(NodeTree*, int, char);		// вывод дерева, рекурсивная часть
    check_code Check(NodeTree*, int, int&);// проверка дерева (рекурсивная часть)
    //bool TreeWalk(NodeTree*, bool*, int);	// обход дерева и сверка значений с массивом (рекурсивная часть)
public:
    BlAndReTree();   //конструктор
    ~BlAndReTree(); // декструктор вызывает удаление дерева
    void AddNode(int);		// добавить узел
    void RemoveObject(int);		// удалить узел
    void Clear();			// удалить дерево
    void FindObject(int);			// найти значение узла
    int GetCount();	// узнать число узлов
};


BlAndReTree::BlAndReTree()
{
    tree_root = 0;
    nodes_count = 0;
    sum = 0;
    SumLists = 0;
    usl = false;
}

BlAndReTree::~BlAndReTree()
{
    Clear(tree_root);
}
void BlAndReTree::Show(NodeTree* node, int depth, char dir)
{
    int n;
    if (!node) return;
    for (n = 0; n < depth; n++) putchar(' ');
    printf("%c[%d] (%s)\n", dir, node->value, node->red ? "red" : "black");
    Show(node->ch1, depth + 2, '-');
    Show(node->ch2, depth + 2, '+');
}

BlAndReTree::NodeTree* BlAndReTree::RotRight(NodeTree* node)
{
    NodeTree* ch1 = node->ch1;
    NodeTree* ch12 = ch1->ch2;
    ch1->ch2 = node;
    node->ch1 = ch12;
    return ch1;
}
int BlAndReTree::ReturnSum()
{
    return SumLists;
}
int BlAndReTree::GetCount()
{
    return nodes_count;
}
BlAndReTree::NodeTree* BlAndReTree::RotLeft(NodeTree* node)
{
    NodeTree* ch2 = node->ch2;
    NodeTree* ch21 = ch2->ch1;
    ch2->ch1 = node;
    node->ch2 = ch21;
    return ch2;
}
BlAndReTree::NodeTree* BlAndReTree::NewNode(int value)
{
    nodes_count++;
    NodeTree* node = new NodeTree;
    node->value = value;
    node->ch1 = node->ch2 = 0;
    node->red = true;
    return node;
}
void BlAndReTree::DelNode(NodeTree* node)
{
    nodes_count--;
    delete node;
}
void BlAndReTree::Clear(NodeTree* node)
{
    if (!node) return;
    Clear(node->ch1);
    Clear(node->ch2);
    DelNode(node);
}
void BlAndReTree::BalanceAddNode(NodeTree** root)
{
    NodeTree* ch1, * ch2, * px1, * px2;
    NodeTree* node = *root;
    if (node->red) return;
    ch1 = node->ch1;
    ch2 = node->ch2;
    if (ch1 && ch1->red) {
        px2 = ch1->ch2;	//ищем две красные рядом
        if (px2 && px2->red) ch1 = node->ch1 = RotLeft(ch1);
        px1 = ch1->ch1;
        if (px1 && px1->red) {
            node->red = true;
            ch1->red = false;
            if (ch2 && ch2->red) {	// перекрашиваем
                px1->red = true;
                ch2->red = false;
                return;
            }
            *root = RotRight(node);
            return;
        }
    }//то же самое только с другой стороны
    if (ch2 && ch2->red) {
        px1 = ch2->ch1;
        if (px1 && px1->red) ch2 = node->ch2 = RotRight(ch2);
        px2 = ch2->ch2;
        if (px2 && px2->red) {
            node->red = true;
            ch2->red = false;
            if (ch1 && ch1->red) {
                px2->red = true;
                ch1->red = false;
                return;
            }
            *root = RotLeft(node);
            return;
        }
    }
}


bool BlAndReTree::DelBalanceLeft(NodeTree** root)
{
    NodeTree* node = *root;
    NodeTree* ch1 = node->ch1;
    NodeTree* ch2 = node->ch2;
    if (ch1 && ch1->red) {
        ch1->red = false; return false;
    }
    if (ch2 && ch2->red) { // случай 1
        node->red = true;
        ch2->red = false;
        node = *root = RotLeft(node);
        if (DelBalanceLeft(&node->ch1)) node->ch1->red = false;
        return false;
    }
    unsigned int mask = 0;
    NodeTree* ch21 = ch2->ch1;
    NodeTree* ch22 = ch2->ch2;
    if (ch21 && ch21->red) mask |= 1;
    if (ch22 && ch22->red) mask |= 2;
    switch (mask)
    {
        case 0:		// случай 2 - if((!ch21 || !ch21->red) && (!ch22 || !ch22->red))
            ch2->red = true;
            return true;
        case 1:
        case 3:		// случай 3 - if(ch21 && ch21->red)
            ch2->red = true;
            ch21->red = false;
            ch2 = node->ch2 = RotRight(ch2);
            ch22 = ch2->ch2;
        case 2:		// случай 4 - if(ch22 && ch22->red)
            ch2->red = node->red;
            ch22->red = node->red = false;
            *root = RotLeft(node);
    }
    return false;
}

bool BlAndReTree::DelBalanceRight(NodeTree** root)
{
    NodeTree* node = *root;
    NodeTree* ch1 = node->ch1;
    NodeTree* ch2 = node->ch2;
    if (ch2 && ch2->red) { ch2->red = false; return false; }
    if (ch1 && ch1->red) { // случай 1
        node->red = true;
        ch1->red = false;
        node = *root = RotRight(node);
        if (DelBalanceRight(&node->ch2)) node->ch2->red = false;
        return false;
    }
    unsigned int mask = 0;
    NodeTree* ch11 = ch1->ch1;
    NodeTree* ch12 = ch1->ch2;
    if (ch11 && ch11->red) mask |= 1;
    if (ch12 && ch12->red) mask |= 2;
    switch (mask) {
        case 0:		// случай 2 - if((!ch12 || !ch12->red) && (!ch11 || !ch11->red))
            ch1->red = true;
            return true;
        case 2:
        case 3:		// случай 3 - if(ch12 && ch12->red)
            ch1->red = true;
            ch12->red = false;
            ch1 = node->ch1 = RotLeft(ch1);
            ch11 = ch1->ch1;
        case 1:		// случай 4 - if(ch11 && ch11->red)
            ch1->red = node->red;
            ch11->red = node->red = false;
            *root = RotRight(node);
    }
    return false;
}


void BlAndReTree::FindObject(int value)
{
    NodeTree* node = tree_root;
    while (node) {
        if (node->value == value)
        {
            usl = true;
            cout << "Элемент " << node->value << " найден, имеет цвет: ";
            if (node->red) printf("Красный");
            else printf("Черный");
            printf("\n");
            break;
        }
        else
        {
            node = node->value > value ? node->ch1 : node->ch2;
        }
    }
    if (!usl)
    {
        cout << "Элемент не найден" << endl;
    }
}
void BlAndReTree::Returne(NodeTree* node)
{
    if (!node->ch1 && !node->ch2) SumLists += node->value;
    {
        sum += node->value;
        if (node->ch1)Returne(node->ch1);
        if (node->ch2)Returne(node->ch2);
    }
}

int BlAndReTree::Returne()
{
    NodeTree* node = tree_root;
    Returne(node);
    return sum;
}



// найти и убрать максимальный узел поддерева
bool BlAndReTree::GetMineObject(NodeTree** root, NodeTree** res)
{
    NodeTree* node = *root;
    if (node->ch1) {
        if (GetMineObject(&node->ch1, res)) return DelBalanceLeft(root);
    }
    else {
        *root = node->ch2;
        *res = node;
        return !node->red;
    }
    return false;
}

bool BlAndReTree::RemoveObject(NodeTree** root, int value)
{
    NodeTree* t, * node = *root;
    if (!node) return false;
    if (node->value < value) {
        if (RemoveObject(&node->ch2, value))	return DelBalanceRight(root);
    }
    else if (node->value > value) {
        if (RemoveObject(&node->ch1, value))	return DelBalanceLeft(root);
    }
    else {
        bool res;
        if (!node->ch2) {
            *root = node->ch1;
            res = !node->red;
        }
        else {
            res = GetMineObject(&node->ch2, root);
            t = *root;
            t->red = node->red;
            t->ch1 = node->ch1;
            t->ch2 = node->ch2;
            if (res) res = DelBalanceRight(root);
        }
        DelNode(node);
        return res;
    }
    return 0;
}
bool BlAndReTree::AddNode(int value, NodeTree** root)
{
    NodeTree* node = *root;  //  true если балансировка не нужна
    if (!node) *root = NewNode(value);
    else {
        if (value == node->value) return true;
        if (AddNode(value, value < node->value ? &node->ch1 : &node->ch2)) return true;
        BalanceAddNode(root);
    }
    return false;
}

void BlAndReTree::Show()
{
    printf("[tree]\n");
    Show(tree_root, 0, '*');
}

void BlAndReTree::AddNode(int value)
{
    AddNode(value, &tree_root);
    if (tree_root) tree_root->red = false;
}

void BlAndReTree::RemoveObject(int value)
{
    RemoveObject(&tree_root, value);
}

void BlAndReTree::Clear()
{
    Clear(tree_root);
    tree_root = 0;
}

BlAndReTree::check_code BlAndReTree::Check(NodeTree* tree, int d, int& h)
{
    if (!tree) {
        // количество чёрных вершин на любом пути одинаковое
        if (h < 0) h = d;
        return h == d ? ok : error_balance;
    }
    NodeTree* ch1 = tree->ch1;
    NodeTree* ch2 = tree->ch2;
    // красная вершина должна иметь чёрных потомков
    if (tree->red && (ch1 && ch1->red || ch2 && ch2->red)) return error_struct;
    if (ch1 && tree->value<ch1->value || ch2 && tree->value>ch2->value) return error_struct;
    if (!tree->red) d++;
    check_code n = Check(ch1, d, h); if (n) return n;
    return Check(ch2, d, h);
}

BlAndReTree::check_code BlAndReTree::Check()
{
    int d = 0;
    int h = -1;
    if (!tree_root) return ok;
    if (tree_root->red) return error_struct;
    return Check(tree_root, d, h);
}

//bool BlAndReTree::TreeWalk(NodeTree* node, bool* array, int size)
//{
//	if (!node) return false;
//	int value = node->value;
//	if (value < 0 || value >= size || !array[value]) return true;
//	array[value] = false;
//	return TreeWalk(node->ch1, array, size) || TreeWalk(node->ch2, array, size);
//}
//
//// обход дерева и сверка значений с массивом
////! \param array массив для сверки
////! \param size  размер массива
//bool BlAndReTree::TreeWalk(bool* array, int size)
//{
//	if (TreeWalk(tree_root, array, size)) return true;
//	for (int n = 0; n < size; n++) if (array[n]) return true;
//	return false;
//}


//================================================================



//! набор тестирующих процедур
int main()
{
    int n;
    float x;
    setlocale(LC_ALL, "ru");
    BlAndReTree tree;
    printf("Введите количество узлов в дереве: ");
    cin >> n;
    int a;
    for (int i = 0; i < n; i++)
    {
        cin >> a;
        tree.AddNode(a);
    }
    printf("Дерево успешно заполнено!\n");
    int b;
    bool tr = true;
    while (tr)
    {
        printf("Меню:\nНапишите 1, чтобы добавить узел\nНапишите 2, чтобы удалить узел\nНапишите 3, чтобы напечатать дерево\nНапишите 4, чтобы расчитать среднее арифметическое всех узлов\nНапишите 5, чтобы найти сумму всех листьев\nНапишите 6, чтобы найти элемент\nНапишите 7, чтобы выйти из меню\n");
        cin >> a;
        switch (a)
        {
            case 1: {
                cin >> b;
                tree.AddNode(b);
                printf("Элемент успешно добавлен!");
                printf("\n");
                break;

            }
            case 2:
            {
                printf("Введите значение удаляемого узла");
                cin >> b;
                tree.RemoveObject(b);
                break;
            }
            case 3:
            {
                printf("Вывод дерева: \n");
                tree.Show();
                printf("\n");
                break;
            }
            case 4:
            {
                x = float(tree.Returne()) / tree.GetCount();
                printf("Среднее арифметическое равно: ");
                cout << x;
                printf("\n");
                break;
            }
            case 5:
            {
                printf("Сумма значений всех листьев равна: ");
                cout << tree.ReturnSum();
                printf("\n");
                break;
            }
            case 6:
            {
                cout << "Введите значние для поиска: ";
                cin >> b;
                tree.FindObject(b);
                break;
            }
            case 7:
            {
                tr = false;
                break;
            }
            default:
            {
                printf("Команд только от 1 до 6, попробуйте еще раз\n");
            }
        }
    }
    switch (tree.Check()) {
        case BlAndReTree::error_struct:   printf("structure error\n\a"); break;
        case BlAndReTree::error_balance:  printf("balance error\n\a");   break;
        default:
            break;
    }
    tree.Returne();
    tree.Clear();
    return 0;
}



