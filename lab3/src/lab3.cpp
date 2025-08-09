#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <queue>

using namespace std;

struct Point
{
    int value;
    static inline int count{};
public:
    Point()
    {
        ++count;
        value = count;
        // value = rand()%100;
    }
    ~Point()
    {
        --count;
    }
};

struct Edge
{
    int weight;
    Point* p1;
    Point* p2;

public:
    Edge()
    {

        // weight = new int{rand()%100};
    }
    Edge(Point* point1, Point* point2) : p1(point1), p2(point2)
    {
        weight =rand()%100;
    }

    ~Edge(){}

    bool operator<(const Edge& g) const {
        return weight < g.weight;
    }
};

class Graph
{
    Point* points;
    // Edge* edges;
    vector<Edge> edges;
    int point_count;

    int edge_count;
        int findParent(std::vector<int>& parent, int i) {
        if (parent[i] != i) {
            parent[i] = findParent(parent, parent[i]);
        }
        return parent[i];
    }
    public:
    Graph()
    {
        srand(time(NULL));
        
        point_count = rand() % 10 + 1;
        edge_count = rand() % 10 + 1;
        
        points = new Point[point_count];

        for (int i = 0; i < edge_count; ++i)
        {
            int idx1 = rand() % point_count;
            int idx2 = rand() % point_count;
            edges.push_back( Edge(&points[idx1], &points[idx2]));
        }
    }
    ~Graph()
    {      
        delete[] points;
        // delete[] edges;
    }
    void print();
    vector<Edge> findMST()
    {
        vector<Edge> mst;
        vector<int> parent(point_count);
        
        // Инициализация DSU (система непересекающихся множеств)
        iota(parent.begin(), parent.end(), 0);
        
        // Сортируем рёбра по весу
        sort(edges.begin(), edges.end());
        
        for (const Edge& e : edges) {
            int root1 = findParent(parent, e.p1->value);
            int root2 = findParent(parent, e.p2->value);
            
            if (root1 != root2) {
                mst.push_back(e);
                parent[root2] = root1;
                
                // Если все вершины соединены, выходим
                if (mst.size() == point_count - 1) break;
            }
        }
        
        return mst;
    }
    vector<vector<int>> buildAdjacencyMatrix(const vector<Edge>& mst) {
        vector<vector<int>> adjMatrix(point_count, vector<int>(point_count, 0));
        
        for (const Edge& e : mst) {
            int u = e.p1->value;
            int v = e.p2->value;
            adjMatrix[u][v] = e.weight;
            adjMatrix[v][u] = e.weight; // Для неориентированного графа
        }
        return adjMatrix;
    }
    vector<vector<int>> findConnectedComponents(const vector<vector<int>>& adjMatrix) {
        vector<bool> visited(point_count, false);
        vector<vector<int>> components;

        for (int i = 0; i < point_count; ++i) {
            if (!visited[i]) {
                vector<int> component;
                queue<int> q;
                q.push(i);
                visited[i] = true;

                while (!q.empty()) {
                    int current = q.front();
                    q.pop();
                    component.push_back(current);

                    for (int neighbor = 0; neighbor < point_count; ++neighbor) {
                        if (adjMatrix[current][neighbor] > 0 && !visited[neighbor]) {
                            visited[neighbor] = true;
                            q.push(neighbor);
                        }
                    }
                }
                components.push_back(component);
            }
        }
        return components;
    }
};

// void Graph::get_span_tree(double **C, int n) {
//     double wmin;
//     int i, j, vm, *B = new int[n];
//     B[0] = -1;
//     for (i = 1; i < n; i++) B[i] = 0;
//     for (i = 1; i < n; i++) {
//         wmin = INT_MIN; vm = 0;
//         for (j = 1; j < n; j++)
//             if (B[j] != -1 && wmin > C[j][B[j]])
//             {
//                 vm = j; wmin = C[j][B[j]];
//             }
//         if (!vm) return;
//         add_edge(vm, B[vm]); B[vm] = -1;
//         for (j = 1; j < n; j++)
//             if (B[j]!=-1 && C[j][B[j]] > C[j][vm])
//                 B[j] = vm;
//     }
//     delete []B;
// }

// void Graph::add_edge(int, int)
// {}

    void Graph::print()
    {
        cout << "edges: \n";
        sort(edges.begin(), edges.end());
        for (int i = 0; i < edge_count; ++i)
        {
            cout << "p1: " << edges[i].p1->value << " p2: " << edges[i].p2->value << endl;
            // cout << edges[i].weight << endl;
        }
        cout << endl<< endl;
    
        for (int i = 0; i < point_count; ++i)
        {
            cout << points[i].value << " ";
        }
        cout << endl;
    }
int main()
{
    Graph graph;
    graph.print();
    auto mst =  graph.findMST();
    for (const auto& edge : mst) {
        std::cout << edge.p1->value << " - " << edge.p2->value 
                  << " : " << edge.weight << std::endl;
    }
    
    int userInput;
    cin >> userInput;
    cout << userInput;
    return 0;
}