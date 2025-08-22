// Projeto Grupo 5
// Francisco Arruda, Miguel Pacheco, João Machado

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>


#define TRUE 1
#define FALSE 0
#define TABLE2 2 // Número de mesas com 2 lugares
#define TABLE4 2 // Número de mesas com 4 lugares
#define TABLE6 2 // Número de mesas com 6 lugares
#define TABLE_MAX (TABLE2 + TABLE4 + TABLE6) // Número total de mesas no restaurante
#define MAX_TIME 5  // Determina o tempo máximo para as operações de sleep em diversas partes do código
#define TIME_LIMIT_SECONDS 60 // Tempo, em segundos, que o programa irá correr


pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER; // Lock mutex usado na fila
pthread_mutex_t restaurant_mutex = PTHREAD_MUTEX_INITIALIZER; // Lock mutex usado nas operações do restaurante
sem_t table_semaphores[TABLE_MAX]; // Semáforo para controlar o acesso às mesas
time_t START_TIME; // Data em que o restaurante foi inicializado
int keep_running = TRUE; // Mantém as threads a funcionar se for TRUE e termina-as se for FALSE


// Estrutura de dados de um grupo de clientes
typedef struct {
    int size; // Tamanho de um grupo de clientes
    time_t arrival_time; // Tempo de chegada de um grupo de clientes ao restaurante
    time_t waiting_time; // Tempo de espera na fila até ter uma mesa disponível para o grupo
} CustomerGroup;

// Estrutura de dados de um nó da fila
typedef struct Node {
    CustomerGroup value; // Grupo de clientes associado ao nó
    struct Node* next; // Nó seguinte
} Node;

// Estrutura de dados da fila de espera
typedef struct {
    Node* front; // Início da fila
    Node* rear; // Fim da fila
} Queue;

// Estrutura de dados de uma mesa
typedef struct {
    int table_size; // Número de lugares disponível na mesa
    int occupied; // Sinalização se a mesa está ocupada ou livre
} Table;

// Estrutura de dados do restaurante
typedef struct {
    Table tables[TABLE_MAX]; // Conjunto das mesas do restaurante
} Restaurant;

// Argumentos usados em algumas threads
struct ThreadArgs {
    Queue* queue; // Fila de espera
    Restaurant* restaurant; // Restaurante
};


// Adiciona um item (grupo de clientes) à fila
void enqueue(Queue* q, CustomerGroup item);
// Remove e retorna um item da fila
CustomerGroup dequeue(Queue* q);
// Retorna o tamanho atual da fila
int getQueueSize(Queue* q);
// Retorna o número de mesas ocupadas no restaurante
int getOccupiedTableCount(Restaurant* restaurant);
// Função executada por uma thread para simular a chegada de grupos de clientes ao restaurante
void* customerArrival(void* arg);
// Função executada por uma thread para alocar mesas aos grupos de clientes
void* allocateTable(void* arg);
// Função executada por uma thread para simular a saída de grupos de clientes do restaurante
void* customerDeparture(void* arg);
// Inicializa o restaurante, definindo o número e tamanhos das mesas
void startRestaurant(Restaurant* restaurant);


// Função que adiciona um item (grupo de clientes) à fila
void enqueue(Queue* q, CustomerGroup item) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->value = item;
    newNode->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

// Função que remove e retorna um item da fila
CustomerGroup dequeue(Queue* q) {
    Node* temp = q->front;
    CustomerGroup item = temp->value;
    q->front = temp->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(temp);
    return item;
}

// Função que retorna o número de mesas ocupadas no restaurante
int getQueueSize(Queue* q) {
    Node* current = q->front;
    int size = 0;
    while (current != NULL) {
        size += 1;
        current = current->next;
    }
    return size;
}

// Função que retorna o número de mesas ocupadas no restaurante
int getOccupiedTableCount(Restaurant* restaurant) {
    int count = 0;
    for (int i = 0; i < TABLE_MAX; ++i) {
        if (restaurant->tables[i].occupied) {
            count++;
        }
    }
    return count;
}

// Função executada por uma thread para simular a chegada de grupos de clientes ao restaurante
void* customerArrival(void* arg) {
    Queue* q = (Queue*)arg;
    while (keep_running) {
        sleep(1 + rand() % MAX_TIME);
        pthread_mutex_lock(&queue_mutex);
        CustomerGroup group = {.size = rand() % 6 + 1, .arrival_time = (time(NULL) - START_TIME)};
        enqueue(q, group);
        printf("    + Grupo de %d pessoas chegou ao restaurante no segundo %ld. Existem %d grupos na fila.\n", group.size, group.arrival_time, getQueueSize(q));
        pthread_mutex_unlock(&queue_mutex);
    }
    return NULL;
}

