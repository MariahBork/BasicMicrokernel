#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

// Inicializa o timer com um intervalo em ciclos de clock
// Ex: timer_init(1000000) = interrupção a cada ~1 segundo no QEMU
void timer_init(uint64_t interval);

// Programa a próxima interrupção do timer
// É chamada automaticamente dentro do trap_handler
void timer_next(void);

#endif
