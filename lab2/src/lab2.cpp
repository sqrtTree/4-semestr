#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <windows.h>
#include <math.h>

using namespace std;

struct Node
{
    int val, balance;
    Node *left, *right, *parent;
    Node(int v)
    {
        val = v;
        balance = 0;
        left = right = parent = NULL; }
};

class Tree
{
private:
    Node *root;		    // указатель на условный корень дерева
    Node *current;		// указатель на текущую вершину
    Node *parent; 		// указатель на родительскую вершину

    // const bool isAvl;

    void avarage_help(Node*, int&, int&, int);
    void get_vector_help(Node*, int, int, vector<int>&);
    // void buildAvlTreeHelp(const vector<int>&, int, int);
    
public:

    // конструктор (пустого дерева или дерева с фиксированной корневой вершиной)
    Tree()
    { 
        root = new Node(INT_MAX);
        current = parent = NULL; 
    }
    // Tree(bool avl): isAvl(avl)
    // {
    //     root = new Node(INT_MAX);
    //     current = parent = NULL;
    // }
    // Tree(bool avl, vector<int> vector): isAvl(avl)
    // {
    //     root = new Node(INT_MAX);
    //     current = parent = NULL;
        
    //     if (!avl)
    //     {
    //         addVector(vector);
    //     }else
    //     {
    //         sort(vector.begin(), vector.end());
    //         int left = 0, right = vector.size() - 1;
    
    //         buildAvlTreeHelp(vector, left, right);
    //     }
    // }

// Node* rightRotate(Node* y) {
//     Node* x = y->left;
//     y->left = x->right;
//     x->right = y;
    
//     y->balance = max(getBalance(y->left), getBalance(y->right)) + 1;
//     x->balance = max(getBalance(x->left), getBalance(x->right)) + 1;
    
//     return x; // Новый корень
// }

// Node* leftRotate(Node* x) {
//     Node* y = x->right;
//     Node* T2 = y->left;
    
//     y->left = x;
//     x->right = T2;
    
//     x->balance = max(getBalance(x->left), getBalance(x->right)) + 1;
//     y->balance = max(getBalance(y->left), getBalance(y->right)) + 1;
    
//     return y; // Новый корень
// }


    //добавление вершины с заданным ключом (если ключ уникальный)
    void add(int);
    
    //построение дерева по заданному массиву случайных целочисленных значений
    void addVector(vector<int>);
    
    //построение оптимального дерева по заданному массиву случайных целочисленных значений
    // void build_opt_tree(const vector<int>&);
    
    //добавление вершины в оптимальное дерево
    // void add_opt_array(int [], int);
    
    //поиск вершины с заданным ключом
    Node* search(int);
    
    //вычисление уровня в дереве у вершины с заданным ключом
    int get_level(int, int);
    
    //удаление вершины с заданным ключом
    void remove(int);
    
    //получение упорядоченного вектора ключей вершин, лежащих в заданном диапазоне значений (поиск в диапазоне)
    vector<int> get_vector(int low, int high);
    
    //расчет максимального и среднего по дереву значений уровня вершин
    int getMax(Node*, int);
    double getAvarage();
    
    //вывод ключей всех вершин
    void print(Node*);
    
    // вывод дерева псевдографикой
    void print_gr_tree(Node* node, string prefix, bool isLeft);
    
    //вспомогательные функции
    Node* get_root_node();
    int getBalance(Node* node)
    {
        return node->balance;
    }
    // void balance();
    Node* check_balance();
};

void Tree::add(int v)
{
    parent = root; current = root->left;
    while (current != NULL)
    {
        parent = current;
        current = (v > current->val)? 
        current->right : current->left;
    }
    current = new Node(v);
    current->parent = parent;
    if (v > parent->val) parent->right = current;
    else parent->left = current;
    
    // if (isAvl) balance();
}

void Tree::addVector(vector<int> vector)
{
    for(auto n: vector)
    {
        add(n);
    }
}

// void Tree::build_opt_tree(const vector<int>& vec)
// {
    //     sort(vec.begin(), vec.end());
    //     int left = 0, right = vec.size() - 1;
    
    //     build_opt_tree_help(vec, left, right);
    // }
    
    // void Tree::buildAvlTreeHelp(const vector<int>& vec, int left, int right)
    // {
    //     if (left > right) return;
        
    //     int n = right - left + 1; //количество элементов для формирования дерева
    //     int mid; // корень дерева(или поддерева)
        
    //     if (n == 1) {
    //         mid = left;
    //     } else if (n == 2) {
    //         mid = left;
    //     } else {
    //         mid = left + (n / 2);
    //     }
        
    //     add(vec[mid]);
        
    //     buildAvlTreeHelp(vec, left, mid - 1);
    //     buildAvlTreeHelp(vec, mid + 1, right);
    // }
    
    Node* Tree::check_balance() 
    {
        parent = current->parent;
        while (parent != root) 
        {
            parent->balance +=
            (current == parent->left)? -1 : 1;
            if (parent->balance == 0) return NULL;
            if (abs(parent->balance) == 2) return parent;
            current = parent; parent = parent->parent;
        }
        return NULL;
    }
    
    Node* Tree::search(int v)
    {
        parent = root; current = root->left;
        while (current != NULL && current->val != v) 
        {
            parent = current;
            current = (v > current->val)? 
            current->right : current->left;
        }
        return current;
    }
    
    int Tree::get_level(int target, int level = 1)
    {
        parent = root; current = root->left;
        while (current != NULL && current->val != target) 
        {
            parent = current;
            current = (target > current->val)? 
            current->right : current->left;
            ++level;
        }
        return level;	// NULL, если вершина не найдена
    }
    
