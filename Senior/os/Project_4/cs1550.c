/*
	FUSE: Filesystem in Userspace
	Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>

	This program can be distributed under the terms of the GNU GPL.
	See the file COPYING.

	gcc -Wall `pkg-config fuse --cflags --libs` cs1550.c -o cs1550
*/

#define	FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

//size of a disk block
#define	BLOCK_SIZE 512

//we'll use 8.3 filenames
#define	MAX_FILENAME 8
#define	MAX_EXTENSION 3

//How many files can there be in one directory?
#define	MAX_FILES_IN_DIR (BLOCK_SIZE - (MAX_FILENAME + 1) - sizeof(int)) / \
	((MAX_FILENAME + 1) + (MAX_EXTENSION + 1) + sizeof(size_t) + sizeof(long))

//How much data can one block hold?
#define	MAX_DATA_IN_BLOCK (BLOCK_SIZE - sizeof(size_t) - sizeof(long))

struct cs1550_directory_entry
{
	char dname[MAX_FILENAME	+ 1];	//the directory name (plus space for a nul)
	int nFiles;			//How many files are in this directory. 
					//Needs to be less than MAX_FILES_IN_DIR

	struct cs1550_file_directory
	{
		char fname[MAX_FILENAME + 1];	//filename (plus space for nul)
		char fext[MAX_EXTENSION + 1];	//extension (plus space for nul)
		size_t fsize;			//file size
		long nStartBlock;		//where the first block is on disk
	} files[MAX_FILES_IN_DIR];		//There is an array of these
};

typedef struct cs1550_directory_entry cs1550_directory_entry;

struct cs1550_disk_block
{
	//Two choices for interpreting size: 
	//	1) how many bytes are being used in this block
	//	2) how many bytes are left in the file
	//Either way, size tells us if we need to chase the pointer to the next
	//disk block. Use it however you want.
	size_t size;	

	//The next disk block, if needed. This is the next pointer in the linked 
	//allocation list
	long nNextBlock;

	//And all the rest of the space in the block can be used for actual data
	//storage.
	char data[MAX_DATA_IN_BLOCK];
};

typedef struct cs1550_disk_block cs1550_disk_block;

char lastpath[MAX_FILENAME + 1];

/*
 * Called whenever the system wants to know the file attributes, including
 * simply whether the file exists or not. 
 *
 * man -s 2 stat will show the fields of a stat structure
 */
