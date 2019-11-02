#include "system.h"

#define ST_READY           1
#define ST_BUSY            3

#define CMD_ACK            1
#define CMD_PRINT          2


static dtpreg_t *print0_reg = (dtpreg_t *) DEV_REG_ADDR(IL_PRINTER, 0);

static int print_putchar(char c)
{
    unsigned int stat;

    stat = print0_reg->status;
    if (stat != ST_READY)
        return -1;

    print0_reg->data0 = c;
    print0_reg->command = CMD_PRINT;

    while ((stat = print0_reg->status) == ST_BUSY)
        ;

    print0_reg->command = CMD_ACK;

    return 0;
}


void print_puts(const char *str)
{
    while (*str)
        if (print_putchar(*str++))
            return;
}
