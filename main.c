
//          Вариант 7  Задание 6
//
//Написать программу для работы по запросам оператора с ориентированным взвешанным графом.
//Каждая вершина графа имеет уникальное имя и задается координатами на плоскости {x,y}.
//граф задается списком смежностей. Вес ребра задается расстоянием между соответствующими вершинами графа.

// Предусмотреть следующие операции:

// -->Добавление новой вершины в граф

// -->Добавление в граф нового ребра между двумя заданными вершинами, с необходимым вычисление м веса узла

// -->Удаление заданной вершины из графа; при этом удаляются все ребра связанные с данной вершиной

// -->Поиск первых трех кратчайших путей между двумя любыми вершинами графа (алг. Флойда-Уоршалла)

// -->Генерация графа случайным образом

// -->Загрузка из файла, сохранение изменений

// -->Таймирование

#include <endian.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

typedef struct Vertex {    //структура вершины
    char* name;            //имя вершины
    int x;                 //координата Х
    int y;                 //координата Y
    struct Adjacent* head; //список смежности данной вершины
    int count;             //количество смежных вершин
}Vertex;
typedef struct Adjacent {  //односвязный список смежности
    Vertex* Verb;//указатель на вершину
    int weight;
    struct Adjacent* next; //указатель на следующую смежную вершину
} Adjacent;
typedef struct Graph {     //структура графа
    Vertex** Vert;         //массив вершин
    int size;              //размер графа
}Graph;
typedef struct path{
    unsigned *Begin;
    unsigned len;
} path;
int F0_Menu();                //меню
int F1_Exit(Graph* G, char* filename);           //завершение программы
int F2_Random(Graph* G,char* filename);        //создание рандомного графа
int F3_AddVertex(Graph* G,char* filename);     //добавление вершины
int F4_AddCommunication(Graph* G, char* filename);//добавление ребра
int F5_DeleteVert(Graph* G, char* filename);    //удаление вершины
int F6_PrintGraph(Graph* G, char* filename);    //вывод графа
int F7_SearchPath(Graph* G, char* filename);        //поиск
int F8_FreeGraph(Graph* G, char* filename);     //удаление графа
//int F9_Timing();              //таймирование

//         [СОЗДАНИЕ]
int CommunicationVerb(Graph*, Vertex*, Vertex*);
Vertex* CreateVertex(Graph*, int x, int y, char* name);
void RandomGraph(Graph* G, int V, int E, int a);
void RandomStr(char* buf, int l);
//          [ФАЙЛ]
int Load(Graph* G, FILE* file);
void Save(Graph* G, char* filename);
int Search(Graph* G, char* filename);
//         [УДАЛЕНИЕ]
void DeleteCommunication(Vertex* v1, Vertex* v2);
void DeleteVertex(Graph* G, int v);

//          [ПОИСК]
int FindVertex(Graph* G, char* name);
void SearchAdjacent(Graph* G, int n, int a, int* color, int* j);
int FindPath(struct Graph* G, int V1, int V2);
path* PathBorn(unsigned **g, unsigned **d, unsigned v1, unsigned v2, unsigned size);
int revealing (unsigned **g, unsigned **d, unsigned v1, unsigned v2, path *path1 , unsigned size); // рекурсивная функция восстановления пути

//          [ВВОД]
char* getstring();    //ввод строчки
int getIntSmall(int *a,int e); //ввод числа

//          [ОБЩИЕ]
int check(Graph* G, int V1, int V2);
int CheckGraph(Graph* G);
int distance(int x1, int x2, int y1, int y2);









const char* msgs[] = { NULL, "Exit programm", "Random Graph", "Add Vertex", "Add Communication", "Delete", "Print graph", "Search", "Clear Graph", "Timing" };
//Главная функция                           +
int main()
{
    Graph G = { NULL, 0};
    int ex = 0, menu = -1, flag = 0, choose;
    int(*fun1[]) (Graph*, char*) = { NULL, F1_Exit, F2_Random, F3_AddVertex, F4_AddCommunication, F5_DeleteVert, F6_PrintGraph, F7_SearchPath, F8_FreeGraph};
    G.Vert = (Vertex**)calloc(1, sizeof(Vertex*));
    G.size = 0;
    printf("Enter file name \n");
    char* filename = getstring();
    Search(&G, filename);
    F0_Menu();
    while (ex != 1) {//выбор пункта меню
        menu = -1;
flag = 0;
        while (menu <= -1 || menu >= 10) {
if (flag == 1)
                printf("Error!\n");
            printf("Choose action-->");
            getIntSmall(&menu, 2);
            flag = 1;
        }
        if (menu == 0)
            F0_Menu();
        else
            ex = (*fun1[menu])(&G, filename);
    }
    return 0;
}


