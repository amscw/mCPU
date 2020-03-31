/*
 * sysexc.c
 *
 *  Created on: 18 мар. 2020 г.
 *      Author: moskvin
 */

#include <system_exc.h>
#include "stm32f7xx.h"
#include "trace.h"
#include "semihosting.h"

//------------------------------------------------------------------------------
// Prototypes
//------------------------------------------------------------------------------
#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
static void dumpExceptionStack(ExceptionStackFrame *frame, uint32_t cfsr, uint32_t mmfar, uint32_t bfar, uint32_t lr);
#endif // defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
#if defined(__ARM_ARCH_6M__)
static void dumpExceptionStack (ExceptionStackFrame* frame, uint32_t lr)
#endif // defined(__ARM_ARCH_6M__)

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------
void __attribute__ ((section(".isr_vector"),weak)) NMI_Handler(void)
{

#if defined(DEBUG)
	__DEBUG_BKPT();
#endif
	while (1) {
	}
}

#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
// Hard Fault handler wrapper in assembly.
// It extracts the location of stack frame and passes it to handler
// in C as a pointer. We also pass the LR value as second
// parameter.
// (Based on Joseph Yiu's, The Definitive Guide to ARM Cortex-M3 and
// Cortex-M4 Processors, Third Edition, Chap. 12.8, page 402).

void __attribute__ ((section(".isr_vector"),weak,naked)) HardFault_Handler(void)
{
	asm volatile(
		" tst lr,#4       \n"
		" ite eq          \n"
		" mrseq r0,msp    \n"
		" mrsne r0,psp    \n"
		" mov r1,lr       \n"
		" ldr r2,=HardFault_Handler_C \n"
		" bx r2"

		: /* Outputs */
		: /* Inputs */
		: /* Clobbers */
	);
}

void __attribute__ ((section(".isr_vector"),weak,used)) HardFault_Handler_C(
	ExceptionStackFrame *frame __attribute__((unused)),	uint32_t lr __attribute__((unused)))
{
#if defined(TRACE)
	uint32_t mmfar = SCB->MMFAR; // MemManage Fault Address
	uint32_t bfar = SCB->BFAR; // Bus Fault Address
	uint32_t cfsr = SCB->CFSR; // Configurable Fault Status Registers
#endif

#if defined(USE_SEMIHOSTING) || defined(USE_TRACE_SEMIHOSTING_STDOUT) || defined(USE_TRACE_SEMIHOSTING_DEBUG)
	// If the BKPT instruction is executed with C_DEBUGEN == 0 and MON_EN == 0,
	// it will cause the processor to enter a HardFault exception, with DEBUGEVT
	// in the Hard Fault Status register (HFSR) set to 1, and BKPT in the
	// Debug Fault Status register (DFSR) also set to 1.
	if (((SCB->DFSR & SCB_DFSR_BKPT_Msk) != 0) && ((SCB->HFSR & SCB_HFSR_DEBUGEVT_Msk) != 0))
	{
		if (isSemihosting(frame, 0xBE00 + (AngelSWI & 0xFF)))
		{
			// Clear the exception cause in exception status.
			SCB->HFSR = SCB_HFSR_DEBUGEVT_Msk;

			// Continue after the BKPT
			return;
		}
	}
#endif // defined(USE_SEMIHOSTING) || defined(USE_TRACE_SEMIHOSTING_STDOUT) || defined(USE_TRACE_SEMIHOSTING_DEBUG)

#if defined(TRACE)
	trace_printf("[HardFault]\n");
	dumpExceptionStack(frame, cfsr, mmfar, bfar, lr);
#endif // defined(TRACE)

#if defined(DEBUG)
	__DEBUG_BKPT();
#endif
	while (1) {
	}
}

#endif // defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)


#if defined(__ARM_ARCH_6M__)

// Hard Fault handler wrapper in assembly.
// It extracts the location of stack frame and passes it to handler
// in C as a pointer. We also pass the LR value as second
// parameter.
// (Based on Joseph Yiu's, The Definitive Guide to ARM Cortex-M0
// First Edition, Chap. 12.8, page 402).

