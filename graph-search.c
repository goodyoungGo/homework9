#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_VERTICAL_NUM 10

// 인접 리스트의 구조체
typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

// 그래프 구조체
typedef struct Graph {
    Node* adjList[MAX_VERTICAL_NUM]; // 인접 리스트
    bool visited[MAX_VERTICAL_NUM]; // 방문 기록
    bool exists[MAX_VERTICAL_NUM]; // 각 정점의 존재 여부를 나타내는 배열
} Graph;


// 큐 구현을 위한 전역 변수
int queue[MAX_VERTICAL_NUM];
// 큐 front, rear 초기화
int front = -1, rear = -1;

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
                printf("init graph.\n");
                break;
            case 'v':
                printf("Enter vertex (0-9): ");
                scanf("%d", &vertex);
                if (vertex >= 0 && vertex < MAX_VERTICAL_NUM) {
                    insertVertex(graph, vertex);
                    printf("정점 %d 삽입.\n", vertex);
                } else
                    printf("Invalid vertex.\n");
                
                break;
            case 'e':
                printf("Enter start vertex (0-9): ");
                scanf("%d", &start);
                printf("Enter end vertex (0-9): ");
                scanf("%d", &end);
                if (start >= 0 && start < MAX_VERTICAL_NUM && end >= 0 && end < MAX_VERTICAL_NUM) {
                    insertEdge(graph, start, end);
                    printf("Edge (%d -> %d) inserted.\n", start, end);
                } else {
                    printf("Invalid edge.\n");
                }
                break;
            case 'd':
                printf("Enter start vertex for DFS (0-9): ");
                scanf("%d", &start);
                if (start >= 0 && start < MAX_VERTICAL_NUM) {
                    clearVisited(graph);
                    printf("DFS starting from vertex %d: ", start);
                    depthFirstSearch(graph, start);
                    printf("\n");
                } else
                    printf("Invalid vertex number.\n");
                
                break;
            case 'b':
                printf("Enter start vertex for BFS (0-9): ");
                scanf("%d", &start);
                if (start >= 0 && start < MAX_VERTICAL_NUM) {
                    clearVisited(graph);
                    printf("BFS starting from vertex %d: ", start);
                    breadthFirstSearch(graph, start);
                    printf("\n");
                } else 
                    printf("Invalid vertex number.\n");
                
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
    for (int i = 0; i < MAX_VERTICAL_NUM; i++) {
        graph->adjList[i] = NULL;
        graph->visited[i] = false;
        graph->exists[i] = false; // 초기화 시 모든 정점은 존재하지 않음
    }
    return graph;
}

// vertex 삽입
void insertVertex(Graph* graph, int vertex) {
    if (!graph->exists[vertex]) {
        graph->exists[vertex] = true;
    } else {
        printf("정점이 이미 존재합니다.\n", vertex);
    }
}

// edge 삽입
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
        printf("정점 삽입 오류\n");
    }
}


// DFS 
void depthFirstSearch(Graph* graph, int vertex) {
    if (!graph->exists[vertex]) {
        printf("정점이 존재하지 않습니다.\n", vertex);
        return;
    }
    Node* adjList = graph->adjList[vertex];
    Node* temp = adjList;

    graph->visited[vertex] = true;
    printf("%d ", vertex); // 방문한 정점을 출력

    while (temp != NULL) {
        int connectedVertex = temp->vertex;

        if (!graph->visited[connectedVertex]) {
            depthFirstSearch(graph, connectedVertex);
        }
        temp = temp->next;
    }
}

// BFS
void breadthFirstSearch(Graph* graph, int startVertex) {
    if (!graph->exists[startVertex]) {
        printf("정점이 존재하지 않습니다.\n", startVertex);
        return;
    }
    clearVisited(graph);
    enqueue(startVertex); // 큐 자료구조를 활용하여 bfs 방식 구현
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
    for (int i = 0; i < MAX_VERTICAL_NUM; i++) {
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
    for (int i = 0; i < MAX_VERTICAL_NUM; i++) {
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
    for (int i = 0; i < MAX_VERTICAL_NUM; i++) {
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

// enqueue
void enqueue(int vertex) {
    if (rear == MAX_VERTICAL_NUM - 1) {
        return;
    }
    if (front == -1) {
        front = 0;
    }
    queue[++rear] = vertex;
}

// dequeue
int dequeue() {
    if (isEmpty()) {
        return -1; // queue empty
    }
    int dequeued = queue[front];
    if (front >= rear) 
        front = rear = -1;
    else
        front++;
    
    return dequeued;
}

// 큐가 비어있는지 확인
bool isEmpty() {
    return front == -1;
}