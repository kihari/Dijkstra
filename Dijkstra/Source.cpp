// Алгоритм Дейкстры позволяет найти кратчайший путь от начальной позиции в графе до любой другой вершины
// Сложность алгоритма: O(n^2)
#include <iostream>
// Для ограничений максимально возможного пути
#include <limits>
using namespace std;
#define MAXV 1000

class EdgeNode
{
public:
    int key; // Идентификатор вершины
    int weight; // Вес ребра
    EdgeNode* next; // Указатель на следующий элемент
    EdgeNode(int, int); // Определение ф-ции EdgeNode
};

// Конструктор класса EdgeNode (создание одного элемента графа)
/*
  Принимает:
  - идентификатор вершины
  - вес ребра
*/
EdgeNode::EdgeNode(int key, int weight)
{
    this->key = key;
    this->weight = weight;
    this->next = NULL;
}

class Graph
{
    bool directed; // ориентированный?
public:
    EdgeNode* edges[MAXV + 1]; // Кол-во вершин
    Graph(bool);
    void insert_edge(int, int, int, bool);
    void print();
};

// Конструктор класса Graph (создание графа, присвоение кол-ва вершин)
/*
  Принимает:
  - ориентированный или нет
*/

/*--------------------------_БЛОЧКУ ДЕЛАТЬ ОТСЮДА-------------------------*/

Graph::Graph(bool directed)
{
    this->directed = directed;
    // Пока i меньше макс. возможного кол-ва вершин
    for (int i = 1; i < (MAXV + 1); i++)
    {
        this->edges[i] = NULL; // Присвоить указатели на элементы
    }
}

// Вставка элемента в граф (составление матрицы смежности)
/*
  Принимает:
  - пару вершин
  - вес ребра
  - ориентированный или нет
*/
void Graph::insert_edge(int x, int y, int weight, bool directed)
{
    // Если ребро больше 0 и ребро меньше максимально возможной и вершина больше 0 и вершина меньше максимально возможной
    if (x > 0 && x < (MAXV + 1) && y > 0 && y < (MAXV + 1))
    {
        EdgeNode* edge = new EdgeNode(y, weight); // Инициализация переменной класса EdgeNode (определяем ребро <y> и его вес)
        // Связывание вершин друг с другом (ноды указывают друг на друга)
        edge->next = this->edges[x];
        this->edges[x] = edge;
        // Если неориентированный
        if (!directed)
        {
            insert_edge(y, x, weight, true); // рекурсивно создаем связку вершин, только наоборот (из y вершины в x)
        }
    }
}

// Инициализация характеристик графа
/*
  Принимает:
  - массивы: просмотренные вершины, пути к вершинам, вершины-родители(запоминание предыдущих)
*/
void init_vars(bool discovered[], int distance[], int parent[])
{
    // Пока i меньше макс. возможного кол-ва вершин
    for (int i = 1; i < (MAXV + 1); i++)
    {
        discovered[i] = false; // массив "просмотренных" путей заполнить false
        distance[i] = std::numeric_limits<int>::max(); // Численное значение пути равно максимально возможному
        parent[i] = -1; // Вершинам-родителям присвоить -1  (Здесь лежат просмотренные номера вершин)
    }
}

// Алгоритм Дейкстры
/*
  Принимает:
  - указатель на граф
  - массивы: вершин-родителей, путей к вершинам
  - начальное значение вершины
*/
void dijkstra_shortest_path(Graph* g, int parent[], int distance[], int start)
{
    bool discovered[MAXV + 1];
    EdgeNode* curr; // Текущий элемент графа
    int v_curr; // Текущая вершина
    int v_neighbor; // Соседняя вершина
    int weight; // Вес ребра
    int smallest_dist; // Кратчайший путь

    // Инициализация характеристик графа
    init_vars(discovered, distance, parent);

    distance[start] = 0; // Присваиваем ноль стартовой вершине (указали 1, путь из 1 в 1 = 0)
    v_curr = start; // Текущая вершина = стартовой

    // Пока не обошли все вершины
    while (discovered[v_curr] == false)
    {
        discovered[v_curr] = true; // Определяем текущую вершину как просмотренную
        curr = g->edges[v_curr]; // Сохраняем указатель на элемент

        // Пока элемент ненулевой
        while (curr != NULL)
        {
            v_neighbor = curr->key; // Соседняя вершина равна текущей
            weight = curr->weight; // Вес ребра равен текущему

            // Если путь к текущему с учетом веса меньше пути к соседней
            if ((distance[v_curr] + weight) < distance[v_neighbor])
            {
                // Путь к соседней вершине равен пути к текущему с учетом веса ребра
                distance[v_neighbor] = distance[v_curr] + weight;
                // Помечаем соседнюю вершину как текущную (просмотрели)
                parent[v_neighbor] = v_curr;
            }
            // Переход к следующему элементу
            curr = curr->next;
        }

        // Инициализация кратчайщего пути
        smallest_dist = std::numeric_limits<int>::max();

        // Пока i меньше максимально возможного
        for (int i = 1; i < (MAXV + 1); i++)
        {
            // Если текущая вершина непросмотренна и кратчайший путь больше i-го
            if (!discovered[i] && (distance[i] < smallest_dist))
            {
                v_curr = i; // Текушая вершина равна i-ой
                smallest_dist = distance[i]; // Запомнить кратчайший путь к текущей вершине
            }
        }
    }
}

// Печать кратчайшего пути по вершинам
/*
  Принимает:
  - начальную вершину
  - массив вершин-родителей
*/
void print_shortest_path(int v, int parent[])
{
    // Если начальная вершина больше 0 и меньше максимально возможной и вершина-родитель определена
    if (v > 0 && v < (MAXV + 1) && parent[v] != -1)
    {
        // Рекурсивно печатаем каждую вершину кратчайшего пути
        cout << parent[v] << " ";
        print_shortest_path(parent[v], parent);
    }
}

// Печать кратчайшего пути для каждой вершины
/*
  Принимает:
  - начальную вершину
  - массив путей к вершинам
*/
void print_distances(int start, int distance[])
{
    cout << endl;

    // Пока i меньше максимально возможного
    for (int i = 1; i < (MAXV + 1); i++)
    {
        // Если путь не максимально возможный
        if (distance[i] != std::numeric_limits<int>::max())
        {
            // Печать кратчайшего пути к вершине
            cout << "Кратчайший путь из " << start << " до " << i << ": " << distance[i] << endl;
        }
    }
}

int main()
{
    Graph* g = new Graph(false); // Инициализация графа
    int parent[MAXV + 1];
    int distance[MAXV + 1];
    int from = 1; // Инициализация начальной вершины
    int to = 5; // Инициализация конечной вершины

    // Составление матрицы смежности
    g->insert_edge(1, 2, 4, false);
    g->insert_edge(1, 3, 1, false);
    g->insert_edge(3, 2, 1, false);
    g->insert_edge(3, 4, 5, false);
    g->insert_edge(2, 4, 3, false);
    g->insert_edge(2, 5, 1, false);
    g->insert_edge(4, 5, 2, false);

    // Поиск кратчайшего пути графа
    dijkstra_shortest_path(g, parent, distance, from);
    //Печать кратчайшего пути к заданной конечной вершине
    print_shortest_path(to, parent);
    // Печать всех возможных путей к вершинам
    print_distances(from, distance);

    delete g; // Удаление графа (освобождение памяти)
    return 0;
}
