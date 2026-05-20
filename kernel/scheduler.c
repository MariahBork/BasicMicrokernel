#include "scheduler.h"
#include "task.h"

extern void context_switch(void*, void*);

static int current = 0;

/* ---------- Round-Robin padrão ---------- */

static int round_robin()
{
    return (current + 1) % task_count;
}

/*
int priority_sched()
{
    int best = 0;

    for (int i = 1; i < task_count; i++)
        if (tasks[i].priority > tasks[best].priority)
            best = i;

    return best;
}


*/

/* ---------- Algoritmo atual ---------- */

static sched_algo_t current_algo = round_robin;

void scheduler_set_algorithm(sched_algo_t algo)
{
    if (algo)
        current_algo = algo;
}

/* ---------- Yield ---------- */

void yield()
{
    int prev = current;
    int next = current_algo();

    current = next;

    context_switch(tasks[prev].regs,
                   tasks[next].regs);
}

/* ---------- Início ---------- */

void scheduler_start()
{
    if (task_count == 0)
        return;

    tasks[0].entry();
}