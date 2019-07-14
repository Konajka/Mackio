/*
 * Mackio variometer
 *
 * Advanced ATMega based variometer.
 *
 * The circuit:
 * - Arduino Nano
 * - MS5611 atmospheric pressure sensor module
 * - Rotary encoder KY-040
 * - Nokia 5110 display
 * - Real time clock
 *
 * @author https://github.com/Konajka
 * @version 0.1 2019-07-07
 *      Project created.       
 */

#include <Arduino.h>
#include <U8g2lib.h>
#include <MS5611.h> 

// Enable serial monitoring
#define SERIAL_LOG

#define TALMAN_FACTOR 0.25

// Setup Nokia 5110 display
#define CLK 4
#define DIN 5
#define DC 6
#define CE 7
#define RST 8
U8G2_PCD8544_84X48_1_4W_SW_SPI u8g2(U8G2_R0, CLK, DIN, CE, DC, RST);  

// Use MS5611 atmospheric pressure and temperature sensor
MS5611 ms5611; 

// Refresh time counter
#define REFRESH_MILLIS 200
unsigned long previousMillis = 0;

// Atmospheric pressure
#define WARMUP_COUNT 80
#define WARMUP_DELAY 2
int32_t pressure = 0;
int32_t qnh1 = 0;
int32_t qnh2 = 0;

boolean useTempCompensation = false;
 
void setup() {
    #ifdef SERIAL_LOG
    Serial.begin(9600);
    Serial.println("Serial monitoring enabled");
    #endif

    // Initialize display and show splash screen
    u8g2.begin();
    // TODO Show splash screen

    // Setup pressure sensor
    // TODO Load sensor resolution from setting
    // TODO Change sensor resolution using ms5611.setOversampling()
    ms5611_osr_t osr = MS5611_ULTRA_HIGH_RES;
    ms5611.begin(osr);

    // TODO Reference pressure
    qnh1 = ms5611.readPressure(useTempCompensation);

    // Do some sensor readings due to warm up filtering
    for (int i = 0; i < WARMUP_COUNT; i++) {
        readSensor(false);
        delay(WARMUP_DELAY);
    }

    // Initialize refresh counter
    previousMillis = millis();
}

void loop()
{
    // Update pressure from sensor in given interval
    unsigned long currentMillis = millis();
    if ((unsigned long)(currentMillis - previousMillis) > REFRESH_MILLIS) {
        readSensor(true);
        updateLiveDisplay();
    }

    // TODO Low power mode
}

float getAltitude(uint32_t qnh) {

    // REMOVE
    Serial.print("pressure = ");
    Serial.print(pressure);
    Serial.print(", qnh = ");
    Serial.println(qnh);

    //return ms5611.getAltitude(pressure);
    return qnh != 0 ? ms5611.getAltitude(pressure) : ms5611.getAltitude(pressure, qnh);
}

void readSensor(boolean useTalmanFilter) {
    // Temperature compensation
    // TODO read from menu    

    // Update pressure
    uint32_t value = ms5611.readPressure(useTempCompensation);
    pressure = useTalmanFilter 
        ? ((1.0 - TALMAN_FACTOR) * pressure) + (TALMAN_FACTOR * value)
        : value;
}

void updateLiveDisplay() {
    char sAlt1[16];
    char sAlt2[16];
    

    // Calculate altitude from pressure
    uint32_t alt1 = getAltitude(qnh1);
    uint32_t alt2 = getAltitude(qnh2);

    // REMOVE
    Serial.print("alt1 = ");
    Serial.print(alt1);
    Serial.print(", alt2 = ");
    Serial.println(alt2);
    
    sprintf(sAlt1, "%d", alt1);
    sprintf(sAlt2, "%d", alt2);

    // TODO Add all shown values to struct and show only if some values changes
    
    // Display calculated values
    u8g2.firstPage();
    do {
        u8g2.setFont(u8g2_font_fur17_tn);
        u8g2.drawStr(0, 24, sAlt1);
        u8g2.setFont(u8g2_font_fur11_tn);
        u8g2.drawStr(0, 36, sAlt2);
    } while (u8g2.nextPage());
}
