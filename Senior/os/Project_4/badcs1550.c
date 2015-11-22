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
			// If the file does exist, for now lets just lie and say
			// these are the permissions
			stbuf->st_mode = S_IFDIR | 0755;
			stbuf->st_nlink = 2;
			
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
						break;
					}
					res = -ENOENT; //no error
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
				printf("Looking for a file\n");
				char *directoryString = malloc( length * sizeof(char));
				strcpy(directoryString, "/");
				strcat(directoryString, directory);
				
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
								printf("found filename!\n");
								res = 0;
								fileFound =1;
								break;
							}
						}
						if ( fileFound )
							break;
						
					}
					res = -ENOENT; //no error
				}
			}
			
		}
		if ( dir_fp )
			fclose(dir_fp);
	}
	
	printf("Out getattr!\n");
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
		res = -ENOENT;
	}
	else
	{
	
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0); 
		// Here we print out all the directories/ files in root
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
	/*
	int length = strlen(path);
	char *directory = malloc( length * sizeof(char) );
	char *filename = malloc( length * sizeof(char) );
	char *extension = malloc( length * sizeof(char) );
	cs1550_directory_entry *temp_dir = malloc( sizeof(cs1550_directory_entry));
	
	printf("In mknod\n");
	
	int numComponents = sscanf(path, "/%[^/]/%[^.].%s", directory, filename, extension);
	
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
				printf("holy crap!\n");
				// Make a file in the current directory
				if ( temp_dir->nFiles != MAX_FILES_IN_DIR )
				{
					printf("Here we go!\n");
					strncpy(temp_dir->files[temp_dir->nFiles].fname, filename, MAX_FILENAME);
					strncpy(temp_dir->files[temp_dir->nFiles].fext, extension, MAX_EXTENSION);
					
					temp_dir->nFiles = temp_dir->nFiles + 1;
					
					// Seek back to where you need to be, write it back
					fseek(dir_fp, - sizeof(cs1550_directory_entry), SEEK_CUR);
					fwrite( temp_dir, sizeof( cs1550_directory_entry), 1, dir_fp);
					printf("We wrote something...\n");
				}
				else
				{
					// error no more room for file
				}
			}
		}
		
		fclose( dir_fp);
	}
	*/
	printf("Out mknod\n");
	return 0;
	
	/*
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
	*/
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
	
	printf("In Read!\n");
	
	sscanf(path, "/%[^/]/%[^.].%s", directory, filename, extension);
	
	printf("Trying to create the path: %s / %s . %s\n", directory, filename, extension);


	
	//check to make sure path exists
	//check that size is > 0
	//check that offset is <= to the file size
	//read in data
	//set size and return, or error

	printf("Leaving Read!\n");
	size = 0;

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

	printf("In Write!\n");
	
	sscanf(path, "/%[^/]/%[^.].%s", directory, filename, extension);
	
	printf("Trying to create the path: %s / %s . %s\n", directory, filename, extension);
	
	//check to make sure path exists
	//check that size is > 0
	//check that offset is <= to the file size
	//write data
	//set size (should be same as input) and return, or error
	
	printf("Leaving Write!\n");

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
