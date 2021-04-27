/*
 * Copyright (C) 2021-2021 Rahmat M. Samik-Ibrahim
 * http://rahmatm.samik-ibrahim.vlsm.org/
 * This program is free script/software. This program is distributed in the 
 * hope that it will be useful, but WITHOUT ANY WARRANTY; without even the 
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * REV08: Sun 04 Apr 07:25:24 WIB 2021
 * REV07: Sun 04 Apr 00:11:43 WIB 2021
 * REV06: Sat 03 Apr 11:00:46 WIB 2021
 * REV05: Tue 30 Mar 14:55:36 WIB 2021
 * REV04: Tue 30 Mar 10:35:13 WIB 2021
 * START: Mon 22 Mar 16:14:36 WIB 2021
 *
# INFO: mylib.c
 */

#include "mylib.h"

void mySHA1(uChrPtr output, uChrPtr input, int length) {
    Chr     cmd[BUFFERSIZE];
    sprintf(cmd, CMDSHA1, input);
    FILE* filePtr = popen(cmd, "r");
    fgets(output, length+1, filePtr);
    output[length]=0;
    pclose(filePtr);
}

void getTimeStamp(uChrPtr timeStamp) {
    time_t tt    =  time(NULL);
    struct tm tm = *localtime(&tt);
    sprintf(timeStamp, "%2.2d%2.2d", tm.tm_min, tm.tm_sec);
}

void chktoken (uChrPtr result, uChrPtr token) {
    uChr timeStamp[] = "MMSS";
    getTimeStamp(timeStamp);
    uChr input [BUFFERSIZE];
    strcpy(input,timeStamp);
    uChrPtr user=getenv("USER");
    strcat(input,user);
    strcat(input,token);
    uChr   output [BUFFERSIZE];
    mySHA1(output, input, SSIZE);
    sprintf(result, "%s %s-%s", user, timeStamp, output);
}

void verifyToken(uChrPtr result, uChrPtr token, uChrPtr input) {
    uChr    tmpStr1[BUFFERSIZE];
    uChr    tmpStr2[BUFFERSIZE];
    strcpy(tmpStr1,input);
    uChrPtr user=strtok(tmpStr1," ");
    uChrPtr timeStamp=strtok(NULL,"-");
    strcpy(tmpStr2,timeStamp);
    strcat(tmpStr2,user);
    strcat(tmpStr2,token);
    uChr   output [BUFFERSIZE];
    mySHA1(output, tmpStr2, SSIZE);
    uChrPtr tmpStr3=strtok(NULL,"-");
    if (strcmp(output, tmpStr3) == 0 ) sprintf(result, "Verified");
    else sprintf(result, "Error");
}

void pickToken (uChrPtr result, uChrPtr token) {
    uChr   tmpStr1[BUFFERSIZE];
    strcpy(tmpStr1,token);
    strtok(tmpStr1," ");
    strcpy(result, strtok(NULL," "));
}

memStructPtr createShareMemory(memStructPtr mymap, int memorySize, ChrPtr memoryName) {
    int      fd    = open(memoryName, MYFLAGS, CHMOD);
    fchmod   (fd, CHMOD);
    ftruncate(fd, memorySize);
    mymap = mmap(NULL, memorySize, MYPROTECTION, MYVISIBILITY, fd, 0);
    close(fd);
    return mymap;
}

