#include "CONFIG.h"
#include "Encoder.h"
#include "CoreButton.h"
#include "Events.h"
#include "ScreenHAL.h"
#include "Screens.h"
#include "Memory.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
RotaryEncoder encoder(ENCODER_A_PIN,ENCODER_B_PIN,ENCODER_PPC);
Button encoderButton;
uint32_t screenIdleTimer = 0;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void screenAction(AbstractHALScreen* screen)
{
   // какое-то действие на экране произошло.
   // тут просто сбрасываем таймер ничегонеделанья.
   screenIdleTimer = millis();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setup() 
{
  MemInit();
  
  Serial.begin(SERIAL_SPEED);
  
  encoder.begin();
  encoderButton.begin(ENCODER_BUTTON_PIN);

  DBGLN(F("Init screen..."));
  Screen.setup();

  DBGLN(F("Add splash screen...")); 
  Screen.addScreen(SplashScreen::create()); // добавляем стартовый экран
  DBGLN(F("Splash screen added.")); 

  
  DBGLN(F("Add main screen...")); 
  Screen.addScreen(MainScreen::create()); // добавляем стартовый экран
  DBGLN(F("Main screen added.")); 

    // переключаемся на экран заставки
  Screen.switchToScreen(Splash);

  screenIdleTimer = millis();
  Screen.onAction(screenAction);


  DBGLN(F("Inited."));
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void pollEncoder()
{
  encoder.update();  

  int changes = encoder.getChange();
  if(changes != 0)
  {
    Events.raise(EncoderPositionChanged, &changes);
  }

  encoderButton.update();
  if(encoderButton.isClicked())
  {
    Events.raise(EncoderButtonClicked,NULL);
  }
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void loop() 
{
  pollEncoder();
  Screen.update();

 // проверяем, какой экран активен. Если активен главный экран - сбрасываем таймер ожидания. Иначе - проверяем, не истекло ли время ничегонеделанья.
  AbstractHALScreen* activeScreen = Screen.getActiveScreen();
  if(activeScreen == StartScreen)
  {
    screenIdleTimer = millis();
  }
  else
  {
      if(millis() - screenIdleTimer > RESET_TO_MAIN_SCREEN_DELAY)
      {
        screenIdleTimer = millis();
        Screen.switchToScreen(StartScreen);
      }
  } // else  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool nestedYield = false;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void yield()
{
  if(nestedYield)
    return;
    
  nestedYield = true;

  pollEncoder();

 nestedYield = false;
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

