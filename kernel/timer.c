#include "timer.h"

// Guarda o valor do intervalo configurado
// Valor padrão: 1 milhão de ciclos (~1 segundo no QEMU)
static uint64_t tick_interval = 1000000;

// Função auxiliar que faz a chamada para o OpenSBI
// O OpenSBI é o programa que roda em modo máquina e controla o hardware
static inline void sbi_set_timer(uint64_t stime_value) {
    // Coloca o valor do tempo no registrador a0
    // Coloca 0 no registrador a7 (código da função set_timer)
    // ecall = chamada para o OpenSBI
    __asm__ volatile (
        "mv a0, %0\n"
        "li a7, 0\n"
        "ecall\n"
        :: "r"(stime_value)
        : "a0", "a7"
    );
}

// Programa a próxima interrupção do timer
void timer_next(void) {
    uint64_t now;  // guarda o tempo atual
    
    // rdtime = instrução que lê o relógio do processador
    __asm__ volatile ("rdtime %0" : "=r"(now));
    
    // Programa para acordar depois do intervalo
    // Ex: agora = 1000, intervalo = 100 -> acorda em 1100
    sbi_set_timer(now + tick_interval);
}

// Inicializa o timer - deve ser chamada uma vez no kernel_main
void timer_init(uint64_t interval) {
    // Se passou um intervalo válido, usa ele
    if (interval != 0) {
        tick_interval = interval;
    }
    
    // Habilita o bit de interrupção de timer no CSR sie
    // sie = Supervisor Interrupt Enable (controla quais interrupções o kernel aceita)
    // Bit 5 = STIE (Supervisor Timer Interrupt Enable)
    unsigned long sie;
    __asm__ volatile ("csrr %0, sie" : "=r"(sie));
    sie |= (1 << 5);  // liga o bit 5
    __asm__ volatile ("csrw sie, %0" :: "r"(sie));
    
    // Habilita interrupções globais no CSR sstatus
    // Bit 1 = SIE (liga/desliga TODAS as interrupções)
    unsigned long sstatus;
    __asm__ volatile ("csrr %0, sstatus" : "=r"(sstatus));
    sstatus |= (1 << 1);  // liga o bit 1
    __asm__ volatile ("csrw sstatus, %0" :: "r"(sstatus));
    
    // Programa a PRIMEIRA interrupção
    timer_next();
}
