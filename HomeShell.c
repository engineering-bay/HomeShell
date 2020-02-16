#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include "HomeShell.h"

int main(void)
{
    int workFlag = 1, i = 0;
    struct timeval tv;
    struct tm* ptm;
    long timeMs;

    WriteToLog(LOG_RECTYPE_CONSOLE, "Programm start!", LOG_RECOPTS_NORMAL);
    
    while(workFlag == 1)
    {
        gettimeofday(&tv, NULL);
        timeMs = tv.tv_usec / 1000;
        ptm = localtime(&tv.tv_sec);
        if(ptm->tm_sec == 0)
        {
            SchedulerCheck(ptm);
            i++;
            if(i >= 999) workFlag = 0;
        }
        sleep(1);
    }
    WriteToLog(LOG_RECTYPE_CONSOLE, "Programm exit!", LOG_RECOPTS_NORMAL);   
    return 0;
}

void WriteToLog(int recordType, char *recMessage, int options)
{
    struct timeval tv;
    struct tm* ptm;
    long timeMs;
    FILE *fLogPtr;
    char *fLogName;
    
    gettimeofday(&tv, NULL);
    timeMs = tv.tv_usec / 1000;
    ptm = localtime(&tv.tv_sec);
    
    switch(recordType)
    {
        case LOG_RECTYPE_CONSOLE:
        {
            switch(options)
            {
                case LOG_RECOPTS_NORMAL:
                {
                    printf("[%04d-%02d-%02d] %02d:%02d:%02d.%03ld %s\n", ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, timeMs, recMessage);
                }break;
                case LOG_RECOPTS_NOMILLIS:
                {
                    printf("[%04d-%02d-%02d] %02d:%02d:%02d %s\n", ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, recMessage);
                }break;
                case LOG_RECOPTS_NOSECONDS:
                {
                    printf("[%04d-%02d-%02d] %02d:%02d %s\n", ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, recMessage);
                }break;
            }
            
        }break;
        case LOG_RECTYPE_FILE:
        {
            sprintf(fLogName, "%04d-%02d-%02d.log", ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday);
            fLogPtr = fopen(fLogName, "a");
            if(fLogPtr != NULL)
            {
                switch(options)
                {
                    case LOG_RECOPTS_NORMAL:
                    {
                        fprintf(fLogPtr, "[%04d-%02d-%02d] %02d:%02d:%02d.%03ld %s\n", ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, timeMs, recMessage);
                    }break;
                    case LOG_RECOPTS_NOMILLIS:
                    {
                        fprintf(fLogPtr, "[%04d-%02d-%02d] %02d:%02d:%02d %s\n", ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, recMessage);
                    }break;
                    case LOG_RECOPTS_NOSECONDS:
                    {
                        fprintf(fLogPtr, "[%04d-%02d-%02d] %02d:%02d %s\n", ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, recMessage);
                    }break;
                }
                fclose(fLogPtr);
            }
            else
            {
                printf("[debug/log] Error opening log file!\n");
            }
        }break;
        default:
        {
            printf("[debug/log] Invalid record type!\n");
        }
    }
}

int SchedulerCheck(struct tm* ptm)
{
    int i, k;
    int matchFlag = 0;
    int matchPool[6];
    char tmp[DEFAULT_STRING_SIZE];
    char taskDescription[DEFAULT_STRING_SIZE];
    char nstr[4] = "";
    FILE *fSchedulePtr;

    matchPool[0] = ptm->tm_year - 100;
    matchPool[1] = ptm->tm_mon + 1;
    matchPool[2] = ptm->tm_mday;
    matchPool[3] = ptm->tm_wday;
    matchPool[4] = ptm->tm_hour;
    matchPool[5] = ptm->tm_min;
    WriteToLog(LOG_RECTYPE_CONSOLE, "Checking the schedule...", LOG_RECOPTS_NORMAL);
    fSchedulePtr = fopen("//home//werwolf//eclipse-workspace//HomeShell//schedule.cfg", "r");
    if(fSchedulePtr != NULL)
    {
        while(!feof(fSchedulePtr))
        {
            if(fgets(tmp, DEFAULT_STRING_SIZE, fSchedulePtr))
            {
                if(tmp[0] != '#')
                {
                	strncpy(nstr, tmp, 3);
                	strcpy(taskDescription, strstr(tmp, "-")+1);
                	k = atoi(nstr);
                    char *pch = strtok (tmp + 4, " ");
                    //while (pch != NULL) //пока есть лексемы
                    for(i = 0; i < 6; i++)
                    {
                    	//printf("\n%s", pch);
                    	if((strstr(pch, "*") != NULL) || (atoi(pch) == matchPool[i])) matchFlag = 1;
                    	else
                    	{
                    		//printf("\nFail: %d\n", i);
                    		matchFlag = 0;
                    		break;
                    	}
                        pch = strtok (NULL, " ");
                    }
                    if(matchFlag == 1) printf("\t\t\t  [%3d] %s", k, taskDescription);
                    //else printf("\t\t[FALSE]\n");
                }
            }
        }
        fclose(fSchedulePtr);
    }
    else
    {
        printf("[debug/scheduler] Schedule file not found!\n");
        return -1;
    }
    return 0;
}
