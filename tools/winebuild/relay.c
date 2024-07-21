/*
 * Relay calls helper routines
 *
 * Copyright 1993 Robert J. Amstadt
 * Copyright 1995 Martin von Loewis
 * Copyright 1995, 1996, 1997 Alexandre Julliard
 * Copyright 1997 Eric Youngdale
 * Copyright 1999 Ulrich Weigand
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include "config.h"

#include <ctype.h>
#include <stdarg.h>

#include "build.h"

/* offset of the stack pointer relative to %fs:(0) */
#define STACKOFFSET 0x10c  /* FIELD_OFFSET(TEB,SystemReserved1) */

/* fix this if the x86_thread_data structure is changed */
#define GS_OFFSET  0x1d8  /* FIELD_OFFSET(TEB,SystemReserved2) + FIELD_OFFSET(struct x86_thread_data,gs) */


/*******************************************************************
 *         BuildCallFrom16Core
 *
 * This routine builds the core routines used in 16->32 thunks:
 * CallFrom16Word, CallFrom16Long, CallFrom16Register, and CallFrom16Thunk.
 *
 * These routines are intended to be called via a far call (with 32-bit
 * operand size) from 16-bit code.  The 16-bit code stub must push %bp,
 * the 32-bit entry point to be called, and the argument conversion
 * routine to be used (see stack layout below).
 *
 * The core routine completes the STACK16FRAME on the 16-bit stack and
 * switches to the 32-bit stack.  Then, the argument conversion routine
 * is called; it gets passed the 32-bit entry point and a pointer to the
 * 16-bit arguments (on the 16-bit stack) as parameters. (You can either
 * use conversion routines automatically generated by BuildCallFrom16,
 * or write your own for special purposes.)
 *
 * The conversion routine must call the 32-bit entry point, passing it
 * the converted arguments, and return its return value to the core.
 * After the conversion routine has returned, the core switches back
 * to the 16-bit stack, converts the return value to the DX:AX format
 * (CallFrom16Long), and returns to the 16-bit call stub.  All parameters,
 * including %bp, are popped off the stack.
 *
 * The 16-bit call stub now returns to the caller, popping the 16-bit
 * arguments if necessary (pascal calling convention).
 *
 * In the case of a 'register' function, CallFrom16Register fills a
 * CONTEXT86 structure with the values all registers had at the point
 * the first instruction of the 16-bit call stub was about to be
 * executed.  A pointer to this CONTEXT86 is passed as third parameter
 * to the argument conversion routine, which typically passes it on
 * to the called 32-bit entry point.
 *
 * CallFrom16Thunk is a special variant used by the implementation of
 * the Win95 16->32 thunk functions C16ThkSL and C16ThkSL01 and is
 * implemented as follows:
 * On entry, the EBX register is set up to contain a flat pointer to the
 * 16-bit stack such that EBX+22 points to the first argument.
 * Then, the entry point is called, while EBP is set up to point
 * to the return address (on the 32-bit stack).
 * The called function returns with CX set to the number of bytes
 * to be popped of the caller's stack.
 *
 * Stack layout upon entry to the core routine (STACK16FRAME):
 *  ...           ...
 * (sp+24) word   first 16-bit arg
 * (sp+22) word   cs
 * (sp+20) word   ip
 * (sp+18) word   bp
 * (sp+14) long   32-bit entry point (reused for Win16 mutex recursion count)
 * (sp+12) word   ip of actual entry point (necessary for relay debugging)
 * (sp+8)  long   relay (argument conversion) function entry point
 * (sp+4)  long   cs of 16-bit entry point
 * (sp)    long   ip of 16-bit entry point
 *
 * Added on the stack:
 * (sp-2)  word   saved gs
 * (sp-4)  word   saved fs
 * (sp-6)  word   saved es
 * (sp-8)  word   saved ds
 * (sp-12) long   saved ebp
 * (sp-16) long   saved ecx
 * (sp-20) long   saved edx
 * (sp-24) long   saved previous stack
 */
