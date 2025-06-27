#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>

// Inicializa o escalonador
void init_scheduler(Scheduler *sched, int quantum) {
    sched->ready_queue = NULL;
    sched->quantum = quantum;
}

// Adiciona processo à fila circular
void add_to_ready_queue(Scheduler *sched, Process *p) {
    p->state = PRONTO;
    p->quantum_remaining = sched->quantum;

    if (sched->ready_queue == NULL) {
        sched->ready_queue = p;
        p->next = p;
    } else {
        p->next = sched->ready_queue->next;
        sched->ready_queue->next = p;
        sched->ready_queue = p;
    }
}

// Escalona o próximo processo PRONTO
Process* schedule(Scheduler *sched) {
    if (sched->ready_queue == NULL) {
        return NULL;
    }

    Process *start = sched->ready_queue->next;
    Process *curr = start;

    do {
        if (curr->state == PRONTO) {
            curr->state = EXECUTANDO;
            curr->total_cpu_time++;
            curr->remaining_time--;
            curr->quantum_remaining--;

            if (curr->remaining_time <= 0) {
                curr->state = TERMINADO;
            }
            else if (curr->quantum_remaining <= 0) {
                curr->state = PRONTO;
                curr->quantum_remaining = sched->quantum;
                sched->ready_queue = curr;
            }
            return curr;
        }
        curr = curr->next;
    } while (curr != start);

    return NULL;
}

// Imprime a fila de prontos
void print_queue(const Scheduler *sched) {
    if (sched->ready_queue == NULL) {
        printf("Fila esta vazia.\n");
        return;
    }

    Process *start = sched->ready_queue->next;
    Process *curr = start;

    do {
        print_process(curr);
        curr = curr->next;
    } while (curr != start);
}