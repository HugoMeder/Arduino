/* 
	Editor: http://www.visualmicro.com
	        visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
	        the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
	        all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
	        note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Arduino/Genuino Uno, Platform=avr, Package=arduino
*/

#define __AVR_ATmega328p__
#define __AVR_ATmega328P__
#define ARDUINO 106011
#define ARDUINO_MAIN
#define F_CPU 16000000L
#define __AVR__
#define F_CPU 16000000L
#define ARDUINO 106011
#define ARDUINO_AVR_UNO
#define ARDUINO_ARCH_AVR

//
//
void test_gyr_acc();
void mpu6050_init();
void print_angle();
void motor_arm();
void motor_idle();
void motor_off();
void stabilize();
void set_power();
void read_two_data();
void calc_wanted_z_ang();
void zero_on_zero_throttle();
void escRead();
void elevatorRead();
void aileronRead();
void pppRead();
void ppp2Read();
void ppp3Read();
void reciever_to_value();
void test_radio_reciev();
void check_radio_signal();
void read_acc_gyr();
int MPU6050_read(int start, uint8_t *buffer, int size);
int MPU6050_write(int start, const uint8_t *pData, int size);
int MPU6050_write_reg(int reg, uint8_t data);

#include "pins_arduino.h" 
#include "arduino.h"
#include "Quadcopter_Code_Arduino.ino"
