#include <stdio.h>
#include <stdlib.h>
#include "rr_scheduler.h"
#include "process.h"
#include "scheduler.h"

void handle_blocked_process(Scheduler* sched, Process* p) {
    if (sched->ready_queue == NULL || p == NULL) {
        return;
    }

    if (sched->ready_queue == p && sched->ready_queue->next == sched->ready_queue) {
        sched->ready_queue = NULL;
        return;
    }

    Process* current = sched->ready_queue;

    while (current->next != p && current->next != sched->ready_queue) {
        current = current->next;
    }

    if (current->next == p) {
        current->next = p->next;

        if (sched->ready_queue == p) {
            sched->ready_queue = current;
        }
    }
}

void handle_terminated_process(Scheduler* sched, Process* p) {
    if (sched->ready_queue == NULL || p == NULL) {
        return;
    }
    
    if (sched->ready_queue == p && sched->ready_queue->next == sched->ready_queue) {
        sched->ready_queue = NULL;
    } else {
        Process* current = sched->ready_queue;

        while (current->next != p && current->next != sched->ready_queue) {
            current = current->next;
        }

        if (current->next == p) {
            current->next = p->next;

            if (sched->ready_queue == p) {
                sched->ready_queue = current;
            }
        }
    }

    destroy_process(p);
}