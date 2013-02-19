
#define CMD_DISPLAY_OFF   0xAE
#define CMD_DISPLAY_ON    0xAF

#define CMD_SET_DISP_START_LINE  0x40
#define CMD_SET_PAGE  0xB0

#define CMD_SET_COLUMN_UPPER  0x10
#define CMD_SET_COLUMN_LOWER  0x00

#define CMD_SET_ADC_NORMAL  0xA0
#define CMD_SET_ADC_REVERSE 0xA1

#define CMD_SET_DISP_NORMAL 0xA6
#define CMD_SET_DISP_REVERSE 0xA7

#define CMD_SET_ALLPTS_NORMAL 0xA4
#define CMD_SET_ALLPTS_ON  0xA5
#define CMD_SET_BIAS_9 0xA2 
#define CMD_SET_BIAS_7 0xA3

#define CMD_RMW  0xE0
#define CMD_RMW_CLEAR 0xEE
#define CMD_INTERNAL_RESET  0xE2
#define CMD_SET_COM_NORMAL  0xC0
#define CMD_SET_COM_REVERSE  0xC8
#define CMD_SET_POWER_CONTROL  0x28
#define CMD_SET_RESISTOR_RATIO  0x20
#define CMD_SET_VOLUME_FIRST  0x81
#define  CMD_SET_VOLUME_SECOND  0
#define CMD_SET_STATIC_OFF  0xAC
#define  CMD_SET_STATIC_ON  0xAD
#define CMD_SET_STATIC_REG  0x0
#define CMD_SET_BOOSTER_FIRST  0xF8
#define CMD_SET_BOOSTER_234  0
#define  CMD_SET_BOOSTER_5  1
#define  CMD_SET_BOOSTER_6  3
#define CMD_NOP  0xE3
#define CMD_TEST  0xF0

/*-----------------------
--- GPU COMMAND CODES ---
-----------------------*/
#define GPU_PRINT_STRING 0x10
#define GPU_SET_PIXEL 0x11
#define GPU_CLEAR_PIXEL 0x12
#define GPU_DRAW_LINE 0x13
#define GPU_DRAW_RECTANGLE 0x14
#define GPU_FILL_RECTANGLE 0x15
#define GPU_DRAW_CIRCLE 0x16
#define GPU_CLEAR_SCREEN 0x17

#define GPU_SET_TIME 0x30
#define GPU_SET_DATE 0x31

/* ------------------- */

void setup_i2c(void);
void execute_instruction(void);
void display_time(uint8_t hour_msB, uint8_t hour_lsB, uint8_t minute_msB, uint8_t minute_lsB);
void display_date(uint8_t weekday, uint8_t day_MSD, uint8_t day_LSD, uint8_t month);
void marqueestring(uint8_t *buff, uint8_t line, uint8_t *c);

void spiwrite(uint8_t c);

void setup(void);
void loop(void);

void st7565_init(void);
void st7565_command(uint8_t c);
void st7565_data(uint8_t c);
void st7565_set_brightness(uint8_t val);

void clear_screen(void);
void clear_buffer(uint8_t *buffer);
void write_buffer(uint8_t *buffer);

