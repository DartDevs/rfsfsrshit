/**
 * @file        include/linux/rfs_fs_i.h
 * @brief       header file for RFS inode
 *
 *---------------------------------------------------------------------------*
 *                                                                           *
 *          COPYRIGHT 2003-2007 SAMSUNG ELECTRONICS CO., LTD.                *
 *                          ALL RIGHTS RESERVED                              *
 *                                                                           *
 *   Permission is hereby granted to licensees of Samsung Electronics        *
 *   Co., Ltd. products to use or abstract this computer program only in     *
 *   accordance with the terms of the NAND FLASH MEMORY SOFTWARE LICENSE     *
 *   AGREEMENT for the sole purpose of implementing a product based on       *
 *   Samsung Electronics Co., Ltd. products. No other rights to reproduce,   *
 *   use, or disseminate this computer program, whether in part or in        *
 *   whole, are granted.                                                     *
 *                                                                           *
 *   Samsung Electronics Co., Ltd. makes no representation or warranties     *
 *   with respect to the performance of this computer program, and           *
 *   specifically disclaims any responsibility for any damages,              *
 *   special or consequential, connected with the use of this program.       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 */
     
#ifndef _LINUX_RFS_FS_I
#define _LINUX_RFS_FS_I

#include <linux/timer.h>

/*
 * RFS file system inode data in memory (in-core)
 */

struct rfs_inode_info {
       __u32   start_clu;      /* start cluster of inode */
       __u32   p_start_clu;    /* parent directory start cluster */
       __u32   index;          /* dir entry index(position) in directory */
       __u32   last_clu;       /* last cluster number */  
       __u8    i_state;        /* rfs-specific inode state */

       /* hint for quick search */
       __u32   hint_last_clu;
       __u32   hint_last_offset;
       
       /* truncate point */
       unsigned long   trunc_start;
       
       spinlock_t              write_lock;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
       /* total block size that inode allocated */
       loff_t                  mmu_private;
       struct inode            vfs_inode;
#else
       /* total block size that inode allocated */
       unsigned long           mmu_private;

       /* ordered transaction */
       struct semaphore        data_mutex;
       struct timer_list       timer;
       struct task_struct      *sleep_proc;
#endif
};

/* get inode info */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
static inline struct rfs_inode_info *RFS_I(struct inode *inode)
{
       return container_of(inode, struct rfs_inode_info, vfs_inode);
}
#else
#define RFS_I(i)       (&((i)->u.rfs_i))
#endif

/* rfs-specific inode state */
#define RFS_I_ALLOC    0x00
#define RFS_I_FREE     0x01

#endif
