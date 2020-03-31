/*
 * startup_.c
 *
 *  Created on: 19 мар. 2020 г.
 *      Author: moskvin
 */

//------------------------------------------------------------------------------
// Variables
//------------------------------------------------------------------------------
// These magic symbols are provided by the linker.
extern void (*__preinit_array_start[]) (void) __attribute__((weak));
extern void (*__preinit_array_end[]) (void) __attribute__((weak));
extern void (*__init_array_start[]) (void) __attribute__((weak));
extern void (*__init_array_end[]) (void) __attribute__((weak));
extern void (*__fini_array_start[]) (void) __attribute__((weak));
extern void (*__fini_array_end[]) (void) __attribute__((weak));

//------------------------------------------------------------------------------
// Prototypes
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------
// Iterate over all the preinit/init routines (mainly static constructors).
inline void __attribute__((always_inline)) __libc_init_array (void)
{
	int count;
	int i;

	count = __preinit_array_end - __preinit_array_start;
	for (i = 0; i < count; i++)
		__preinit_array_start[i] ();

	// If you need to run the code in the .init section, please use
	// the startup files, since this requires the code in crti.o and crtn.o
	// to add the function prologue/epilogue.
	//_init(); // DO NOT ENABE THIS!

	count = __init_array_end - __init_array_start;
	for (i = 0; i < count; i++)
		__init_array_start[i] ();
}

// Run all the cleanup routines (mainly static destructors).
inline void __attribute__((always_inline)) __libc_fini_array (void)
{
	int count;
	int i;

	count = __fini_array_end - __fini_array_start;
	for (i = count; i > 0; i--)
		__fini_array_start[i - 1] ();

	// If you need to run the code in the .fini section, please use
	// the startup files, since this requires the code in crti.o and crtn.o
	// to add the function prologue/epilogue.
	//_fini(); // DO NOT ENABE THIS!
}
