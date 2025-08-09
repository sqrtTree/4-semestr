#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <math.h>

using namespace std;

struct Node
{
    int val, balance;            //ключ
    Node *left, *right, *parent;
    Node(int v)
    {
        val = v;
        balance = 0;           //баланс
        left = right = parent = NULL; }
};

class Tree
{
private:
    Node *root;		    // указатель на условный корень дерева
    Node *current;		// указатель на текущую вершину
    Node *parent; 		// указатель на родительскую вершину

    void av(Node*, int&, int&, int);
    void get_vector_help(Node* node, int low, int high, vector<int>& result);
    
public:
    Node* check_balance();



    //конструктор (пустого дерева или дерева с фиксированной корневой вершиной)
    Tree()
    { 
        root = new Node(INT_MAX);
        current = parent = NULL; 
    }
    //добавление вершины с заданным ключом (если ключ уникальный)
    void add(int);
    
    //построение дерева по заданному массиву случайных целочисленных значений
    void add_array(vector<int>);
    
    //построение оптимального дерева по заданному массиву случайных целочисленных значений
    void build_opt_tree(const vector<int>&);
    void build_opt_tree_help(const vector<int>&, int left, int right);
    
    //добавление вершины в оптимальное дерево
    void add_opt_array(int [], int);

    //поиск вершины с заданным ключом
    Node* search(int);
    
    //вычисление уровня в дереве у вершины с заданным ключом
    int get_level(int, int);
    
    //удаление вершины с заданным ключом
    void remove(int);

    //получение упорядоченного вектора ключей вершин, лежащих в заданном диапазоне значений (поиск в диапазоне)
    vector<int> get_vector(int low, int high);
    // vector<vector<int>> get_vector();
    
    //расчет максимального и среднего по дереву значений уровня вершин
    int max(Node*, int);
    double avarage();
    
    //вывод ключей всех вершин
    void print(Node*);

    //вспомогательные функции
    Node* get_root_node();

    // вывод дерева псевдографикой
    void print_gr_tree(Node* node, std::string prefix, bool isLeft);

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
}

void Tree::add_array(vector<int> v)
{
    for(int i = 0; i < v.size(); i++)
    {
        add(v[i]);
    }
}

void Tree::build_opt_tree(const vector<int>& vec)
{
    int left = 0, right = vec.size() - 1;
    
    build_opt_tree_help(vec, left, right);
    if (check_balance() == NULL){
        cout << "Tree is balanced" << endl;
    }else {
        cout << check_balance()->val << endl;
    }
}
void Tree::build_opt_tree_help(const vector<int>& vec, int left, int right)
{
    if (left > right) return;

    int n = right - left + 1;
    int mid;

    if (n == 1) {
        mid = left;
    } else if (n == 2) {
        mid = left + 1 - 1; // left
    } else {
        // "Центр массы" — середина
        mid = left + (n / 2);
    }

    add(vec[mid]);

    build_opt_tree_help(vec, left, mid - 1);
    build_opt_tree_help(vec, mid + 1, right);
}
// void Tree::add_opt_array(int v[], int size)
// {
//     add_array(v, size);
//     check_balance();
// }

