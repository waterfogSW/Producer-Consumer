#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 3

char buffer[MAX];
int fill_ptr = 0;
int use_ptr = 0;
int count = 0;
int loops = 3;

pthread_cond_t empty, fill, pop;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct Enqueue_arg {
	char            _value;     // Push value of queue
}EA;

typedef struct Dequeue_arg {
    int             volume;
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

void *producer(void *input) {
    char *car = (char*)input;
    for (int i = 0; i < 9; i++) {
        pthread_mutex_lock(&mutex);
        while (count == MAX)
            pthread_cond_wait(&empty, &mutex);
        put(car[i]);
        pthread_cond_broadcast(&fill);
        pthread_mutex_unlock(&mutex);
    }
}

void *consumer_A(void *multi_arg) {
    DA *arg = (DA *)multi_arg;
    char *value  = arg->_value;
    for (int i = 0; i < arg->volume; i++) {
        pthread_mutex_lock(&mutex);
        while (count == 0)
            pthread_cond_wait(&fill, &mutex);
        while (buffer[use_ptr] != 'A')
            pthread_cond_wait(&pop, &mutex);
        get(value);
        pthread_cond_broadcast(&pop);
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
    }
}

void *consumer_B(void *multi_arg) {
    DA *arg = (DA *)multi_arg;
    char *value  = arg->_value;

    for (int i = 0; i < arg->volume; i++) {
        pthread_mutex_lock(&mutex);
        while (count == 0)
            pthread_cond_wait(&fill, &mutex);
        while (buffer[use_ptr] != 'B')
            pthread_cond_wait(&pop, &mutex);
        get(value);
        pthread_cond_broadcast(&pop);
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
    }
}

void *consumer_C(void *multi_arg) {
    DA *arg = (DA *)multi_arg;
    char *value  = arg->_value;

    for (int i = 0; i < arg->volume; i++) {
        pthread_mutex_lock(&mutex);
        while (count == 0)
            pthread_cond_wait(&fill, &mutex);
        while (buffer[use_ptr] != 'C')
            pthread_cond_wait(&pop, &mutex);
        get(value);
        pthread_cond_broadcast(&pop);
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

    char input[9] = {'A','A','B','C','A','C','C','B','B'};

    DA dequeue_arg1;
    char pop_value;
	dequeue_arg1._value = &pop_value;
    dequeue_arg1.volume = 3;

    DA dequeue_arg2;
	dequeue_arg2._value = &pop_value;
    dequeue_arg2.volume = 3;

    DA dequeue_arg3;
	dequeue_arg3._value = &pop_value;
    dequeue_arg3.volume = 3;

    pthread_create(&p, NULL, producer,&input);
    pthread_create(&c1, NULL, consumer_A,&dequeue_arg1);
    pthread_create(&c2, NULL, consumer_B,&dequeue_arg2);
    pthread_create(&c3, NULL, consumer_C,&dequeue_arg3);

    pthread_join(p,NULL);
    pthread_join(c1,NULL);
    pthread_join(c2,NULL);
    pthread_join(c3,NULL);

    // pthread_join(c3,NULL);
    printf("\n");
}