//            [МЕНЮ]
//Вывод меню                                +
int F0_Menu()
{
    printf("\n      MENU\n");
    for (int i = 1; i < 10; ++i)
        printf("%d. %s\n", i, msgs[i]);
}
//Выход + Сохранение                        +
int F1_Exit(Graph* G, char* filename) {
    if (CheckGraph(G)) {
        free(G->Vert);
        return 1;
    }
    else {
        Save(G, filename);
        F8_FreeGraph(G,"");
        printf("Graph is saved.\n");
        return 1;
    }
}
//создание рандомного графа                 +
int F2_Random(Graph* G, char* filename)
{
    printf("\n  [CREATE RANDOM GRAPH]\n");
    int V, E, flag = 0;
    do {
        if (flag)
            printf("too many edges: (ne <= nv*(nv-1))");
        printf("\"Enter the number of vertexes:\"");
        getIntSmall(&V, 2);
        printf("Enter the number of edges:");
        getIntSmall(&E, 2);
        flag = 1;
    } while (E > V * (V - 1));
    RandomGraph(G, V, E, 1);
    printf("Graph is created.\n");
}
//добавление вершины(только вершины, без ребер)  +
int F3_AddVertex(Graph* G, char* filename)
{
    printf("\n     [ADD VERTEX]\n");
    int x, y, flag = 0;
    char* str;
    G->Vert = (Vertex**)realloc(G->Vert, (G->size + 1) * sizeof(Vertex*));
    do {
        if (flag)
            printf("Vertex already exists. Use another name!\n");
        printf("\"Enter the vertexs' name:\"");
        str = getstring();
        flag = 1;
    } while (FindVertex(G, str) != -1);
    printf("Enter the X coord: ");
    getIntSmall(&x, 2);
    printf("\"Enter the Y coord:");
    getIntSmall(&y, 2);
    CreateVertex(G, x, y, str);
    free(str);
    printf("Vertex is added.\n");
}
//создание связи между вершинами            +
int F4_AddCommunication(Graph* G, char* filename)
{
    printf("\n   [CREATE COMMUNICATION]\n");
    char* str;
    int i = 0, V1, V2;
    printf("Enter the vertexs'-1 name:");
    str = getstring();
    V1 = FindVertex(G, str);
    free(str);
    if (V1 == -1) {
        printf("The vertex is not found.\n");
        return 0;
    }
    printf("Enter the vertexs'-2 name:");
    str = getstring("Enter the vertexs'-2 name:");
    V2 = FindVertex(G, str);
    free(str);
    if (V2 == -1) {
        printf("The vertex is not found.\n");
        return 0;
    }
    if (V1 == V2) {
        printf("It is one vertex.\n");
        return 0;
    }
    if (check(G, V1, V2)) {
        printf("Vertexes have communication.\n");
        return 0;
    }
    else
        CommunicationVerb(G, G->Vert[V1], G->Vert[V2]);
    printf("Communication is added.\n");
}
//удаление вершины                          +
int F5_DeleteVert(Graph* G, char* filename)
{
    printf("\n   [DELETE VERTEX]\n");
    if (CheckGraph(G))
        return 0;
    char* name;
    int i = 0, V;
    printf("Enter vertexs' name:");
    name = getstring();
    V = FindVertex(G, name);
    if (V == -1) {
        printf("The vertex wasn't found.\n");
        return 0;
    }
    DeleteVertex(G, V);
    printf("The vertex is deleted.\n");
}
//вывод графа                               +
int F6_PrintGraph(Graph* G, char* filename)
{
    printf("\n   [PRINT]\n");
    if (CheckGraph(G))
        return 0;
    printf("\nGraph:\n");
    int i = 0;
    printf("Ind:     Name:    Coord:     Degree:\n");
    for (i = 0; i < G->size; i++) {
        printf("\n");
        printf("[%d]  %7s      {%d;%d}     %d\n", i, G->Vert[i]->name, G->Vert[i]->x, G->Vert[i]->y, G->Vert[i]->count);
        if (G->Vert[i]->head != NULL) {
            Adjacent* V = G->Vert[i]->head;
            while (1) {
                if (V->Verb != NULL)
                    printf("           -->%s(%d)\n", V->Verb->name,V->weight);
                else
                    break;
                if (V->next == NULL)
                    break;
                else
                    V = V->next;
            }
        }
    }
}
//поиск                                     +
//int F7_Search(Graph* G, char* filename)
//{
//    printf("\n   [SEARCH]\n");
//    if (CheckGraph(G))
//        return 0;
//    char* name;
//    int V, j=0;
//    printf("Enter vertexs' name:");
//    name = getstring("");
//    V = FindVertex(G, name);
//    if (V == -1) {
//        printf("The vertex wasn't found.\n");
//        return 0;
//    }
//    int* color = (int*)calloc(G->size, sizeof(int)); //0-белая,1-серая,-1-черная
//    SearchAdjacent(G, V, 0, color, &j);
//    printf("Number of connected vertexes [%d]\n", j);
//    free(color);
//}

