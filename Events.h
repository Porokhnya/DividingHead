#pragma once

#include <Arduino.h>
#include "CoreArray.h"
#include "CONFIG.h"
//--------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
  EncoderPositionChanged, // смена позиции энкодера
  EncoderButtonClicked, // кликнута кнопка энкодера
  ButtonStateChanged, // состояние кнопки изменилось
  RotationRequested, // запросили вращение шаговика
  StepperWorkDone, // мотор закончил движение
  StepsRequested, // запросили шагание на определённое кол-во шагов
  #ifdef USE_KEYBOARD
  KeyboardEvent, // какой-то символ введён на клавиатуре
  #endif
  
} Event;
//--------------------------------------------------------------------------------------------------------------------------------------
#define BUTTON_PRESSED 1
#define BUTTON_CLICKED 2
#define BUTTON_DBLCLICKED 4
#define BUTTON_RETENTION 8
//--------------------------------------------------------------------------------------------------------------------------------------
#define LEFT_BUTTON 1
#define RIGHT_BUTTON 2
//--------------------------------------------------------------------------------------------------------------------------------------
typedef struct
{
  uint8_t button;
  uint8_t state;
  
} ButtonEventParam;
//--------------------------------------------------------------------------------------------------------------------------------------
typedef struct
{
  bool start;
  bool ccw;
  uint8_t speed;
  
} RotationEventParam;
//--------------------------------------------------------------------------------------------------------------------------------------
typedef struct
{
  bool start;
  bool ccw;
  uint8_t speed;
  uint16_t steps;
  
} StepsEventParam;
//--------------------------------------------------------------------------------------------------------------------------------------
struct IEventSubscriber
{
  virtual void onEvent(Event event, void* param) = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------
typedef Vector<IEventSubscriber*> IEventSubscriberList;
typedef void* EventSender;
//--------------------------------------------------------------------------------------------------------------------------------------
class EventsClass
{
  private:

    IEventSubscriberList list;
    EventSender sender;
  
public:
  EventsClass();

  void raise(EventSender sender, Event event, void* param);
  void subscribe(IEventSubscriber* s);
  void unsubscribe(IEventSubscriber* s);

  EventSender getEventSender(){ return sender; }

};
//--------------------------------------------------------------------------------------------------------------------------------------
extern EventsClass Events;
