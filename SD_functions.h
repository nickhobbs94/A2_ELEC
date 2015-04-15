#ifndef SD_FUNCTIONS_H
#define SD_FUNCTIONS_H
#include <stdio.h>
#include <string.h>
#include "alt_types.h"
#include "efs.h"
#include "ls.h"
#define SD_MAX_PATH_LENGTH 200

EmbeddedFileSystem** SD_mount();
void SD_unmount(void);
alt_8 SD_getFileAttribute(alt_u8 input);
char* SD_getCurrentPath();
void SD_updatePath(alt_8* currentPath, alt_8 argument[]);

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

void SD_unmount(void){
	EmbeddedFileSystem** efsl;
	efsl = SD_mount();
	fs_umount(&((*efsl)->myFs));
	*efsl=0;
	printf("Successfully unmounted the SD card\n");
}

alt_8 SD_getFileAttribute(alt_u8 input){
	alt_8 attribute=0;
	if ((input & 0x10) != 0){
		attribute='d'; // Directory
	} else {
		attribute='f'; // File
	}
	return attribute;
}

char* SD_getCurrentPath(){
	static char path[SD_MAX_PATH_LENGTH];
	static char* pathPointer;
	if (pathPointer != NULL) return pathPointer;

	memset(path,'\0',sizeof(path));
	strcpy(path,"/");
	pathPointer = path;

	return pathPointer;
}

void SD_updatePath(alt_8* currentPath, alt_8 argument[]){
	strcpy(currentPath,argument);
}

#endif
