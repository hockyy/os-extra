/*
 * Copyright (C) 2021-2021 Rahmat M. Samik-Ibrahim
 * http://rahmatm.samik-ibrahim.vlsm.org/
 * This program is free script/software. This program is distributed in the 
 * hope that it will be useful, but WITHOUT ANY WARRANTY; without even the 
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# INFO: mylib.c
 * REV04: Sat 17 Apr 17:45:34 WIB 2021
 * REV03: Fri 16 Apr 15:15:00 WIB 2021
 * REV02: Thu 15 Apr 09:09:41 WIB 2021
 * REV01: Tue 13 Apr 21:07:19 WIB 2021
 * START: Sat 10 Apr 16:13:36 WIB 2021
 */

#include "mylib.h"

voidPtr createShareMemory(ChrPtr memoryName, int memorySize) {
    int      fd    = open(memoryName, MYFLAGS, CHMOD);
    fchmod   (fd, CHMOD);
    ftruncate(fd, memorySize);
    voidPtr mymap = mmap(NULL, memorySize, MYPROTECTION, MYVISIBILITY, fd, 0);
    close(fd);
    if (mymap == MAP_FAILED) {
        printf("ZCZC SM1 problem with shared memory %s (FAILED)\n", memoryName);
        exit(0);
    }
    printf("ZCZC SM2 shared memory is OK (SUCCESS)\n");
    return mymap;
}

uChrPtr createStamp (uChrPtr input) {
    uChrPtr stamp=getTimeStamp();
    uChr   tmpBuffer[BUFFERSIZE];
    strcpy(tmpBuffer, stamp);
    uChrPtr user=getenv("USER");
    strcat(tmpBuffer, user);
    strcat(tmpBuffer, input);
    uChrPtr tmpPtr=mySHA1(tmpBuffer,SHA1SIZE);
    strcpy(tmpBuffer, user);
    strcat(tmpBuffer," ");
    strcat(tmpBuffer, stamp);
    strcat(tmpBuffer," ");
    strcat(tmpBuffer,tmpPtr);
    free(tmpPtr);
    tmpPtr=malloc(strlen(tmpBuffer));
    strcpy(tmpPtr,tmpBuffer);
    return tmpPtr;
}

int deltaTime(ChrPtr stamp1, ChrPtr stamp2) {
    int st=str2sec(stamp2) - str2sec(stamp1);
    while (st < 0) st+=3600*24;
    return st;
}

uChrPtr getTimeStamp(void) {
    uChr    tmpBuffer[BUFFERSIZE];
    FILE*   filePtr   = popen(CMDDATE, "r");
    fgets  (tmpBuffer, BUFFERSIZE, filePtr);
    pclose (filePtr);
    int     length    = strlen(tmpBuffer)-1;
    tmpBuffer[length] = 0;
    uChrPtr output    = malloc(length);
    strcpy (output,   tmpBuffer);
    return output;
}

void mySleep(int second) {
    int  fd = open(RANDOMDEV, O_RDONLY);
    uLongInt myLong;
    read(fd, &myLong, sizeof(long));
    myTime mTS = {second, myLong%1000000000};
    nanosleep(&mTS,NULL);
}

uChrPtr mySHA1(uChrPtr input, int length) {
    uChr    tmpBuffer[BUFFERSIZE];
    sprintf(tmpBuffer, CMDSHA1, input);
    FILE*   filePtr = popen(tmpBuffer, "r");
    fgets  (tmpBuffer, length+1, filePtr);
    pclose (filePtr);
    tmpBuffer[length]=0;
    uChrPtr output=malloc(strlen(tmpBuffer));
    strcpy (output,tmpBuffer);
    return output;
}

void    sameAndExit      (ChrPtr string1, ChrPtr string2, ChrPtr message) {
    if (strcmp(string1, string2) == 0) {
        printf("%s", message);
        exit(0);
    }
}

void    sameOrExit       (ChrPtr string1, ChrPtr string2, ChrPtr message) {
    if (strcmp(string1, string2) != 0) {
        printf("%s", message);
        exit(0);
    }
}

int str2sec(ChrPtr stamp) {
    return (stamp[12]-'0')+
        10*(stamp[11]-'0')+
          ((stamp[10]-'0')+
        10*(stamp [9]-'0'))*60+
          ((stamp [8]-'0')+
        10*(stamp [7]-'0'))*3600+
          ((stamp [5]-'0')+
        10*(stamp [4]-'0'))*86400;
}

void testtest (void) {
    printf("TESTTEST\n");
}

uChrPtr verifyStamp (uChrPtr input, uChrPtr stamp) {
    uChr    tmpBuffer1[BUFFERSIZE];
    uChr    tmpBuffer2[BUFFERSIZE];
    strcpy (tmpBuffer1,stamp);
    uChrPtr user     =strtok(tmpBuffer1," ");
    uChrPtr timeStamp=strtok(NULL," ");
    uChrPtr sha      =strtok(NULL," ");
    strcpy (tmpBuffer2,timeStamp);
    strcat (tmpBuffer2,user);
    strcat (tmpBuffer2,input);
    uChrPtr tmpPtr=mySHA1(tmpBuffer2,SHA1SIZE);
    if (strcmp(tmpPtr, sha) == 0)
        sha="Verified";
    else 
        sha="Error";
    free(tmpPtr);
    return sha;
}