// Função executada por uma thread para alocar mesas aos grupos de clientes
void* allocateTable(void* arg) {
    struct ThreadArgs* threadArgs = (struct ThreadArgs*)arg;
    Queue* queue = threadArgs->queue;
    Restaurant* restaurant = threadArgs->restaurant;
    while (keep_running) {
        pthread_mutex_lock(&queue_mutex);
        if (queue->front != NULL) {
            pthread_mutex_unlock(&queue_mutex);
            pthread_mutex_lock(&restaurant_mutex);
            CustomerGroup earliestGroup = {.arrival_time = time(NULL) + 1};
            int earliestGroupIndex = -1;
            int allocated = FALSE;
            for (int i = 0; i < TABLE_MAX; ++i) {
                sem_wait(&table_semaphores[i]);
                if (!restaurant->tables[i].occupied) {
                    Node* current = queue->front;
                    int currentIndex = 0;
                    while (current != NULL) {
                        if (current->value.size <= restaurant->tables[i].table_size && current->value.arrival_time < earliestGroup.arrival_time) {
                            earliestGroup = current->value;
                            earliestGroupIndex = currentIndex;
                            allocated = TRUE;
                        }
                        current = current->next;
                        currentIndex += 1;
                    }
                }
                sem_post(&table_semaphores[i]);
            }
            if (allocated) {
                Node* current = queue->front;
                Node* previous = NULL;
                int currentIndex = 0;
                while (currentIndex < earliestGroupIndex) {
                    previous = current;
                    current = current->next;
                    currentIndex += 1;
                }
                if (previous != NULL) {
                    previous->next = current->next;
                } else {
                    queue->front = current->next;
                }
                if (current == queue->rear) {
                    queue->rear = previous;
                }
                free(current);
                for (int i = 0; i < TABLE_MAX; ++i) {
                    if (restaurant->tables[i].table_size >= earliestGroup.size && !restaurant->tables[i].occupied) {
                        sem_wait(&table_semaphores[i]);
                        restaurant->tables[i].occupied = TRUE;
                        earliestGroup.waiting_time = time(NULL) - earliestGroup.arrival_time - START_TIME;
                        sem_post(&table_semaphores[i]);
                        printf("* Grupo de %d pessoas sentado na mesa %d para %d pessoas passados %ld segundos na fila. Estão %d grupos de pessoas no restaurante.\n", 
                        earliestGroup.size, i, restaurant->tables[i].table_size, earliestGroup.waiting_time, getOccupiedTableCount(restaurant));
                        break;
                    }
                }
            }
            pthread_mutex_unlock(&restaurant_mutex);
        } else {
            pthread_mutex_unlock(&queue_mutex);
        }
        sleep(1);
    }
    return NULL;
}

// Função executada por uma thread para simular a saída de grupos de clientes do restaurante
void* customerDeparture(void* arg) {
    Restaurant* restaurant = (Restaurant*)arg;
    while (keep_running) {
        sleep(1 + rand() % MAX_TIME);
        pthread_mutex_lock(&restaurant_mutex);
        int tableIndex = rand() % TABLE_MAX;
        if (restaurant->tables[tableIndex].occupied == TRUE) {
            sem_wait(&table_semaphores[tableIndex]);
            restaurant->tables[tableIndex].occupied = FALSE;
            printf("- Grupo saindo da mesa %d. Estão %d grupos de pessoas no restaurante.\n", tableIndex, getOccupiedTableCount(restaurant));
            sem_post(&table_semaphores[tableIndex]);
        }
        pthread_mutex_unlock(&restaurant_mutex);
    }
    return NULL;
}

// Função que inicializa o restaurante, definindo o número e tamanhos das mesas
void startRestaurant(Restaurant* restaurant) {
    printf("\nPortas do restaurante abertas!\n\n");
    int num_mesa = 0;
    // Adiciona mesas com 2 lugares ao restaurante
    for (int i = 0; i < TABLE2; ++i) {
        restaurant->tables[num_mesa].table_size = 2;
        restaurant->tables[num_mesa].occupied = FALSE;
        sem_init(&table_semaphores[num_mesa], 0, 1);
        printf("    -> Mesa %d com %d lugares disponíveis.\n", num_mesa, 2);
        num_mesa += 1;
    }
    // Adiciona mesas com 4 lugares ao restaurante
    for (int i = 0; i < TABLE4; ++i) {
        restaurant->tables[num_mesa].table_size = 4;
        restaurant->tables[num_mesa].occupied = FALSE;
        sem_init(&table_semaphores[num_mesa], 0, 1);
        printf("    -> Mesa %d com %d lugares disponíveis.\n", num_mesa, 4);
        num_mesa += 1;
    }
    // Adiciona mesas com 6 lugares ao restaurante
    for (int i = 0; i < TABLE6; ++i) {
        restaurant->tables[num_mesa].table_size = 6;
        restaurant->tables[num_mesa].occupied = FALSE;
        sem_init(&table_semaphores[num_mesa], 0, 1);
        printf("    -> Mesa %d com %d lugares disponíveis.\n", num_mesa, 6);
        num_mesa += 1;
    }
    //Data em que o restaurante foi inicializado
    START_TIME = time(NULL);
}

// Função principal do programa
int main() {
    // Random Number Seed (para haver sempre resultados diferentes)
    srand((unsigned)time(NULL));
    // Redireciona o output para um ficheiro
    freopen("so-proj-G-5.txt", "w", stdout);
    // Inicializa o restaurante
    Restaurant restaurant;
    startRestaurant(&restaurant);
    // Inicializa a fila
    Queue queue = {NULL, NULL};
    // Inicializa as threads
    pthread_t threadArrival, threadAllocation, threadDeparture;
    struct ThreadArgs threadArgs = {&queue, &restaurant};
    pthread_create(&threadArrival, NULL, customerArrival, &queue);
    pthread_create(&threadAllocation, NULL, allocateTable, &threadArgs);
    pthread_create(&threadDeparture, NULL, customerDeparture, &restaurant);
    // Aguarda um número específico de segundos para depois avançar e terminar o programa
    sleep(TIME_LIMIT_SECONDS);
    // Sinaliza as threads para pararem
    keep_running = FALSE;
    pthread_join(threadArrival, NULL);
    pthread_join(threadAllocation, NULL);
    pthread_join(threadDeparture, NULL);
    // Fecha o ficheiro com o output
    fclose(stdout);
    // Termina o programa
    return 0;
}

// Comandos para correr o programa
// gcc -pthread so-proj-G-5.c -o so-proj-G-5.x
// ./so-proj-G-5.x
