// Função chamada pelo trap_handler para trocar de task durante interrupção
// Recebe um frame com os registradores salvos na stack
void schedule_from_trap(uint64_t *frame) {
    // Só troca se tiver mais de uma task
    if (task_count < 2) return;
    
    extern TCB tasks[];
    extern int task_count;
    extern int current;
    extern sched_algo_t current_algo;
    
    int prev = current;           // task que estava rodando
    int next = current_algo();    // próxima task pelo Round-Robin
    
    if (prev == next) return;     // mesma task, não faz nada
    
    // SALVA o contexto da task atual (do frame para o TCB)
    for (int i = 0; i < 31; i++) {
        tasks[prev].regs[i] = frame[i];
    }
    
    // SALVA o CSR sepc (endereço onde a task foi interrompida)
    unsigned long sepc_val;
    __asm__ volatile ("csrr %0, sepc" : "=r"(sepc_val));
    tasks[prev].sepc = sepc_val;
    
    // MUDA para a próxima task
    current = next;
    
    // RESTAURA o contexto da nova task (do TCB para o frame)
    for (int i = 0; i < 31; i++) {
        frame[i] = tasks[next].regs[i];
    }
    
    // RESTAURA o CSR sepc da nova task
    __asm__ volatile ("csrw sepc, %0" :: "r"(tasks[next].sepc));
}
