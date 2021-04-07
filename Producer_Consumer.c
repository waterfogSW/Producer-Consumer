#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 10

char buffer[MAX];
int fill_ptr = 0;
int use_ptr = 0;
int count = 0;
int loops = 1;

pthread_cond_t empty, fill;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct Enqueue_arg {
	char            _value;     // Push value of queue
}EA;

typedef struct Dequeue_arg {
	char            *_value;     // Pop value of queue
}DA;

void put(char value) {
    buffer[fill_ptr] = value;
    fill_ptr = (fill_ptr + 1) % MAX;
    count++;
}

void get(char* value) {
    char tmp = buffer[use_ptr];
    use_ptr = (use_ptr + 1) % MAX;
    count--;
    *value = tmp;
}

void *producer(void *multi_arg) {
    EA *arg = (EA *)multi_arg;
    char value   = arg->_value;
    for (int i = 0; i < loops; i++) {
        pthread_mutex_lock(&mutex);
        while (count == MAX)
            pthread_cond_wait(&empty, &mutex);
        put(value);
        pthread_cond_broadcast(&fill);
        pthread_mutex_unlock(&mutex);
    }
}

void *consumer_A(void *multi_arg) {
    DA *arg = (DA *)multi_arg;
    char *value  = arg->_value;

    for (int i = 0; i < loops; i++) {
        pthread_mutex_lock(&mutex);
        while (count == 0 || buffer[use_ptr] != 'A')
            pthread_cond_wait(&fill, &mutex);
        get(value);
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
    }
}

void *consumer_B(void *multi_arg) {
    DA *arg = (DA *)multi_arg;
    char *value  = arg->_value;

    for (int i = 0; i < loops; i++) {
        pthread_mutex_lock(&mutex);
        while (count == 0 || buffer[use_ptr] != 'B')
            pthread_cond_wait(&fill, &mutex);
        get(value);
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
    }
}

void *consumer_C(void *multi_arg) {
    DA *arg = (DA *)multi_arg;
    char *value  = arg->_value;

    for (int i = 0; i < loops; i++) {
        pthread_mutex_lock(&mutex);
        while (count == 0 || buffer[use_ptr] != 'C')
            pthread_cond_wait(&fill, &mutex);
        get(value);
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
    }
}

void printQueue() {
    for (int i = 0; i < count; i++) {
        int index = (use_ptr + i) % MAX;
        printf("%c ", buffer[index]);
    }
    printf("\n");
}

int main() {
    pthread_t p,c1,c2,c3;

    EA *enqueue_arg1;
    enqueue_arg1 = (EA *)malloc(sizeof(EA));
    enqueue_arg1->_value = 'A';

    EA *enqueue_arg2;
    enqueue_arg2 = (EA *)malloc(sizeof(EA));
    enqueue_arg2->_value = 'B';

    EA *enqueue_arg3;
    enqueue_arg3 = (EA *)malloc(sizeof(EA));
    enqueue_arg3->_value = 'C';

    DA *dequeue_arg;
    char pop_value;
    dequeue_arg = (DA *)malloc(sizeof(DA));
	dequeue_arg->_value = &pop_value;

    pthread_create(&p, NULL, producer,enqueue_arg1);
    pthread_create(&p, NULL, producer,enqueue_arg2);
    pthread_create(&p, NULL, producer,enqueue_arg3);

    pthread_join(p,NULL);

    printQueue();

    pthread_create(&c1, NULL, consumer_A,dequeue_arg);
    pthread_create(&c2, NULL, consumer_B,dequeue_arg);
    pthread_create(&c3, NULL, consumer_C,dequeue_arg);

    pthread_join(c1,NULL);
    // pthread_join(c2,NULL);
    // pthread_join(c3,NULL);
    

    printQueue();
    printf("\n");
}