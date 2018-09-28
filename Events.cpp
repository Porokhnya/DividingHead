#include "Events.h"
#include "CONFIG.h"
//--------------------------------------------------------------------------------------------------------------------------------------
EventsClass Events;
//--------------------------------------------------------------------------------------------------------------------------------------
EventsClass::EventsClass()
{
  
}
//--------------------------------------------------------------------------------------------------------------------------------------
void EventsClass::raise(Event evt, void* param)
{
  #ifdef _DEBUG
    switch(evt)
    {
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

