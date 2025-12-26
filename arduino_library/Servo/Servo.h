#ifndef Servo_h
#define Servo_h

#include <inttypes.h>

// 아키텍처별 헤더 포함 (기존 코드 유지)
#if defined(ARDUINO_ARCH_AVR)
#include "avr/ServoTimers.h"
#elif defined(ARDUINO_ARCH_SAM)
#include "sam/ServoTimers.h"
#elif defined(ARDUINO_ARCH_SAMD)
#include "samd/ServoTimers.h"
#elif defined(ARDUINO_ARCH_STM32F4)
#include "stm32f4/ServoTimers.h"
#elif defined(ARDUINO_ARCH_NRF52)
#include "nrf52/ServoTimers.h"
#elif defined(ARDUINO_ARCH_MEGAAVR)
#include "megaavr/ServoTimers.h"
#elif defined(ARDUINO_ARCH_MBED)
#include "mbed/ServoTimers.h"
#elif defined(ARDUINO_ARCH_RENESAS)
#include "renesas/ServoTimers.h"
#elif defined(ARDUINO_ARCH_XMC)
#include "xmc/ServoTimers.h"
#else
#error "This library only supports boards with an AVR, SAM, SAMD, NRF52, STM32F4, Renesas or XMC processor."
#endif

#define Servo_VERSION           2

// [수정 1] Geekservo 지원을 위해 범위를 넓게 잡음
#define MIN_PULSE_WIDTH       500     // Geekservo 최소폭 (기존 544 -> 500)
#define MAX_PULSE_WIDTH      5000     // Geekservo 최대폭 (기존 2400 -> 5000)
#define DEFAULT_PULSE_WIDTH  1500     
#define REFRESH_INTERVAL    20000     

#define SERVOS_PER_TIMER       12     
#define MAX_SERVOS   (_Nbr_16timers  * SERVOS_PER_TIMER)
#define INVALID_SERVO         255     

#if !defined(ARDUINO_ARCH_STM32F4) && !defined(ARDUINO_ARCH_XMC)

typedef struct  {
  uint8_t nbr        :6 ;             
  uint8_t isActive   :1 ;             
} ServoPin_t   ;

typedef struct {
  ServoPin_t Pin;
  volatile unsigned int ticks;
} servo_t;

class Servo
{
public:
  Servo();
  uint8_t attach(int pin);           // 기본 연결 (일반 서보용 안전장치 포함됨)
  uint8_t attach(int pin, int min, int max); // 사용자 지정 연결 (Geekservo용)
  void detach();
  void write(int value);             // 일반 0~180도 제어
  void writeMicroseconds(int value); 
  int read();                        
  int readMicroseconds();            
  bool attached();                   

  // [수정 2] Geekservo 전용 함수 선언
  void writeGeekAngle(int degrees);      // Geekservo 360도 절대 각도 제어 (0~360)
  void writeGeekWheel(int speed, bool cw); // Geekservo DC모터 모드 (속도, 방향)

private:
   uint8_t servoIndex;               
   // [수정 3] 범위 확장을 위해 int8_t를 int로 변경 (중요!)
   int min;                       
   int max;                       
};

#endif
#endif