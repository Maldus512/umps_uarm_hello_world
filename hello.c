#include "system.h"
#include "termprint.h"

int main(void)
{
	unsigned const buffsize = 64;
    char str[buffsize];
    int i;

	term_puts("ENTER SOME TEXT\n> ");

    i = term_gets(str, buffsize);
	str[i] = '\0';
    term_puts(str);

    return 0;
}
