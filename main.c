
#define STDOUT_FD 1

#define SYSCALL_NUM 0x80
#define WRITE_NUM 4

int main();

static int write( int fd, char* data, unsigned long long len );

// Tell the compiler this is freestanding
__attribute__((naked, noreturn))
void _start(void) {
    __asm__ volatile (
        // Align stack to 16 bytes before call
        "and $-16, %rsp\n"

        // Call main(argc, argv, envp)
        "call main\n"

        // exit(main_return)
        "mov %rax, %rdi\n"          // status
        "mov $60, %rax\n"           // SYS_exit
        "syscall\n"
    );
}

int main( void )
{
    write( STDOUT_FD, "hello\n", 6 );
    return 0;
}

static int write( int fd, char* data, unsigned long long len )
{
    long ret;

    __asm__ volatile (
        "mov $1, %%rax\n"
        "syscall\n"
        : "=a"(ret)
        : "D"(fd),        // rdi
          "S"(data),      // rsi
          "d"(len)        // rdx
        : "rcx", "r11", "memory"
    );

    return len;
}