vector<int> Tree::get_vector(int low, int high) {
    vector<int> result;
    get_vector_help(root->left, low, high, result); // root->left — фактический корень дерева
    return result;
}

void Tree::get_vector_help(Node* node, int low, int high, vector<int>& result) {
    if (!node) return;
    
    if (node->val > low)
    get_vector_help(node->left, low, high, result);
    
    if (node->val >= low && node->val <= high)
    result.push_back(node->val);
    
    if (node->val < high)
    get_vector_help(node->right, low, high, result);
}

void Tree::remove(int v)
{ 
    if (!search(v)) return;
    
    if (!current->left && !current->right)// удаляемая вершина – лист
    {
        if (current == parent->left) 
        parent->left = NULL;
        else parent->right = NULL;
    }
    else if (!current->left) //если слева пусто
    {
        if (current == parent->left) 
        parent->left = current->right;
        else parent->right = current->right;
    }
    // у удаляемой вершины есть только  левое  поддерево(справа пусто)
    else if (!current->right)
    {
        if (current == parent->left) 
        parent->left = current->left;
        else parent->right = current->left;
    }
    // у удаляемой вершины есть 2  поддерева
    else
    { // запоминаем current  и идем в левое поддерево
        Node *found = current;
        parent = current; current = current->left;
        while (current->right != NULL)   // ищем максимум
        {
            parent = current; current = current->right;
        }
        if (current == parent->left)		// вариант 1
        parent->left = current->left;
        else parent->right = current->left;
        found->val = current->val;      // перенос значения
    }
    delete(current);
    current = parent->left;//ну нужноже же current дать какие то значения
    current->parent = parent;
}

void Tree::print(Node* node)
{
    if(node == NULL) return;
    cout << node->val << " ";
    print(node->left);
    print(node->right);
}

int Tree::getMax(Node* node, int count = 0)
{
    if (node == NULL) return INT_MIN;
    
    int res = count;
    ++count;
    int lres = getMax(node->left, count);
    int rres = getMax(node->right, count);
    
    if (lres > res) res = lres;
    if (rres > res) res = rres;
    return res;
}

double Tree::getAvarage()
{
    int  count = -1, sum = 0;   //root level = 0
    avarage_help(root, count, sum, 0);
    return static_cast<double>(sum)/count;
}

void Tree::avarage_help(Node* node, int& count, int& sum, int lv)
{
    if (node == NULL) return;
    
    sum += lv;
    ++count;
    ++lv;
    
    avarage_help(node->left, count, sum, lv);
    avarage_help(node->right, count, sum, lv);
}

Node* Tree::get_root_node()
{
    return root;
}

void Tree::print_gr_tree(Node* node, string prefix = "", bool isLeft = true)
{
    if (node == nullptr)
    return;
    
    cout << prefix;
    cout << (isLeft ?   "└──":"├──");
    cout << node->val << endl;
    
    if (node->left || node->right)
    {
        print_gr_tree(node->right, prefix + (isLeft ?  "    ":"│   " ), false);
        print_gr_tree(node->left, prefix + (isLeft ?  "    ":"│   " ), true);
    }
}

// void Tree::balance() // балансировка узла p
// {
//     Node* p = check_balance();
// 	if (p == NULL) return;

//     int hl = p->left->balance;
// 	int hr = p->right->balance;
// 	p->balance = (hl>hr?hl:hr)+1;

// 	if((p->right->balance) - (p->left->balance) == 2 )
// 	{
// 		if( (p->right->right->balance) - (p->right->left->balance) < 0 )
// 			p->right = rightRotate(p->right);
//         leftRotate(p);
// 		return;
// 	}
// 	if( (p->right->balance) - (p->left->balance) == -2 )
// 	{
// 		if( (p->left->right->balance) - (p->left->left->balance) > 0  )
// 			p->left = leftRotate(p->left);
//         rightRotate(p);
// 		return;
// 	}
// 	return; // балансировка не нужна
// }

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    srand(time(NULL));
    
    /*----объявление и инициализация переменных для тестирования----*/
    Tree tree;                          //объект класса Tree
    const int arr_length = 10;          //длина массива
    vector<int> vector(arr_length);      //массив случайных чисел длины arr_length
    int testNumber = 4;                 //подопытное число

    //заполнение массива случайными числами
    for(int i = 0; i < arr_length; i++)
    {
        vector[i] = rand()%100;
        cout << vector[i] << " ";
    }

    /*----демонастрация работы методов класса Tree----*/
    cout << "Adding testNumbers: " << testNumber << endl;
    tree.add(testNumber);

    cout << "Adding random vector..." << endl;
    tree.addVector(vector);
    // tree.add_opt_array(vector, arr_length);

    cout << endl << "demonstration of methods" << endl;
    cout << "search -- " << tree.search(testNumber)->val << endl;
    cout << "get_Level " << testNumber << " -- " << tree.get_level(testNumber) << endl;
    
    cout << "removing -- " << testNumber << endl;
    tree.remove(testNumber);

    cout << "get_vector: ";
    for (int val : tree.get_vector(4, 500)) {
        cout << val << " ";
    }
    cout << endl;

    cout << "print: ";
    tree.print(tree.get_root_node());

    cout << endl << "max: " << tree.getMax(tree.get_root_node()) << endl;
    cout << "avarage: " << tree.getAvarage() << endl;

    tree.print_gr_tree(tree.get_root_node()->left);
    if (tree.check_balance() == NULL)   cout << "Tree is balanced" << endl;
    else cout << "Tree is not balanced, unbalanced node: " << tree.check_balance()->val << endl; 

    return 0;
}