#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/sched.h>

char *buffer =NULL;
int pointer= 0;
int N_THREADS=0;
int BUFFER_SIZE=0;
int POLICY_TYPE=0;
int POLICY_PRIORITY = 0;

pthread_mutex_t mutex;

// volatile int running = 1;

void *run(void *data)
{
    int id = (int) data;
    char letra = 0x41+id;
    //while (running);

    printf("inicia thread %c\n", letra);
    while (pointer < BUFFER_SIZE){
        // preenche buffer

        pthread_mutex_lock(&mutex);
        buffer[pointer] = letra ; //colcar de forma dinamica
        pointer++;
        pthread_mutex_unlock(&mutex);
    }
}


void print_sched(int policy)
{
    int priority_min, priority_max;

    switch(policy){
        // TEMPO REAL
        case SCHED_FIFO: // 1
            printf("SCHED_FIFO");
            break;
        case SCHED_RR: // 2
            printf("SCHED_RR");
            break;
        // CFS
        case SCHED_NORMAL: // 0
            printf("SCHED_OTHER");
            break;
        case SCHED_BATCH: // 3
            printf("SCHED_BATCH");
            break;
        case SCHED_IDLE: // 5
            printf("SCHED_IDLE");
            break;
        default:
            printf("unknown\n");
    }
    priority_min = sched_get_priority_min(policy);
    priority_max = sched_get_priority_max(policy);
    printf(" PRI_MIN: %d PRI_MAX: %d\n", priority_min, priority_max);
}

int setpriority(pthread_t *thread, int newpolicy, int newpriority)
{
    int policy, ret;
    struct sched_param param;

    if(sched_get_priority_max(newpolicy) == 0){
        newpriority = 0;
    }
//    else {
//        printf("Prioridade inválida: MIN: %d, MAX: %d, recebida: %d\n", sched_get_priority_min(newpolicy), sched_get_priority_max(newpolicy), newpriority);
//        exit(0);
//    }

//    pthread_getschedparam(*thr, &policy, &param);
//    printf("current: ");
//    print_sched(policy);

    param.sched_priority = newpriority;
    ret = pthread_setschedparam(*thread, newpolicy, &param);
    if (ret != 0)
        perror("perror(): ");

    pthread_getschedparam(*thread, &policy, &param);
    printf("new: ");
    printf("priority %d  ", param.sched_priority);
    print_sched(policy);

    return 0;
}

int main(int argc, char **argv)
{
    if (argc < 5){
        printf("usage: %s <número_de_threads> <tamanho_do_buffer(kb)> <politica> <prioridade>\n\n", argv[0]);

        return 0;
    }

    N_THREADS = atoi(argv[1]);
    BUFFER_SIZE= atoi(argv[2])*1024;
    POLICY_TYPE= atoi(argv[3]);
    POLICY_PRIORITY= atoi(argv[4]);
    if(POLICY_TYPE < 0 || POLICY_TYPE > 6 || POLICY_TYPE == 4){
        printf("Lista de politicas disponíveis:\n");
        printf("0 - > SCHED_OTHER\n");
        printf("1 - > SCHED_FIFO\n");
        printf("2 - > SCHED_RR\n");
        printf("3 - > SCHED_BATCH\n");
        printf("5 - > SCHED_IDLE\n");
        exit(0);
    }

    pthread_t thr[N_THREADS];
    char *escalonados = calloc(BUFFER_SIZE, sizeof(*escalonados));               // buffers de chars escalonados
    int (*vezes_escalonadas)[1] = calloc(N_THREADS, sizeof(*vezes_escalonadas)); // calculador de qnts vezes foi escalonado

    buffer = (char*) malloc(sizeof(char) * BUFFER_SIZE);
    memset(buffer, 0, sizeof(char) * BUFFER_SIZE);

    pthread_mutex_init(&mutex, NULL);

    //cria tds threads
    for(int i=0; i< N_THREADS; i++){
        pthread_create(&thr[i], NULL, run, (void*) i);
        setpriority(&thr[i], POLICY_TYPE, (i+1)*2);
    }

    // espera execucao de tds threads
    for(int i=0; i< N_THREADS; i++){
        pthread_join(thr[i], NULL); 
    }

    printf("IMPRIMINDO BUFFER\n");
    int last_escalonado = 0;
    int esc_idx = 0;
    for(int i=0; i< BUFFER_SIZE; i++){
        //printf("%c",buffer[i]);
        if(buffer[i] != last_escalonado){
            last_escalonado = buffer[i];
            escalonados[esc_idx++] = last_escalonado;
        }
    }

    printf("\n\n");
    // imprime o "aglutinado" de escalonamento, e soma a qtd de vezes que foi escalonada
    for(int i = 0; i < esc_idx; i++){
        printf("%c", escalonados[i]);
        vezes_escalonadas[escalonados[i]-65][0] += 1;
    }

    printf("\n\n");
    // imprime LETRA = X vezes escalonada
    for(int i = 0; i < N_THREADS; i++){
        printf("%c = %d\n", i+65, vezes_escalonadas[i][0]);
    }

    free(vezes_escalonadas);
    free(escalonados);
    free(buffer); 
    return 0;
}
