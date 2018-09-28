#pragma once
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "ScreenHAL.h"
#include "UTFT_Buttons_Rus.h"
#include "Events.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// главный экран
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class MainScreen : public AbstractHALScreen, public IEventSubscriber
{
  public:

  static AbstractHALScreen* create()
  {
    return new MainScreen();
  }
  
   virtual void onActivate();
   virtual void onDeactivate();

   virtual void onEvent(Event event, void* param);


protected:
  
    virtual void doSetup(HalDC* hal);
    virtual void doUpdate(HalDC* hal);
    virtual void doDraw(HalDC* hal);

private:
    MainScreen();
    ~MainScreen();


    UTFT_Buttons_Rus* buttons;
    void drawGUI();

    int divideDegreeButton, dividePartsButton, jogButton, rotationButton, settingsButton, lastActiveButton;

  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern MainScreen* StartScreen;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class SplashScreen : public AbstractHALScreen
{
  public:

  static AbstractHALScreen* create()
  {
    return new SplashScreen();
  }
  
   virtual void onActivate();
   virtual void onDeactivate();


protected:
  
    virtual void doSetup(HalDC* hal);
    virtual void doUpdate(HalDC* hal);
    virtual void doDraw(HalDC* hal);

private:
    SplashScreen();

    uint32_t showTime;

  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern SplashScreen* Splash;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

