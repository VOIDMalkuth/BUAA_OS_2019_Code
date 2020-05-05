#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>
#include <stackframe.h>
#include <unistd.h>

/*** exercise 4.2 ***/
NESTED(handle_sys,TF_SIZE, sp)
    SAVE_ALL                            // Macro used to save trapframe
    CLI                                 // Clean Interrupt Mask
    nop
    .set at                             // Resume use of $at

    // TODO: Fetch EPC from Trapframe, calculate a proper value and store it back to trapframe.
    
    // !: no syscall in delay branch, just ignore
    lw t0, TF_EPC(sp)
    addiu t0, t0, 4
    sw t0, TF_EPC(sp)

    // TODO: Copy the syscall number into $a0.
    lw  a0, TF_REG4(sp)
    
    addiu   a0, a0, -__SYSCALL_BASE     // a0 <- relative syscall number
    sll     t0, a0, 2                   // t0 <- relative syscall number times 4
    la      t1, sys_call_table          // t1 <- syscall table base
    addu    t1, t1, t0                  // t1 <- table entry of specific syscall
    lw      t2, 0(t1)                   // t2 <- function entry of specific syscall

    lw      t0, TF_REG29(sp)            // t0 <- user's stack pointer

    // get all regs ready before sp changes
    // save a1 to t3 (syscall param cnt)
    lw t3, TF_REG5(sp)
    
    beq t3, zero, s_noParam
    li t9, 1
    beq t3, t9, s_1Param
    li t9, 2
    beq t3, t9, s_2Param
    nop
    // if 3 params and more 
        lw a3, 16(t0)
    s_2Param:
        lw a2, TF_REG7(sp)
    s_1Param:
        lw a1, TF_REG6(sp)
    s_noParam:
    
    // old sp is in t4 now
    move t4, sp
    // t5 holds stack length
    addiu t5, t3, 1
    sll t5, t5, 2
    // save s0 before function
    addiu sp, sp, -4
    sw s0, (sp)
    // save length to s0
    move s0, t5
    // new sp
    subu sp, sp, t5

    // copy to kernel stack as long as more than 3 param
    // user 6 (syscall param 4) -- kernel 5
    li t5, 4
    s_startCopyStack:
    blt t3, t5, s_endCopyStack
    nop
        addiu t6, t5, 1
        sll t6, t6, 2
        addu t6, t6, t0

        lw t7, (t6)

        addiu t6, t5, 0
        sll t6, t6, 2
        addu t6, t6, sp

        sw t7, (t6)

        addiu t5, t5, 1
        j s_startCopyStack
        // in fact we can use delay branch to save t7, but for understanding...
        nop
    s_endCopyStack:

    // Finished stack operation, call real handler now

    jalr    t2                          // Invoke sys_* function
    nop
    
    // TODO: Resume current kernel stack
    addu sp, sp, s0
    lw s0, (sp)
    addiu sp, sp, 4

    sw      v0, TF_REG2(sp)             // Store return value of function sys_* (in $v0) into trapframe

    j       ret_from_exception          // Return from exeception
    nop
END(handle_sys)

sys_call_table:                         // Syscall Table
.align 2
    .word sys_putchar
    .word sys_getenvid
    .word sys_yield
    .word sys_env_destroy
    .word sys_set_pgfault_handler
    .word sys_mem_alloc
    .word sys_mem_map
    .word sys_mem_unmap
    .word sys_env_alloc
    .word sys_set_env_status
    .word sys_set_trapframe
    .word sys_panic
    .word sys_ipc_can_send
    .word sys_ipc_recv
    .word sys_cgetc
    .word sys_super_multi_parameter
    .word sys_ipc_can_multi_send
