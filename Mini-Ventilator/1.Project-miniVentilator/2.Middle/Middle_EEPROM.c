#include "Middle_EEPROM.h"

/*!
	@brief 		 Initialize EEPROM GPIO.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Mid_EEPROM_Init(){	
    Dri_EEPROM_SCLENWC_Init();
    EEPROM_SDA_OUT;
}

/*!
	@brief 		 Write one byte to EEPROM.
	@param[in]	 w_address:Target address.
                 w_data:Write data.
	@param[out]	 none
	@retval		 none
*/
void Mid_EEPROM_WriteByte(uint16_t w_address,uint8_t w_data){
    if(w_address > MAX_EEPROMADDRESS)
        return;

    EEPROM_WC_Low;
    udelay_1us(2);

    EEPROM_Start();
    EEPROM_Write_Byte(EEPROM_WRITE_CMD);
    EEPROM_Wait_Ack();
    EEPROM_Write_Byte((w_address>>8));
    EEPROM_Wait_Ack();
    EEPROM_Write_Byte((uint8_t)w_address);
    EEPROM_Wait_Ack();
    EEPROM_Write_Byte(w_data);
    EEPROM_Wait_Ack();
    EEPROM_Stop();

    EEPROM_WC_High;
    udelay_1us(2);
}	

/*!
	@brief 		 Write n bytes to EEPROM.
	@param[in]	 w_address:Start address.
                 w_data:Write data.
                 data_length:....
	@param[out]	 none
	@retval		 none
*/
void Mid_EEPROM_WriteNBytes(uint16_t w_address,uint8_t* w_data,uint8_t data_length){
    if(w_address > MAX_EEPROMADDRESS || (w_address+data_length) > MAX_EEPROMADDRESS)
        return;
    
    EEPROM_WC_Low;
    udelay_1us(2);

    EEPROM_Start();
    EEPROM_Write_Byte(EEPROM_WRITE_CMD);
    EEPROM_Wait_Ack();
    EEPROM_Write_Byte((w_address>>8));
    EEPROM_Wait_Ack();
    EEPROM_Write_Byte((uint8_t)w_address);
    EEPROM_Wait_Ack();
    for(uint8_t i = 0;i < data_length;i++){
        EEPROM_Write_Byte(w_data[i]);
        EEPROM_Wait_Ack();
    }
    EEPROM_Stop();

    EEPROM_WC_High;
    udelay_1us(2);
}	

/*!
	@brief 		 Read one byte from EEPROM.
	@param[in]	 r_address:Target address.
                 r_data:Read out data.
	@param[out]	 none
	@retval		 none
*/
void Mid_EEPROM_ReadByte(uint16_t r_address,uint8_t* r_data){
    if(r_address > MAX_EEPROMADDRESS)
        return;

    EEPROM_Start();
    EEPROM_Write_Byte(EEPROM_WRITE_CMD);
    EEPROM_Wait_Ack();
    EEPROM_Write_Byte((r_address>>8));
    EEPROM_Wait_Ack();
    EEPROM_Write_Byte((uint8_t)r_address);
    EEPROM_Wait_Ack();

    EEPROM_Start();
    EEPROM_Write_Byte(EEPROM_READ_CMD);
    EEPROM_Wait_Ack();
    *r_data = EEPROM_Read_Byte();
    EEPROM_No_Ack();
    EEPROM_Stop();
}	

/*!
	@brief 		 Read n bytes from EEPROM.
	@param[in]	 r_address:Start address.
                 r_data:Read out data.
                 data_length:.....
	@param[out]	 none
	@retval		 none
*/
void Mid_EEPROM_ReadNBytes(uint16_t r_address,uint8_t* r_data,uint8_t data_length){
    if(r_address > MAX_EEPROMADDRESS || (r_address+data_length-1) > MAX_EEPROMADDRESS)
        return;
    
    EEPROM_Start();
    EEPROM_Write_Byte(EEPROM_WRITE_CMD);
    EEPROM_Wait_Ack();
    EEPROM_Write_Byte((r_address>>8));
    EEPROM_Wait_Ack();
    EEPROM_Write_Byte((uint8_t)r_address);
    EEPROM_Wait_Ack();

    EEPROM_Start();
    EEPROM_Write_Byte(EEPROM_READ_CMD);
    EEPROM_Wait_Ack();
    for(uint8_t i = 0;i < data_length-1;i++){
        r_data[i] = EEPROM_Read_Byte();
        EEPROM_Wait_Ack();
    }
    r_data[data_length-1] = EEPROM_Read_Byte();
    EEPROM_No_Ack();
    EEPROM_Stop();
}	




