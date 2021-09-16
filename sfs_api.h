#ifndef _INCLUDE_SFS_API_H_
#define _INCLUDE_SFS_API_H_

/*****************INCLUDES****************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "disk_emu.h"

/***************DEFINES****************/

#define MAXFILENAME 20
#define NUM_BLOCKS 100000
#define NUM_INODES 50
#define NUM_INDIRECT NUM_BLOCKS / sizeof(unsigned int)
#define JITS_DISK "sfs_adeis.disk"
#define BLOCK_SZ 1024
#define NUM_INODE_BLOCKS (sizeof(inode_t) * NUM_INODES / BLOCK_SZ + 1) 
#define NUM_DIR_BLOCKS (sizeof(entry_t) * NUM_INODES / BLOCK_SZ + 1) 
#define MAX_RWPTR ((12*BLOCK_SZ) + (BLOCK_SZ/4)*BLOCK_SZ)
#define SIZE (NUM_BLOCKS/8)
#define FREE_BIT(_data, _which_bit) \
    _data = _data | (1 << _which_bit)

#define USE_BIT(_data, _which_bit) \
    _data = _data & ~(1 << _which_bit)

/************STRUCTS********************/

/*
 * magic
 * block_size           Block size of the file system
 * fs_size              Size of the file system
 * inode_table_len      Length of the inode table
 * root_dir_inode       pointer to the inode of the root
 */
typedef struct{
    uint64_t magic;
    uint64_t block_size;
    uint64_t fs_size;
    uint64_t inode_table_len;
    uint64_t root_dir_inode;
} superblock_t;

/*
 * used             if this inode is being used
 * mode
 * link_cnt
 * uid
 * gid
 * size             size of the file
 * data_ptrs        direct data pointer
 * indirect_ptrs    pointer to the structure containing the indirect pointer
 */
typedef struct {
    unsigned int used;
    unsigned int mode;
    unsigned int link_cnt;
    unsigned int uid;
    unsigned int gid;
    unsigned int size;
    unsigned int data_ptrs[12];
    unsigned int indirect_ptrs;
} inode_t;

/*
 * data_ptr     indirect pointer
 */
typedef struct {
    unsigned  int data_ptr[ NUM_INDIRECT ];
} indirect_t;

/*
 * used     if this entry is being used
 * inode    which inode this entry describes
 * name     name of the file associated with the inode
 */
typedef struct {
    uint64_t used;
    uint64_t inode;
    char* name;
} entry_t;

/*
 * inode    which inode this entry describes
 * rwptr    where in the file to start
 */
typedef struct {
    uint64_t used;
    uint64_t inode;
    uint64_t rwptr;
} file_descriptor;

/************PROTOTYPES***************/

//SFS API
void mksfs(int fresh);
int sfs_get_next_filename(char *fname);
int sfs_GetFileSize(const char* path);
int sfs_fopen(char *name);
int sfs_fclose(int fileID);
int sfs_fread(int fileID, char *buf, int length);
int sfs_fwrite(int fileID, const char *buf, int length);
int sfs_frseek(int fileID, int loc);
int sfs_fwseek(int fileID, int loc);
int sfs_remove(char *file);


/*
 * @short force an index to be set.
 * @long Use this to setup your superblock, inode table and free bit map
 *       This has been left unimplemented. You should fill it out.
 *
 * @param index index to set 
 *
 */
void force_set_index(uint32_t index);

/*
 * @short find the first free data block
 * @return index of data block to use
 */
uint32_t get_index();

/*
 * @short frees an index
 * @param index the index to free
 */
void rm_index(uint32_t index);

/*@short Get bitmap*/
uint8_t* get_bitmap(void);

#endif //_INCLUDE_SFS_API_H_
