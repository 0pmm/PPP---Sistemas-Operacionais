#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "process.h"
#include "scheduler.h"
#include "rr_scheduler.h"

#define MAX_PROCESSOS 10
#define PROB_CRIACAO 0.3
#define PROB_BLOQUEIO 0.1
#define TEMPO_MIN 3
#define TEMPO_MAX 15
#define PROB_DESBLOQUEIO 0.2

int main() {
    srand(time(NULL));
    Scheduler escalonador;
    int proximo_pid = 1;
    int ciclo = 0;

    // Inicializa o escalonador com quantum 4
    init_scheduler(&escalonador, 4);

    // Cria o primeiro processo para garantir que a simulação sempre começa com pelo menos um processo na fila
    Process *primeiro = create_process(proximo_pid++, TEMPO_MIN + rand() % (TEMPO_MAX - TEMPO_MIN + 1));
    add_to_ready_queue(&escalonador, primeiro);
    printf("Processo %d criado.\n", primeiro->pid);

    while (ciclo < 30) {
        ciclo++;
        printf("\n=== Ciclo %d ===\n", ciclo);

        // Tenta criar um novo processo com chance de 30%
        if ((double)rand() / RAND_MAX < PROB_CRIACAO && proximo_pid <= MAX_PROCESSOS) {
            int tempo = TEMPO_MIN + rand() % (TEMPO_MAX - TEMPO_MIN + 1);
            Process *novo = create_process(proximo_pid++, tempo);
            add_to_ready_queue(&escalonador, novo);
            // Aqui mostramos o PID do processo recém-criado. Não é o total de processos, é só o identificador único dele.
            printf("Processo %d criado.\n", novo->pid);
        }

        // Tenta desbloquear algum processo bloqueado com chance de 20% 
        if ((double)rand() / RAND_MAX < PROB_DESBLOQUEIO) {
            if (escalonador.ready_queue != NULL) {
                Process *start = escalonador.ready_queue->next;
                Process *curr = start;
                do {
                    if (curr->state == BLOQUEADO) {
                        curr->state = PRONTO;
                        printf("Processo %d desbloqueado!\n", curr->pid);
                        break;
                    }
                    curr = curr->next;
                } while (curr != start);
            }
        }

        // Chama o escalonador para decidir qual processo vai executar neste ciclo
        Process *executando = schedule(&escalonador);

        if (executando) {
            printf("Executando: ");
            print_process(executando);

            // Sorteia se o processo em execução será bloqueado neste ciclo (10% de chance)
            if ((double)rand() / RAND_MAX < PROB_BLOQUEIO) {
                executando->state = BLOQUEADO;
                printf("Processo %d bloqueado.\n", executando->pid);
                // Quando um processo está bloqueado, ele fica na fila mas não é escolhido para executar até ser desbloqueado
            } else if (executando->state == TERMINADO) {
                printf("Processo %d terminou!\n", executando->pid);
                handle_terminated_process(&escalonador, executando);
            }
        } else {
            printf("Nenhum processo para executar neste ciclo.\n");
        }

        
        printf("Fila de prontos:\n");
        print_queue(&escalonador);

        printf("Pressione ENTER para continuar...");
        getchar();

        // Se não houver mais processos prontos ou bloqueados, encerra a simulação
        if (escalonador.ready_queue == NULL) {
            printf("Todos os processos foram finalizados!\n");
            break;
        }
    }

    return 0;
}