#include "task.h"
#include "scheduler.h"
#include "memory.h"
#include "timer.h"
#include "trap.h"
#include "uart.h"

// Função em assembly que é o ponto de entrada para traps
extern void trap_entry(void);

// Task 1 - não tem yield(), a troca é forçada pelo timer
void task1(void) {
    while (1) {
        uart_print("Task 1 running\n");
        // delay simples para não floodar o console
        for (volatile int i = 0; i < 100000; i++);
    }
}

// Task 2 - também sem yield()
void task2(void) {
    while (1) {
        uart_print("Task 2 running\n");
        for (volatile int i = 0; i < 100000; i++);
    }
}

void kernel_main(void) {
    // Inicializa o gerenciador de memória (heap de 64KB)
    memory_init();
    
    uart_print("\n=== Preemptive Round-Robin Kernel ===\n");
    uart_print("Starting scheduler...\n");
    
    // Cria as duas tasks com stack de 2KB cada
    xTaskCreate(task1, 2048, 1);
    xTaskCreate(task2, 2048, 1);
    
    // Configura o vetor de trap - diz ao processador para onde pular
    // quando acontecer uma interrupção ou exceção
    __asm__ volatile ("csrw stvec, %0" :: "r"(trap_entry));
    
    // Inicializa o timer com intervalo de 1 milhão de ciclos
    // No QEMU isso é aproximadamente 1 segundo
    timer_init(1000000);
    
    // Inicia o escalonador (começa executando a task1)
    scheduler_start();
    
    // Se chegar aqui (não deveria), fica em loop infinito
    while (1);
}
