#ifndef PIC16F877A_HCSR04_H
#define PIC16F877A_HCSR04_H

#include "PIC16F877A_input_capture.h"

void hcsr04Init(void);
void hcsr04Trigger(void);
uint8_t hcsr04Distance(void);
float getDistance(void);

#endif