#ifndef _VSARDUINO_H_
#define _VSARDUINO_H_
//Board = Arduino Duemilanove w/ ATmega328
#define __AVR_ATmega328P__
#define ARDUINO 101
#define __AVR__
#define F_CPU 16000000L
#define __cplusplus
#define __attribute__(x)
#define __inline__
#define __asm__(x)
#define __extension__
#define __ATTR_PURE__
#define __ATTR_CONST__
#define __inline__
#define __asm__ 
#define __volatile__
#define __builtin_va_list
#define __builtin_va_start
#define __builtin_va_end
#define __DOXYGEN__
#define prog_void
#define PGM_VOID_P int
#define NOINLINE __attribute__((noinline))

typedef unsigned char byte;
extern "C" void __cxa_pure_virtual() {;}

//
//
void updatePanel();
void unplugSlaves();
void loadBar();
void uploadFont();
void panDistance(int first_col, int first_line);
void panFdata();
void panTemp(int first_col, int first_line);
void panEff(int first_col, int first_line);
void panRSSI(int first_col, int first_line);
void panCALLSIGN(int first_col, int first_line);
void panWindSpeed(int first_col, int first_line);
void panOff();
void panCur_A(int first_col, int first_line);
void panAlt(int first_col, int first_line);
void panClimb(int first_col, int first_line);
void panHomeAlt(int first_col, int first_line);
void panVel(int first_col, int first_line);
void panAirSpeed(int first_col, int first_line);
void panWarn(int first_col, int first_line);
void panThr(int first_col, int first_line);
void panBatteryPercent(int first_col, int first_line);
void panTime(int first_col, int first_line);
void panHomeDis(int first_col, int first_line);
void panHorizon(int first_col, int first_line);
void panPitch(int first_col, int first_line);
void panRoll(int first_col, int first_line);
void panBatt_A(int first_col, int first_line);
void panGPL(int first_col, int first_line);
void panGPSats(int first_col, int first_line);
void panGPS(int first_col, int first_line);
void panHeading(int first_col, int first_line);
void panRose(int first_col, int first_line);
void panBoot(int first_col, int first_line);
void panWPDis(int first_col, int first_line);
void panHomeDir(int first_col, int first_line);
void panFlightMode(int first_col, int first_line);
void showArrow(uint8_t rotate_arrow,uint8_t method);
void showHorizon(int start_col, int start_row);
void printHit(byte col, byte row, byte subval);
void do_converts();
void read_serial();

#include "D:\Sicherung\Desktop\Privates\arduino\ArduPlane\arduino-1.0.1\hardware\arduino\variants\standard\pins_arduino.h" 
#include "D:\Sicherung\Desktop\Privates\arduino\ArduPlane\arduino-1.0.1\hardware\arduino\cores\arduino\arduino.h"
#include "D:\Sicherung\Desktop\Privates\arduino\ArduPlane\ArduPlane_PhD\ArduCAM_OSD_SELF\ArduCAM_OSD\ArduCAM_OSD.ino"
#include "D:\Sicherung\Desktop\Privates\arduino\ArduPlane\ArduPlane_PhD\ArduCAM_OSD_SELF\ArduCAM_OSD\ArduCam_Max7456.cpp"
#include "D:\Sicherung\Desktop\Privates\arduino\ArduPlane\ArduPlane_PhD\ArduCAM_OSD_SELF\ArduCAM_OSD\ArduCam_Max7456.h"
#include "D:\Sicherung\Desktop\Privates\arduino\ArduPlane\ArduPlane_PhD\ArduCAM_OSD_SELF\ArduCAM_OSD\ArduNOTES.ino"
#include "D:\Sicherung\Desktop\Privates\arduino\ArduPlane\ArduPlane_PhD\ArduCAM_OSD_SELF\ArduCAM_OSD\BOOT_Func.ino"
#include "D:\Sicherung\Desktop\Privates\arduino\ArduPlane\ArduPlane_PhD\ArduCAM_OSD_SELF\ArduCAM_OSD\Font.ino"
#include "D:\Sicherung\Desktop\Privates\arduino\ArduPlane\ArduPlane_PhD\ArduCAM_OSD_SELF\ArduCAM_OSD\MAVLink.ino"
#include "D:\Sicherung\Desktop\Privates\arduino\ArduPlane\ArduPlane_PhD\ArduCAM_OSD_SELF\ArduCAM_OSD\OSD_Config.h"
#include "D:\Sicherung\Desktop\Privates\arduino\ArduPlane\ArduPlane_PhD\ArduCAM_OSD_SELF\ArduCAM_OSD\OSD_Config_Func.ino"
#include "D:\Sicherung\Desktop\Privates\arduino\ArduPlane\ArduPlane_PhD\ArduCAM_OSD_SELF\ArduCAM_OSD\OSD_Func.h"
#include "D:\Sicherung\Desktop\Privates\arduino\ArduPlane\ArduPlane_PhD\ArduCAM_OSD_SELF\ArduCAM_OSD\OSD_Panels.ino"
#include "D:\Sicherung\Desktop\Privates\arduino\ArduPlane\ArduPlane_PhD\ArduCAM_OSD_SELF\ArduCAM_OSD\OSD_Vars.h"
#include "D:\Sicherung\Desktop\Privates\arduino\ArduPlane\ArduPlane_PhD\ArduCAM_OSD_SELF\ArduCAM_OSD\Spi.cpp"
#include "D:\Sicherung\Desktop\Privates\arduino\ArduPlane\ArduPlane_PhD\ArduCAM_OSD_SELF\ArduCAM_OSD\Spi.h"
#include "D:\Sicherung\Desktop\Privates\arduino\ArduPlane\ArduPlane_PhD\ArduCAM_OSD_SELF\ArduCAM_OSD\read_serial.ino"
#endif
