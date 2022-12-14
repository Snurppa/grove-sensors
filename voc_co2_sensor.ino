#define BASELINE_IS_STORED_FLAG  (0X55)
#include <EEPROM.h>

void array_to_u32(u32* value, u8* array) {
    (*value) = (*value) | (u32)array[0] << 24;
    (*value) = (*value) | (u32)array[1] << 16;
    (*value) = (*value) | (u32)array[2] << 8;
    (*value) = (*value) | (u32)array[3];
}
void u32_to_array(u32 value, u8* array) {
    if (!array) {
        return;
    }
    array[0] = value >> 24;
    array[1] = value >> 16;
    array[2] = value >> 8;
    array[3] = value;
}

/*
    Reset baseline per hour, store it in EEPROM;
*/
void  store_baseline(void) {
    static u32 i = 0;
    u32 j = 0;
    u32 iaq_baseline = 0;
    u8 value_array[4] = {0};
    i++;
    Serial.println(i);
    if (i == 3600) {
        i = 0;
        if (sgp_get_iaq_baseline(&iaq_baseline) != STATUS_OK) {
            Serial.println(F("get baseline failed!"));
        } else {
            Serial.println(iaq_baseline, HEX);
            Serial.println(F("get baseline"));
            u32_to_array(iaq_baseline, value_array);
            for (j = 0; j < 4; j++) {
                EEPROM.write(j, value_array[j]);
                Serial.print(value_array[j]);
                Serial.println(F("..."));
            }
            EEPROM.write(j, BASELINE_IS_STORED_FLAG);
        }
    }
}

/*  Read baseline from EEPROM and set it.If there is no value in EEPROM,retrun .
    Another situation: When the baseline record in EEPROM is older than seven days,Discard it and return!!

*/
void set_baseline(void) {
    u32 i = 0;
    u8 baseline[5] = {0};
    u32 baseline_value = 0;
    for (i = 0; i < 5; i++) {
        baseline[i] = EEPROM.read(i);
        Serial.print(baseline[i], HEX);
        Serial.print(F(".."));
    }
    if (baseline[4] != BASELINE_IS_STORED_FLAG) {
        Serial.println(F("There is no baseline value in EEPROM"));
        return;
    }
    array_to_u32(&baseline_value, baseline);
    sgp_set_iaq_baseline(baseline_value);
    Serial.println(baseline_value, HEX);
}

void gas_sensor_setup() {
    s16 err;
    u16 scaled_ethanol_signal, scaled_h2_signal;

    /*  Init module,Reset all baseline,The initialization takes up to around 15 seconds, during which
        all APIs measuring IAQ(Indoor air quality ) output will not change.Default value is 400(ppm) for co2,0(ppb) for tvoc*/
    while (sgp_probe() != STATUS_OK) {
        Serial.println(F("SGP failed"));
        while (1);
    }
    /*Read H2 and Ethanol signal in the way of blocking*/
    err = sgp_measure_signals_blocking_read(&scaled_ethanol_signal,
                                            &scaled_h2_signal);
    if (err == STATUS_OK) {
        Serial.println(F("get ram signal!"));
    } else {
        Serial.println(F("error reading signals"));
    }
    
    //err = sgp_iaq_init(); // comment out when..
    // when with baseline, use this and comment out above sgp_iaq_init
     set_baseline();
}

void gas_sensor_main() {
    s16 err = 0;
    u16 tvoc_ppb, co2_eq_ppm;
    err = sgp_measure_iaq_blocking_read(&tvoc_ppb, &co2_eq_ppm);
    if (err == STATUS_OK) {
        Serial.print(F("tVOC = "));
        Serial.print(tvoc_ppb);
        Serial.println(F("ppb"));

        Serial.print(F("CO2eq = "));
        Serial.print(co2_eq_ppm);
        Serial.println(F("ppm"));
    } else {
        Serial.println(F("error reading IAQ values\n"));
    }
    store_baseline();
}