static void BuildCallFrom16Core( int reg_func, int thunk )
{
    /* Function header */
    if (thunk) output_function_header( "__wine_call_from_16_thunk", 1 );
    else if (reg_func) output_function_header( "__wine_call_from_16_regs", 1 );
    else output_function_header( "__wine_call_from_16", 1 );

    /* Create STACK16FRAME (except STACK32FRAME link) */
    output( "\tpushw %%gs\n" );
    output( "\tpushw %%fs\n" );
    output( "\tpushw %%es\n" );
    output( "\tpushw %%ds\n" );
    output( "\tpushl %%ebp\n" );
    output( "\tpushl %%ecx\n" );
    output( "\tpushl %%edx\n" );

    /* Save original EFlags register */
    if (reg_func) output( "\tpushfl\n" );

    if ( UsePIC )
    {
        output( "\tcall 1f\n" );
        output( "1:\tpopl %%ecx\n" );
        output( "\tmovl %%cs:%s-1b(%%ecx),%%edx\n", asm_name("CallTo16_DataSelector") );
    }
    else
        output( "\tmovl %%cs:%s,%%edx\n", asm_name("CallTo16_DataSelector") );

    /* Load 32-bit segment registers */
    output( "\tmovw %%dx, %%ds\n" );
    output( "\tmovw %%dx, %%es\n" );

    if ( UsePIC )
        output( "\tmovw %s-1b(%%ecx), %%fs\n", asm_name("CallTo16_TebSelector") );
    else
        output( "\tmovw %s, %%fs\n", asm_name("CallTo16_TebSelector") );

    output( "\tmov %%fs:(%d),%%gs\n", GS_OFFSET );

    /* Translate STACK16FRAME base to flat offset in %edx */
    output( "\tmovw %%ss, %%dx\n" );
    output( "\tandl $0xfff8, %%edx\n" );
    output( "\tshrl $1, %%edx\n" );
    if (UsePIC)
        output( "\taddl .Lwine_ldt_copy_ptr-1b(%%ecx),%%edx\n" );
    else
        output( "\taddl .Lwine_ldt_copy_ptr,%%edx\n" );
    output( "\tmovl (%%edx), %%edx\n" );
    output( "\tmovzwl %%sp, %%ebp\n" );
    output( "\tleal %d(%%ebp,%%edx), %%edx\n", reg_func ? 0 : -4 );

    /* Get saved flags into %ecx */
    if (reg_func) output( "\tpopl %%ecx\n" );

    /* Get the 32-bit stack pointer from the TEB and complete STACK16FRAME */
    output( "\tmovl %%fs:(%d), %%ebp\n", STACKOFFSET );
    output( "\tpushl %%ebp\n" );

    /* Switch stacks */
    output( "\tmovw %%ss, %%fs:(%d)\n", STACKOFFSET + 2 );
    output( "\tmovw %%sp, %%fs:(%d)\n", STACKOFFSET );
    output( "\tpushl %%ds\n" );
    output( "\tpopl %%ss\n" );
    output( "\tmovl %%ebp, %%esp\n" );
    output( "\taddl $0x20,%%ebp\n");  /* FIELD_OFFSET(STACK32FRAME,ebp) */


    /* At this point:
       STACK16FRAME is completely set up
       DS, ES, SS: flat data segment
       FS: current TEB
       ESP: points to last STACK32FRAME
       EBP: points to ebp member of last STACK32FRAME
       EDX: points to current STACK16FRAME
       ECX: contains saved flags
       all other registers: unchanged */

    /* Special case: C16ThkSL stub */
    if ( thunk )
    {
        /* Set up registers as expected and call thunk */
        output( "\tleal 0x1a(%%edx),%%ebx\n" );  /* sizeof(STACK16FRAME)-22 */
        output( "\tleal -4(%%esp), %%ebp\n" );

        output( "\tcall *0x26(%%edx)\n");  /* FIELD_OFFSET(STACK16FRAME,entry_point) */

        /* Switch stack back */
        output( "\tmovw %%fs:(%d), %%ss\n", STACKOFFSET+2 );
        output( "\tmovzwl %%fs:(%d), %%esp\n", STACKOFFSET );
        output( "\tpopl %%fs:(%d)\n", STACKOFFSET );

        /* Restore registers and return directly to caller */
        output( "\taddl $8, %%esp\n" );
        output( "\tpopl %%ebp\n" );
        output( "\tpopw %%ds\n" );
        output( "\tpopw %%es\n" );
        output( "\tpopw %%fs\n" );
        output( "\tpopw %%gs\n" );
        output( "\taddl $20, %%esp\n" );

        output( "\txorb %%ch, %%ch\n" );
        output( "\tpopl %%ebx\n" );
        output( "\taddw %%cx, %%sp\n" );
        output( "\tpush %%ebx\n" );

        output( "\t.byte 0x66\n" );
        output( "\tlret\n" );

        output_function_size( "__wine_call_from_16_thunk" );
        return;
    }


    /* Build register CONTEXT */
    if ( reg_func )
    {
        output( "\tsubl $0x2cc,%%esp\n" );       /* sizeof(CONTEXT86) */

        output( "\tmovl %%ecx,0xc0(%%esp)\n" );  /* EFlags */

        output( "\tmovl %%eax,0xb0(%%esp)\n" );  /* Eax */
        output( "\tmovl %%ebx,0xa4(%%esp)\n" );  /* Ebx */
        output( "\tmovl %%esi,0xa0(%%esp)\n" );  /* Esi */
        output( "\tmovl %%edi,0x9c(%%esp)\n" );  /* Edi */

        output( "\tmovl 0x0c(%%edx),%%eax\n");   /* FIELD_OFFSET(STACK16FRAME,ebp) */
        output( "\tmovl %%eax,0xb4(%%esp)\n" );  /* Ebp */
        output( "\tmovl 0x08(%%edx),%%eax\n");   /* FIELD_OFFSET(STACK16FRAME,ecx) */
        output( "\tmovl %%eax,0xac(%%esp)\n" );  /* Ecx */
        output( "\tmovl 0x04(%%edx),%%eax\n");   /* FIELD_OFFSET(STACK16FRAME,edx) */
        output( "\tmovl %%eax,0xa8(%%esp)\n" );  /* Edx */

        output( "\tmovzwl 0x10(%%edx),%%eax\n"); /* FIELD_OFFSET(STACK16FRAME,ds) */
        output( "\tmovl %%eax,0x98(%%esp)\n" );  /* SegDs */
        output( "\tmovzwl 0x12(%%edx),%%eax\n"); /* FIELD_OFFSET(STACK16FRAME,es) */
        output( "\tmovl %%eax,0x94(%%esp)\n" );  /* SegEs */
        output( "\tmovzwl 0x14(%%edx),%%eax\n"); /* FIELD_OFFSET(STACK16FRAME,fs) */
        output( "\tmovl %%eax,0x90(%%esp)\n" );  /* SegFs */
        output( "\tmovzwl 0x16(%%edx),%%eax\n"); /* FIELD_OFFSET(STACK16FRAME,gs) */
        output( "\tmovl %%eax,0x8c(%%esp)\n" );  /* SegGs */

        output( "\tmovzwl 0x2e(%%edx),%%eax\n"); /* FIELD_OFFSET(STACK16FRAME,cs) */
        output( "\tmovl %%eax,0xbc(%%esp)\n" );  /* SegCs */
        output( "\tmovzwl 0x2c(%%edx),%%eax\n"); /* FIELD_OFFSET(STACK16FRAME,ip) */
        output( "\tmovl %%eax,0xb8(%%esp)\n" );  /* Eip */

        output( "\tmovzwl %%fs:(%d), %%eax\n", STACKOFFSET+2 );
        output( "\tmovl %%eax,0xc8(%%esp)\n" );  /* SegSs */
        output( "\tmovzwl %%fs:(%d), %%eax\n", STACKOFFSET );
        output( "\taddl $0x2c,%%eax\n");         /* FIELD_OFFSET(STACK16FRAME,ip) */
        output( "\tmovl %%eax,0xc4(%%esp)\n" );  /* Esp */
#if 0
        output( "\tfsave 0x1c(%%esp)\n" ); /* FloatSave */
#endif

        /* Push address of CONTEXT86 structure -- popped by the relay routine */
        output( "\tmovl %%esp,%%eax\n" );
        output( "\tandl $~15,%%esp\n" );
        output( "\tsubl $4,%%esp\n" );
        output( "\tpushl %%eax\n" );
    }
    else
    {
        output( "\tsubl $8,%%esp\n" );
        output( "\tandl $~15,%%esp\n" );
        output( "\taddl $8,%%esp\n" );
    }

    /* Call relay routine (which will call the API entry point) */
    output( "\tleal 0x30(%%edx),%%eax\n" ); /* sizeof(STACK16FRAME) */
    output( "\tpushl %%eax\n" );
    output( "\tpushl 0x26(%%edx)\n");  /* FIELD_OFFSET(STACK16FRAME,entry_point) */
    output( "\tcall *0x20(%%edx)\n");  /* FIELD_OFFSET(STACK16FRAME,relay) */

    if ( reg_func )
    {
        output( "\tleal -748(%%ebp),%%ebx\n" ); /* sizeof(CONTEXT) + FIELD_OFFSET(STACK32FRAME,ebp) */

        /* Switch stack back */
        output( "\tmovw %%fs:(%d), %%ss\n", STACKOFFSET+2 );
        output( "\tmovzwl %%fs:(%d), %%esp\n", STACKOFFSET );
        output( "\tpopl %%fs:(%d)\n", STACKOFFSET );

        /* Get return address to CallFrom16 stub */
        output( "\taddw $0x14,%%sp\n" ); /* FIELD_OFFSET(STACK16FRAME,callfrom_ip)-4 */
        output( "\tpopl %%eax\n" );
        output( "\tpopl %%edx\n" );

        /* Restore all registers from CONTEXT */
        output( "\tmovw 0xc8(%%ebx),%%ss\n");   /* SegSs */
        output( "\tmovl 0xc4(%%ebx),%%esp\n");  /* Esp */
        output( "\taddl $4, %%esp\n" );  /* room for final return address */

        output( "\tpushw 0xbc(%%ebx)\n");  /* SegCs */
        output( "\tpushw 0xb8(%%ebx)\n");  /* Eip */
        output( "\tpushl %%edx\n" );
        output( "\tpushl %%eax\n" );
        output( "\tpushl 0xc0(%%ebx)\n");  /* EFlags */
        output( "\tpushl 0x98(%%ebx)\n");  /* SegDs */

        output( "\tpushl 0x94(%%ebx)\n");  /* SegEs */
        output( "\tpopl %%es\n" );
        output( "\tpushl 0x90(%%ebx)\n");  /* SegFs */
        output( "\tpopl %%fs\n" );
        output( "\tpushl 0x8c(%%ebx)\n");  /* SegGs */
        output( "\tpopl %%gs\n" );

        output( "\tmovl 0xb4(%%ebx),%%ebp\n");  /* Ebp */
        output( "\tmovl 0xa0(%%ebx),%%esi\n");  /* Esi */
        output( "\tmovl 0x9c(%%ebx),%%edi\n");  /* Edi */
        output( "\tmovl 0xb0(%%ebx),%%eax\n");  /* Eax */
        output( "\tmovl 0xa8(%%ebx),%%edx\n");  /* Edx */
        output( "\tmovl 0xac(%%ebx),%%ecx\n");  /* Ecx */
        output( "\tmovl 0xa4(%%ebx),%%ebx\n");  /* Ebx */

        output( "\tpopl %%ds\n" );
        output( "\tpopfl\n" );
        output( "\tlret\n" );

        output_function_size( "__wine_call_from_16_regs" );
    }
    else
    {
        /* Switch stack back */
        output( "\tmovw %%fs:(%d), %%ss\n", STACKOFFSET+2 );
        output( "\tmovzwl %%fs:(%d), %%esp\n", STACKOFFSET );
        output( "\tpopl %%fs:(%d)\n", STACKOFFSET );

        /* Restore registers */
        output( "\tpopl %%edx\n" );
        output( "\tpopl %%ecx\n" );
        output( "\tpopl %%ebp\n" );
        output( "\tpopw %%ds\n" );
        output( "\tpopw %%es\n" );
        output( "\tpopw %%fs\n" );
        output( "\tpopw %%gs\n" );

        /* Return to return stub which will return to caller */
        output( "\tlret $12\n" );

        output_function_size( "__wine_call_from_16" );
    }
}


