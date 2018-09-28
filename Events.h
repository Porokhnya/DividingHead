#pragma once

#include <Arduino.h>
#include "CoreArray.h"
//--------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
  EncoderPositionChanged, // смена позиции энкодера
  EncoderButtonClicked, // кликнута кнопка энкодера
  
} Event;
//--------------------------------------------------------------------------------------------------------------------------------------
struct IEventSubscriber
{
  virtual void onEvent(Event event, void* param) = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------
typedef Vector<IEventSubscriber*> IEventSubscriberList;
//--------------------------------------------------------------------------------------------------------------------------------------
class EventsClass
{
  private:

    IEventSubscriberList list;
  
public:
  EventsClass();

  void raise(Event event, void* param);
  void subscribe(IEventSubscriber* s);
  void unsubscribe(IEventSubscriber* s);

};
//--------------------------------------------------------------------------------------------------------------------------------------
extern EventsClass Events;