Node* Tree::check_balance() 
{
    parent = current->parent;
    while (parent != root) 
    {
        cout << endl << current->val << endl;
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

// vector<vector<int>> Tree::get_vector()
// {
    // vector<int> result;
    // if (!root) return result;

    // stack<Node*> stack;
    // stack.push(root);

    // while (!stack.empty())
    // {
    //     Node* node = stack.top();
    //     stack.pop();
    //     result.push_back(node->val);

    //     if (node->right) stack.push(node->right);
    //     if (node->left) stack.push(node->left);
    // }

    // return result;
    //===========================
//     queue<Node*> q;
//     vector<vector<int>> res;

//     q.push(root);
//     int currlevel = 0;

//     while (!q.empty())
//     {
//         int len = q.size();
//         res.push_back({});
//         for(int i = 0; i < len; i++)
//         {
//             Node * node = q.front();
//             q.pop();

//             res[currlevel].push_back(node->val);
//             if (node->left != nullptr) q.push(node->left);
//             if (node->right != nullptr) q.push(node->right);
//         }
//         currlevel++;
//     }
//     return res;
// }

void Tree::remove(int v)
{ 
    if (!search(v)) return;
    // удаляемая вершина – лист
    if (!current->left && !current->right)
    {
        if (current == parent->left) 
            parent->left = NULL;
        else parent->right = NULL;
    }
    else if (!current->left) 
    {
        if (current == parent->left) 
            parent->left = current->right;
        else parent->right = current->right;
    }
    // у удаляемой вершины есть только  левое  поддерево
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
}

void Tree::print(Node* node)
{
    if(node == NULL)
        return;
    cout << node->val << " ";
    print(node->left);
    print(node->right);
}

int Tree::max(Node* node, int count = 0)
{
    if (node == NULL) return INT_MIN;

    int res = count;
    ++count;
    int lres = max(node->left, count);
    int rres = max(node->right, count);
    
    if (lres > res) res = lres;
    if (rres > res) res = rres;
    return res;
}

double Tree::avarage()
{
    int  count = -1, sum = 0;   //root level = 0
    av(root, count, sum, 0);
    return static_cast<double>(sum)/count;
}

void Tree::av(Node* node, int& count, int& sum, int lv)
{
    if (node == NULL) return;

    sum += lv;
    ++count;
    ++lv;

    av(node->left, count, sum, lv);
    av(node->right, count, sum, lv);
}

Node* Tree::get_root_node()
{
    return root;
}

void Tree::print_gr_tree(Node* node, std::string prefix, bool isLeft)
{
    if (node == nullptr)
        return;

    std::cout << prefix;

    std::cout << (isLeft ? "├──" : "└──");

    std::cout << node->val << std::endl;

    // рекурсивно выводим поддеревья
    if (node->left || node->right)
    {
        print_gr_tree(node->left, prefix + (isLeft ? "│   " : "    "), true);
        print_gr_tree(node->right, prefix + (isLeft ? "│   " : "    "), false);
    }
}


int main()
{
    SetConsoleOutputCP(CP_UTF8);
    // setlocale(LC_ALL, "");
    srand(time(NULL));
    
    /*----объявление и инициализация переменных для тестирования----*/
    Tree tree;                  //объект класса Tree
    const int arr_length = 10;   //длина массива
    vector<int> array(arr_length);      //массив случайных чисел длины arr_length
    int testNumber = 4;         //подопытное число

    //заполнение массива случайными числами
    for(int i = 0; i < arr_length; i++)
    {
        array[i] = rand()%100;
        cout << array[i] << " ";
    }

    /*----демонастрация работы методов класса Tree----*/
    cout << "Adding testNumbers: " << testNumber << endl;
    tree.add(testNumber);

    cout << "Adding random array..." << endl;
    tree.add_array(array);
    //tree.add_opt_array(array, arr_length);

    // cout << endl << "demonstration of methods" << endl;
    // cout << "search -- " << tree.search(testNumber)->val << endl;
    // cout << "get_Level " << testNumber << " -- " << tree.get_level(testNumber) << endl;
    
    // cout << "removing -- " << testNumber << endl;
    // tree.remove(testNumber);
    
    // cout << "get_vector: ";
    // for (int val : tree.get_vector(4, 500)) {
    //     cout << val << " ";
    // }
    cout << endl;

    cout << "print: ";
    tree.print(tree.get_root_node());
    // cout << endl << "max: " << tree.max(tree.get_root_node()) << endl;
    // cout << "avarage: " << tree.avarage() << endl;
    
cout << endl;
    // Node *af = NULL;
    // cout << af;
    tree.print_gr_tree(tree.get_root_node()->left, "", true);
    if (tree.check_balance() == NULL)   cout << "Tree is balanced" << endl;
    else cout << "Tree is not balanced, unbalanced node: " << tree.check_balance()->val << endl; 
    
    
    // Tree Stree;
    // Stree.build_opt_tree(array);
    // Stree.print_gr_tree(Stree.get_root_node()->left, "", true);

    // tree.print_gr_tree(tree.get_root_node()->left, "", true);

    return 0;
}