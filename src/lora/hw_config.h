
// #define MODULE_JUMPER_AION_TX
// #define MODULE_HMSLIM_PRO_TX

#ifdef MODULE_JUMPER_AION_TX
#define PIN_LORA_NSS        5
#define PIN_LORA_DIO1       4
#define PIN_LORA_NRST       14
#define PIN_LORA_BUSY       21

#define PIN_PA_TXEN         26
#define PIN_PA_RXEN         27

#define POWER_VALUES {-18,-15,-11,-8,-4,3}

#endif

#ifdef MODULE_HMSLIM_PRO_TX
#define PIN_LORA_NSS        5
#define PIN_LORA_DIO1       4
#define PIN_LORA_NRST       14
#define PIN_LORA_BUSY       21

#define PIN_PA_TXEN         26
#define PIN_PA_RXEN         27

#define PIN_FAN             17

#define POWER_VALUES {-18,-15,-12,-7,-4,2}
#endif

typedef enum
{
    PWR_10mW = 0,
    PWR_25mW = 1,
    PWR_50mW = 2,
    PWR_100mW = 3,
    PWR_250mW = 4,
    PWR_500mW = 5,
    PWR_1000mW = 6,
    PWR_2000mW = 7,
    PWR_COUNT = 8,
    PWR_MATCH_TX = PWR_COUNT,
} PowerLevels_e;

int getPowerValues(PowerLevels_e pwr) {
    int dBmList[] = POWER_VALUES;
    if (pwr > PowerLevels_e::PWR_COUNT)
        pwr = PWR_COUNT;
    else if (pwr < PowerLevels_e::PWR_10mW)
        pwr = PWR_10mW;

    return dBmList[pwr];
}