/*******************************************************************
 *         BuildCallTo16Core
 *
 * This routine builds the core routines used in 32->16 thunks:
 *
 * extern DWORD WINAPI wine_call_to_16( FARPROC16 target, DWORD cbArgs, PEXCEPTION_HANDLER handler );
 * extern void WINAPI wine_call_to_16_regs( CONTEXT86 *context, DWORD cbArgs, PEXCEPTION_HANDLER handler );
 *
 * These routines can be called directly from 32-bit code.
 *
 * All routines expect that the 16-bit stack contents (arguments) and the
 * return address (segptr to CallTo16_Ret) were already set up by the
 * caller; nb_args must contain the number of bytes to be conserved.  The
 * 16-bit SS:SP will be set accordingly.
 *
 * All other registers are either taken from the CONTEXT86 structure
 * or else set to default values.  The target routine address is either
 * given directly or taken from the CONTEXT86.
 */
static void BuildCallTo16Core( int reg_func )
{
    const char *name = reg_func ? "wine_call_to_16_regs" : "wine_call_to_16";
    const char *func_name = is_pe() ? strmake( "%s@12", name ) : name;

    /* Function header */
    output_function_header( func_name, 1 );

    /* Function entry sequence */
    output_cfi( ".cfi_startproc" );
    output( "\tpushl %%ebp\n" );
    output_cfi( ".cfi_adjust_cfa_offset 4" );
    output_cfi( ".cfi_rel_offset %%ebp,0" );
    output( "\tmovl %%esp, %%ebp\n" );
    output_cfi( ".cfi_def_cfa_register %%ebp" );

    /* Save the 32-bit registers */
    output( "\tpushl %%ebx\n" );
    output_cfi( ".cfi_rel_offset %%ebx,-4" );
    output( "\tpushl %%esi\n" );
    output_cfi( ".cfi_rel_offset %%esi,-8" );
    output( "\tpushl %%edi\n" );
    output_cfi( ".cfi_rel_offset %%edi,-12" );
    output( "\tmov %%gs,%%fs:(%d)\n", GS_OFFSET );

    /* Setup exception frame */
    output( "\tpushl %%fs:(%d)\n", STACKOFFSET );
    output( "\tpushl 16(%%ebp)\n" ); /* handler */
    output( "\tpushl %%fs:(0)\n" );
    output( "\tmovl %%esp,%%fs:(0)\n" );

    /* Call the actual CallTo16 routine (simulate a lcall) */
    output( "\tpushl %%cs\n" );
    output( "\tcall .L%s\n", name );

    /* Remove exception frame */
    output( "\tpopl %%fs:(0)\n" );
    output( "\taddl $4, %%esp\n" );
    output( "\tpopl %%fs:(%d)\n", STACKOFFSET );

    if ( !reg_func )
    {
        /* Convert return value */
        output( "\tandl $0xffff,%%eax\n" );
        output( "\tshll $16,%%edx\n" );
        output( "\torl %%edx,%%eax\n" );
    }
    else
    {
        /*
         * Modify CONTEXT86 structure to contain new values
         *
         * NOTE:  We restore only EAX, EBX, ECX, EDX, EBP, and ESP.
         *        The segment registers as well as ESI and EDI should
         *        not be modified by a well-behaved 16-bit routine in
         *        any case.  [If necessary, we could restore them as well,
         *        at the cost of a somewhat less efficient return path.]
         */

        output( "\tmovl 0x14(%%esp),%%edi\n" ); /* FIELD_OFFSET(STACK32FRAME,target) - FIELD_OFFSET(STACK32FRAME,edi) */
                /* everything above edi has been popped already */

        output( "\tmovl %%eax,0xb0(%%edi)\n");  /* Eax */
        output( "\tmovl %%ebx,0xa4(%%edi)\n");  /* Ebx */
        output( "\tmovl %%ecx,0xac(%%edi)\n");  /* Ecx */
        output( "\tmovl %%edx,0xa8(%%edi)\n");  /* Edx */
        output( "\tmovl %%ebp,0xb4(%%edi)\n");  /* Ebp */
        output( "\tmovl %%esi,0xc4(%%edi)\n");  /* Esp */
                 /* The return glue code saved %esp into %esi */
    }

    /* Restore the 32-bit registers */
    output( "\tpopl %%edi\n" );
    output_cfi( ".cfi_same_value %%edi" );
    output( "\tpopl %%esi\n" );
    output_cfi( ".cfi_same_value %%esi" );
    output( "\tpopl %%ebx\n" );
    output_cfi( ".cfi_same_value %%ebx" );

    /* Function exit sequence */
    output( "\tpopl %%ebp\n" );
    output_cfi( ".cfi_def_cfa %%esp,4" );
    output_cfi( ".cfi_same_value %%ebp" );
    output( "\tret $12\n" );
    output_cfi( ".cfi_endproc" );


    /* Start of the actual CallTo16 routine */

    output( ".L%s:\n", name );

    /* Switch to the 16-bit stack */
    output( "\tmovl %%esp,%%edx\n" );
    output( "\tmovw %%fs:(%d),%%ss\n", STACKOFFSET + 2);
    output( "\tmovw %%fs:(%d),%%sp\n", STACKOFFSET );
    output( "\tmovl %%edx,%%fs:(%d)\n", STACKOFFSET );

    /* Make %bp point to the previous stackframe (built by CallFrom16) */
    output( "\tmovzwl %%sp,%%ebp\n" );
    output( "\tleal 0x2a(%%ebp),%%ebp\n");  /* FIELD_OFFSET(STACK16FRAME,bp) */

    /* Add the specified offset to the new sp */
    output( "\tsubw 0x2c(%%edx), %%sp\n");  /* FIELD_OFFSET(STACK32FRAME,nb_args) */

    if (reg_func)
    {
        /* Push the called routine address */
        output( "\tmovl 0x28(%%edx),%%edx\n");  /* FIELD_OFFSET(STACK32FRAME,target) */
        output( "\tpushw 0xbc(%%edx)\n");  /* SegCs */
        output( "\tpushw 0xb8(%%edx)\n");  /* Eip */

        /* Get the registers */
        output( "\tpushw 0x98(%%edx)\n");  /* SegDs */
        output( "\tpushl 0x94(%%edx)\n");  /* SegEs */
        output( "\tpopl %%es\n" );
        output( "\tmovl 0xb4(%%edx),%%ebp\n");  /* Ebp */
        output( "\tmovl 0xa0(%%edx),%%esi\n");  /* Esi */
        output( "\tmovl 0x9c(%%edx),%%edi\n");  /* Edi */
        output( "\tmovl 0xb0(%%edx),%%eax\n");  /* Eax */
        output( "\tmovl 0xa4(%%edx),%%ebx\n");  /* Ebx */
        output( "\tmovl 0xac(%%edx),%%ecx\n");  /* Ecx */
        output( "\tmovl 0xa8(%%edx),%%edx\n");  /* Edx */

        /* Get the 16-bit ds */
        output( "\tpopw %%ds\n" );
    }
    else  /* not a register function */
    {
        /* Push the called routine address */
        output( "\tpushl 0x28(%%edx)\n"); /* FIELD_OFFSET(STACK32FRAME,target) */

        /* Set %fs and %gs to the value saved by the last CallFrom16 */
        output( "\tpushw -22(%%ebp)\n" ); /* FIELD_OFFSET(STACK16FRAME,fs)-FIELD_OFFSET(STACK16FRAME,bp) */
        output( "\tpopw %%fs\n" );
        output( "\tpushw -20(%%ebp)\n" ); /* FIELD_OFFSET(STACK16FRAME,gs)-FIELD_OFFSET(STACK16FRAME,bp) */
        output( "\tpopw %%gs\n" );

        /* Set %ds and %es (and %ax just in case) equal to %ss */
        output( "\tmovw %%ss,%%ax\n" );
        output( "\tmovw %%ax,%%ds\n" );
        output( "\tmovw %%ax,%%es\n" );
    }

    /* Jump to the called routine */
    output( "\tlretw\n" );

    /* Function footer */
    output_function_size( func_name );
}


