#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
int Tribunales, Carabineros;
int MaxProcesados = 10;

pthread_mutex_t mutex, lock;
sem_t *sem_vacio, *sem_lleno;

pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;
char arrestado, formalizado;
const char *datos[2] = {"Lorem", "Ipsum"};
char pasarela[MaxProcesados];
int head, tail, size;

void *SubirDato(void *arg){
    pthread_mutex_lock(&lock);              //Pone a esperar los hilos comisarías hasta
    pthread_cond_wait(&cond_var, &lock);    //que se creen los tribunales para así                                      
    pthread_mutex_unlock(&lock);            //evitar que se llene la pasarela
    sem_wait(sem_vacio);                    // Opciónal: ciclo for i hasta i*Cantidad de arrestados que subira solo una comisaria
    pthread_mutex_lock(&mutex);
    if (size < MaxProcesados){
        if(head == MaxProcesados){
            if(pasarela[0] == NULL){
                head = 0;
            }else{
                printf("error Sistema lleno");
            }
        }
        pasarela[head] = datos;
        arrestado = pasarela[head][0];
        sleep(1);
        head++;
        printf("Subiendo arrestado%d \n", arrestado);
        Carabineros--;
        size++;
    }else{
        printf("Sistema lleno");
    }
    pthread_mutex_lock(&mutex);
    sem_post(llena);
}

void *ProcesarFormalizar(void *arg){
    pthread_mutex_lock(&lock);            
    pthread_cond_post(&cond_var, &lock);         //Liberamos hilos comisaria                              
    pthread_mutex_unlock(&lock);
    sem_wait(sem_lleno);                // Opciónal: ciclo for i hasta i*Cantidad de formalizados
    pthread_mutex_lock(&mutex);
    Tribunales--;
    if(size > 0){
        if(tail == MaxProcesados){
            if(pasarela[0] != NULL){
                tail = 0;
            }else{
                printf("error Sistema vacio");
            }
        }
        formalizado = pasarela[tail][0];
        pasarela[tail] = NULL;
        sleep(1);
        tail++;
        printf("Procesando y formalizando %d \n", formalizado);
        size--;
    }else{
        printf("Sistema vacio");
    }
    if(size > 0){
    Tribunales++;
    }
    pthread_mutex_unlock(&mutex);
    sem_post(vacio);
    pthread_exit(NULL);
}


int main (int argc, char *argv[]){       //Recibe el valor de k, m, n
    //MaxProcesados = atoi(argv[1]);
    Tribunales = atoi(argv[2]);
    Carabineros = atoi(argv[3]);
    pthread_t comisariaID, tribunalID, restantesID;

    sem_init(&sem_vacio, 0, MaxProcesados);
    sem_init(&sem_lleno, 0, 0);
    
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&lock, NULL);

    int auxCarabineros = Carabineros;
    int auxTribunales = Tribunales;
    for (int i = 0; i <= auxCarabineros, i++){
        pthread_create(&comisariaID, NULL, SubirDato, NULL);
    }
    while(Tribunales){
        pthread_create(&tribunalID, NULL, ProcesarFormalizar, NULL);
    }
    pthread_join(tribunalID, NULL);
    pthread_join(comisariaID, NULL);

    return 0;
}
