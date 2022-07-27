/*
 * @Author: 高逸博 gaoyibo.09@bytedance.com
 * @Date: 2022-07-27 09:33:18
 * @LastEditors: 高逸博 gaoyibo.09@bytedance.com
 * @LastEditTime: 2022-07-27 11:09:16
 * @FilePath: /aodv-route/aodv_timeout.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
//
// Created by ByteDance on 2022/7/27.
//

#ifndef AODV_ROUTE_AODV_TIMEOUT_H
#define AODV_ROUTE_AODV_TIMEOUT_H

#ifndef NS_NO_GLOBALS
#include "defs.h"
#endif				/* NS_NO_GLOBALS */

#ifndef NS_NO_DECLARATIONS
void route_delete_timeout(void *arg);
void local_repair_timeout(void *arg);
void route_discovery_timeout(void *arg);
void route_expire_timeout(void *arg);
void hello_timeout(void *arg);
void rrep_ack_timeout(void *arg);
void wait_on_reboot_timeout(void *arg);
void packet_queue_timeout(void *arg);
#endif				/* NS_NO_DECLARATIONS */


#endif //AODV_ROUTE_AODV_TIMEOUT_H
