// Prabhas Kumra
// CS-370
// Project 3

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "date.h"

int main(int argc, char * argv[])
{

    int num = -1;
    int ret;
    int flag;
    
    char * timeZone = (char *)malloc(128 * sizeof(char));
    memset(timeZone, 0, 128*sizeof(char));
    
    if(argc == 1){
        num = 0;
    }
    else if(argc == 2){
        ret = strcmp(argv[1], "1");
        if(ret == 0){
            num = 1;
        }
        ret = strcmp(argv[1], "0");
        if(ret == 0){
            num = 0;
        }
        if(num > 1 || num < 0){
            printf(1, "Flag bit unrecognized\n");
            exit();
        }        
    }
    else if(argc == 3){
        num = 0;
        ret = strcmp(argv[1], "-t");
        if(ret == 0){
            num = 0;
        }
        else{
            printf(1, "Error, command line options\n");
            exit();
        }
        timeZone = argv[2];
    }

    else if (argc == 4) {
        
        ret = strcmp(argv[1], "1");
        if(ret == 0){
            num = 1;
        }
        ret = strcmp(argv[1], "0");
        if(ret == 0){
            num = 0;
        }

        if(num > 1 || num < 0){
            printf(1, "Error, command line options\n");
            exit();
        }

        ret = strcmp(argv[2], "-t");
        if(ret == 0){
        }
        else{
            printf(1, "Error, command line options\n");
            exit();
        }
        
        timeZone = argv[3];
    }
    else{
        printf(1, "Usage: date [Optional: 12/12hf flag] {Optional: -t TIMEZONE\n1 for 12 hour, 0 for 24 hour\nUse -t TIMEZONE for a timezone. Default is GMT\n");
        exit();
    }

    char  * sign = (char *)malloc(128 * sizeof(char));
    sign[0] = timeZone[0];
    ret = -1;
    int ret1 = -1;
    char * zoneNum = (char *)malloc(128 * sizeof(char));
    

    ret = strcmp( "-", sign);
    if (ret == 0) {
        flag = 0;
    }

    ret1 = strcmp("+", sign);
    if (ret1 == 0) {
        flag = 1;
    }

    if(ret1 < 0 && ret < 0){
        printf(1, "Incorrect time zone format\n");
        exit();
    }
    int zone;
    
    zoneNum[0] = timeZone[1];
    zoneNum[1] = timeZone[2];
    zoneNum[2] = timeZone[3];
    zone = atoi(zoneNum);
    
    if(flag == 0){
        if(zone > 12 ){
            printf(1, "Error, time zone does not exist\n");
            exit();
        }
        else{
            zone = -1 * zone;
        }
    }
    else if(flag == 1){
        if(zone > 14){
            printf(1, "Error, time zone does not exist\n");
            exit();
        }
    }
    struct rtcdate r;

    date(&r);
  
    int hours = r.hour;
    int minutes = r.minute;
    int seconds = r.second;
    int months = r.month;
    int days = r.day;
    int years = r.year;

        if(flag == 0){

            hours = hours + zone;

            if(hours <= 0){
                hours = 24 + hours;
                
                if( months == 5 || months == 7 || months == 10 || months == 12 ){
                    if(days == 1){
                        days = 30;
                        months--;
                    }
                    else{
                        days--;
                    }
                }
                else if(months == 2 || months == 4 || months == 6 || months == 8 || months == 9 || months == 11 ){
                    if(days == 1){
                        days = 31;
                        months--;
                    }
                    else{
                        days--;
                    }
                }
                else if(months == 1){
                    if(days == 1){
                        days = 31;
                        months = 12;
                        years--;
                    }
                    else{
                        days--;
                    }
                }
                else if(months == 3){
                    if(days == 1){
                        if((years%4)==0){
                            days = 29;
                            months--;
                        }
                        else{
                            days = 28;
                            months--;
                        }
                    }
                    else{
                        days--;
                    }
                }
            } 
        }
        else if(flag == 1){
            hours = hours + zone;
            //abs(hours);
            if(hours >= 24){
                hours = hours - 24;
                //if(days)
                if(months == 1 || months == 3 || months == 5 || months == 7 || months == 8 || months == 10 || months == 12 ){
                    if(days == 31){
                        days = 1;

                        if(months == 12){
                            months = 1;
                            years++;
                        }
                        else{
                            months++;
                        }
                    }
                    else{
                        days++;
                    }
                    
                }
                else if(months == 4 || months == 6 || months == 9 || months == 11){
                    if(days == 30){
                        days = 1;
                        months++;
                    }
                    else{
                        days++;
                    }
                }
                else if (months == 2){
                    if((years%4)== 0){
                        if(days == 29){
                            days = 1;
                            months++;
                        }
                        else{
                            days++;
                        }
                    }
                    else{
                        if(days == 28){
                            days = 1;
                            months++;
                        }
                        else{
                            days++;
                        }
                    }
                    
                }    
            }
        }


    if(num == 0){
        printf(1, "The date is: %d:%d:%d on %d/%d/%d\n", hours, minutes, seconds, months, days, years);
    } 

    else if(num == 1){
        if(hours > 12){
            hours = hours -12;
        }
    
        printf(1, "The date is: %d:%d:%d on %d/%d/%d\n", hours, minutes, seconds, months, days, years);
    }
    

    exit();
}