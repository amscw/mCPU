/*
 * semihosting.c
 *
 *  Created on: 17 мар. 2020 г.
 *      Author: moskvin
 */

#include <system_exc.h>
#include "semihosting.h"
#include "stm32f7xx.h"
#include "string.h"

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------
/**
 * This function provides the minimum functionality to make a semihosting program execute even without the debugger present.
 * NB: функция имитирует обработку семихостинга. Семихостинг обрабатывает процессор вместо отладочного агента
 * @param frame pointer to an exception stack frame.
 * @param opCode the 16-bin word of the BKPT instruction.
 * @return 1 if the instruction was a valid semihosting call; 0 otherwise.
 */
int isSemihosting(ExceptionStackFrame *frame, uint16_t opCode)
{
	uint16_t *pw = (uint16_t*) frame->pc;

	if (*pw == opCode)
	{
		uint32_t r0 = frame->r0;
#if defined(DEBUG_SEMIHOSTING_FAULTS) || defined(USE_SEMIHOSTING) || defined(USE_TRACE_SEMIHOSTING_STDOUT)
		uint32_t r1 = frame->r1;
#endif
#if defined(USE_SEMIHOSTING) || defined(USE_TRACE_SEMIHOSTING_STDOUT)
		uint32_t* blk = (uint32_t*) r1;
#endif

#if defined(DEBUG_SEMIHOSTING_FAULTS)
		// trace_printf ("sh r0=%d\n", r0);
#endif
		switch (r0)
		{
#if defined(USE_SEMIHOSTING)
		case SEMIHOSTING_SYS_CLOCK:
        case SEMIHOSTING_SYS_ELAPSED:
        case SEMIHOSTING_SYS_FLEN:
        case SEMIHOSTING_SYS_GET_CMDLINE:
        case SEMIHOSTING_SYS_REMOVE:
        case SEMIHOSTING_SYS_RENAME:
        case SEMIHOSTING_SYS_SEEK:
        case SEMIHOSTING_SYS_SYSTEM:
        case SEMIHOSTING_SYS_TICKFREQ:
        case SEMIHOSTING_SYS_TMPNAM:
        case SEMIHOSTING_SYS_ISTTY:
        	frame->r0 = (uint32_t)-1; // the call is not successful or not supported
        	break;

        case SEMIHOSTING_SYS_CLOSE:
        	frame->r0 = 0; // call is successful
        	break;

        case SEMIHOSTING_SYS_ERRNO:
        	frame->r0 = 0; // the value of the C library errno variable.
        	break;

        case SEMIHOSTING_SYS_HEAPINFO:
        	blk[0] = 0; // heap_base
        	blk[1] = 0; // heap_limit
        	blk[2] = 0; // stack_base
        	blk[3] = 0; // stack_limit
        	break;

        case SEMIHOSTING_SYS_ISERROR:
        	frame->r0 = 0; // 0 if the status word is not an error indication
        	break;

        case SEMIHOSTING_SYS_READ:
        	// If R0 contains the same value as word 3, the call has
        	// failed and EOF is assumed.
        	frame->r0 = blk[2];
        	break;

        case SEMIHOSTING_SYS_READC:
        	frame->r0 = '\0'; // the byte read from the console.
        	break;

        case SEMIHOSTING_SYS_TIME:
        	frame->r0 = 0; // the number of seconds since 00:00 January 1, 1970.
        	break;

        case SEMIHOSTING_ReportException:
        	NVIC_SystemReset ();
        	// Should not reach here
        	return 0;
#endif // defined(USE_SEMIHOSTING)

#if defined(USE_SEMIHOSTING) || defined(USE_TRACE_SEMIHOSTING_STDOUT)

#define HANDLER_STDIN   (1)
#define HANDLER_STDOUT  (2)
#define HANDLER_STDERR  (3)

        case SEMIHOSTING_SYS_OPEN:
        	// Process only standard io/out/err and return 1/2/3
        	if (strcmp ((char*) blk[0], ":tt") == 0)
            {
        		if ((blk[1] == 0))
                {
        			frame->r0 = HANDLER_STDIN;
        			break;
                } else if (blk[1] == 4) {
                	frame->r0 = HANDLER_STDOUT;
                	break;
                } else if (blk[1] == 8) {
                	frame->r0 = HANDLER_STDERR;
                	break;
                }
            }
        	frame->r0 = (uint32_t)-1; // the call is not successful or not supported
        	break;

        case SEMIHOSTING_SYS_WRITE:
        	// Silently ignore writes to stdout/stderr, fail on all other handler.
        	if ((blk[0] == HANDLER_STDOUT) || (blk[0] == HANDLER_STDERR))
        	{
#if defined(DEBUG_SEMIHOSTING_FAULTS)
        		frame->r0 = (uint32_t)blk[2] - trace_write((char*) blk[1], blk[2]);
#else
        		frame->r0 = 0; // all sent, no more.
#endif // defined(DEBUG_SEMIHOSTING_FAULTS)
            } else {
            	// If other handler, return the total number of bytes
            	// as the number of bytes that are not written.
            	frame->r0 = blk[2];
            }
        	break;
#endif // defined(USE_SEMIHOSTING) || defined(USE_TRACE_SEMIHOSTING_STDOUT)

#if defined(USE_SEMIHOSTING) || defined(USE_TRACE_SEMIHOSTING_STDOUT) || defined(USE_TRACE_SEMIHOSTING_DEBUG)
        case SEMIHOSTING_SYS_WRITEC:
#if defined(DEBUG_SEMIHOSTING_FAULTS)
        	{
        		char ch = *((char*) r1);
        		trace_write (&ch, 1);
        	}
#endif
        	// Register R0 is corrupted.
			break;

        case SEMIHOSTING_SYS_WRITE0:
#if defined(DEBUG_SEMIHOSTING_FAULTS)
        	{
        		char* p = ((char*) r1);
        		trace_write (p, strlen (p));
        	}
#endif
			// Register R0 is corrupted.
			break;

#endif // defined(USE_SEMIHOSTING) || defined(USE_TRACE_SEMIHOSTING_STDOUT) || defined(USE_TRACE_SEMIHOSTING_DEBUG)
			default:
			return 0;
		}

		// Alter the PC to make the exception returns to
		// the instruction after the faulty BKPT.
		frame->pc += 2;
		return 1;
	}
	return 0;
}
