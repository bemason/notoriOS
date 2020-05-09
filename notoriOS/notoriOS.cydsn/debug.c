//drivers for UART debug 

#include "debug.h"
uint8 stillWriting;
#include <stdarg.h>



void debug_start(){
    #if USE_DEBUG
       Debug_UART_Start();
       //setvbuf(stdout, NULL, _IOFBF, Debug_UART_TX_BUFFER_SIZE);//this sets the printf() bufefr to zero, so there are not delays
       //setbuf(stdout, NULL);
    #endif
}

void debug_stop(){
    #if USE_DEBUG
        Debug_UART_Stop();
    #endif
}

void debug_sleep(){
   #if USE_DEBUG
    if (stillWriting)
        CyDelay(1u);//need to delay for 1ms to allow printf to finish writing (it's non blocking)
    Debug_UART_Sleep(); 
   #endif
   
}

void debug_wakeup(){
    #if USE_DEBUG
        Debug_UART_Wakeup();
        stillWriting = 0;
    #endif
}

/* For GCC compiler revise _write() function for printf functionality */
//this makes it so that when we call printf() it will write to UART
//NOTE: For this to work HEAP size in "System" must be set to 0x300 or more
#if USE_DEBUG
    
int _write(int file, char *ptr, int len)
{
    int i;
    file = file;
    for (i = 0; i < len; i++)
    {   
       Debug_UART_PutChar(*ptr++);
    }
    
    stillWriting = 1;//flag ongoing write
    return (len);
}

#endif

//use like printf, but this will add an depoch timestamp to the printput
void printNotif(uint8 type, char* format, ...){
    #if USE_DEBUG
    va_list argptr;
    va_start(argptr, format);

    //basically, just hijack printf and inject the timestamp infront
    printf("{ ");
    printf("\"time\":\"%ld\" " , getTimeStamp());
    if(type == NOTIF_TYPE_EVENT){
        printf("\"event\":\"notif\" \"value\":\"");
    }else if(type == NOTIF_TYPE_WARNING){
        printf("\"event\":\"warning\" \"value\":\"");
    }else if(type == NOTIF_TYPE_ERROR){
        printf("\"event\":\"error\" \"value\":\"");
    }else{
       printf("\"event\":\"undefined\" \"value\":\""); 
    }
   
    
    vfprintf(stdout,format, argptr);
    va_end(argptr);

    
    printf("\"}\r\n");
    #endif
    
}

void printTestStatus(test_t test){
#if USE_DEBUG
    printf("{ ");
    printf("\"time\":\"%ld\" " , getTimeStamp());
    
    printf("\"event\":\"test\" ");
    printf("\"name\":\"%s\" ", test.test_name);
    printf("\"status\":\"%d\" ", test.status);
    printf("\"reason\":\"%s\" ", test.reason);

    printf("}\r\n");
    #endif
   
}





    



//returns local times in epoch seconds (seconds since Jan 1, 1970)
//can also cast this to a t_of_day struct
long getTimeStamp(){
    
    RTC_TIME_DATE *localTime = RTC_ReadTime();
   
    struct tm t;
    time_t t_of_day;

    t.tm_sec = localTime->Sec;
    t.tm_min = localTime->Min;
    t.tm_hour = localTime->Hour;
    t.tm_year = localTime->Year - 1900;  // Year - 1900
    t.tm_mon = localTime->Month;           // Month, where 0 = jan
    t.tm_mday = localTime->DayOfMonth;          // Day of the month
    t.tm_isdst = 0;        // Is Daylight Savings on? 1 = yes, 0 = no, -1 = unknown
    
    t_of_day = mktime(&t);

    return (long) t_of_day;
}


void stripEscapeCharacters(char *string){

  char * pch;
  pch=strchr(string,'\n');
  while (pch!=NULL)
  {
    string[pch-string+1] = 0;
    pch=strchr(pch+1,'s');
  }
}

char *strextract(const char input_str[], char output_str[],
                 const char search_start[], const char search_end[]) {
    if (input_str == NULL) return NULL;
    char *begin, *end = NULL;

    if ((begin = strstr(input_str, search_start))) {
        begin += strlen(search_start);
        if ((end = strstr(begin, search_end))) {
            strncpy(output_str, begin, end - begin);
            output_str[end - begin] = '\0';
        }
    }

    return end;
}