void __attribute__ ((section(".isr_vector"),weak,naked))
HardFault_Handler (void)
{
  asm volatile(
      " movs r0,#4      \n"
      " mov r1,lr       \n"
      " tst r0,r1       \n"
      " beq 1f          \n"
      " mrs r0,psp      \n"
      " b   2f          \n"
      "1:               \n"
      " mrs r0,msp      \n"
      "2:"
      " mov r1,lr       \n"
      " ldr r2,=HardFault_Handler_C \n"
      " bx r2"

      : /* Outputs */
      : /* Inputs */
      : /* Clobbers */
  );
}

void __attribute__ ((section(".isr_vector"),weak,used))
HardFault_Handler_C (ExceptionStackFrame* frame __attribute__((unused)),
                     uint32_t lr __attribute__((unused)))
{
	// There is no semihosting support for Cortex-M0, since on ARMv6-M
	// faults are fatal and it is not possible to return from the handler.

#if defined(TRACE)
	trace_printf ("[HardFault]\n");
	dumpExceptionStack (frame, lr);
#endif // defined(TRACE)

#if defined(DEBUG)
	__DEBUG_BKPT();
#endif
	while (1) {
    }
}

#endif // defined(__ARM_ARCH_6M__)

#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
void __attribute__ ((section(".isr_vector"),weak)) MemManage_Handler(void)
{
	#if defined(DEBUG)
	__DEBUG_BKPT();
	#endif
	while (1) {
	}
}

void __attribute__ ((section(".isr_vector"),weak,naked)) BusFault_Handler(void)
{
	asm volatile(
		" tst lr,#4       \n"
		" ite eq          \n"
		" mrseq r0,msp    \n"
		" mrsne r0,psp    \n"
		" mov r1,lr       \n"
		" ldr r2,=BusFault_Handler_C \n"
		" bx r2"

		: /* Outputs */
		: /* Inputs */
		: /* Clobbers */
	);
}

void __attribute__ ((section(".isr_vector"),weak,used)) BusFault_Handler_C(
	ExceptionStackFrame *frame __attribute__((unused)),
	uint32_t lr __attribute__((unused)))
{
#if defined(TRACE)
	uint32_t mmfar = SCB->MMFAR; // MemManage Fault Address
	uint32_t bfar = SCB->BFAR; // Bus Fault Address
	uint32_t cfsr = SCB->CFSR; // Configurable Fault Status Registers

	trace_printf("[BusFault]\n");
	dumpExceptionStack(frame, cfsr, mmfar, bfar, lr);
#endif // defined(TRACE)

#if defined(DEBUG)
	__DEBUG_BKPT();
#endif
	while (1) {
	}
}

void __attribute__ ((section(".isr_vector"),weak,naked)) UsageFault_Handler(void)
{
	asm volatile(
		" tst lr,#4       \n"
		" ite eq          \n"
		" mrseq r0,msp    \n"
		" mrsne r0,psp    \n"
		" mov r1,lr       \n"
		" ldr r2,=UsageFault_Handler_C \n"
		" bx r2"

		: /* Outputs */
		: /* Inputs */
		: /* Clobbers */
	);
}

void __attribute__ ((section(".isr_vector"),weak,used)) UsageFault_Handler_C(
		ExceptionStackFrame *frame __attribute__((unused)),
		uint32_t lr __attribute__((unused)))
{
#if defined(TRACE)
	uint32_t mmfar = SCB->MMFAR; // MemManage Fault Address
	uint32_t bfar = SCB->BFAR; // Bus Fault Address
	uint32_t cfsr = SCB->CFSR; // Configurable Fault Status Registers
#endif

#if defined(DEBUG_SEMIHOSTING_FAULTS)
	if ((cfsr & (1UL << 16)) != 0) // UNDEFINSTR
    {
		// For testing purposes, instead of BKPT use 'setend be'.
		if (isSemihosting (frame, AngelSWITestFaultOpCode))
		{
			return;
        }
    }
#endif

#if defined(TRACE)
	trace_printf("[UsageFault]\n");
	dumpExceptionStack(frame, cfsr, mmfar, bfar, lr);
#endif // defined(TRACE)

#if defined(DEBUG)
	__DEBUG_BKPT();
#endif
	while (1) {
	}
}

