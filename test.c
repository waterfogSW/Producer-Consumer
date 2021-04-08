#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define LINE_SIZE 5

typedef struct {
    char    c_name;     // Car name
    int     star_t;     // Start time
    int     time_q;     // Time quantum
    int     rema_v;     // Remain Volume
    int     volume;     // Production volume
} Prod_Info;

typedef struct car_queue {
    char    *buffer;
	int     balance;
    int     front;
    int     rear;
    int     size;
} CarQueue;

CarQueue    cq;
int         p_num;
int         c_num;
int         total;

void init_info(int _total) {
    p_num = 0;
    c_num = 0;
    total = _total;
}

void init_cq() {
    cq.buffer   = (char*)malloc(sizeof(char) * LINE_SIZE);
    cq.balance  = 0;
    cq.front    = 0;
    cq.rear     = 0;
}

void put(char value) {
    cq.buffer[cq.rear] = value;
    cq.rear = (cq.rear + 1) % LINE_SIZE;
    cq.balance++;
    p_num++;
}

void get() {
    cq.front = (cq.front + 1) % LINE_SIZE;
    cq.balance--;
    c_num++;
}

void *u_prod(void *input) {
    char *car = (char*)input;
    while(p_num < total) {
        if(cq.balance < LINE_SIZE){
            put(car[p_num]);
        }
    }
}

void *u_consA(void *arg) {
    while(c_num < total) {
        if(cq.balance > 0 && cq.buffer[cq.front] != 'A') 
            get();
    }
}

void *u_consB(void *arg) {
    while(c_num < total) {
        if(cq.balance > 0 && cq.buffer[cq.front] != 'B') 
            get();
    }
}

void *u_consC(void *arg) {
    while(c_num < total) {
        if(cq.balance > 0 && cq.buffer[cq.front] != 'C') 
            get();
    }
}

void *u_consD(void *arg) {
    while(c_num < total) {
        if(cq.balance > 0 && cq.buffer[cq.front] != 'D') 
            get();
    }
}

void *u_consE(void *arg) {
    while(c_num < total) {
        if(cq.balance > 0 && cq.buffer[cq.front] != 'E') 
            get();
    }
}

void printQ() {
    for (int i = 0; i < cq.balance; i++) {
        int index = cq.front + i;
        printf("%c ", cq.buffer[index]);
    }
}

int main() {
    pthread_t p,c_a,c_b,c_c,c_d,c_e;
    init_info(9);
    init_cq();

    char input[9] = {'A','A','B','C','A','C','C','B','B'};
    
    pthread_create(&p,NULL,u_prod,&input);
    pthread_create(&c_a,NULL,u_consA,NULL);
    pthread_create(&c_b,NULL,u_consB,NULL);
    pthread_create(&c_c,NULL,u_consC,NULL);
    pthread_create(&c_d,NULL,u_consD,NULL);
    pthread_create(&c_e,NULL,u_consE,NULL);
    

    printQ();
}