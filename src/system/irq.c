#include "irq.h"
#include "freedom.h"

// Interrupt enabling and disabling
inline void enable_irq(int n) {
	NVIC_ICPR |= 1 << (n - 16);
	NVIC_ISER |= 1 << (n - 16);
}

// TODO:  IRQ disable
inline void __enable_irq(void) {
	asm volatile ("cpsie i");
}
inline void __disable_irq(void) {
	asm volatile ("cpsid i");
}


// Defs from example. TODO: Lookup..
#ifdef false

void enable_irq (int irq)
{
    /* Make sure that the IRQ is an allowable number. Up to 32 is
     * used.
     *
     * NOTE: If you are using the interrupt definitions from the header
     * file, you MUST SUBTRACT 16!!!
     */
    if (irq <= 32){
      /* Set the ICPR and ISER registers accordingly */
      NVIC_ICPR |= 1 << (irq%32);
      NVIC_ISER |= 1 << (irq%32);
    }
}
void disable_irq (int irq)
{

    /* Make sure that the IRQ is an allowable number. Right now up to 48 is
     * used.
     */
    if (irq > 32)
        while (1);  /* error trap*/

    /* Determine which of the NVICICERs corresponds to the irq */

    NVIC_ICER = 1 << (irq%32);
 }

#endif