static int cs1550_getattr(const char *path, struct stat *stbuf)
{
	int res = 0;

	printf("In getattr!\n");
	memset(stbuf, 0, sizeof(struct stat));
	cs1550_directory_entry *temp_dir = malloc( sizeof(cs1550_directory_entry));
	
	int length = strlen(path);
	char *directory = malloc( length * sizeof(char) );
	char *filename = malloc( length * sizeof(char) );
	char *extension = malloc( length * sizeof(char) );
	
	int numComponents = sscanf(path, "/%[^/]/%[^.].%s", directory, filename, extension);
	printf("Num components = %d\n", numComponents);
	
	
	
	printf("Trying to create the path: %s / %s . %s\n", directory, filename, extension);
	
	//is path the root dir?
	if (strcmp(path, "/") == 0) 
	{
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	} 
	else 
	{
		FILE *dir_fp;
		
		dir_fp = fopen( ".directories","rb" );
		
		// If the file .directories can not be opened, the directory
		// does not exist, unless its the root dir, but thats covered
		// in the first portion
		if ( !dir_fp )
		{
			res = -ENOENT;
		}
		else
		{
			
			// See how many components were read out of the scanf to determine what it is we are looking for
			if ( numComponents == 1 )
			{
				// This is only a directory
				printf("Looking for a directory\n");
							
				while( fread(temp_dir, sizeof(cs1550_directory_entry), 1, dir_fp) == 1 )
				{
					if ( strcmp(temp_dir->dname, path) == 0 )
					{
						printf("found directory only!\n");
						res = 0;
						stbuf->st_mode = S_IFDIR | 0755;
						stbuf->st_nlink = 2;
						break;
					}
					res = -ENOENT;
				}
			}
			else if ( numComponents == 2 )
			{
				// This breaks the subdirectory clause
				res = -ENOENT;
			}
			else if ( numComponents == 3 )
			{
				// This is a file within a directory
				printf("getattr: Looking for a file\n");
				char *directoryString = malloc( length * sizeof(char));
				strcpy(directoryString, "/");
				strcat(directoryString, directory);
				
				while( fread(temp_dir, sizeof(cs1550_directory_entry), 1, dir_fp) == 1 )
				{
					if ( strcmp(temp_dir->dname, directoryString) == 0 )
					{
						printf("getattr: Found directory of filename\n");
						int i = 0;
						int fileFound = 0;
						printf("getattr: Num files: %d\n",temp_dir->nFiles);
						for ( i = 0; i < temp_dir->nFiles; i++ )
						{
							printf("getattr: testfilename: %s\n", temp_dir->files[i].fname);
							printf("getattr: realfilename: %s\n\n", filename);
							printf("getattr: testextension: %s\n", temp_dir->files[i].fext);
							printf("getattr: realextension: %s\n", extension);
							
							if ( strcmp( temp_dir->files[i].fname, filename ) == 0 && strcmp( temp_dir->files[i].fext, extension) == 0 )
							{
								printf("getattr: found filename!\n");
								res = 0;
								stbuf->st_mode = S_IFREG | 0666;
								stbuf->st_nlink = 1;
								stbuf->st_size = temp_dir->files[i].fsize;
								fileFound =1;
								printf("Name of file: %s\n", temp_dir->files[i].fname);
								printf("Num bytes in file is: %ld\n", temp_dir->files[i].fsize);
								break;
							}
						}
						if ( fileFound )
							break;
						
					}
					res = -ENOENT;
				}
			}
			
		}
		if ( dir_fp )
			fclose(dir_fp);
	}

	return res;
	
	
}

/* 
 * Called whenever the contents of a directory are desired. Could be from an 'ls'
 * or could even be when a user hits TAB to do autocompletion
 */
static int cs1550_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			 off_t offset, struct fuse_file_info *fi)
{
	//Since we're building with -Wall (all warnings reported) we need
	//to "use" every parameter, so let's just cast them to void to
	//satisfy the compiler
	
	(void) offset;
	(void) fi;
	int res = 0;
	FILE *dir_fp;
	cs1550_directory_entry *temp_dir = malloc( sizeof(cs1550_directory_entry));
	dir_fp = fopen( ".directories","rb" );

	//This line assumes we have no subdirectories, need to change
	if (strcmp(path, "/") != 0)
	{

		while( fread(temp_dir, sizeof(cs1550_directory_entry), 1, dir_fp) == 1 )
		{
			// Find the directory we are in
			if ( strcmp(temp_dir->dname, path) == 0 )
			{
			
				// Iterate over the files in that directory, print them out
				int i;
				for ( i = 0; i < temp_dir->nFiles; i++ )
				{
					// Format the filename so it looks nice
					char* filename = malloc( (MAX_FILENAME + MAX_EXTENSION + 2) * sizeof( char ) );
					strncpy( filename, temp_dir->files[i].fname, MAX_FILENAME);
					strncat( filename, ".", 1 );
					strncat( filename, temp_dir->files[i].fext, MAX_EXTENSION );
				
					// Display it
					filler( buf, filename, NULL, 0 );
			
				}
			}
			
		}
		// Return success?
		res = 0;
	}
	else
	{
	
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0); 
		// Here we print out all the directories in root
		if ( dir_fp )
		{	
			while( fread(temp_dir, sizeof(cs1550_directory_entry), 1, dir_fp) == 1 )
			{
				filler(buf,temp_dir->dname + 1, NULL,0);
			}
		}
		res = 0;
	}
	
	//the filler function allows us to add entries to the listing
	//read the fuse.h file for a description (in the ../include dir)
	if ( dir_fp )
		fclose( dir_fp );

	return res;
}

