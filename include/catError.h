/**
 * @file catError.h
 * @brief 打印错误消息
 *        using: return catError("server", "fail to send");
 * @author catfee (648137125@qq.com)
 * @version 0.1
 * @date 2020-11-15
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef CATFEE_CATERROR_H
#define CATFEE_CATERROR_H
/*******************include start**********************/
#include <stdio.h>
/*******************include end************************/
/*******************define start**********************/

/*******************define end************************/
/*******************declaration start**********************/
int catError(const char *sender, const char *error);
/*******************declaration end************************/

#endif