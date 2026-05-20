#include <stdint.h>

#define UART0 0x10000000L

static inline void putc(char c)
{
    *(volatile uint8_t*)UART0 = c;
}

static void print(const char *s)
{
    while (*s)
        putc(*s++);
}

void kernel_main()
{
    print("Mini OS started!\n");

    while (1);
}