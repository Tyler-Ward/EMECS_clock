#include "mbed.h"

#include "PCF8583Driver.h"
#include "DateTime.h"


#define RTCAddr 0xA0
#define ControlRegAddr 0x00
#define SecondsRegAddr 0x02
#define MinuitesRegAddr 0x03
#define HoursRegAddr    0x04
#define AlarmControlRegAddr 0x08
#define AlarmSecondsRegAddr 0x0A


PCF8583::PCF8583(I2C * i2cport)
{
    PCF8583::i2c = i2cport;
    int ControlReg;
    ControlReg=FetchReg(ControlRegAddr);
    
    //put into 50hz mode
    ControlReg &= 0xC0;
    ControlReg |= 0x10;
    
    SetReg(ControlRegAddr,0x10);
}

uint8_t IntToBCD(int value)
{
    return ((value/10) << 4) + (value % 10);
}
uint8_t BCDToInt(uint8_t value)
{
    return ((0x0F & (value>>4)) * 10 ) + (0x0F & value);
}

void PCF8583::SetTime(int UnixTimeStamp)
{

    DateTime date(UnixTimeStamp);
    DigitalOut myled(LED3);
    myled = 1;
    wait(0.2);
    myled = 0;
    wait(0.2);
            
    PCF8583::i2c->start();
    PCF8583::i2c->write(RTCAddr);
    PCF8583::i2c->write(SecondsRegAddr);
    PCF8583::i2c->write(IntToBCD(date.second()));
    PCF8583::i2c->write(IntToBCD(date.minute()));
    PCF8583::i2c->write(0x3F & (IntToBCD(date.hour())));
    PCF8583::i2c->write(0x3F & (IntToBCD(date.day())));
    PCF8583::i2c->write(0x1F & (IntToBCD(date.month())));
    PCF8583::i2c->stop();

}

int PCF8583::GetSeconds(void)
{
/*    uint8_t bcdsecs = FetchReg(SecondsRegAddr);
    DigitalOut myled(LED4);
    myled = 1;

    uint8_t secs = BCDToInt(bcdsecs);
    wait(0.2);
    myled = 0;
    return secs; */
    return (BCDToInt(FetchReg(SecondsRegAddr)));
}

int PCF8583::GetMinuites(void)
{
    return (BCDToInt(FetchReg(MinuitesRegAddr)));
}

int PCF8583::GetHours(void)
{
    return (BCDToInt(0x3F & FetchReg(HoursRegAddr)));
}


uint8_t PCF8583::FetchReg(uint8_t RegAddr)
{
    uint8_t RegContents;
    
    PCF8583::i2c->start();
    PCF8583::i2c->write(RTCAddr);
    PCF8583::i2c->write(RegAddr);
    PCF8583::i2c->start();
    PCF8583::i2c->write(RTCAddr+1);
    RegContents = PCF8583::i2c->read(0);
    PCF8583::i2c->stop();
    return RegContents;
}

void PCF8583::SetReg(uint8_t RegAddr,uint8_t value)
{
    PCF8583::i2c->start();
    PCF8583::i2c->write(RTCAddr);
    PCF8583::i2c->write(RegAddr);
    PCF8583::i2c->write(value);
    PCF8583::i2c->stop();
}

void PCF8583::SetAlarm(uint8_t seconds, uint8_t minutes, uint8_t hours)
{
    //set alarm times
    PCF8583::i2c->start();
    PCF8583::i2c->write(RTCAddr);
    PCF8583::i2c->write(AlarmSecondsRegAddr);
    PCF8583::i2c->write(IntToBCD(seconds));
    PCF8583::i2c->write(IntToBCD(minutes));
    PCF8583::i2c->write(0x3F & (IntToBCD(hours)));
    PCF8583::i2c->write(0x3F & (IntToBCD(1)));
    PCF8583::i2c->write(0x1F & (IntToBCD(2)));
    PCF8583::i2c->stop();
    
    //set alarm register into active mode
    
    SetReg(AlarmControlRegAddr,0x9C);//(FetchReg(AlarmControlRegAddr) & 0x0F ) | 0x90);
    SetReg(ControlRegAddr,(FetchReg(ControlRegAddr)) | 0x04);
    
    // await allarm
      
    
}

void PCF8583::SetAlarm(int UnixTimeStamp)
{

    DateTime date(UnixTimeStamp);
    PCF8583::i2c->start();
    PCF8583::i2c->write(RTCAddr);
    PCF8583::i2c->write(AlarmSecondsRegAddr);
    PCF8583::i2c->write(IntToBCD(date.second()));
    PCF8583::i2c->write(IntToBCD(date.minute()));
    PCF8583::i2c->write(0x3F & (IntToBCD(date.hour())));
    PCF8583::i2c->write(0x1F & (IntToBCD(date.day())));
    PCF8583::i2c->write(0x7F);
    PCF8583::i2c->stop();
    
    //set alarm register into active mode
    
    SetReg(AlarmControlRegAddr,0x90);//(FetchReg(AlarmControlRegAddr) & 0x0F ) | 0x90);
    SetReg(ControlRegAddr,(FetchReg(ControlRegAddr) | 0x04));
    
    // await allarm

}

void PCF8583::SecondToggle(void)
{
    SetReg(AlarmControlRegAddr,0x08);
}


