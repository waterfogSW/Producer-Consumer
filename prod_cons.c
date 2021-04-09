#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define LINE_SIZE 10

char order[100] = {'D','D','D','D','D','A','B','C','A','C','E','E','E','E','E','A','C','B','B','A','A','B','C','A','C','C','B','B','A','A','B','C','A','C','C','B','B','A','A','B','C','A','C','C','B','B','A','A','B','C','A','C','C','B','B','A','A','B','C','A','C','C','B','B','A','A','B','C','A','C','C','B','B','A','A','B','C','A','C','C','B','B','A','A','B','C','A','C','C','B','B','A','A','B','C','A','C','C','B','B'};

char buffer[LINE_SIZE];
int rear = 0;
int front = 0;
int balance = 0;

int total = 100;
int pnum = 0;
int cnum = 0;

int info[5] = {30,30,30,5,5};

pthread_cond_t empty;
pthread_cond_t fill;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void put(char value) {
    buffer[rear] = value;
    rear = (rear + 1) % LINE_SIZE;
    pnum++;
    balance++;
}

void get(char* value) {
    char tmp = buffer[front];
    front = (front + 1) % LINE_SIZE;
    balance--;
    cnum++;
    *value = tmp;
}

void *c_prod(void *input) {
    for (int i = 0; i < total; i++) {
        pthread_mutex_lock(&mutex);
        while (balance == LINE_SIZE)
            pthread_cond_wait(&empty, &mutex);
        put(order[i]);
        pthread_cond_broadcast(&fill);
        pthread_mutex_unlock(&mutex);
    }
    while(balance > 0) {
        pthread_cond_broadcast(&fill);
    }
    return NULL;
}

void *c_consA(void *multi_arg) {
    char value;
    for (int i = 0; i < info[0]; i++) {
        pthread_mutex_lock(&mutex);
        while (balance == 0 || buffer[front] != 'A')
            pthread_cond_wait(&fill, &mutex);
        get(&value);
        printf("A: %c \n", value);
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *c_consB(void *multi_arg) {
    char value;
    for (int i = 0; i < info[1]; i++) {
        pthread_mutex_lock(&mutex);
        while (balance == 0 || buffer[front] != 'B')
            pthread_cond_wait(&fill, &mutex);
        get(&value);
        printf("B: %c \n", value);
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *c_consC(void *multi_arg) {
    char value;
    for (int i = 0; i < info[2]; i++) {
        pthread_mutex_lock(&mutex);
        while (balance == 0 || buffer[front] != 'C')
            pthread_cond_wait(&fill, &mutex);
        get(&value);
        printf("C: %c \n", value);
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *c_consD(void *multi_arg) {
    char value;
    for (int i = 0; i < info[3]; i++) {
        pthread_mutex_lock(&mutex);
        while (balance == 0 || buffer[front] != 'D')
            pthread_cond_wait(&fill, &mutex);
        get(&value);
        printf("D: %c \n", value);
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *c_consE(void *multi_arg) {
    char value;
    for (int i = 0; i < info[4]; i++) {
        pthread_mutex_lock(&mutex);
        while (balance == 0 || buffer[front] != 'E')
            pthread_cond_wait(&fill, &mutex);
        get(&value);
        printf("E: %c \n", value);
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void printQueue() {
    for (int i = 0; i < balance; i++) {
        int index = (front + i) % LINE_SIZE;
        printf("%c ", buffer[index]);
    }
    printf("\n");
}

int main() {
    pthread_t p,c1,c2,c3,c4,c5;

    pthread_create(&p, NULL, c_prod,NULL);
    pthread_create(&c1, NULL, c_consA,NULL);
    pthread_create(&c2, NULL, c_consB,NULL);
    pthread_create(&c3, NULL, c_consC,NULL);
    pthread_create(&c4, NULL, c_consD,NULL);
    pthread_create(&c5, NULL, c_consE,NULL);

    pthread_join(p,NULL);
    pthread_join(c1,NULL);
    pthread_join(c2,NULL);
    pthread_join(c3,NULL);
    pthread_join(c4,NULL);
    pthread_join(c5,NULL);

    printQueue();
    printf("%d %d", pnum, cnum);
    printf("\n");
}