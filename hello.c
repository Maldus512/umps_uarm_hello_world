#include "term.h"
#include "printprint.h"
#include "system.h"

int main(void)
{
    char str[64];

    int i = term_gets(str, 64);
    str[i] = '\0';
    term_puts("Stringa letta!");

    print_puts("Stringa letta: ");
    print_puts(str);

    /* Go to sleep indefinetely */
    while (1) 
        WAIT();
    return 0;
}

