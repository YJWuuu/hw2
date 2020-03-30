#include "mbed.h"



DigitalIn  Switch(SW3);
DigitalOut redLED(LED1);
DigitalOut greenLED(LED2);

BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);
char table[20] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F,
                    0xBF, 0X86, 0XDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xEF};

AnalogOut Aout(DAC0_OUT);
AnalogIn Ain(A0);
float ADCdata0;

Serial pc(USBTX, USBRX);
int sample = 1000;
float ADCdata[1000];

int main(void)
{
    bool done = false;
    int Ts = 0, freq = 0;
    int F[3] = {0};

    redLED = 1;
    greenLED = 1;
    display = 0x00;

    for (int i = 0; i < sample; i++) {
        Aout = Ain;
        ADCdata[i] = Ain;
        wait(1. / sample);
    }
    for (int i = 0; i < sample; i++) {
        pc.printf("%1.3f\r\n", ADCdata[i]);
        wait(0.1);
    }
    
    while (1) {
        while (!done) {
            ADCdata0 = Ain;
            wait(0.001);
            if ((ADCdata0 * 3.3 <= 1) && (Ain * 3.3 >= 1)) {
                while (Ain * 3.3 >= 1) {
                    Ts++;
                    wait(0.001);
                }
                done = true;
                freq = int(1 / (Ts * 0.001 * 2));
                int temp = freq;
                for (int i = 2; i >= 0; i--) {
                    F[i] = temp % 10;
                    temp /= 10;
                }
            }
        }
        if (Switch == 1) { // NOT PRESS
            greenLED = 0;   // Light Green
            redLED = 1;
            display = 0x00;
            for (float i = 0; i < 2; i += 0.05) {
                Aout = 0.5 + 0.5 * sin(i * 3.14159);
                wait((1. / freq) / (2. / 0.05));
            }
        }
        else {
            redLED = 0;
            greenLED = 1;
            display = table[F[0]];
            wait(1);
            display = table[F[1]];
            wait(1);
            display = table[F[2] + 10];
            wait(1);   
        }
    }
}