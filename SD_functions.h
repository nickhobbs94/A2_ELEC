#ifndef SD_FUNCTIONS_H
#define SD_FUNCTIONS_H

/* Includes */
#include <stdio.h>
#include "altstring.h"
#include "alt_types.h"
#include "efs.h"
#include "ls.h"

/* Magic numbers */
#define SD_MAX_PATH_LENGTH 200
#define SD_FOLDER_BITMASK 0x10

/* Function prototypes */
EmbeddedFileSystem** SD_mount();
void SD_unmount(void);
alt_8 SD_getFileAttribute(alt_u8 input);
alt_8* SD_getCurrentPath();
void SD_updatePath(alt_8* currentPath, alt_8 argument[]);

/* ------------------------- Functions -------------------------- */

/* Mount the SD card */
EmbeddedFileSystem** SD_mount(){
	static EmbeddedFileSystem someFileSystem;
	static EmbeddedFileSystem* someFileSystemPointer;

	if (someFileSystemPointer!=NULL) return &someFileSystemPointer;

	someFileSystemPointer = &someFileSystem;
	esint8 check;
	printf("Will init efsl now\n");
	check = efs_init(someFileSystemPointer,"/dev/sda");
	if (check==0){
		printf("Filesystem correctly initialized\n");
	} else {
		printf("Could not init filesystem\n");
		someFileSystemPointer = NULL;
	}
	return &someFileSystemPointer;
}

/* Unmount the SD card */
void SD_unmount(void){
	EmbeddedFileSystem** efsl;
	efsl = SD_mount();
	fs_umount(&((*efsl)->myFs));
	*efsl=0;
	printf("Successfully unmounted the SD card\n");
}

/* Tell whether the current entry is a directory or a file */
alt_8 SD_getFileAttribute(alt_u8 input){
	alt_8 attribute=0;
	if ((input & SD_FOLDER_BITMASK) != 0){
		attribute='d'; // Directory
	} else {
		attribute='f'; // File
	}
	return attribute;
}

/* Get the path of the current directory */
alt_8* SD_getCurrentPath(){
	static alt_8 path[SD_MAX_PATH_LENGTH];
	static alt_8* pathPointer;
	if (pathPointer != NULL) return pathPointer;

	altmemset(path,'\0',sizeof(path));
	altstrcpy(path,(alt_8*)"/");
	pathPointer = path;

	return pathPointer;
}

/* Update the path given */
void SD_updatePath(alt_8* currentPath, alt_8 argument[]){
	if (argument[0]=='/'){
		altstrcpy(currentPath,argument);
	} else {
		if (currentPath[altstrlen(currentPath)-1] != '/'){
			altstrcat(currentPath,(alt_8*)"/");
		}
		altstrcat(currentPath,argument);
	}
}

#endif

