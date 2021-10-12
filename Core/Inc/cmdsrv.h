/*
 * cmdsrv.h
 *
 *  Created on: 8 окт. 2021 г.
 *      Author: ziged
 */

#ifndef INC_CMDSRV_H_
#define INC_CMDSRV_H_

void takeUsbBuf(uint8_t *buf, uint32_t len);
bool takeCmd(char *buf, uint32_t bufLength);

#endif /* INC_CMDSRV_H_ */
