/**
 * @file timeCounter.cpp
 * @brief  用于计算文件传输时间的类         
 * @author catfee (648137125@qq.com)
 * @version 1.0
 * @date 2020-11-06
 * 
 * @copyright Copyright （c） 2020 catfee
 * 
 * @par 修改日志:
 * Date           Version     Author      Description
 * 2020-11-06     1.0         catfee      内容
 */
#include "../include/timeCounter.h"

int timeCounter::startCounter(void){
    gettimeofday(&this->start, NULL);
    return 1;
}

int timeCounter::stopCounter(void){
    gettimeofday(&this->stop, NULL);
    return 1;
}

long int timeCounter::getDurationUs(void){
    this->microstart = (suseconds_t) (100000*(this->start.tv_sec))+ this->start.tv_usec;
    this->microstop = (suseconds_t) (100000*(this->stop.tv_sec))+ this->stop.tv_usec;
    this->microdelta = microstop - microstart;
    
    this->delta.tv_usec = this->microdelta%100000;
    this->delta.tv_sec = (time_t)(this->microdelta/100000);
    
    if((this->delta.tv_sec < 0) || (this->delta.tv_usec < 0)){
        return -1;
    }
    else{
        return 0;
    }

}

int timeCounter::printDuration(void){
    this->getDurationUs();
    printf("time used : %ld.%ld \n",this->delta.tv_sec, this->delta.tv_usec);
    return 0;
}
