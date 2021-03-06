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
#include <console/command.h>
#include <proc/locks/mutex.h>
#include <fs/fs.h>
#include <mem/mem.h>

extern void io_halt();

COMMAND test;

uint32_t critical_value = 0;
//1 locked 0 unlocked
int mutex = MUTEX_UNLOCK;

void test1_subroutine(void){
    int i = 0;
    uart_putln("Entered into the subroutine");
    uart_putln("Entered into the subroutine");
    uart_putln("Entered into the subroutine");
    uart_putln("Entered into the subroutine");
    uart_putln("test1: trying to take lock");
    take_lock(&mutex);
    while(1){
        if(i == 100000000){
            uart_putln("I'm test1 -->");
            i = 0;
        }
        i++;
    }
}

void test1(void){
    uart_putln("Executing test1");
    uart_putln("Executing test2");
    uart_putln("Executing test3");
    uart_putln("Executing test4");
    uart_putln("Executing test5");
    test1_subroutine();
}

void test2(void){
    uint32_t i = 0;
    uart_putln("test2: Trying to take lock");
    take_lock(&mutex);
    uart_putln("test2: lock taked by test2");

    while (1) {
        if(i == 1000000){
            uart_putln("I'm test2 -->");
            i = 0;
        }
        i++;    
    }
}

void test_command_handler(int argc, char** argv) {
    MARK_UNUSED(argc);
    MARK_UNUSED(argv);
    print("This is a test:");
    enrichedPrintLn(" Test passed.", &GREEN, NULL);
}

extern void pointer_test(void);

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {   
    (void) r0;
    (void) r1;
    (void) atags;
    pin_set_function(17, OUTPUT);
    
    test.helpText = "This is a test command";
    test.key = "test";
    test.trigger = test_command_handler;

    regcomm(&test);

    /* UART */
    uart_init();
    uart_puts(">> Uart init");
    uart_putln("[OK]");

    /* DYNAMIC MEMORY */
    uart_puts(">> (uart)Dynamic memory: ");
    mem_init(((atag_t *)atags));
    uart_putln(" [OK]");

    /* HDMI */
    gpu_init();
    print(">> Dynamic memory: ");
    enrichedPrintLn("[OK]", &GREEN, NULL);
    print(">> GPU init: ");
    enrichedPrintLn("[OK]", &GREEN, NULL);

    /* Processes */
    print(">> Processes init: ");
    process_init();
    enrichedPrintLn("[OK]", &GREEN, NULL);

    /* Commands */
    print("\t- Commands init: ");
    init_commands();
    enrichedPrintLn("[OK]", &GREEN, NULL);

    /* Console */
    print(">> Console init: ");
    start_console();
    enrichedPrintLn("[OK]", &GREEN, NULL);

    /* Filesystem */
    print(">> Filesystem init: ");
    fs_init();
    enrichedPrintLn("[OK]", &GREEN, NULL);
    
    /* INTERRUPTS */
    print(">> Interrupts init: ");
    interrupts_init();
    enrichedPrintLn("[OK]", &GREEN, NULL);

    /* LOCAL TIMER */
    print("\t- Register timer handler and clearer: ");
    register_irq_handler(ARM_TIMER, local_timer_handler, local_timer_clearer);
    enrichedPrintLn("[OK]", &GREEN, NULL);

    print(">> Local timer init: ");
    local_timer_init(VIRTUAL_SYS, QUANTUM);
    enrichedPrintLn("[OK]", &GREEN, NULL);

    //TEST PROCESS SECTION
    //create_kernel_thread(&pointer_test, "nested call test", 16);
    create_kernel_thread(&test1, "Proc1", 5);
    create_kernel_thread(&test2, "Kezo", 5);
 
    print_processes();

}

extern uint8_t __end; 


void kernel_main2(uint32_t r0, uint32_t r1, uint32_t atags) {   
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

    fs_init();

    delay(1000);
    uart_puts("Bienvenido a BerryOS\n");
    delay(1000);
    
    while (1) {
        
        char c;
        unsigned int i = 0;
        unsigned int j = 0;
        char buf[20];
        char text[100];
        int num = 0;
        
        uart_puts("¿Que quieres hacer?\n > 1 --> Crear archivo\n > 2 --> Crear directorio \n > 3 --> Escribir archivo\n > 4 --> Leer archivo\n > 5 --> Borrar archivo/directorio\n > 6 --> Cambiar directorio\n > 7 --> Imprimir estructura\n > ");
        
        c = uart_recv();
        uart_putln("PETOOOOOOOOOOOOOOO");
        uart_putc(c);
        uart_putc('\n');
        
        if(c == '1'){
    
            uart_puts("Inserte nombre > ");
             
            
            while((c = uart_recv()) != '\r')  {
                uart_putc(c);
                buf[i++] = c;
            }
            buf[i++] = '\0';
            uart_putc('\n');

            createFile(buf);

            printFs();
            
        }
        else if (c == '2'){
            uart_puts("Inserte nombre > ");
             
            while((c = uart_recv()) != '\r')  {
                uart_putc(c);
                buf[i++] = c;
            }
            buf[i++] = '\0';
            uart_putc('\n');

            createDir(buf);

            printFs();
        }
        else if (c == '3'){
            uart_puts("Inserte nombre > ");
             
            while((c = uart_recv()) != '\r')  {
                uart_putc(c);
                buf[i++] = c;
            }
            buf[i++] = '\0';
            uart_putc('\n');
            uart_puts("Inserte texto > ");
            while((c = uart_recv()) != '\r')  {
                uart_putc(c);
                text[j++] = c;
            }
            text[j++] = '\0';
            uart_putc('\n');
            
            getFsInterface()->write(buf, text);
        }
        else if (c == '4'){
            uart_puts("Inserte nombre > ");
            while((c = uart_recv()) != '\r')  {
                uart_putc(c);
                buf[i++] = c;
            }
            buf[i++] = '\0';
            uart_putc('\n');

            uart_puts("Inserte bytes > ");
            while((c = uart_recv()) != '\r')  {
                num = num*10 + (c - '0');
                uart_putc(c);
            }
            uart_putc('\n');

            char* aux = read(buf, num);;

            uart_puts("    Texto > ");
            uart_putln(aux);
            kfree(aux);            
        }
        else if(c == '5'){
            uart_puts("Inserte nombre > ");
             
            
            while((c = uart_recv()) != '\r')  {
                uart_putc(c);
                buf[i++] = c;
            }
            buf[i++] = '\0';
            uart_putc('\n');

            delete(buf);

            printFs();
        }
        else if(c == '6'){
            uart_puts("Inserte nombre > ");
             
        
            while((c = uart_recv()) != '\r')  {
                uart_putc(c);
                buf[i++] = c;
            }
            buf[i++] = '\0';

            changeDir(buf);
        }
        else if(c == '7'){
            getFsInterface()->printFs();
        }
        

    }


}