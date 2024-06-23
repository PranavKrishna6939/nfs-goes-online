#ifndef MYFILESYSTEM_H
#define MYFILESYSTEM_H

#define FSIMAGE "/home/jjbigdub/gitrepo/nfs-goes-online/Assignment_3/fsimage"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ufs.h"
#include "mfs.h"

// Global variable to keep track of the number of bytes read
int g_readbytes;

int openFs() {
    int fd = open(FSIMAGE, O_RDWR);
    if (fd < 0) {
        perror("Error opening file system image");
        exit(1);
    }
    return fd;
}

inode_t findInode(int inum, int fd) {
    // Read super block
    super_t superBlock;
    g_readbytes = pread(fd, &superBlock, sizeof(super_t), 0);
    if (g_readbytes != sizeof(super_t)) {
        perror("Error reading superblock");
        exit(1);
    }
    printf("Inode region address: %d\n", superBlock.inode_region_addr);

    // Read the inode
    inode_t inode;
    g_readbytes = pread(fd, &inode, sizeof(inode_t), 
                        superBlock.inode_region_addr * UFS_BLOCK_SIZE + inum * sizeof(inode_t));
    printf("Inode type: %d\nInode size: %d\n", inode.type, inode.size);
    return inode;
}

int MFS_Lookup(int pinum, char *name) {
    int fd = openFs();
    inode_t parentInode = findInode(pinum, fd);

    for (int i = 0; i < DIRECT_PTRS; i++) {
        printf("parentInode.direct[%d]: %d\n", i, parentInode.direct[i]);
        if (parentInode.direct[i] != -1) {
            dir_block_t dirBlock;
            g_readbytes = pread(fd, &dirBlock, sizeof(dir_block_t), 
                                parentInode.direct[i] * UFS_BLOCK_SIZE);
            if (g_readbytes != sizeof(dir_block_t)) {
                perror("Error reading directory block");
                exit(1);
            }

            for (int j = 0; j < 128; j++) {
                if (strcmp(dirBlock.entries[j].name, name) == 0) {
                    printf("Found: %s\n", name);
                    close(fd);
                    return dirBlock.entries[j].inum;
                }
                if (dirBlock.entries[j].inum == -1) {
                    break;
                }
            }
        } else {
            break;
        }
    }

    close(fd);
    return -1;  // Not found
}

int MFS_Stat(int inum, MFS_Stat_t *m) {
    int fd = openFs();
    inode_t inode = findInode(inum, fd);

    if (inode.size == 0) {
        close(fd);
        return -1;  // Invalid inode
    }

    m->size = inode.size;
    m->type = inode.type;
    close(fd);
    return 0;
}

int MFS_Write(int inum, char *buffer, int offset, int nbytes) {
    int fd = openFs();
    inode_t fileInode = findInode(inum, fd);

    int blocks_required = (offset + nbytes + UFS_BLOCK_SIZE - 1) / UFS_BLOCK_SIZE;
    if (blocks_required > DIRECT_PTRS) {
        fprintf(stderr, "Error: File too large\n");
        close(fd);
        return -1;
    }

    int bytes_written = 0;
    int start_block = offset / UFS_BLOCK_SIZE;


    close(fd);
    return bytes_written;
}

#endif // MYFILESYSTEM_H