/* 
 * Creates a directory. We can ignore mode since we're not dealing with
 * permissions, as long as getattr returns appropriate ones for us.
 */
static int cs1550_mkdir(const char *path, mode_t mode)
{
	(void) path;
	(void) mode;
	int numSlashes = 0;
	int length = strlen(path);
	cs1550_directory_entry *new_dir = malloc( sizeof(cs1550_directory_entry));
	cs1550_directory_entry *temp_dir = malloc( sizeof(cs1550_directory_entry));
	char *path_cpy = malloc ( sizeof( char ) * length );
	
	FILE *dir_fp;
	dir_fp = fopen( ".directories","ab" );
	
	strcpy( path_cpy, path);
	
	// Check if the directory name is too long
	if ( strlen(path) > MAX_FILENAME + 1 )
	{
		return -ENAMETOOLONG;
	}
	
	// Check to see if the request is only one directory level deep ( has one slash )
	int i = 0;
	for( i = 0; i < length; i++ )
	{
		if ( *path_cpy == '/' )
		{
			numSlashes++;
		}
		path_cpy++;
	}
	
	// If we have more than 1 slash in the name, we're going too deep, deny it
	if ( numSlashes > 1 )
		return -EPERM;
	
	if ( dir_fp )
	{
		// Check to see if the directory already exists, ( doesnt getattr already do this for us?)
		while( fread(temp_dir, sizeof(cs1550_directory_entry), 1, dir_fp) == 1 )
		{
			if ( strcmp(temp_dir->dname, path) == 0 )
			{
				fclose( dir_fp );
				return -EEXIST;
			}
		}
	}
	
	// If we have no errors, make the directory
	strncpy(new_dir->dname, path, MAX_FILENAME);
	new_dir->nFiles = 0;

	fwrite( new_dir, sizeof( cs1550_directory_entry), 1, dir_fp);
	
	fclose( dir_fp);
	
	return 0;
}

/* 
 * Removes a directory.
 */
static int cs1550_rmdir(const char *path)
{
	(void) path;
    return 0;
}

/* 
 * Does the actual creation of a file. Mode and dev can be ignored.
 *
 */
