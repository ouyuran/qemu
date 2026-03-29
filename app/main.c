#define MYDEV (*(volatile unsigned int*)0x40000000)

void _start(void)
{
    MYDEV = 123;   // 写寄存器

    while (1);
}