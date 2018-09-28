//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
 ВСЕ НАСТРОЙКИ ПРОШИВКИ - В ФАЙЛЕ CONFIG.H (ВТОРАЯ ВКЛАДКА СЛЕВА) !!!
 
 В ПАПКЕ libraries архива с проектов - используемые в проекте библиотеки, их надо установить в среду Arduino IDE, даже если такая же библиотека
 уже стоит (UTFT, например - используется изменённый!!!)
 
 */
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "CONFIG.h"
#include "Encoder.h"
#include "CoreButton.h"
#include "Events.h"
#include "ScreenHAL.h"
#include "Screens.h"
#include "Memory.h"
#include "MotorController.h"
#include <Wire.h>
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
RotaryEncoder encoder(ENCODER_A_PIN,ENCODER_B_PIN,ENCODER_PPC);
Button encoderButton, leftButton, rightButton;
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
   #if EEPROM_USED_MEMORY != EEPROM_BUILTIN
    Wire.begin();
   #endif
  
  MemInit();
  
  Serial.begin(SERIAL_SPEED);
  
  encoder.begin();
  encoderButton.begin(ENCODER_BUTTON_PIN);
  leftButton.begin(LEFT_BUTTON_PIN);
  rightButton.begin(RIGHT_BUTTON_PIN);

  DBGLN(F("Init screen..."));
  Screen.setup();

  DBGLN(F("Add splash screen...")); 
  Screen.addScreen(SplashScreen::create()); // добавляем экран заставки
  DBGLN(F("Splash screen added.")); 

  
  DBGLN(F("Add main screen...")); 
  Screen.addScreen(MainScreen::create()); // добавляем стартовый экран
  DBGLN(F("Main screen added.")); 

  DBGLN(F("Add rotation screen...")); 
  Screen.addScreen(RotationScreen::create()); // добавляем экран вращения
  DBGLN(F("Rotation screen added.")); 

    // переключаемся на экран заставки
  Screen.switchToScreen(Splash);

  screenIdleTimer = millis();
  Screen.onAction(screenAction);

  DBGLN(F("Init stepper...")); 
  MotorController.init();
  DBGLN(F("Stepper inited."));   


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
void raiseButtonState(Button& btn, uint8_t btnID)
{
  ButtonEventParam p;
  p.state = 0;
  p.state |= btn.isPressed() ? BUTTON_PRESSED : 0;
  p.state |= btn.isClicked() ? BUTTON_CLICKED : 0;
  p.state |= btn.isDoubleClicked() ? BUTTON_DBLCLICKED : 0;
  p.state |= btn.isRetention() ? BUTTON_RETENTION : 0;


  if(p.state)
  {
    p.button = btnID;
    Events.raise(ButtonStateChanged, &p);
  }
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void pollButtons()
{
  leftButton.update();
  rightButton.update();

  raiseButtonState(leftButton, LEFT_BUTTON);
  raiseButtonState(rightButton, RIGHT_BUTTON);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void loop() 
{
  pollEncoder();
  pollButtons();
  
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
  pollButtons();

 nestedYield = false;
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

