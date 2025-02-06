#ifndef __PLLFUNCTION_H_
#define __PLLFUNCTION_H_

typedef struct
{    
    int16 ialp_sampling;
    int16 ialp;
    int16 ialp_LSB;
    int16 ialp_pre;    
    int16 ualp;
    int16 ualp_LSB;
    int16 ualp_ubase;
    int16 ualp_ubase_LSB;
    int16 ialp_err;
    int16 ealp;
    int16 ealp_LSB;
    int16 ibet_sampling;
    int16 ibet;
    int16 ibet_LSB;
    int16 ibet_pre;    
    int16 ubet;
    int16 ubet_LSB;
    int16 ubet_ubase;
    int16 ubet_ubase_LSB;
    int16 ibet_err;
    int16 ebet;
    int16 ebet_LSB;
    int16 PLLTheta;
    int16 Theta_pre;
    int16 PLLTheta_pre;
    int16 Theta_err;
    int16 BEMFBase_Update_Count;
    int16 Vfactor;
    int16 BEMFBase;
    int16 e_part;
    int16 e_part_LSB;
    int16 e_alp_delta;
    int16 e_bet_delta;
    int16 u_part;
    int16 u_part_LSB;    
    uint8 PLLFunctionFlag;
    int16 test;
    uint16 count;
    int16 Speed_PLL;
    int16 Speed_PLL_LSB;
    int16 Speed_PLL1;
    uint16 Switch_Count;
    int16  UBASE_Factor;

}PLLVar;

extern PLLVar  data PLLfunction;
extern void PLLSoftwareFunction(void);
extern void PLLFunctionInit(void);
extern void PLLStateFunction(void);
extern void RampFucntion(void);
#endif