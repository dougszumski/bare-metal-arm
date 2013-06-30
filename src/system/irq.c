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