#endif // defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)

void __attribute__ ((section(".isr_vector"),weak)) SVC_Handler(void)
{
#if defined(DEBUG)
	__DEBUG_BKPT();
#endif
	while (1) {
	}
}

#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
void __attribute__ ((section(".isr_vector"),weak)) DebugMon_Handler(void)
{
#if defined(DEBUG)
	__DEBUG_BKPT();
#endif
	while (1) {
	}
}
#endif // defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)

void __attribute__ ((section(".isr_vector"),weak)) PendSV_Handler(void) {
#if defined(DEBUG)
	__DEBUG_BKPT();
#endif
	while (1) {
	}
}

void __attribute__ ((section(".isr_vector"),weak)) SysTick_Handler(void) {
	// DO NOT loop, just return.
	// Useful in case someone (like STM HAL) inadvertently enables SysTick.
	;
}

//------------------------------------------------------------------------------
// Functions (private)
//------------------------------------------------------------------------------

#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)

// The values of BFAR and MMFAR stay unchanged if the BFARVALID or
// MMARVALID is set. However, if a new fault occurs during the
// execution of this fault handler, the value of the BFAR and MMFAR
// could potentially be erased. In order to ensure the fault addresses
// accessed are valid, the following procedure should be used:
// 1. Read BFAR/MMFAR.
// 2. Read CFSR to get BFARVALID or MMARVALID. If the value is 0, the
//    value of BFAR or MMFAR accessed can be invalid and can be discarded.
// 3. Optionally clear BFARVALID or MMARVALID.
// (See Joseph Yiu's book).

static void dumpExceptionStack(ExceptionStackFrame *frame, uint32_t cfsr, uint32_t mmfar, uint32_t bfar, uint32_t lr)
{
	trace_printf("Stack frame:\n");
	trace_printf(" R0 =  %08X\n", frame->r0);
	trace_printf(" R1 =  %08X\n", frame->r1);
	trace_printf(" R2 =  %08X\n", frame->r2);
	trace_printf(" R3 =  %08X\n", frame->r3);
	trace_printf(" R12 = %08X\n", frame->r12);
	trace_printf(" LR =  %08X\n", frame->lr);
	trace_printf(" PC =  %08X\n", frame->pc);
	trace_printf(" PSR = %08X\n", frame->psr);
	trace_printf("FSR/FAR:\n");
	trace_printf(" CFSR =  %08X\n", cfsr);
	trace_printf(" HFSR =  %08X\n", SCB->HFSR);
	trace_printf(" DFSR =  %08X\n", SCB->DFSR);
	trace_printf(" AFSR =  %08X\n", SCB->AFSR);

	if (cfsr & (1UL << 7)) {
		trace_printf(" MMFAR = %08X\n", mmfar);
	}
	if (cfsr & (1UL << 15)) {
		trace_printf(" BFAR =  %08X\n", bfar);
	}
	trace_printf("Misc\n");
	trace_printf(" LR/EXC_RETURN= %08X\n", lr);
}

#endif // defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)

#if defined(__ARM_ARCH_6M__)

static void dumpExceptionStack (ExceptionStackFrame* frame, uint32_t lr)
{
  trace_printf ("Stack frame:\n");
  trace_printf (" R0 =  %08X\n", frame->r0);
  trace_printf (" R1 =  %08X\n", frame->r1);
  trace_printf (" R2 =  %08X\n", frame->r2);
  trace_printf (" R3 =  %08X\n", frame->r3);
  trace_printf (" R12 = %08X\n", frame->r12);
  trace_printf (" LR =  %08X\n", frame->lr);
  trace_printf (" PC =  %08X\n", frame->pc);
  trace_printf (" PSR = %08X\n", frame->psr);
  trace_printf ("Misc\n");
  trace_printf (" LR/EXC_RETURN= %08X\n", lr);
}

#endif // defined(__ARM_ARCH_6M__)
