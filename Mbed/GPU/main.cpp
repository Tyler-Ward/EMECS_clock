#include "mbed.h"
#include "gpu.h"
#include <string.h>

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

I2C * gpu_i2c;
DigitalIn sck(p11);


void wait();

int main()
{

    // Create I2C object
    gpu_i2c = new I2C(p9,p10);

    gpu_i2c->frequency(400000);
    char buffer[25];
    buffer[0] = GLOBAL_SET_TIME;
    buffer[1] = 1;
    buffer[2] = 2;
    buffer[3] = 3;
    buffer[4] = 5;
    gpu_i2c->start();
    gpu_i2c->write(SSD_I2C_ADDR);
    while(!(sck.read())) wait();
    gpu_i2c->write(buffer[0]);
    while(!(sck.read())) wait();
    gpu_i2c->write(buffer[1]);
    while(!(sck.read())) wait();
    gpu_i2c->write(buffer[2]);
    while(!(sck.read())) wait();
    gpu_i2c->write(buffer[3]);
    while(!(sck.read())) wait();
    gpu_i2c->write(buffer[4]);
    while(!(sck.read())) wait();
    gpu_i2c->stop();
    led2=1;

    while(1) {
        led1 = 1;
        wait(0.2);
        led1 = 0;
        wait(0.2);
    }
}

void wait(){
    for(long x = 0; x < 10000; x++){
        x++;
        led4=x%2;
    }
    led4=0;
}
