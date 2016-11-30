/*
 *  linux/arch/arm/kernel/sys_arm.c
 *
 *  Copyright (C) People who wrote linux/arch/i386/kernel/sys_i386.c
 *  Copyright (C) 1995, 1996 Russell King.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  This file contains various random system calls that
 *  have a non-standard calling sequence on the Linux/arm
 *  platform.
 */
#include <linux/export.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/sem.h>
#include <linux/msg.h>
#include <linux/shm.h>
#include <linux/stat.h>
#include <linux/syscalls.h>
#include <linux/mman.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/ipc.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/uidgid.h>
#include <linux/unistd.h>
#include <linux/types.h>
#include <linux/atomic.h>



//extern struct user_struct *find_user(kuid_t* id);

/*
 * Since loff_t is a 64 bit type we avoid a lot of ABI hassle
 * with a different argument ordering.
 */
asmlinkage long sys_arm_fadvise64_64(int fd, int advice,
				     loff_t offset, loff_t len)
{
	return sys_fadvise64_64(fd, offset, len, advice);
}

struct User_info {
	int proccesses;
	int pending;
	long watches;
} userinfo;

asmlinkage long sys_hello(struct User_info* userinfo)
{	
	kuid_t id = get_current_user()->uid;
	struct user_struct *userstruct = find_user(id);

	int proccesses = atomic_read(&userstruct->processes);
	int pending = atomic_read(&userstruct->sigpending);
	long watches = atomic_long_read(&userstruct->epoll_watches);

	long res1 = copy_to_user(&userinfo->proccesses, &proccesses, sizeof(proccesses));
	long res2 = copy_to_user(&userinfo->pending, &pending, sizeof(pending));
	long res3 = copy_to_user(&userinfo->watches, &watches, sizeof(watches));

	if(res1 == -1|| res2 == -1|| res3 == -1)
		return -1;
	return 1337;
}
