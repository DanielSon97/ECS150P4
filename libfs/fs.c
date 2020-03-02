#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "disk.h"
#include "fs.h"

/*define data structures for meta-information blocks*/
//packed data structure for superblock
struct __attribute__((__packed__)) superBlock {
    int8_t signature[8];        //Signature (must be equal to “ECS150FS”)
    uint16_t numBlocks;          //Total amount of blocks of virtual disk
    uint16_t rootIndex;          //Root directory block index
    uint16_t dataIndex;          //Data block start index
    uint16_t numDBlocks;         //Amount of data blocks
    uint8_t numFBlocks;          //Number of blocks for FAT
    int8_t unused[4079];        //Unused/Padding
};

//packed data structure for file descriptor
struct __attribute__((__packed__)) fd {
    int8_t filename[16];        //Filename (including NULL character)
    uint32_t size;               //Size of the file (in bytes)
    uint16_t firstIndex;        //Index of first data block
    int8_t padding[10];         //Unused/Padding
};

//packed data structure for root directory
struct __attribute__((__packed__)) rootDirectory {
    struct fd fds[128];    //entries of file discriptors
};

//intialize variables for meta-information blocks*/
struct superBlock *sb;
int16_t *fat;
struct rootDirectory *root;


/*functions*/
//mounts the passed file system
int fs_mount(const char *diskname)
{
    //check if disk can be opened
    if (block_disk_open(diskname) == -1) {
        return -1;
    }

	//if disk is successfully opened, then intialize meta-information
    sb = (struct superBlock*)malloc(sizeof(struct superBlock));

    /*SUPERBLOCK*/
    //check if superblock can be read
    if (block_read(0, sb) == -1) {
        return -1;
    }
    //checking signature
    char signatureCheck[9];
    strcpy(signatureCheck, "ECS150FS"); 
    for (int i = 0; signatureCheck[i] != '\0'; i++) { 
        if ((char)(sb->signature[i]) != signatureCheck[i]) {
            return -1;
        }
    }
    //checking total amount of blocks of virtual disk
    if (sb->numBlocks != block_disk_count()) {
        return -1;
    }
    //checking if numFBlocks is correct
    int expectedFB = (sb->numDBlocks * 2) / 4096;
    if ((sb->numDBlocks * 2) % 4096 > 0) {
        expectedFB++;
    }
    if (sb->numFBlocks != expectedFB) {
        return -1;
    }
    //checking if rootIndex is correct
    if (sb->rootIndex != 1 + sb->numFBlocks) {
        return -1;
    }
    //checking if rootIndex is correct
    if (sb->dataIndex != 1 + sb->rootIndex) {
        return -1;
    }
    //checking if numDBlocks is correct
    if (sb->numDBlocks != sb->numBlocks - sb->dataIndex) {
        return -1;
    }
    
    /*FILE ALLOCATION TABLE*/
    fat = (int16_t*)malloc(sizeof(struct superBlock) * sb->numFBlocks);
    //check if file allocation table can be read
    //cycle through each FAT block and read
    int fatIndex = 1;
    for (int i = 0; i < sb->numFBlocks; i++) {
        if (block_read(fatIndex, fat + (2048 * i)) == -1) {
            return -1;
        }
        fatIndex++;
    }

    /*ROOT DIRECTORY*/
    root = (struct rootDirectory*)malloc(sizeof(struct rootDirectory));
    //check if root directory can be read
    if (block_read(sb->rootIndex, root) == -1) {
        return -1;
    }
    //return 0 if successfully mounted
    return 0;
}

int fs_umount(void)
{
    /*WRITING BACK TO DISK*/
	//write superblock back to disk
    if (block_write(0, sb) == -1) {
        return -1;
    }
       
    //write file allocation table data back to disk
    int fatIndex = 1;
    for (int i = 0; i < sb->numFBlocks; i++) {
        if (block_write(fatIndex, fat + (2048 * i)) == -1) {
            return -1;
        }
        fatIndex++;
    }
    //write root directory back to disk
    if (block_write(sb->rootIndex, root) == -1) {
        return -1;
    }
        
    /*FREEING VARIABLES*/
    free(sb);
    free(fat);
    free(root);

    //close disk
    if (block_disk_close() == -1) {
        return -1;
    }

    //return 0 if successfully unmounted
    return 0;
}

int fs_info(void)
{
	//check if a virtual disk was opened
    if (sb == NULL) {
        return -1;
    }

    //printing basic info
    printf("FS Info:\n");
    printf("total_blk_count=%d\n", sb->numBlocks);
    printf("fat_blk_count=%d\n", sb->numFBlocks);
    printf("rdir_blk=%d\n", sb->rootIndex);
    printf("data_blk=%d\n", sb->dataIndex);
    printf("data_blk_count=%d\n", sb->numDBlocks);

    //calculating fat free ratio
    int freeFat = 0;
    for (int i = 0; i < sb->numDBlocks; i++) {
        if (fat[i] == 0) {
            freeFat++;
        }
    }
    printf("fat_free_ratio=%d/%d\n", freeFat, sb->numDBlocks);

    //calculate rdir free ratio
    //set variable as max possible. cycle through and decrement for each empty fd
    int freeFd = 0;
    for (int i = 0; i < 128; i++) {
        if ((root->fds[i].filename[0]) == '\0') {
            freeFd++;
        }
    }
    printf("rdir_free_ratio=%d/128\n", freeFd);

    //return 0 if info has been successfully printed
    return 0;
}

int fs_create(const char *filename)
{
	/* TODO: Phase 2 */
    return 0;
}

int fs_delete(const char *filename)
{
	/* TODO: Phase 2 */
    return 0;
}

int fs_ls(void)
{
	/* TODO: Phase 2 */
    return 0;
}

int fs_open(const char *filename)
{
	/* TODO: Phase 3 */
    return 0;
}

int fs_close(int fd)
{
	/* TODO: Phase 3 */
    return 0;
}

int fs_stat(int fd)
{
	/* TODO: Phase 3 */
    return 0;
}

int fs_lseek(int fd, size_t offset)
{
	/* TODO: Phase 3 */
    return 0;
}

int fs_write(int fd, void *buf, size_t count)
{
	/* TODO: Phase 4 */
    return 0;
}

int fs_read(int fd, void *buf, size_t count)
{
	/* TODO: Phase 4 */
    return 0;
}

