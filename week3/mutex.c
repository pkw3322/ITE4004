// mutex: lock,unlock
// semaphore: 순서를 정해줌
// process, thread

// 1. Bingo 5X5
// 2. 틱택토 5X5
// 2명이서 하는 게임이기 때문에 mutax와 semaphore 적절하게 섞어서 진행되어야 함

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>

#define MAX_THREAD 100

long long num = 0;
pthread_mutex_t mutex;

void* thread_inc(void* arg);
void* thread_dec(void* arg);

int main(int argc,char* argv[]){
    pthread_t thread_id[MAX_THREAD];
    int i;

    pthread_mutex_init(&mutex,NULL);

    for (i = 0; i < MAX_THREAD; i++){
        if(i%2){
            pthread_create(&thread_id[i],NULL,thread_inc,NULL);
        }
        else{
            pthread_create(&thread_id[i],NULL,thread_dec,NULL);
        }
    }
    for(i = 0; i < MAX_THREAD; i++){
        pthread_join(thread_id[i],NULL);
    }
    printf("result: %lld\n", num);
    pthread_mutex_destroy(&mutex);
    return 0;
}

void* thread_inc(void *arg){
    int i;
    pthread_mutex_lock(&mutex);
    for(i = 0; i < 10000000; i++){
        num += 1;
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* thread_dec(void *arg){
    int i;
    pthread_mutex_lock(&mutex);
    for(i = 0; i < 10000000; i++){
        num -= 1;
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}