    /*
    Псевдокод

    Semaphore Mutex = 1;                    //Отвечает за разделяемую память
    Semaphore empty = N; // емкость буфера  //Показывает есть ли место в памяти
    Semaphore full = 0;

    //V - выставляет 
    //P - ждет

    Producer:
    wlile(1) {
        produce_item;
        P(empty);
        P(mutex);
        put_item;
        V(mutex);
        V(full);
    }

    Consumer:
    while(1) {
        P(full);
        P(mutex);
        get_item;
        V(mutex);
        V(empty);
        consumer_item;
    }
    */


   /*
   * Задача:
   * Есть 10 тарелок
   * Мыть тарелку - 1 секунда       (секунды ждем через sleep)
   * Вытирать тарелку - 3 секунды
   * На стол можно поcтавить 4 тарелки (N)
   * 
   * Указания:
   * Есть 2 процесса: 1 процесс - моет, 2 процесс - вытирает
   * Начальные значения семофоров:
   * Mutex = +1
   * empty = N = 4
   * full = 0
   * 
   * цикл из условно 10 тарелок
   * 
   * Процесс 1:
   * если не срабатывает empty => printf(vait)
   * printf("моет")
   * sleep(1)
   * "Кладет на стол" // считаем что мгновенно
   * 
   * Процесс 2:
   * "ждет" // в теории ждать не будет
   * "взял"
   * "вытер"
   * sleep(3)
   */


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

const int N = 4;            // How much plates on tables
const int PLATES = 10;      // All plates

sem_t mutex;                // Critical section semafore
sem_t empty;                // semofore to check if there is place
sem_t full;                 // semafore to check dirty plates

void* producer() {
    for (int i = 0; i < PLATES; i++) {
        sem_wait(&mutex); 
        sem_post(&full);
        printf("Моет тарелку %i\n", i);
        printf("Кладет на стол тарелку %i\n", i);
        sem_post(&mutex); 
        sem_wait(&empty);
        sleep(1);
    }
}

void* consumer() {
    for (int i = 0; i < PLATES; i++) {
        sem_wait(&mutex); 
        sem_post(&empty);
        printf("Взял тарелку %i\n", i);
        printf("Вытер тарелку %i\n", i);
        sem_post(&mutex); 
        sem_wait(&full);
        sleep(3);
    }
}


int main() {
    pthread_t prod, cons;

    // Initializing
    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, N);
    sem_init(&full, 0, 0);

    // Create proccess
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    // Wait until all process stop works
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    // delete sems
    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    return 0;
}

