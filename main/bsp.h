#ifndef _BSP_H_
#define _BSP_H_

#define RED_LED 21
#define BLUE_LED 23
#define GREEN_LED 18

#define LED_ON 1U
#define LED_OFF 0U

#define PIN_SW1 22
#define SW1 1U << PIN_SW1
#define BSP_TICKS_PER_SECOND 100U

void BSP_init(void);
void BSP_ledRedOn(void);
void BSP_ledRedOff(void);
void BSP_ledBlueOn(void);
void BSP_ledBlueOff(void);
void BSP_ledGreenOn(void);
void BSP_ledGreenOff(void);
uint32_t BSP_tick_ctr(void);
uint8_t BSP_SW1(void);

#endif