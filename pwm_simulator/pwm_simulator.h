#include <stdbool.h> // Para tipo bool

// --- Definições ---

#define LED_GPIO_PIN        31U     // GPIO do LED2 (Azul) na LaunchPadXL

#define PWM_COMPARE_MASK    0x03FFU // Máscara para bits 0-9 (valor de comparação)
#define PWM_ENABLE_BIT      (1U << 10) // Bit 10: habilita PWM

#define PWM_PERIOD_US       1000U   // Período total do PWM em microssegundos

extern float g_dutyCyclePercent;
extern bool g_enable_pwm;

// Protótipos de Funções
void initSystemPeripherals(void);
void initLEDGPIO(void);
void enablePWM(void);
void disablePWM(void);
void calculatePWMOnOffTimes(unsigned int compareValue);
unsigned int calculateCompareValueFromDutyCycle(float dutyCycle);
void setPWMDutyCycleAndRegister(float dutyCycle);
void generateSoftwarePWM(void);