/*******************************************************************
 *         BuildRet16Func
 *
 * Build the return code for 16-bit callbacks
 */
static void BuildRet16Func(void)
{
    output_function_header( "__wine_call_to_16_ret", 1 );

    /* Save %esp into %esi */
    output( "\tmovl %%esp,%%esi\n" );

    /* Restore 32-bit segment registers */

    output( "\tmovl %%cs:%s", asm_name("CallTo16_DataSelector") );
    output( "-%s,%%edi\n", asm_name("__wine_call16_start") );
    output( "\tmovw %%di,%%ds\n" );
    output( "\tmovw %%di,%%es\n" );

    output( "\tmov %%cs:%s", asm_name("CallTo16_TebSelector") );
    output( "-%s,%%fs\n", asm_name("__wine_call16_start") );

    output( "\tmov %%fs:(%d),%%gs\n", GS_OFFSET );

    /* Restore the 32-bit stack */

    output( "\tmovw %%di,%%ss\n" );
    output( "\tmovl %%fs:(%d),%%esp\n", STACKOFFSET );

    /* Return to caller */

    output( "\tlret\n" );
    output_function_size( "__wine_call_to_16_ret" );
}


/*******************************************************************
 *         output_asm_relays16
 *
 * Build all the 16-bit relay callbacks
 */
void output_asm_relays16(void)
{
    /* File header */

    output( "\t.text\n" );
    output( "%s:\n\n", asm_name("__wine_spec_thunk_text_16") );

    output( "%s\n", asm_globl("__wine_call16_start") );

    /* Standard CallFrom16 routine */
    BuildCallFrom16Core( 0, 0 );

    /* Register CallFrom16 routine */
    BuildCallFrom16Core( 1, 0 );

    /* C16ThkSL CallFrom16 routine */
    BuildCallFrom16Core( 0, 1 );

    /* Standard CallTo16 routine */
    BuildCallTo16Core( 0 );

    /* Register CallTo16 routine */
    BuildCallTo16Core( 1 );

    /* Standard CallTo16 return stub */
    BuildRet16Func();

    output( "%s\n", asm_globl("__wine_call16_end") );
    output_function_size( "__wine_spec_thunk_text_16" );

    /* Declare the return address and data selector variables */
    output( "\n\t.data\n\t.align %d\n", get_alignment(4) );
    output( "%s\n\t.long 0\n", asm_globl("CallTo16_DataSelector") );
    output( "%s\n\t.long 0\n", asm_globl("CallTo16_TebSelector") );
}
