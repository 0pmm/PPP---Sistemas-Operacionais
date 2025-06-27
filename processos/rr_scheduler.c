#include <stdio.h>
#include <stdlib.h>
#include "rr_scheduler.h"
#include "process.h"
#include "scheduler.h"

// Remove processo bloqueado da fila de prontos
void handle_blocked_process(Scheduler* sched, Process* p) {
    if (sched->ready_queue == NULL || p == NULL) {
        return;
    }

    // Caso especial: único processo na fila
    if (sched->ready_queue == p && sched->ready_queue->next == sched->ready_queue) {
        sched->ready_queue = NULL;
        return;
    }

    Process* current = sched->ready_queue;

    // Procura o processo anterior na fila circular
    while (current->next != p && current->next != sched->ready_queue) {
        current = current->next;
    }

    // Remove o processo da fila
    if (current->next == p) {
        current->next = p->next;

        if (sched->ready_queue == p) {
            sched->ready_queue = current;
        }
    }
}

// Remove processo terminado da fila e libera memória
void handle_terminated_process(Scheduler* sched, Process* p) {
    if (sched->ready_queue == NULL || p == NULL) {
        return;
    }
    
    // Caso especial: único processo na fila
    if (sched->ready_queue == p && sched->ready_queue->next == sched->ready_queue) {
        sched->ready_queue = NULL;
    } else {
        Process* current = sched->ready_queue;

        // Procura o processo anterior na fila circular
        while (current->next != p && current->next != sched->ready_queue) {
            current = current->next;
        }

        // Remove o processo da fila
        if (current->next == p) {
            current->next = p->next;

            if (sched->ready_queue == p) {
                sched->ready_queue = current;
            }
        }
    }

    // Libera a memória do processo
    destroy_process(p);
}