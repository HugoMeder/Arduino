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
#define _VMDEBUG 1
#define ARDUINO 106011
#define ARDUINO_MAIN
#define F_CPU 16000000L
#define __AVR__
#define F_CPU 16000000L
#define ARDUINO 106011
#define ARDUINO_AVR_UNO
#define ARDUINO_ARCH_AVR

byte ReadRegisterValue(int RegisterAddress);
void ReadRegisters();
void ResetCameraRegisters();
void SetupCamera();
void InitializeOV7670Camera();
void SetupCameraAdvancedAutoWhiteBalanceConfig();
void SetupCameraUndocumentedRegisters();
void SetupCameraFor30FPS();
void SetupCameraABLC();
void SetupOV7670ForVGARawRGB();
void SetupOV7670ForVGAProcessedBayerRGB();
void SetupCameraAverageBasedAECAGC();
void SetCameraHistogramBasedAECAGC();
void SetupOV7670ForQVGAYUV();
void SetupCameraNightMode();
void SetupCameraSimpleAutomaticWhiteBalance();
void SetupCameraAdvancedAutomaticWhiteBalance();
void SetupCameraGain();
void SetCameraSaturationControl();
void SetCameraColorMatrixYUV();
void SetCameraFPSMode();
void SetCameraAEC();
void SetupCameraAWB();
void SetupCameraDenoise();
void SetupCameraEdgeEnhancement();
void SetupCameraDenoiseEdgeEnhancement();
void SetupCameraArrayControl();
void SetupCameraADCControl();
void SetupOV7670ForQQVGAYUV();
void CaptureOV7670Frame();
String CreatePhotoFilename();
String CreatePhotoInfoFilename();
String CreatePhotoInfo();
void CreatePhotoInfoFile();
byte ConvertPinValueToByteValue(int PinValue, int PinPosition);
void ReadTransmitCapturedFrame();
void TakePhoto();
void PulseLowEnabledPin(int PinNumber, int DurationMicroSecs);
void PulsePin(int PinNumber, int DurationMicroSecs);
String ParseI2CResult(int result);
int OV7670Write(int start, const byte *pData, int size);
int OV7670WriteReg(int reg, byte data);
int OV7670Read(int start, byte *buffer, int size);
int OV7670ReadReg(int reg, byte *data);
void WriteFileTest(String Filename);
void ReadPrintFile(String Filename);
//
void ExecuteCommand(String Command);
boolean ProcessRawCommandElement(String Element);
void ParseRawCommand(String RawCommandLine);
void DisplayHelpCommandsParams();
void  DisplayHelpMenu();
void DisplayCurrentCommand();
void CheckRemoveFile(String Filename);
//
int ParseCommand(const char* commandline, char splitcharacter, String* Result);

#include "pins_arduino.h" 
#include "arduino.h"
#include "OV7670FIFO.ino"