static int cs1550_mknod(const char *path, mode_t mode, dev_t dev)
{
	(void) mode;
	(void) dev;
	printf("In mknod\n");
	int length = strlen(path);
	char *directory = malloc( length * sizeof(char) );
	char *filename = malloc( length * sizeof(char) );
	char *extension = malloc( length * sizeof(char) );
	cs1550_directory_entry *temp_dir = malloc( sizeof(cs1550_directory_entry));
	
	// Get the components out of the path
	int numComponents = sscanf(path, "/%[^/]/%[^.].%s", directory, filename, extension);
	
	// If we have a file, check if the name is too long
	if( numComponents == 3 )
	{
		if ( strlen(filename) > MAX_FILENAME || strlen(extension) > MAX_EXTENSION )
		{
			return -ENAMETOOLONG;
		}
	}
	else
	{
		if ( strlen(directory) < 1 )
		{
			return -EPERM;
		}
	}
	
	
	printf("Trying to create the path: %s / %s . %s\n", directory, filename, extension);
	
	
	FILE *dir_fp;
	dir_fp = fopen( ".directories","rb+" );
	
	// If we have a directories file.... we have to?
	if ( dir_fp )
	{
		char *directoryString = malloc( length * sizeof(char));
		strcpy(directoryString, "/");
		strcat(directoryString, directory);
		
		printf("Directory string : %s\n", directoryString);
		
		// Check to see if the directory already exists, ( doesnt getattr already do this for us?)
		while( fread(temp_dir, sizeof(cs1550_directory_entry), 1, dir_fp) == 1 )
		{
			if ( strcmp(temp_dir->dname, directoryString) == 0 )
			{
				// Make a file in the current directory
				if ( temp_dir->nFiles != MAX_FILES_IN_DIR )
				{
					printf("Here we go!\n");
					
					// Write the filename
					strncpy(temp_dir->files[temp_dir->nFiles].fname, filename, MAX_FILENAME);
					
					// Write the extension
					strncpy(temp_dir->files[temp_dir->nFiles].fext, extension, MAX_EXTENSION);
					
					// We can write the size from write()
					// We can write the next block from write()
					
					FILE *disk_fp;
					disk_fp = fopen( ".disk","rb+" );
					
					// This should always be true....
					if ( disk_fp )
					{
						// Allocate for the free_space / data_block blocks
						cs1550_disk_block *free_space = malloc( sizeof(cs1550_disk_block));
						
						// Read the first block of the file
						fread(free_space, sizeof(cs1550_disk_block), 1, disk_fp);
						
						// If it still has the 0's from initialization, lets fill the correct values in
						if ( free_space->nNextBlock == 0 )
						{
							printf("not initialized\n");
							
							free_space->size = 2;
							
							// This is 1 because 0 is the freespace block
							free_space->nNextBlock = 1;
						}
						
						printf("hello\n");
						//Allocate the block
						temp_dir->files[temp_dir->nFiles].nStartBlock = free_space->nNextBlock;
						
						printf("Block used is: %ld\n", free_space->nNextBlock);
						//Update it in the free space tracking algo
						free_space->nNextBlock = free_space->nNextBlock + 1;
						printf("Next block is: %ld\n", free_space->nNextBlock);
						
						//Seek back to the beginning of the disk
						fseek( disk_fp, 0, SEEK_SET);
							
						// Write the freespace block back
						fwrite( free_space, sizeof( cs1550_disk_block), 1, disk_fp);
						
						// Free free_space
						free( free_space );
						
						fclose(disk_fp);
					}
					else
					{
						printf("no disk_fp\n");
					}
					
					temp_dir->files[temp_dir->nFiles].fsize = 0;
					
					// Increment the number of files the directory has
					temp_dir->nFiles++;
					
					printf("Directory has %d files\n", temp_dir->nFiles);
					
					
					// Seek back to where you need to be, write it back
					fseek(dir_fp, - sizeof(cs1550_directory_entry), SEEK_CUR);
					fwrite( temp_dir, sizeof( cs1550_directory_entry), 1, dir_fp);
					
					// Free temp dir
					free( temp_dir );
					printf("We wrote something...\n");
				}
				else
				{
					// error no more room for file
				}
			}
		}
		
		free( directoryString );
		fclose( dir_fp);
	}
	
	// Free them all
	
	free( directory );
	free( extension );
	free( filename );
	printf("Out mknod\n");
	return 0;
	
}

/*
 * Deletes a file
 */
static int cs1550_unlink(const char *path)
{
    (void) path;

    return 0;
}

/* 
 * Read size bytes from file into buf starting from offset
 *
 */
static int cs1550_read(const char *path, char *buf, size_t size, off_t offset,
			  struct fuse_file_info *fi)
{
	printf("In Read!\n");
	(void) buf;
	(void) offset;
	(void) fi;
	(void) path;
	
	int length = strlen(path);
	char *directory = malloc( length * sizeof(char) );
	char *filename = malloc( length * sizeof(char) );
	char *extension = malloc( length * sizeof(char) );
	cs1550_directory_entry *temp_dir = malloc( sizeof(cs1550_directory_entry));
	
	FILE *dir_fp;
	dir_fp = fopen( ".directories","rb+" );
	
	sscanf(path, "/%[^/]/%[^.].%s", directory, filename, extension);
	
	printf("Trying to create the path: %s / %s . %s\n", directory, filename, extension);
	
	char *directoryString = malloc( length * sizeof(char));
	strcpy(directoryString, "/");
	strcat(directoryString, directory);
	
