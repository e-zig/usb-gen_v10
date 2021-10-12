/*
 * lcdsrv.h
 *
 *  Created on: 8 окт. 2021 г.
 *      Author: ziged
 */

#ifndef INC_LCDSRV_H_
#define INC_LCDSRV_H_

void lcdInit(uint32_t firmware_version);
void lcdWrite(uint32_t line, char *str);
void lcdWrite(char *str);
int lcdprintf(uint32_t line, const char *str, ...);
void lcdClear();
void lcdUpdate();

#endif /* INC_LCDSRV_H_ */
