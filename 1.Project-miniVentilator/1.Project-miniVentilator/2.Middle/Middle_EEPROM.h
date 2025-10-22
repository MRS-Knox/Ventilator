#ifndef Middle_EEPROM_H
#define Middle_EEPROM_H

#include "Driver_EEPROM.h"

/* Initialize EEPROM GPIO. */
void Mid_EEPROM_Init(void);
/* Write one byte to EEPROM. */
void Mid_EEPROM_WriteByte(uint16_t w_address,uint8_t w_data);
/* Write n bytes to EEPROM. */
void Mid_EEPROM_WriteNBytes(uint16_t w_address,uint8_t* w_data,uint8_t data_length);
/* Read one byte from EEPROM. */
void Mid_EEPROM_ReadByte(uint16_t r_address,uint8_t* r_data);
/* Read n bytes from EEPROM. */
void Mid_EEPROM_ReadNBytes(uint16_t r_address,uint8_t* r_data,uint8_t data_length);

#endif
