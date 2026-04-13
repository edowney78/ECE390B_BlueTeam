#include <AVRouting/io.h>
#include <util/delay.h>
#include <stdio.h>

#define F_CPU 16000000UL

float vin;
float vout;

float R2 = 1000000.0;
float R3 = 2200000.0;
float ratio = R3/(R2+R3); 

Serial.begin(9600);
pinMode(A4, INPUT);

int main(void){

    vin = analogRead(A4); // analogRead reads voltages from 0V to 5V and maps them to values 0-1023  
    vout = vin * (5.0 / 1023.0) / ratio; // vin * (5 / 1023) gets V accros R3 but / ratio gets actual Vbatt
    Serial.print("Voltage: ");
    Serial.println(vout);
    delay(1000);

}