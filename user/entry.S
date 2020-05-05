#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>
#include <mmu.h>
#include <trap.h>
#include <unistd.h>
	.data

	.p2align 12

	.globl fsipcbuf
fsipcbuf:
	.space BY2PG
	.globl fdtab
fdtab:
	.space BY2PG


	.globl envs
envs:
	.word UENVS

	.globl pages
pages:
	.word UPAGES

	.globl vpt
vpt:
	.word UVPT

	.globl vpd
vpd:
	.word (UVPT+(UVPT>>12)*4)

	.globl __pgfault_handler
	__pgfault_handler:
	.word 0


	.extern libmain

	.text
	.globl _start
_start:
	//1: j 1b
	lw	a0, 0(sp)
	lw	a1, 4(sp)
	//sw	a0, -4(sp)
	//sw	a1, -0(sp)
nop
	jal	libmain
	nop

.set noreorder
.text
.globl __asm_pgfault_handler
__asm_pgfault_handler:
	// save the caller-save registers
	//	(your code here)
//1: j 1b
nop
	lw	a0, TF_BADVADDR(sp)
	//sw	t0, (sp)
	//subu	sp,16
	lw	t1, __pgfault_handler
	jalr	t1

nop
	// push trap-time eip, eflags onto trap-time stack
	//	(your code here)
	//addu	sp,16
	lw	v1,TF_LO(sp)                                       
		mtlo	v1                               
		lw	v0,TF_HI(sp)                                         
		lw	v1,TF_EPC(sp)                    
		mthi	v0                               
		mtc0	v1,CP0_EPC                                             
		lw	$31,TF_REG31(sp)                 
		lw	$30,TF_REG30(sp)                 
		lw	$28,TF_REG28(sp)                 
		lw	$25,TF_REG25(sp)                 
		lw	$24,TF_REG24(sp)                 
		lw	$23,TF_REG23(sp)                 
		lw	$22,TF_REG22(sp)                 
		lw	$21,TF_REG21(sp)                 
		lw	$20,TF_REG20(sp)                 
		lw	$19,TF_REG19(sp)                 
		lw	$18,TF_REG18(sp)                 
		lw	$17,TF_REG17(sp)                 
		lw	$16,TF_REG16(sp)                 
		lw	$15,TF_REG15(sp)                 
		lw	$14,TF_REG14(sp)                 
		lw	$13,TF_REG13(sp)                 
		lw	$12,TF_REG12(sp)                 
		lw	$11,TF_REG11(sp)                 
		lw	$10,TF_REG10(sp)                 
		lw	$9,TF_REG9(sp)                   
		lw	$8,TF_REG8(sp)                   
		lw	$7,TF_REG7(sp)                   
		lw	$6,TF_REG6(sp)                   
		lw	$5,TF_REG5(sp)                   
		lw	$4,TF_REG4(sp)                   
		lw	$3,TF_REG3(sp)                   
		lw	$2,TF_REG2(sp)                   
		lw	$1,TF_REG1(sp) 
		lw	k0,TF_EPC(sp) 	//atomic operation needed 
		jr	k0			//
		lw	sp,TF_REG29(sp)  /* Deallocate stack */  


.globl syscall_ipc_can_multi_send
syscall_ipc_can_multi_send:
	// send a3 (env_count) into t0
	move t0, a3
	// send a2 (perm) into t3
	move t3, a2
	// set up reg param
	move a3, a1
	move a2, a0
	// set up a0 (sysno)
	li a0, SYS_ipc_can_multi_send
	// set up a1 (total param)
	addiu a1, t0, 4

    move t1, sp         // t1 holds old sp
	// save ra, s0, sp
	addiu sp, sp, -4
	sw ra, (sp)
	addiu sp, sp, -4
	sw s0, (sp)
	// cal stack length
	addiu t2, t0, 6 	// 1 sysno, 1 param cnt, 3 param in reg and 1 env_count
	sll t2, t2, 2
	move s0, t2
	subu sp, sp, t2

	sw t3, 16(sp)
	sw t0, 20(sp)

	li t3, 0
	s_sicmsParamCopyLoop:
	bge t3, t0, s_sicmsParamCopyLoop_end
	nop
		addiu t4, t3, 4
		sll t4, t4, 2
		
		addu t5, t1, t4
		lw t6, (t5)

		addu t5, sp, t4
		addiu t5, t5, 8
		sw t6,(t5)

		addiu t3, t3, 1
		j s_sicmsParamCopyLoop
		nop
	s_sicmsParamCopyLoop_end:

    jal msyscall
	nop

	addu sp, sp, s0
	lw s0, (sp)
	addiu sp, sp, 4
	lw ra, (sp)
	addiu sp, sp, 4

	jr ra
	nop
