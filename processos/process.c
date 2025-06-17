#include "process.h"
#include <stdio.h>
#include <stdlib.h>

Process* create_process(int pid, int total_time){
    Process* p = (Process*)malloc(sizeof(Process));
    if(p == NULL) {
        printf("Erro ao alocar o processo\n");
        return 0;
    }
    p->pid = pid;
    p->state = PRONTO;
    p->total_cpu_time = 0;
    p->remaining_time = total_time;
    p->quantum_remaining = 0;
    p->next = NULL;
    return p;
}

void destroy_process(Process* p) {
    free(p);
}

const char* tratamento_estado(ProcessState estado){
    switch(estado) {
        case PRONTO: return "PRONTO";
        case EXECUTANDO: return "EXECUTANDO";
        case BLOQUEADO: return "BLOQUEADO";
        case TERMINADO: return "TERMINADO";
    }
}

void print_process(const Process *p) {
    printf("PID: %d | Estado: %s | CPU: %d | Restante: %d | Quantum: %d\n", p->pid, tratamento_estado(p->state), p->total_cpu_time, p->remaining_time, p->quantum_remaining);
}

void testa_codigo (){
    Process* p = create_process(99,35);
    print_process(p);
    destroy_process(p);
}
