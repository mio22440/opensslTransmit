/**
 * @file timeCounter.h
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
#ifndef TIME_COUNTER_H
#define TIME_COUNTER_H

/************************ include start ********************/
//时间
//*注意只能用sys/time.h
#include <sys/time.h>//gettimeofday()得到当前时间
#include <stdio.h>
/************************ include end ********************/

class timeCounter{
private:
    struct timeval start, stop, delta;
    suseconds_t microstart, microstop, microdelta;//用于计算准确差值时间的变量
public:
    timeCounter();

    /**
     * @brief           开始计时（取得当前时间）
     * @return int      返回1时成功
     */
    int startCounter(void);

    /**
     * @brief           停止计时（再次取得时间）
     * @return int      返回1时成功
     */
    int stopCounter(void);

    /**
     * @brief           计算用时（差值，单位为毫秒）
     * @return long int 用时
     */
    long int getDurationUs(void);

    /**
     * @brief           打印用时(会自动计算时间差值)
     * @return int 
     */
    int printDuration(void);
    

};

#endif