int F7_SearchPath(Graph* G, char* filename){
    printf("\n   [DELETE VERTEX]\n");
    if (CheckGraph(G))
        return 0;
    char* name;
    int i = 0, V1, V2;
    printf("Enter 1st vertex name:");
    name = getstring();
    V1 = FindVertex(G, name);
    if (V1 == -1) {
        printf("The vertex wasn't found.\n");
        return 0;
    }
    printf("Enter 2nd vertex name:");
    name = getstring();
    V2 = FindVertex(G, name);
    if (V2 == -1) {
        printf("The vertex wasn't found.\n");
        return 0;
    }
    printf("Path: \n");
    int Length = FindPath(G, V1, V2);
    printf("%d",Length);

}
//удаление графа                            +
int F8_FreeGraph(Graph* G, char* filename)
{
    int i = 0;
    for (i = 0; i < G->size; i++) {
        if (G->Vert[i]->head != NULL) { //удаление списка смежности
            Adjacent* V = G->Vert[i]->head;
            Adjacent* last;
            while (1) {
                last = V;
                if (V->next == NULL)
                    break;
                else
                    V = V->next;
                free(last);
            }
            free(V);
        }
        free(G->Vert[i]->name);
    }
    free(G->Vert);
    G->Vert = NULL;
    G->size = 0;
    printf("Memory is cleaned.\n");
}

//удаление вершины                                        +
void DeleteVertex(Graph* G, int v) {
    if (G->Vert[v]->head != NULL)  //удаление списка смежности
        if (G->Vert[v]->head->Verb != NULL) {
            Adjacent *V = G->Vert[v]->head;
            Adjacent *last = V;

//            while (1) {
//                last = V;
//                if (V->next == NULL) {
//                    DeleteCommunication(G->Vert[v], last->Verb);
//                    free(V);
//                    break;
//                } else
//                    V = V->next;
//                DeleteCommunication(G->Vert[v], last->Verb);
//                free(last);
//            }
            for (int i = 0; i < G->size; i++) {
                Adjacent *a;
                if (G->Vert[i]->head != NULL) {
                    a = G->Vert[i]->head;
                    for (int j = 0; j < G->Vert[i]->count; j++) {
                        if (a!=NULL && a->Verb == G->Vert[v])
                            DeleteCommunication(G->Vert[v], G->Vert[i]);
                        a = a->next;
                    }
                }
            }
            free(G->Vert[v]->name);
            G->Vert[v] = G->Vert[G->size - 1];
            G->Vert[G->size - 1] = NULL;
            G->Vert = (Vertex **) realloc(G->Vert, (G->size - 1) * sizeof(Vertex *));
            G->size--;
        }
}
//удаление заданной вершины из списка смежности           +
void DeleteCommunication(Vertex* v1, Vertex* v2)
{//v1-что удаляем, v2-откуда удаляем
    Adjacent* V = v2->head;
    Adjacent* last = V;
    int i = 0;
    while (V->Verb != v1) {
        last = V;
        V = V->next;
        i++;
    }
    if (i != 0) {
        if (V->next != NULL)
            last->next = V->next;
        else
            last->next = NULL;
    }
    else {
        if (V->next != NULL)
            v2->head = V->next;
        else
            v2->head = NULL;
    }
    v2->count--;
    free(V);
}


