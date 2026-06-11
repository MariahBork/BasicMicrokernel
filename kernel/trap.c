#include "trap.h"
#include "timer.h"
#include "scheduler.h"

void trap_handler(uint64_t *frame) {
    unsigned long scause;
    
    // Lê o CSR scause que diz a CAUSA do trap
    __asm__ volatile ("csrr %0, scause" : "=r"(scause));
    
    // Bit 63 = 1 significa que é uma INTERRUPÇÃO (não exceção)
    // Exceções são erros tipo instrução inválida, page fault
    if (scause & (1UL << 63)) {
        // Pega o código da interrupção (últimos 8 bits)
        long code = scause & 0xFF;
        
        // Código 5 = Supervisor Timer Interrupt
        if (code == 5) {
            // Programa a próxima interrupção
            timer_next();
            
            // Chama o escalonador para trocar de task
            // O frame tem os registradores da task que foi interrompida
            schedule_from_trap(frame);
        }
    }
    // Se não for interrupção de timer, a gente ignora
    // Nesse trabalho só precisamos tratar o timer mesmo
}
