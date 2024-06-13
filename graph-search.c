#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_VERTICES 10

// 인접 리스트의 노드 구조체
typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

// 그래프 구조체
typedef struct Graph {
    Node* adjList[MAX_VERTICES];
    bool visited[MAX_VERTICES];
    bool exists[MAX_VERTICES]; // 각 정점의 존재 여부를 나타내는 배열
} Graph;

// 함수 선언
Graph* initializeGraph();
void insertVertex(Graph* graph, int vertex);
void insertEdge(Graph* graph, int start, int end);
void depthFirstSearch(Graph* graph, int vertex);
void breadthFirstSearch(Graph* graph, int startVertex);
void printGraph(Graph* graph);
void freeGraph(Graph* graph);
void clearVisited(Graph* graph);
Node* createNode(int vertex);
void enqueue(int vertex);
int dequeue();
bool isEmpty();

// 큐 구현을 위한 전역 변수
int queue[MAX_VERTICES];
int front = -1;
int rear = -1;

// main 함수
int main() {
    printf("[----- [Goh Geon Young]  [2020017027] -----]\n");

    Graph* graph = initializeGraph();
    char command;
    int vertex, start, end;

    printf("----------------------------------------------------------------\n");
    printf("                         Graph Searches                         \n");
    printf("----------------------------------------------------------------\n");
    printf(" Initialize Graph   = z                                  \n");
    printf(" Insert Vertex      = v           Insert Edge         = e\n");
    printf(" Depth First Search = d           Breath First Search = b\n");
    printf(" Print Graph        = p           Quit                = q\n");
    printf("----------------------------------------------------------------\n");

    while (true) {
        printf("\nEnter command: ");
        scanf(" %c", &command);
        switch (command) {
            case 'z':
                freeGraph(graph);
                graph = initializeGraph();
                printf("Graph initialized.\n");
                break;
            case 'v':
                printf("Enter vertex (0-9): ");
                scanf("%d", &vertex);
                if (vertex >= 0 && vertex < MAX_VERTICES) {
                    insertVertex(graph, vertex);
                    printf("Vertex %d inserted.\n", vertex);
                } else {
                    printf("Invalid vertex number.\n");
                }
                break;
            case 'e':
                printf("Enter start vertex (0-9): ");
                scanf("%d", &start);
                printf("Enter end vertex (0-9): ");
                scanf("%d", &end);
                if (start >= 0 && start < MAX_VERTICES && end >= 0 && end < MAX_VERTICES) {
                    insertEdge(graph, start, end);
                    printf("Edge (%d -> %d) inserted.\n", start, end);
                } else {
                    printf("Invalid vertex number.\n");
                }
                break;
            case 'd':
                printf("Enter start vertex for DFS (0-9): ");
                scanf("%d", &start);
                if (start >= 0 && start < MAX_VERTICES) {
                    clearVisited(graph);
                    printf("DFS starting from vertex %d: ", start);
                    depthFirstSearch(graph, start);
                    printf("\n");
                } else {
                    printf("Invalid vertex number.\n");
                }
                break;
            case 'b':
                printf("Enter start vertex for BFS (0-9): ");
                scanf("%d", &start);
                if (start >= 0 && start < MAX_VERTICES) {
                    clearVisited(graph);
                    printf("BFS starting from vertex %d: ", start);
                    breadthFirstSearch(graph, start);
                    printf("\n");
                } else {
                    printf("Invalid vertex number.\n");
                }
                break;
            case 'p':
                printGraph(graph);
                break;
            case 'q':
                freeGraph(graph);
                printf("Quitting program.\n");
                return 0;
            default:
                printf("Invalid command.\n");
        }
    }
    return 0;
}

// 그래프 초기화
Graph* initializeGraph() {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    for (int i = 0; i < MAX_VERTICES; i++) {
        graph->adjList[i] = NULL;
        graph->visited[i] = false;
        graph->exists[i] = false; // 초기화 시 모든 정점은 존재하지 않음
    }
    return graph;
}

// 정점 삽입
void insertVertex(Graph* graph, int vertex) {
    if (!graph->exists[vertex]) {
        graph->exists[vertex] = true;
    } else {
        printf("Vertex %d already exists.\n", vertex);
    }
}

// 간선 삽입
void insertEdge(Graph* graph, int start, int end) {
    if (graph->exists[start] && graph->exists[end]) {
        Node* newNode = createNode(end);
        if (graph->adjList[start] == NULL) {
            graph->adjList[start] = newNode;
        } else {
            Node* temp = graph->adjList[start];
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    } else {
        printf("One or both vertices do not exist.\n");
    }
}


// DFS 구현
void depthFirstSearch(Graph* graph, int vertex) {
    if (!graph->exists[vertex]) {
        printf("Vertex %d does not exist.\n", vertex);
        return;
    }
    Node* adjList = graph->adjList[vertex];
    Node* temp = adjList;

    graph->visited[vertex] = true;
    printf("%d ", vertex);

    while (temp != NULL) {
        int connectedVertex = temp->vertex;

        if (!graph->visited[connectedVertex]) {
            depthFirstSearch(graph, connectedVertex);
        }
        temp = temp->next;
    }
}

// BFS 구현
void breadthFirstSearch(Graph* graph, int startVertex) {
    if (!graph->exists[startVertex]) {
        printf("Vertex %d does not exist.\n", startVertex);
        return;
    }
    clearVisited(graph);
    enqueue(startVertex);
    graph->visited[startVertex] = true;

    while (!isEmpty()) {
        int currentVertex = dequeue();
        printf("%d ", currentVertex);

        Node* temp = graph->adjList[currentVertex];

        while (temp) {
            int adjVertex = temp->vertex;

            if (!graph->visited[adjVertex]) {
                graph->visited[adjVertex] = true;
                enqueue(adjVertex);
            }
            temp = temp->next;
        }
    }
}

// 그래프 출력
void printGraph(Graph* graph) {
    for (int i = 0; i < MAX_VERTICES; i++) {
        if (graph->exists[i]) {
            Node* temp = graph->adjList[i];
            printf("\nVertex %d: ", i);
            while (temp) {
                printf("%d -> ", temp->vertex);
                temp = temp->next;
            }
            printf("NULL");
        }
    }
    printf("\n");
}

// 그래프 메모리 해제
void freeGraph(Graph* graph) {
    for (int i = 0; i < MAX_VERTICES; i++) {
        Node* temp = graph->adjList[i];
        while (temp) {
            Node* toFree = temp;
            temp = temp->next;
            free(toFree);
        }
    }
    free(graph);
}

// 방문 배열 초기화
void clearVisited(Graph* graph) {
    for (int i = 0; i < MAX_VERTICES; i++) {
        graph->visited[i] = false;
    }
}

// 새로운 노드 생성
Node* createNode(int vertex) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = vertex;
    newNode->next = NULL;
    return newNode;
}

// 큐에 삽입
void enqueue(int vertex) {
    if (rear == MAX_VERTICES - 1) {
        return;
    }
    if (front == -1) {
        front = 0;
    }
    queue[++rear] = vertex;
}

// 큐에서 삭제
int dequeue() {
    if (isEmpty()) {
        return -1;
    }
    int dequeued = queue[front];
    if (front >= rear) {
        front = rear = -1;
    } else {
        front++;
    }
    return dequeued;
}

// 큐가 비어있는지 확인
bool isEmpty() {
    return front == -1;
}