//           [ФАЙЛ]
//загрузка таблицы                                        +
int Search(Graph* G, char* filename)
{
    int choose = 1;
    FILE *file = fopen(filename, "r+b");
    if (file == NULL) {
        printf("File wasn't found. Creating new one");
        file = fopen(filename, "w+b");

    } else {
        printf("Upload data from a file?\nYes[1]    No[2]\n");

        do {
            if (choose != 1 && choose != 2)
                printf("Error! Repeat:");
            printf("Enter:");
            getIntSmall(&choose, 2);
            if (choose == 0 || choose == 2)
                return 0;
        } while (choose != 1 && choose != 2);

    Load(G, file);}

    return 0;
}
//считываем информацию из файла(если он существует)       +
int Load(Graph* G, FILE* file)
{
    int i = 0, n, k=-1;
    char name[10] = "";
    char* Name;
    fread(&G->size, sizeof(int), 1, file);
    G->Vert = (Vertex**)calloc(G->size, sizeof(Vertex*));
    for (int i = 0; i < G->size; i++) {
        Vertex* V = (Vertex*)calloc(1, sizeof(Vertex));
        k = -1;
        do {
            k++;
            fread(&name[k], sizeof(char), 1, file);
        } while (name[k] != 0);
        fread(&V->x, sizeof(int), 1, file);
        fread(&V->y, sizeof(int), 1, file);
        fread(&V->count, sizeof(int), 1, file);
        V->name = strdup(name);
        G->Vert[i] = V;
    }
    for (int i = 0; i < G->size; i++) {
        if (G->Vert[i]->count != 0) {
            for (int j = 0; j < G->Vert[i]->count; j++) {
                k = -1;
                do {
                    k++;
                    fread(&name[k], sizeof(char), 1, file);
                } while (name[k] != 0);
                Name = strdup(name);
//                if (check(G, i, FindVertex(G, Name)))  //проверка на существование связи
//                    continue;
//                else {
                    G->Vert[i]->count--;
//                    G->Vert[FindVertex(G, Name)]->count--;
                    CommunicationVerb(G, G->Vert[i], G->Vert[FindVertex(G, Name)]);
//                }
            }
        }
    }
    fclose(file);
    return 1;
}
//сохранение графа в файл                                 +
void Save(Graph* G, char* filename)
{
    FILE* file = fopen(filename, "w+b");
    Adjacent* A = NULL;

    if (!file) return;
    fseek(file, 0, SEEK_SET);
    fwrite(&G->size, sizeof(int), 1, file);
    for (int i = 0; i < G->size; i++) {
        Vertex b = *G->Vert[i];
        fwrite(G->Vert[i]->name, sizeof(char)*(strlen(G->Vert[i]->name) + 1), 1, file);
        fwrite(&G->Vert[i]->x, sizeof(int), 1, file);
        fwrite(&G->Vert[i]->y, sizeof(int), 1, file);
        fwrite(&G->Vert[i]->count, sizeof(int), 1, file);
    }
    for (int i = 0; i < G->size; i++) {
        if (G->Vert[i]->head != NULL) {
                 A = G->Vert[i]->head;
            for (int j = 0; j < G->Vert[i]->count; j++) {
                fwrite(A->Verb->name, sizeof(char)*(strlen(A->Verb->name) + 1), 1, file);
                A = A->next;
            }
        }
    }
    fclose(file);
    printf("Graph was saved.");
}


