/**
 * This library is intended to make easier the interaction with
 * the gpio pins/ports. Currently it's compatible with raspi 2 and 3.
 * 
 * Tomás Golomb Durán - Nov. 12, 2019
 * Alejandro Cancelo Correia - Nov. 25, 2019
 * 
 */

#ifndef _GPIO_H
#define _GPIO_H

#include <stdint.h>
#include <utils/mm.h>
#include <io/base.h>

/**
 * @dependency
 *
 * Base address for the gpio registers, currently supports pi2 and pi3.
 * For future devices this address may change.
 */
#define GPIO_BASE   (PHYSICAL_PBASE + 0x00200000)

//GPIO function select (registers 0 to 9)
#define GPFSEL0     ((volatile uint32_t*)(GPIO_BASE + 0x000))
//GPIO function select (registers 10 to 19)
#define GPFSEL1     ((volatile uint32_t*)(GPIO_BASE + 0x004))
//GPIO function select (registers 20 to 29)
#define GPFSEL2     ((volatile uint32_t*)(GPIO_BASE + 0x008))
//GPIO function select (registers 30 to 39)
#define GPFSEL3     ((volatile uint32_t*)(GPIO_BASE + 0x00C))
//GPIO function select (registers 40 to 49)
#define GPFSEL4     ((volatile uint32_t*)(GPIO_BASE + 0x010))
//GPIO function select (registers 50 to 53)
#define GPFSEL5     ((volatile uint32_t*)(GPIO_BASE + 0x014))
//GPIO pin output set (GPIO pin 0-31)
#define GPSET0      ((volatile uint32_t*)(GPIO_BASE + 0x01C))
//GPIO pin output set (GPIOpin 32-53)
#define GPSET1      ((volatile uint32_t*)(GPIO_BASE + 0x020))
//GPIO pin output clear (GPIO pin 0-31)
#define GPCLR0      ((volatile uint32_t*)(GPIO_BASE + 0x028))
//GPIO pin output clear (GPIOpin 32-53)
#define GPCLR1      ((volatile uint32_t*)(GPIO_BASE + 0x02C))
//GPIO pin level (GPIO pin 0-31)
#define GPLEV0      ((volatile uint32_t*)(GPIO_BASE + 0x034))
//GPIO pin level (GPIO pin 32-53)
#define GPLEV1      ((volatile uint32_t*)(GPIO_BASE + 0x038))
//GPIO pin event detect status 0 (GPIO pin 0-31)
#define GPEDS0      ((volatile uint32_t*)(GPIO_BASE + 0x040))
//GPIO pin event detect status 0 (GPIO pin 32-53)
#define GPEDS1      ((volatile uint32_t*)(GPIO_BASE + 0x044))
//GPIO rising-edge pin detect enable (GPIO pin 0-31)
#define GPREN0      ((volatile uint32_t*)(GPIO_BASE + 0x04C))
//GPIO rising-edge pin detect enable (GPIO pin 32-53)
#define GPREN1      ((volatile uint32_t*)(GPIO_BASE + 0x050))
//GPIO falling-edge pin detect enable (GPIO pin 0-31)
#define GPFEN0      ((volatile uint32_t*)(GPIO_BASE + 0x058))
//GPIO falling-edge pin detect enable (GPIO pin 32-53)
#define GPFEN1      ((volatile uint32_t*)(GPIO_BASE + 0x05C))
//GPIO pin high detect enable (GPIO pin 0-31)
#define GPHEN0      ((volatile uint32_t*)(GPIO_BASE + 0x064))
//GPIO pin high detect enable (GPIO pin 32-53)
#define GPHEN1      ((volatile uint32_t*)(GPIO_BASE + 0x068))
//GPIO pin low detect enable (GPIO pin 0-31)
#define GPLEN0      ((volatile uint32_t*)(GPIO_BASE + 0x070))
//GPIO pin low detect enable (GPIO pin 32-53)
#define GPLEN1      ((volatile uint32_t*)(GPIO_BASE + 0x074))
//GPIO Pin Async. rising edge detect (GPIO pin 0-31)
#define GPAREN0     ((volatile uint32_t*)(GPIO_BASE + 0x07C))
//GPIO Pin Async. rising edge detect (GPIO pin 32-53)
#define GPAREN1     ((volatile uint32_t*)(GPIO_BASE + 0x080))
//GPIO Pin Async.falling edge detect (GPIO pin 0-31)
#define GPALEN0     ((volatile uint32_t*)(GPIO_BASE + 0x088))
//GPIO Pin Async.falling edge detect (GPIO pin 32-53)
#define GPALEN1     ((volatile uint32_t*)(GPIO_BASE + 0x08C))
//GPIO pin pull-up/down register
#define GPPUD       ((volatile uint32_t*)(GPIO_BASE + 0x094))
//GPIO pin pull-up/down enable clock (GPIO pin 0-31)
#define GPPUDCLK0   ((volatile uint32_t*)(GPIO_BASE + 0x098))
//GPIO pin pull-up/down enable clock (GPIO pin 32-53)
#define GPPUDCLK1   ((volatile uint32_t*)(GPIO_BASE + 0x09C))

/*
 * alternative pin functions type
 */
typedef enum {
    INPUT  = 0b000,
    OUTPUT = 0b001,
    ALT0   = 0b100,
    ALT1   = 0b101,
    ALT2   = 0b110,
    ALT3   = 0b111,
    ALT4   = 0b011,
    ALT5   = 0b010
} pin_alt_funct;

