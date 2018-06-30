#ifndef __JABIL_LCR_H
#define __JABIL_LCR_H

#define U1_IN1_IN2		PBout(12)
#define U2_IN1_IN2		PBout(1)

extern  uint8_t	KeyS_actOK_LCR;
extern 	uint8_t KeyS_actEN_LCR;

void KeyS_act_JABIL_LCR(void);
void SW_init_JABIL_LCR(void);

//
#endif