//         [ДОБАВЛЕНИЕ/СОЗДАНИЕ]
//создание вершины                                                 +
Vertex* CreateVertex(Graph* G, int x, int y, char* name)
{
    int i;
    Vertex* v = (Vertex*)calloc(1,sizeof(Vertex));
    v->name = strdup(name);
    v->x = x;
    v->y = y;
    for (i = 0; G->Vert[i] != NULL && i < G->size; i++);
    if (i == G->size) {
        G->Vert = (Vertex**)realloc(G->Vert, (G->size + 1) * sizeof(Vertex*));
        G->size++;
    }
    G->Vert[i] = v;
    return v;
}
//создание рандомного набора букв                                  +
void RandomStr(char* buf, int l)
{
    for (int i = 0; i < l; i++)
        buf[i] = rand() % 27 + 'A';
    buf[l] = '\0';
}
//создание рандомного графа                                        +
void RandomGraph(Graph* G, int V, int E, int a)
{      srand((unsigned int) time);
    if (a == 1) {
        F8_FreeGraph(G,""); //очистка прошлого графа, если он был
        G->Vert = (Vertex**)calloc(V, sizeof(Vertex*));
    }
    char name[6];
    int i = 0;
    for (int t = 0; t < V; t++) {
        RandomStr(name, 5);
        if (G->size != 0)
            if (FindVertex(G, name) != -1) {
                t--;
                continue;
            }
        CreateVertex(G, rand() % 1000, rand() % 1000, name); //создание вершины
    }

for (int t = 0; t < E; t++)
        if (CommunicationVerb(G, NULL, NULL))
            t--;
}
//создание связи между вершинами                                   +
int CommunicationVerb(Graph* G, Vertex* v1, Vertex* v2) {
    Adjacent* N1;
    Adjacent* N2;
    Vertex* V1 = v1;
    Vertex* V2 = v2;
    //srand(time(NULL));
    if (v1 == NULL && v2 == NULL) { //если работает рандом
        int n1 = rand() % G->size;
        int n2 = rand() % G->size;
        if (n1 == n2)
            return 1;
        if (check(G, n1, n2))
            return 1;
        if (G->Vert[n1]->head == NULL) //если не имеет списка смежности, то мы его создаем
            G->Vert[n1]->head = (Adjacent*)calloc(1, sizeof(Adjacent));
//        if (G->Vert[n2]->head == NULL) //если не имеет списка смежности, то мы его создаем
//            G->Vert[n2]->head = (Adjacent*)calloc(1, sizeof(Adjacent));
        N1 = G->Vert[n1]->head;
       // N2 = G->Vert[n2]->head;
        G->Vert[n1]->count++;
        //G->Vert[n2]->count++;
        V1 = G->Vert[n1];
        V2 = G->Vert[n2];
    }
    else { //если добавление ребра
        if (v1->head == NULL)
            v1->head = (Adjacent*)calloc(1, sizeof(Adjacent));
//        if (v2->head == NULL)
//            v2->head = (Adjacent*)calloc(1, sizeof(Adjacent));
        N1 = v1->head;
        //N2 = v2->head;
        v1->count++;
        //v2->count++;
    }
    while (N1->Verb != NULL) {
        if (N1->next == NULL) {
            N1->next = (Adjacent*)calloc(1, sizeof(Adjacent));
            N1 = N1->next;
        }
        else
            N1 = N1->next;
    }
    N1->Verb = V2;
    N1->weight = distance(V1->x, V2->x, V1->y, V2->y);
//    while (N2->Verb != NULL) {
//        if (N2->next == NULL) {
//            N2->next = (Adjacent*)calloc(1, sizeof(Adjacent));
//            N2 = N2->next;
//        }
//        else
//            N2 = N2->next;
//    }
//    N2->Verb = V1;
    return 0;
}

char* GetStr(char* q)
{
    printf(q);
    char* ptr = (char*)malloc(1);
    char buf[81];
    int n, len = 0;
    *ptr = '\0';
    do {
        n = scanf("%80[^\n]", buf);
        if (n < 0) {
            free(ptr);
            ptr = NULL;
            continue;
        }
        if (n == 0)
            scanf("%*c");
        else {
            len += strlen(buf);
            ptr = (char*)realloc(ptr, len + 1);
            strcat(ptr, buf);
        }
    } while (n > 0);
    return ptr;
}


//            [ОБЩИЕ]
//проверка графа на пустоту              +
int CheckGraph(Graph* G)
{
    if (G->size == 0) {
        printf("The graph is free.\n");
        return 1;
    }
    return 0;
}
//проверка смежности двух вершин         +
int check(Graph* G, int V1, int V2)
{//1 - yes, 2 - No
    Adjacent* V = G->Vert[V1]->head;
    if (V != NULL) {
        for (V; V->Verb != G->Vert[V2] && V->next != NULL; V = V->next);
        if (V->Verb != G->Vert[V2])
            return 0;
        else
            return 1;
    }
    return 0;
}

char *getstring() {
    char *ptr = (char *) malloc(1);
    char buf[81];
    unsigned int n, len = 0;
    *ptr = '\0';
    fflush(stdin);
    do {
        n = scanf("%5[^\n]", buf);
        if (n < 0) {
            free(ptr);
            ptr = NULL;
            continue;
        }
        if (n == 0) scanf("%*c");
        else {
            len += strlen(buf);
            ptr = (char *) realloc(ptr, len + 1);
            strncat(ptr, buf, len);
        }
    } while (n > 0);
    return ptr;
}

