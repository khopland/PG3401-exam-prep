#include <stdio.h>
#include "debugger.h"

int main(void)
{
    debug("test %d", 1);
    debug("test %d", 2);
    debug("test %d", 2);
    debug("test %d", 2);
    error("error test");
    return 0;
}