#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Numero de threads e o intervalo de numeros que vai ser procurado a quantidade de numeros primos
#define num_threads 2
#define comeca_numero 1
#define ate_numero 1000

// Variavel que vai contar os primos e seu mutex
pthread_mutex_t mutex_resultado;
int resultado = 0;

// Funcao que verifica se o numero e primo
bool primo(int num) {
    if (num == 1){
        return false;
    }
    int k;
    int resto;
    for (k=2; k <= num/2; k++) {
        resto = num%k;
        if (resto==0) {
            return false;
        };
    };
    return true;
};

// Funcao para contar os primos
void conta_primos(int num) {
    num = num + comeca_numero;
    while (num < ate_numero) {
        if (primo(num)) {
            pthread_mutex_lock(&mutex_resultado);
            resultado = resultado + 1;
            pthread_mutex_unlock(&mutex_resultado);
        };
        num = num + num_threads;
    };
};

// Thread function
void *t_function_primos(void *arg) {
    int num = (int)arg;
    conta_primos(num);
};

// Execucao com threads
void exeucao_threads() {
    pthread_mutex_init(&mutex_resultado, NULL);
    pthread_t threads_p[num_threads];
    int i;
    for (i=0; i<num_threads; i++) {
        pthread_create(&threads_p[i], NULL, t_function_primos, i);
    };
    for (i=0; i<num_threads; i++) {
        pthread_join(threads_p[i], NULL);
    };
    pthread_mutex_destroy(&mutex_resultado);
    printf("Existem %d numeros primos no intervalo.\n", resultado);
}

// Execucao sequencial 
void execucao_sequencial() {
    int i;
    for (i=comeca_numero; i<ate_numero; i++) {
        if (primo(i)) {
            resultado = resultado + 1;
        };
    };
    printf("Existem %d numeros primos no intervalo.\n", resultado);
};

// Main
int main() {
    struct timeval t1, t2;
    gettimeofday(&t1, NULL);
    
    exeucao_threads();
    // execucao_sequencial();
    
    gettimeofday(&t2, NULL);
    double t_total = (t2.tv_sec - t1.tv_sec) + ((t2.tv_usec -
    t1.tv_usec)/1000000.0);
    printf("tempo total = %f\n", t_total);
    
    
}