int getIntSmall(int *a,int e) {
    int n;

    if (e == 1) {
        srand(rand()*time(0));
        printf("%s\n", "random");
        *a = rand() % 10000 + 1;
        return 1;
    }

    if (e == 2)
    { do {
            fflush(stdin);
            n = scanf("%i", a);
            printf("%s\n", "manual");
            if (n < 0)
                return 0;
            if (n == 0) {
                printf("%s\n", "Error! Repeat input");
                scanf("%*[^\n]", 0);
            }
        } while (n == 0);
        scanf("%*c");
        return 1;
    }
}

int FindVertex(Graph* G, char* name)
{
    if (G->size != 0)
        for (int i = 0; i < G->size; i++)
            if (!strcmp(G->Vert[i]->name, name))
                return i;
    return -1;
}

void SearchAdjacent(Graph* G, int n, int a, int* color, int* j)
{
    int i = 0, p = 0, k, l;
    Vertex** Q = (Vertex**)calloc(G->size, sizeof(Vertex*));
    color[n] = 1; //1 - серый, -1 - черный, 0 - белый
    Q[0] = G->Vert[n];
    while (Q[0] != NULL) {
        Vertex* u = Q[0];
        Q[0] = NULL;
        for (k = 1; Q[k] != NULL; Q[k - 1] = Q[k], Q[k] = NULL, k++);//сдвиг элементов в Q
        Adjacent* A = u->head;
        if (*j != 0) {
            if (a == 0)
                printf(" %s\n", u->name);
        }
        if (A != NULL)
            for (k = 0; k < u->count; k++) {//обработка всех смежных вершин
                p = FindVertex(G, A->Verb->name);
                if (color[p] == 0) {
                    color[p] = 1;
                    for (l = 0; Q[l] != NULL; l++); //поиск свободного места в Q
                    Q[l] = A->Verb;
                }
                A = A->next;
            }
        color[FindVertex(G, u->name)] = -1;
        (*j)++;
    }
    (*j)--;
    free(Q);
}
int distance(int x1, int x2, int y1, int y2)
{
    float dist;
    dist = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
    return dist;
}

int FindPath(Graph* G, int V1, int V2)
{   long long unsigned size = G->size;
     unsigned g[size] [size];
     unsigned d[size] [size];

    for(int i = 0; i < size; i++)
    {   Adjacent *kl;
        for (int j = 0; j < size; j++) {
            kl = G->Vert[i]->head;
            while (kl != 0 && kl->Verb!=G->Vert[j])
                kl = kl->next;
           if(kl != NULL && G->Vert[j] == kl->Verb) {
               g[i][j] = 0;
               d[i][j] = kl->weight;
           } else {
               g[i][j] = 0;
               d[i][j] = 20000000;
           }
        }
    }

    path *Short = (path*) calloc(1,sizeof(path));
    Short->Begin = (int*) calloc(1,sizeof(int));
    Short->len = 0;
//    int* mom = Short->Begin;
//    int count = 0;
    for (int k = 0; k < size; k++)
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++) {
                if (d[i][j] > d[i][k] + d[k][j])
                {
                    d[i][j] = d[i][k] + d[k][j];
                    g[i][j] = k;// матрица предков по которой востанавливается путь
               }
        }
    PathBorn(g, d, V1, V2, G->size);
    return d[V1][V2];
}

int revealing (unsigned **g, unsigned **d, unsigned v1, unsigned v2, path *path1,unsigned size) // рекурсивная функция восстановления пути
{
    unsigned r;
    r = *(g+v1*size+v2);
    if(v1 == v2)
        return 0;
    revealing(g, d, v1, r, path1, size);
    unsigned *ptr = path1->Begin;
    while (ptr+1 != NULL)
    {
        ptr++;
    }
    path1->len++;
    path1->Begin = realloc(path1->Begin,path1->len);
    *ptr = r;
    printf("%d", r);
    revealing(g, d, r, v2, path1, size);
    return 0;
}

path* PathBorn(unsigned **g, unsigned **d, unsigned v1, unsigned v2, unsigned size)
{
    path *path1 = calloc(1,sizeof(path));
    path1->Begin = calloc(1,sizeof(unsigned));
    revealing(g, d, v1, v2, path1, size);
    return path1;
}