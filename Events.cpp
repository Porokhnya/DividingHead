#include "Events.h"
#include "CONFIG.h"
//--------------------------------------------------------------------------------------------------------------------------------------
EventsClass Events;
//--------------------------------------------------------------------------------------------------------------------------------------
EventsClass::EventsClass()
{
  
}
//--------------------------------------------------------------------------------------------------------------------------------------
void EventsClass::raise(EventSender s, Event evt, void* param)
{
  sender = s;
  #ifdef _DEBUG
    switch(evt)
    {
      case StepperWorkDone: // мотор закончил работу
      {
        DBGLN(F("Stepper switched to idle mode!"));
      }
      break; // StepperWorkDone
      
      case EncoderPositionChanged: // смена позиции энкодера
      {
        int* changes = (int*) param;
        DBG(F("Encoder changes: "));
        DBGLN(*changes);        
      }
      break; // EncoderPositionChanged

      case EncoderButtonClicked: // кликнута кнопка энкодера
      {
        DBGLN(F("Encoder button clicked!")); 
      }
      break; // EncoderButtonClicked

      case ButtonStateChanged: // изменение состояния кнопки
      {
        ButtonEventParam* p = (ButtonEventParam*) param;
        DBG(F("Button state changed, button #"));
        DBG(p->button);
        DBG(F(", state = "));
        DBGLN(p->state);
      }
      break; // ButtonStateChanged

      case RotationRequested: // запросили запустить или остановить вращение шаговика
      {
        RotationEventParam* p = (RotationEventParam*) param;
        if(p->start)
        {
          DBG(F("Start"));
        }
        else
        {
          DBG(F("Stop"));
        }

        DBG(F(" rotation, ccw="));
        
        if(p->ccw)
        {
          DBG(F("true"));
        }
        else
        {
          DBG(F("false"));
        }

        DBG(F(", speed="));
        DBGLN(p->speed);
      }
      break; // RotationRequested

      case StepsRequested: // запросили запустить или остановить шагание на определённое кол-во шагов
      {
        StepsEventParam* p = (StepsEventParam*) param;
        if(p->start)
        {
          DBG(F("Start"));
        }
        else
        {
          DBG(F("Stop"));
        }

        DBG(F(" STEPPING, ccw="));
        
        if(p->ccw)
        {
          DBG(F("true"));
        }
        else
        {
          DBG(F("false"));
        }

        DBG(F(", speed="));
        DBG(p->speed);        

        DBG(F(", steps="));
        DBGLN(p->steps);        
        
      }
      break; // StepsRequested
      
    } // switch
  #endif // _DEBUG

  size_t to = list.size();
  for(size_t i=0;i<to;i++)
  {
    list[i]->onEvent(evt, param);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------
void EventsClass::subscribe(IEventSubscriber* s)
{
  if(!s)
    return;
    
  unsubscribe(s);
  list.push_back(s);
}
//--------------------------------------------------------------------------------------------------------------------------------------
void EventsClass::unsubscribe(IEventSubscriber* s)
{
  if(!s || !list.size())
    return;

  IEventSubscriberList cpy;
  size_t to = list.size();
  for(size_t i=0;i<to;i++)
  {
    if(list[i] != s)
      cpy.push_back(list[i]);
  }

  list = cpy;
}
//--------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------

