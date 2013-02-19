#ifndef __PCF_H__
#define __PCF_H__

class PCF8583 {
    public:
        I2C * i2c;
        PCF8583(I2C * i2cport);
        uint8_t FetchReg(uint8_t RegAddr);
        void SetReg(uint8_t RegAddr,uint8_t value);
        void SetAlarm(uint8_t seconds, uint8_t minuites, uint8_t hours);
        void SetAlarm(int UnixTimeStamp);
        void SecondToggle(void);
        void SetTime(int UnixTimeStamp);
        int GetSeconds(void);
        int GetMinuites(void);
        int GetHours(void);
    
};

#endif /* __PCF_H__ */
