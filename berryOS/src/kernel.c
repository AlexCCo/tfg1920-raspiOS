#include <stddef.h>
#include <stdint.h>
#include <interrupts.h>
#include <local_timer.h>
#include <io/stdio.h>
#include <io/uart.h>
#include <io/gpu.h>
#include <io/gpio.h>
#include <proc/pcb.h>
#include <console/console.h>

extern void io_halt();


void saluda(void){
    int a = 2;
    int i = 0;
    while(1){
        if(i == 100000000){
            uart_puts("I'm the CREATED function process --> a = ");
            uart_putln(itoa(a));
            i = 0;
        }
       i++;
    }
}

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {   
    (void) r0;
    (void) r1;
    (void) atags;
    pin_set_function(17, OUTPUT);
    
    /* UART */
    uart_init();
    uart_puts(">> Uart init");
    uart_putln("[OK]");

    /* DYNAMIC MEMORY */
    uart_puts(">> Dynamic memory: ");
    mem_init(((atag_t *)atags));
    uart_putln(" [OK]");

    /* HDMI */
    gpu_init();
    print(">> GPU init: ");
    enrichedPrintLn("[OK]", &GREEN, NULL);

    /* INTERRUPTS */
    print(">> Interrupts init: ");
    interrupts_init();
    enrichedPrintLn("[OK]", &GREEN, NULL);
    
    /* LOCAL TIMER */
    print(" - Register timer handler and clearer: ");
    register_irq_handler(ARM_TIMER, local_timer_handler, local_timer_clearer);
    enrichedPrintLn("[OK]", &GREEN, NULL);

    print(">> Local timer init: ");
    local_timer_init(VIRTUAL_SYS, 1000);
    enrichedPrintLn("[OK]", &GREEN, NULL);

    /* Processes */
    print(">> Processes init: ");
    process_init();
    enrichedPrintLn("[OK]", &GREEN, NULL);

    printLn("");
    printLn("");
    for (size_t i = 0; i < 28; i++) {
        print(itoa(i));
        print(" ");
    }
    printLn("");
    
    printLn("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    
    start_console();

    print_processes();
    //TEST PROCESS SECTION
    create_kernel_thread(&saluda, "Proc1", 5);

    //print_processes();
}