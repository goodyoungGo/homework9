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

// 함수 미리 선언
Graph* initGraph();
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

// main 함수
int main() {
    printf("[----- [Goh Geon Young]  [2020017027] -----]\n");

    Graph* graph = NULL; // graph 선언

    // 필요 변수 선언
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
        printf("\n명령어 입력: ");
        scanf(" %c", &command);
        switch (command) {
            case 'z':
                if (graph) // graph에 값이 있으면 메모리 해제
                    freeGraph(graph);
                graph = initGraph(); // graph 초기화
                
                if (graph == NULL) {
                    printf(stderr, "그래프 초기화 실패\n");
                    return -1; // 그래프 초기화 실패 시 -1 반환하여 프로그램 종료
                }

                printf("init graph.\n");
                break;
            case 'v':
                printf("정점 입력 (0~9): ");
                scanf("%d", &vertex);
                if (vertex >= 0 && vertex < MAX_VERTICAL_NUM) {
                    insertVertex(graph, vertex); // 정점 삽입
                    printf("정점 %d 삽입.\n", vertex);
                } else
                    printf("Invalid vertex.\n");
                
                break;
            case 'e':
                printf("간선 입력 (0~9): ");
                scanf("%d", &start);
                scanf("%d", &end);
                if (start >= 0 && start < MAX_VERTICAL_NUM && end >= 0 && end < MAX_VERTICAL_NUM) {
                    insertEdge(graph, start, end); // 간선 삽입
                    printf("간선 (%d -> %d) 삽입.\n", start, end);
                } else {
                    printf("Invalid edge.\n");
                }
                break;
            case 'd':
                printf("DFS (0~9): ");
                scanf("%d", &start);
                if (start >= 0 && start < MAX_VERTICAL_NUM) {
                    clearVisited(graph); // graph 방문 배열 초기화
                    printf("[DFS] 순회 결과: %d: ", start);
                    depthFirstSearch(graph, start); // dfs 시작
                    printf("\n");
                } else
                    printf("Invalid vertex number.\n");
                
                break;
            case 'b':
                printf("BFS (0~9): ");
                scanf("%d", &start);
                if (start >= 0 && start < MAX_VERTICAL_NUM) {
                    clearVisited(graph); // graph 방문 배열 초기화
                    printf("[BFS] 순회 결과 %d: ", start);
                    breadthFirstSearch(graph, start); // bfs 시작
                    printf("\n");
                } else 
                    printf("Invalid vertex number.\n");
                
                break;
            case 'p':
                printGraph(graph);
                break;
            case 'q':
                freeGraph(graph); // 동적 해제
                printf("Quitting program.\n");
                return 0;
            default:
                printf("Invalid command.\n");
        }
    }
    return 0;
}

// 그래프 초기화
Graph* initGraph() {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (graph == NULL) { // 메모리 할당 실패 시 NULL 반환
        printf(stderr, "메모리 할당 실패\n");
        return NULL; 
    }
    for (int i = 0; i < MAX_VERTICAL_NUM; i++) {
        // graph 초기화
        graph->adjList[i] = NULL;
        graph->visited[i] = false;
        graph->exists[i] = false; 
    }
    return graph;
}

// vertex 삽입
void insertVertex(Graph* graph, int vertex) {
    if (!graph->exists[vertex]) // 정점이 존재하지 않으면
        graph->exists[vertex] = true; // true로 변경
    else
        printf("정점이 이미 존재합니다.\n");
    
}

// edge 삽입
void insertEdge(Graph* graph, int start, int end) {
    if (graph->exists[start] && graph->exists[end]) { // start, end의 정점 존재하면
        Node* newNode = createNode(end); // 새 노드 생성
        if (graph->adjList[start] == NULL) // 인접리스트가 비어있으면
            graph->adjList[start] = newNode; // 할당
        else {
            Node* temp = graph->adjList[start]; // start의 인접리스트
            Node* prev = NULL; // prev
            
            // 정렬된 위치를 찾아 삽입
            while (temp != NULL && temp->vertex < end) { // end보다 작을 때 까지 (정렬을 위하여)
                prev = temp;
                temp = temp->next;
            }

            if (prev == NULL) { // 삽입할 위치가 리스트의 맨 앞인 경우 -> 맨 앞으로 변경
                newNode->next = graph->adjList[start];
                graph->adjList[start] = newNode;
            } else { // 삽입할 위치가 중간 또는 끝인 경우 -> 중간에 삽입
                prev->next = newNode;
                newNode->next = temp;
            }

        }
    } else { // start, end의 정점이 존재하지 않으면
        printf("정점 삽입 오류\n");
    }
}


