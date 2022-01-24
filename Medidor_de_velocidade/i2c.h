/*
 * i2c.h
 *
 *  Created on: 9 de nov de 2019
 *      Author: Vinicius Almeida
 */

#ifndef I2C_H_
#define I2C_H_

void i2cInit();
uint8_t i2cTst(uint8_t addr);
uint8_t i2cSend(uint8_t addr, uint8_t *data, uint8_t count);
uint8_t i2cGet(uint8_t addr, uint8_t *data, uint8_t count);
uint8_t i2cSendByte(uint8_t addr, uint8_t byte);
uint8_t i2cGetByte(uint8_t addr);
uint16_t convADinit(uint8_t pin);
void delay(long limite);
void PCF_write(char dado);





#endif /* I2C_H_ */
