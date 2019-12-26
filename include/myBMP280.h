#ifndef myBMP280
#define myBMP280
#ifndef place
  #include "place.h"
 #endif
#ifndef defs
 #include "defs.h"
 #endif
#ifdef Lz428266YE
  #include <Adafruit_Sensor.h> //pio lib install "Adafruit Unified Sensor" 
  #include "Adafruit_BMP280.h"
 #endif 

Adafruit_BMP280 bmp;



bool initmyBMP280(void){
 if (!bmp.begin(0x77)) return false;
 bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */ 
return true;
}

float gettemp(void){
    if (bmpgood) return bmp.readTemperature(); else return -3.141592653589;
}
float getpress(void){
    return bmp.readPressure();
}
float getalt(void){
    return bmp.readAltitude(1013.25);
}
#endif