// DFS 
void depthFirstSearch(Graph* graph, int vertex) {
    if (!graph->exists[vertex]) { // 해당 정점이 없을 경우
        printf("정점이 존재하지 않습니다.\n");
        return;
    }
    
    Node* pTemp = graph->adjList[vertex]; // 파라미터로 받은 정점의 인접리스트

    graph->visited[vertex] = true; // 파라미터로 받은 정점 방문 처리
    printf("%d ", vertex); // 방문한 정점을 출력

    while (pTemp != NULL) {
        int connectedVertex = pTemp->vertex; // 인접한 정점

        if (!graph->visited[connectedVertex]) // 인접한 정점이 방문하지 않았다면
            depthFirstSearch(graph, connectedVertex); // 재귀 호출
        
        pTemp = pTemp->next; // 다음 이동
    }
}

// BFS
void breadthFirstSearch(Graph* graph, int startVertex) {
    if (!graph->exists[startVertex]) { // 시작 정점이 없을 경우
        printf("정점이 존재하지 않습니다.\n"); 
        return;
    }
    enqueue(startVertex); // 큐 자료구조를 활용하여 bfs 방식 구현
    graph->visited[startVertex] = true; // 시작 정점 방문 처리

    while (!isEmpty()) { // 큐가 비어있지 않다면 
        int currentVertex = dequeue(); // dequeue
        printf("%d ", currentVertex); // 뺀 값(방문) 출력

        Node* temp = graph->adjList[currentVertex]; // 해당 인접리스트 가져오기

        while (temp) { // 비어있을 때 까지
            int adjVertex = temp->vertex; // 인접한 정점

            if (!graph->visited[adjVertex]) { // 인접한 정점이 방문하지 않았다면
                graph->visited[adjVertex] = true; // 방문 처리
                enqueue(adjVertex); // 인접한 정점을 enqueue
            }
            temp = temp->next; // 다음 순환
        }
    }
}

// 그래프 출력
void printGraph(Graph* graph) {
    for (int i = 0; i < MAX_VERTICAL_NUM; i++) {
        if (graph->exists[i]) { // Vertex 존재한다면
            Node* pTemp = graph->adjList[i];
            printf("\n정점 %d: ", i);
            while (pTemp) {
                printf("%d -> ", pTemp->vertex);
                pTemp = pTemp->next;
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
            Node* pTemp = temp;
            temp = temp->next;
            free(pTemp); // 인접리스트 안 노드 메모리 해제
        }
    }
    free(graph); // graph 메모리 해제
}

// 방문 배열 초기화
void clearVisited(Graph* graph) {
    for (int i = 0; i < MAX_VERTICAL_NUM; i++) {
        graph->visited[i] = false;
    }
}

// 새로운 노드 생성
Node* createNode(int vertex) {
    Node* newNode = (Node*)malloc(sizeof(Node)); // 새로운 노드 동적 할당
    
    if (newNode == NULL) {
        printf("메모리 할당 실패\n");
        return; // 메모리 할당 실패 시 NULL 반환
    }
    newNode->vertex = vertex; // vertex 삽입
    newNode->next = NULL; // 다음 node NULL
    return newNode;
}

// enqueue
void enqueue(int vertex) {
    if (rear == MAX_VERTICAL_NUM - 1) // 꽉 차있다면
        return;
    if (isEmpty()) // queue empty
        front = 0;
    
    queue[++rear] = vertex;
}

// dequeue
int dequeue() {
    if (isEmpty()) 
        return -1; // queue empty
    
    int dequeued = queue[front];
    if (front >= rear) 
        front = rear = -1; // 큐 비어있는 처리
    else
        front++;
    
    return dequeued;
}

// 큐가 비어있는지 확인
bool isEmpty() {
    return front == -1;
}