/**
 * Defines the operation for a GPIO pin. 
 *
 * @param pin 1 out of the 54 pins {0 - 53}
 * @param fun_sel The operation type for the pin. There are up to 8 alternatives for each pin, 
 * however not every pin has a defined function for the 8 alternatives, these are listed in
 * the Alternative Function Assignments Table provided in the ARM Peripherals documentation.
 * @return void
 */
void pin_set_function(unsigned int pin, pin_alt_funct fun_sel);

/**
 * Sets the desired pin as output.
 *
 * @param pin 1 out of the 54 pins {0 - 53}
 * @return void
 */
void pin_set_output(unsigned int pin);

/**
 * Clears the output signaling in the give pin
 * 
 * @param pin 1 out of the 54 pins {0 - 53}
 * @return void
 */
void pin_clear_output(unsigned int pin);

/**
 * Get the current pin level
 * 
 * @param pin 1 out of the 54 pins {0 - 53}
 * @return 0 if low 1 if high and -1 if the pin is wrong
 */
int pin_get_level(unsigned int pin);

/**
 * Get the current status for the programmed event in the given pin
 * 
 * @param pin 1 out of the 54 pins {0 - 53}
 * @param clear 0 to prevent the flag from being cleared
 * @return 1 if the event has been detected, 0 otherwise
 */
unsigned int pin_get_event_status_flag(unsigned int pin, int clear);

/**
 * Clears the event status flag of the given pin.
 * The cleaning will take effect only if the 
 * status has been detected.
 * 
 * @param pin 1 out of the 54 pins {0 - 53}
 * @return void
 */
void pin_clear_event_status_flag(unsigned int pin);

/**
 * Switches the event of a rising edge signal (0 to 1)
 * over a pin. To know if the event has been
 * triggered use pin_get_event_status_flag function.
 * Disabling this event will clear the flag if
 * the event has been fired.
 * 
 * @param pin 1 out of the 54 pins {0 - 53}
 * @param enable 0 to disable otherwise to enable
 * @return void
 * @see pin_get_event_status_flag
 */
void pin_switch_rising_edge_event(unsigned int pin, int enable);

/**
 * Switches the event of a falling edge signal (1 to 0)
 * over a pin. To know if the event has been
 * triggered use pin_get_event_status_flag function.
 * Disabling this event will clear the flag if
 * the event has been fired.
 * 
 * @param pin 1 out of the 54 pins {0 - 53}
 * @param enable 0 to disable otherwise to enable
 * @return void
 * @see pin_get_event_status_flag
 */
void pin_switch_falling_edge_event(unsigned int pin, int enable);

/**
 * Switches the detection of a high event (1)
 * over a pin. To know if the event has been
 * triggered use pin_get_event_status_flag function.
 * Disabling this event will clear the flag if
 * the event has been fired.
 * 
 * @param pin 1 out of the 54 pins {0 - 53}
 * @param enable 0 to disable otherwise to enable the event
 * @return void
 * @see pin_get_event_status_flag
 */
void pin_switch_high_event(unsigned int pin, int enable);

/**
 * Switches the detection of a low event (0)
 * over a pin. To know if the event has been
 * triggered use pin_get_event_status_flag function.
 * Disabling this event will clear the flag if
 * the event has been fired.
 * 
 * @param pin 1 out of the 54 pins {0 - 53}
 * @param enable 0 to disable otherwise to enable the event
 * @return void
 * @see pin_get_event_status_flag
 */
void pin_switch_low_event(unsigned int pin, int enable);

/**
 * Switches the detection of a rising event (0 to 1)
 * over a pin. Async means that the signal is not sampled
 * with the system clock. To know if the event has been
 * triggered use pin_get_event_status_flag function.
 * Disabling this event will clear the flag if
 * the event has been fired.
 * 
 * @param pin 1 out of the 54 pins {0 - 53}
 * @param enable 0 to disable otherwise to enable the event
 * @return void
 * @see pin_get_event_status_flag
 */
void pin_switch_async_rising_event(unsigned int pin, int enable);

/**
 * Switches the detection of a falling event (1 to 0)
 * over a pin. Async means that the signal is not sampled
 * with the system clock. To know if the event has been
 * triggered use pin_get_event_status_flag function.
 * Disabling this event will clear the flag if
 * the event has been fired.
 * 
 * @param pin 1 out of the 54 pins {0 - 53}
 * @param enable 0 to disable otherwise to enable the event
 * @return void
 * @see pin_get_event_status_flag
 */
void pin_switch_async_falling_event(unsigned int pin, int enable);

/**
 * Switches the actuation of the internal pull-up/down (pud)
 * to all GPIO pins. This function is used in conjuction with
 * the GPPUDCLKn register.
 * 
 * NOTE: the state is preserved even in power-down (core off) mode
 *       and can't be read.
 * NOTE: changes require 150 cycles to set-up
 * 
 * @param state { 0b00 = Off – disable pull-up/down | 0b01 = Enable Pull Down control |
 *                0b10 = Enable Pull Up control }
 * @return void
 * @see pin_switch_pud
 */
void switch_pud_control(unsigned int state);

/**
 * Set the state of the pull-up/down resistor to a pin.
 * This function will commit all the required events in order
 * to set-up the resistors state (including a 300 cycles delay). 
 * 
 * 
 * @param pins pin
 * @param state { 0b00 = Off – disable pull-up/down | 0b01 = Enable Pull Down control |
 *                0b10 = Enable Pull Up control }
 * @return void
 */
void pin_switch_pud(unsigned int pin, unsigned int state);

#endif /* _GPIO_H */