	if ( dir_fp )
	{
		while( fread(temp_dir, sizeof(cs1550_directory_entry), 1, dir_fp) == 1 )
		{
			if ( strcmp(temp_dir->dname, directoryString) == 0 )
			{
				printf("Found directory of filename\n");
				int i = 0;
				int fileFound = 0;
				for ( i = 0; i < temp_dir->nFiles; i++ )
				{
					if ( strcmp( temp_dir->files[i].fname, filename ) == 0 && strcmp( temp_dir->files[i].fext, extension) == 0 )
					{
						fileFound = 1;
						printf("Found the filename\n");
						
						// Allocate space for the block
						cs1550_disk_block *free_space = malloc( sizeof( cs1550_disk_block ) );
						
						FILE *disk_fp;
						disk_fp = fopen( ".disk","rb+" );
						
						fread( free_space, sizeof(cs1550_disk_block), 1, disk_fp);
						
						printf("Next open block: %ld\n", free_space->nNextBlock);

						// Allocate space for the block
						cs1550_disk_block *block = malloc( sizeof( cs1550_disk_block ) );
						
						printf("block to get for this file: %ld\n", temp_dir->files[i].nStartBlock);
						
						// Go to the start block of the file
						fseek( disk_fp, temp_dir->files[i].nStartBlock * sizeof (cs1550_disk_block), SEEK_SET);
						
						// Read the disk block
						fread( block, sizeof(cs1550_disk_block), 1, disk_fp);
						
						
						// If its a multiblock file
						if ( block->nNextBlock != -1 )
						{
							
							printf("data in buf: %s\n",buf);
							
							size = 0;
							// Go to the start block of the file
							fseek( disk_fp, (block->nNextBlock) * sizeof (cs1550_disk_block), SEEK_SET);
							
							strcpy( buf, "" );
							while ( block->nNextBlock != -1 )
							{
								strcat( buf, block->data);
								printf("data in buf: %s\n",buf);
							
								size += strlen(block->data);
								
								printf("first size: %ld\n", size);
								// Go to the start block of the file
								
								fseek( disk_fp, (block->nNextBlock) * sizeof (cs1550_disk_block), SEEK_SET);
								
								// Read the disk block
								fread( block, sizeof(cs1550_disk_block), 1, disk_fp);
						
							}
							
							strcat( buf, block->data);
							
							size += strlen(block->data);
							
						}
						else
						{
							printf("Put : %s into buf\n", block->data);
							printf("block size: %ld\n", block->size);
							printf("nextblock: %ld\n", block->nNextBlock);
							strncpy( buf, block->data, block->size);
							size = block->size;

						} 
						
						// free blocks
						free( free_space );
						free( block );
						
						printf("data in buf: %s\n",buf);
						fclose(disk_fp);
						break;
					}
				}
				printf("Found the File!\n");
				if ( fileFound )
					break;
		
			}
			// not found....
		}
		fclose(dir_fp);
	}


	
	//check to make sure path exists
	//check that size is > 0
	//check that offset is <= to the file size
	//read in data
	//set size and return, or error

	// Free them all
	free( temp_dir);
	free( directoryString );
	free( directory );
	free( extension );
	free( filename );
	printf("Leaving Read!\n");
	printf("Returning size of %ld\n",size);
	return size;
}

/* 
 * Write size bytes from buf into file starting from offset
 *
 */
