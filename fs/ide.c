/*
 * operations on IDE disk.
 */

#include "fs.h"
#include "lib.h"
#include <mmu.h>

int ideOperation(u_int op, u_int diskno, u_int offset) {
	u_int ideMemBase = 0x13000000;
	volatile u_char *ideOpAddr = (volatile u_char *)(ideMemBase + 0x0020);
	volatile u_int *ideDiskAddr = (volatile u_int *)(ideMemBase + 0x0010);
	volatile u_int *ideOffsetAddr = (volatile u_int *)(ideMemBase + 0x0000);
	volatile u_int *ideResultAddr = (volatile u_int *)(ideMemBase + 0x0030);
	const u_int ideOp_read = 0;
	const u_int ideOp_write = 1;

	if (op != ideOp_read && op != ideOp_write) {
		user_panic("IDE Operation %d illegal!\n", op);
	}
	
	if ((offset & (BY2SECT - 1)) != 0) {
		user_panic("IDE Offset 0x%x is not aligned!\n", offset);
	}
    
    syscall_write_dev(&diskno, ideDiskAddr, sizeof(diskno));
    syscall_write_dev(&offset, ideOffsetAddr, sizeof(offset));
    syscall_write_dev(&op, ideOpAddr, sizeof(op));
    
    int res = 0;
    syscall_read_dev(&res, ideResultAddr, sizeof(res));

	return res;
}

// Overview:
// 	read data from IDE disk. First issue a read request through
// 	disk register and then copy data from disk buffer
// 	(512 bytes, a sector) to destination array.
//
// Parameters:
//	diskno: disk number.
// 	secno: start sector number.
// 	dst: destination for data read from IDE disk.
// 	nsecs: the number of sectors to read.
//
// Post-Condition:
// 	If error occurred during read the IDE disk, panic. 
// 	
// Hint: use syscalls to access device registers and buffers
void
ide_read(u_int diskno, u_int secno, void *dst, u_int nsecs)
{
    // 0x200: the size of a sector: 512 bytes.
	int offset_begin = secno * 0x200;
	int offset_end = offset_begin + nsecs * 0x200;
	int offset = 0;

	u_int ideDataBase = 0x13000000 + 0x4000;
	const u_int ideOp_read = 0;

	while (offset_begin + offset < offset_end) {
		int r = ideOperation(ideOp_read, diskno, offset_begin + offset);
		// 0 means failure in ide!
		if (r == 0) {
			user_panic("IDE operation failed, op: %d, diskno: %d, offset: 0x%x\n", ideOp_read, diskno, offset_begin + offset);
		}
		r = syscall_read_dev(dst + offset, ideDataBase, BY2SECT);
		if (r != 0) {
			user_panic("Read_dev(IDE data) failed in IDE, va: 0x%x, return val is %d\n", dst + offset, r);
		}
		offset += BY2SECT;
	}

	return;
}


// Overview:
// 	write data to IDE disk.
//
// Parameters:
//	diskno: disk number.
//	secno: start sector number.
// 	src: the source data to write into IDE disk.
//	nsecs: the number of sectors to write.
//
// Post-Condition:
//	If error occurred during read the IDE disk, panic.
//	
// Hint: use syscalls to access device registers and buffers
void
ide_write(u_int diskno, u_int secno, void *src, u_int nsecs)
{
    int offset_begin = secno * BY2SECT;
	int offset_end = offset_begin + nsecs * BY2SECT;
	int offset = 0;
	
	u_int ideDataBase = 0x13000000 + 0x4000;
	const u_int ideOp_write = 1;

#ifdef DETAIL_OUTPUT
	writef("diskno: %d\n", diskno);
#endif /* DETAIL_OUTPUT */
	while (offset_begin + offset < offset_end) {
	    int r = syscall_write_dev(src + offset, ideDataBase, BY2SECT);
		if (r != 0) {
			user_panic("Write_dev(IDE data) failed in IDE, va: 0x%x\n", src + offset);
		}
		r = ideOperation(ideOp_write, diskno, offset_begin + offset);
		// 0 means failure in ide!
		if (r == 0) {
			user_panic("IDE operation failed, op: %d, diskno: %d, offset: 0x%x\n", ideOp_write, diskno, offset_begin + offset);
		}
		offset += BY2SECT;
    }

	return;
}

