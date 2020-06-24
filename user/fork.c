// implement fork from user space

#include "lib.h"
#include <mmu.h>
#include <env.h>


/* ----------------- help functions ---------------- */

/* Overview:
 * 	Copy `len` bytes from `src` to `dst`.
 *
 * Pre-Condition:
 * 	`src` and `dst` can't be NULL. Also, the `src` area 
 * 	 shouldn't overlap the `dest`, otherwise the behavior of this 
 * 	 function is undefined.
 */
void user_bcopy(const void *src, void *dst, size_t len)
{
	void *max;

	//	writef("~~~~~~~~~~~~~~~~ src:%x dst:%x len:%x\n",(int)src,(int)dst,len);
	max = dst + len;

	// copy machine words while possible
	if (((int)src % 4 == 0) && ((int)dst % 4 == 0)) {
		while (dst + 3 < max) {
			*(int *)dst = *(int *)src;
			dst += 4;
			src += 4;
		}
	}

	// finish remaining 0-3 bytes
	while (dst < max) {
		*(char *)dst = *(char *)src;
		dst += 1;
		src += 1;
	}

	//for(;;);
}

/* Overview:
 * 	Sets the first n bytes of the block of memory 
 * pointed by `v` to zero.
 * 
 * Pre-Condition:
 * 	`v` must be valid.
 *
 * Post-Condition:
 * 	the content of the space(from `v` to `v`+ n) 
 * will be set to zero.
 */
void user_bzero(void *v, u_int n)
{
	char *p;
	int m;

	p = v;
	m = n;

	while (--m >= 0) {
		*p++ = 0;
	}
}
/*--------------------------------------------------------------*/

/* Overview:
 * 	Custom page fault handler - if faulting page is copy-on-write,
 * map in our own private writable copy.
 * 
 * Pre-Condition:
 * 	`va` is the address which leads to a TLBS exception.
 *
 * Post-Condition:
 *  Launch a user_panic if `va` is not a copy-on-write page.
 * Otherwise, this handler should map a private writable copy of 
 * the faulting page at correct address.
 */
/*** exercise 4.13 ***/
static void
pgfault(u_int va)
{
    u_int tmp = UTOP - 2 * BY2PG;
	//	writef("fork.c:pgfault():\t va:%x\n",va);

	u_int perm = ((Pte *)(*vpt))[VPN(va)] & 0xfff;

	if ((perm & PTE_COW) == 0) {
		user_panic("Not a COW page\n");
	}

	//map the new page at a temporary place
	u_int newPerm = (perm | PTE_R | PTE_V) & (~PTE_COW);
	int r = syscall_mem_alloc(0, tmp, newPerm);
	if (r < 0) {
		user_panic("Page alloc failed\n");
	}
	//copy the content
	user_bcopy(ROUNDDOWN(va, BY2PG), tmp, BY2PG);
	//map the page on the appropriate place
	r = syscall_mem_map(0, tmp, 0, va, newPerm);
	if (r < 0) {
		user_panic("Map to va failed\n");
	}
	//unmap the temporary place
	r = syscall_mem_unmap(0, tmp);
	if (r < 0) {
		user_panic("Unmap failed\n");
	}
    
    struct Trapframe *tfaddr = (void *)env->env_xstacktop - sizeof(struct Trapframe);

	u_int *pc = (u_int *) tfaddr->cp0_epc;
	u_int instr = *pc;
	u_int opcode = (instr >> 26) & 0x3f;
	u_int rs = (instr >> 21) & 0x1f;
	u_int rt = (instr >> 16) & 0x1f;
	env->env_runs++;
	writef("Env: 0x%x, Instr: 0x%x, opcode: %b, reg_rs: %d, reg_rt: %d, OUT_count: %d, COW_count: %d\n",
		   env->env_id, instr, opcode, rs, rt, env->env_nop, env->env_runs);
}

/* Overview:
 * 	Map our virtual page `pn` (address pn*BY2PG) into the target `envid`
 * at the same virtual address. 
 *
 * Post-Condition:
 *  if the page is writable or copy-on-write, the new mapping must be 
 * created copy on write and then our mapping must be marked 
 * copy on write as well. In another word, both of the new mapping and
 * our mapping should be copy-on-write if the page is writable or 
 * copy-on-write.
 * 
 * Hint:
 * 	PTE_LIBRARY indicates that the page is shared between processes.
 * A page with PTE_LIBRARY may have PTE_R at the same time. You
 * should process it correctly.
 */
/*** exercise 4.10 ***/
static void
duppage(u_int envid, u_int pn)
{
    u_int addr;
	u_int perm;

	addr = pn * BY2PG;

	perm = (((Pte *) (*vpt))[pn]) & 0xfff;

	int r = 0;
	if ((perm & PTE_R) == 0) {
		r = syscall_mem_map(0, addr, envid, addr, perm);
		if (r < 0) {
			user_panic("ERROR in readonly map\n");
		}
	} else if ((perm & PTE_LIBRARY) != 0) {
		syscall_mem_map(0, addr, envid, addr, perm);
		if (r < 0) {
			user_panic("ERROR in libaray map\n");
		}
	} else if ((perm & PTE_COW) != 0) {
		syscall_mem_map(0, addr, envid, addr, perm);
		if (r < 0) {
			user_panic("ERROR in already COW map\n");
		}
	} else {
		syscall_mem_map(0, addr, envid, addr, perm | PTE_COW);
		if (r < 0) {
			user_panic("ERROR in map to child map\n");
		}
		syscall_mem_map(0, addr, 0, addr, perm | PTE_COW);
		if (r < 0) {
			user_panic("ERROR in map to self map\n");
		}
	}
}

/* Overview:
 * 	User-level fork. Create a child and then copy our address space
 * and page fault handler setup to the child.
 *
 * Hint: use vpd, vpt, and duppage.
 * Hint: remember to fix "env" in the child process!
 * Note: `set_pgfault_handler`(user/pgfault.c) is different from 
 *       `syscall_set_pgfault_handler`. 
 */
/*** exercise 4.9 4.15***/
extern void __asm_pgfault_handler(void);
int
fork(void)
{
	// Your code here.
	u_int newenvid;
	extern struct Env *envs;
	extern struct Env *env;
	u_int i;

    //The parent installs pgfault using set_pgfault_handler
	set_pgfault_handler(pgfault);
	//alloc a new alloc
    newenvid = syscall_env_alloc();

	if (newenvid < 0) {
		return newenvid;
	}

	if (newenvid != 0) {
        for (i = 0; i < VPN(USTACKTOP); i++) {
			if ((((Pde *)(*vpd))[(i >> 10)] & PTE_V) != 0 && (((Pte *)(*vpt))[i] & PTE_V) != 0) {
                duppage(newenvid, i);
            }
        }
		i = syscall_mem_alloc(newenvid, UXSTACKTOP - BY2PG, PTE_V | PTE_R);
		if (i < 0) {
			user_panic("Error in allocing uxstack\n");
		}
		i = syscall_set_pgfault_handler(newenvid, __asm_pgfault_handler, UXSTACKTOP);
		if (i < 0) {
			user_panic("Error in setting pgfault_handler\n");
		}
		i = syscall_set_env_status(newenvid, ENV_RUNNABLE);
		if (i < 0) {
			user_panic("Error in setting child's status\n");
		}
    }
	else
	{
		// child
		int envid;
		envid = syscall_getenvid();
		envid = ENVX(envid);
		env = &envs[envid];
    }

	return newenvid;
}

// Challenge!
int
sfork(void)
{
	user_panic("sfork not implemented");
	return -E_INVAL;
}