static int cs1550_write(const char *path, const char *buf, size_t size, 
			  off_t offset, struct fuse_file_info *fi)
{
	printf("In Write!\n");
	(void) buf;
	(void) offset;
	(void) fi;
	(void) path;
	
	int length = strlen(path);
	char *directory = malloc( length * sizeof(char) );
	char *filename = malloc( length * sizeof(char) );
	char *extension = malloc( length * sizeof(char) );
	cs1550_directory_entry *temp_dir = malloc( sizeof(cs1550_directory_entry));
	
	FILE *dir_fp;
	dir_fp = fopen( ".directories","rb+" );
	
	sscanf(path, "/%[^/]/%[^.].%s", directory, filename, extension);
	
	printf("Trying to create the path: %s / %s . %s\n", directory, filename, extension);
	
	//check to make sure path exists
	// This is a file within a directory
	printf("Checking to see the file exists\n");
	char *directoryString = malloc( length * sizeof(char));
	
	strcpy(directoryString, "/");
	strcat(directoryString, directory);
	
	if ( dir_fp )
	{
		while( fread(temp_dir, sizeof(cs1550_directory_entry), 1, dir_fp) == 1 )
		{
			int i = 0;
			int fileFound = 0;
			if ( strcmp(temp_dir->dname, directoryString) == 0 )
			{
				printf("Found directory of filename\n");

				for ( i = 0; i < temp_dir->nFiles; i++ )
				{
					if ( strcmp( temp_dir->files[i].fname, filename ) == 0 && strcmp( temp_dir->files[i].fext, extension) == 0 )
					{
						if ( size > 0 && offset <= size )
						{
							printf("Found file to write to\n");
							FILE *disk_fp;
							disk_fp = fopen( ".disk","rb+" );
								
							// Allocate space for the block
							cs1550_disk_block *block = malloc( sizeof( cs1550_disk_block ) );
							
							// place the size in the block structure
							block->size = size;
							
							
							// Set the size of the file in the directory entry
							
							int blockToWrite = temp_dir->files[i].nStartBlock;
							
							size_t sizeOfFile = 0;
							if ( temp_dir->files[i].fsize == 0 )
							{
							
								
								// If our file is bigger than the size of a block, we'll need another
								if ( size > MAX_DATA_IN_BLOCK)
								{
									// Allocate space for the block
									cs1550_disk_block *free_space =(cs1550_disk_block*) malloc( sizeof( cs1550_disk_block ) );
									// Read the disk block
									fread( free_space, sizeof(cs1550_disk_block), 1, disk_fp);
									temp_dir->files[i].fsize = strlen(buf);
									int count = 0;
							
									while ( size > MAX_DATA_IN_BLOCK )
									{
										// place the data in the block structure
										strncpy(block->data, buf, MAX_DATA_IN_BLOCK );
										// do this later..
										// update free space tracker
										// write data
										// this needs to be a loop of some kind...
										block->nNextBlock = free_space->nNextBlock;
										free_space->nNextBlock++;
							
										size -= MAX_DATA_IN_BLOCK;
										sizeOfFile += strlen(block->data);
								
										// Go to the start block of the file
										fseek( disk_fp, (blockToWrite + count ) * sizeof (cs1550_disk_block), SEEK_SET);
						
										printf("Data to be written to file: %s\n", block->data);
										printf("Next Free block: %ld\n", free_space->nNextBlock);
						
										// Write the data to the .disk	
										fwrite( block, sizeof( cs1550_disk_block), 1, disk_fp);
										
										// Increase number
										count++;
								
										// Lets try to move the pointer
										// CHECK THIS
										buf += MAX_DATA_IN_BLOCK;
									}
							
									// Here we have to write the leftovers
									// place the data in the block structure
									strncpy(block->data, buf, BLOCK_SIZE );
									// do this later..
									// update free space tracker
									// write data
									// this needs to be a loop of some kind...
									block->nNextBlock = -1;
							
									sizeOfFile += strlen(block->data);
							
									// Go to the start block of the file
									fseek( disk_fp, (blockToWrite + count ) * sizeof (cs1550_disk_block), SEEK_SET);
					
									printf("Data to be written to file: %s\n", block->data);
									printf("Next Free block: %ld\n", free_space->nNextBlock);
					
									// Write the data to the .disk
									fwrite( block, sizeof( cs1550_disk_block), 1, disk_fp);
									
									// Seek to the beginning
									fseek( disk_fp, 0, SEEK_SET);

							
									// Write the free_space block back
									fwrite( free_space, sizeof( cs1550_disk_block), 1, disk_fp);
							
									free(free_space);
							
								}
								else
								{
									temp_dir->files[i].fsize = strlen(buf);
									// place the data in the block structure
									strncpy(block->data, buf, strlen(buf) );
									block->nNextBlock = -1;
							
									// Set the size of the file
									sizeOfFile = strlen(block->data);
							
							
									// Go to the start block of the file
									fseek( disk_fp, temp_dir->files[i].nStartBlock * sizeof (cs1550_disk_block), SEEK_SET);
						
									printf("Data to be written to file: %s\n", block->data);
									printf("In the else i think im in next block is: %ld\n", block->nNextBlock);
						
									// Write the data to the .disk
									fwrite( block, sizeof( cs1550_disk_block), 1, disk_fp);
							
								}
							
							}
							else
							{
								//append to the file
								if ( size + temp_dir->files[i].fsize > BLOCK_SIZE)
								{
									// Did not implement
								
								}
								else
								{
									char* newString = malloc( MAX_DATA_IN_BLOCK* sizeof(char));
									
									strcpy(newString,"");
									strcat(newString, block->data);
									strcat(newString, buf);
									
									strncpy(block->data, newString, strlen(newString));
									block->nNextBlock = -1;
									sizeOfFile = strlen(block->data);
									
									printf("Data to be written to file: %s\n", block->data);
									
									fseek( disk_fp, temp_dir->files[i].nStartBlock * sizeof (cs1550_disk_block), SEEK_SET);
										
									fwrite( block, sizeof( cs1550_disk_block), 1, disk_fp);
									
									
									free( newString);
								
								}
							
							
							}
							// set the size of the file							
							temp_dir->files[i].fsize = sizeOfFile;
							printf("Size of the file is: %ld\n", temp_dir->files[i].fsize);
							printf("Name of file: %s\n", temp_dir->files[i].fname);
							
							// seek back 
							fseek(dir_fp, - sizeof(cs1550_directory_entry), SEEK_CUR);
							
							// Write the entry
							fwrite( temp_dir, sizeof( cs1550_directory_entry), 1, dir_fp);
							
							free( temp_dir);
							free(block);

							//printf("before close: %x\n", disk_fp);
							fclose(disk_fp);
							break;
						}
					}
				}
			}
			// not found....
			printf("Found the File!\n");
			if ( fileFound )
				break;
		}
		
		fclose(dir_fp);
	}
	
	
	//check that size is > 0
	//check that offset is <= to the file size
	//write data
	//set size (should be same as input) and return, or error
	
	free( directoryString );
	free( directory );
	free( extension );
	free( filename );
	
	printf("Leaving Write!\n");
	printf("returning size of %ld\n",size);
	return size;
}

