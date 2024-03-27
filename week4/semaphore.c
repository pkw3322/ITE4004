#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<semaphore.h>

static sem_t sem_one;
static sem_t sem_two;
int num;

void* read(void* arg);
void* calc(void* arg);

int main(int argc,char *argv[]){
    pthread_t id_t1,id_t2;
    
    int state1 = sem_init(&sem_one,0,0);
    int state2 = sem_init(&sem_two,0,1);

    if(!state1 || !state2){
        puts("Semaphore initialization failed");
        exit(1);
    }
    
    pthread_create(&id_t1,NULL,read,NULL);
    pthread_create(&id_t2,NULL,calc,NULL);

    pthread_join(id_t1,NULL);
    pthread_join(id_t2,NULL);

    sem_destroy(&sem_one);
    sem_destroy(&sem_two);

    return 0;
}

void* read(void *arg){
    int i;
    for(i=0; i < 5; i++){
        fputs("Input num: ",stdout);
        sem_wait(&sem_one);
        scanf("%d",&num);
        sem_post(&sem_two);
    }
    return NULL;
}

void* calc(void *arg){
    int sum=0, i;
    for(i=0; i < 5; i++){
        sem_wait(&sem_two);
        sum += num;
        sem_post(&sem_one);
    }
    printf("Result: %d\n",sum);
    return NULL;
}