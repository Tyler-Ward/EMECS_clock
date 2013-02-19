#include "mbed.h"
#include "PCF8583Driver.h"
#include "DateTime.h"

DigitalOut myled(LED1);
DigitalOut secled(LED2);
I2C * rtci2c;

Serial pc(USBTX, USBRX); // tx, rx

int main() {

rtci2c = new I2C(p9,p10);

PCF8583 Rtc(rtci2c);
Rtc.SetTime(1359749180);
Rtc.SetAlarm(1359749190);
//Rtc.SecondToggle();

char data[20];
rtci2c->read(0xA0,data,20);
int i;
for(i=0;i<=20;i++){
pc.printf("%u",data[i]);
}

    while(1) {
        myled = 1;
        wait(0.5);
        myled = 0;
        wait(0.5);
        uint8_t data = Rtc.GetSeconds();
        //pc.printf("%u",data);
        if( data >= 30 )
        {
            secled=1;
        }
        else
        {
            secled=0;
        }
        
    }
}