/******************************************************************************
 *
 *  DO NOT MODIFY ANYTHING BELOW THIS LINE
 *
 *****************************************************************************/

/*
 * truncate is called when a new file is created (with a 0 size) or when an
 * existing file is made shorter. We're not handling deleting files or 
 * truncating existing ones, so all we need to do here is to initialize
 * the appropriate directory entry.
 *
 */
static int cs1550_truncate(const char *path, off_t size)
{
	(void) path;
	(void) size;

    return 0;
}


/* 
 * Called when we open a file
 *
 */
static int cs1550_open(const char *path, struct fuse_file_info *fi)
{
	(void) path;
	(void) fi;
    /*
        //if we can't find the desired file, return an error
        return -ENOENT;
    */

    //It's not really necessary for this project to anything in open

    /* We're not going to worry about permissions for this project, but 
	   if we were and we don't have them to the file we should return an error

        return -EACCES;
    */

    return 0; //success!
}

/*
 * Called when close is called on a file descriptor, but because it might
 * have been dup'ed, this isn't a guarantee we won't ever need the file 
 * again. For us, return success simply to avoid the unimplemented error
 * in the debug log.
 */
static int cs1550_flush (const char *path , struct fuse_file_info *fi)
{
	(void) path;
	(void) fi;

	return 0; //success!
}


//register our new functions as the implementations of the syscalls
static struct fuse_operations hello_oper = {
    .getattr	= cs1550_getattr,
    .readdir	= cs1550_readdir,
    .mkdir	= cs1550_mkdir,
	.rmdir = cs1550_rmdir,
    .read	= cs1550_read,
    .write	= cs1550_write,
	.mknod	= cs1550_mknod,
	.unlink = cs1550_unlink,
	.truncate = cs1550_truncate,
	.flush = cs1550_flush,
	.open	= cs1550_open,
};

//Don't change this.
int main(int argc, char *argv[])
{
	return fuse_main(argc, argv, &hello_oper, NULL);
}
