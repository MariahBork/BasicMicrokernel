#ifndef TRAP_H
#define TRAP_H

#include <stdint.h>

// Função que trata as interrupções e exceções
// Recebe um ponteiro para os registradores salvos na stack
// Esse ponteiro é passado pelo trap_entry.S em assembly
void trap_handler(uint64_t *frame);

#endif
