//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
 Прошивка управления шаговым двигателем для Arduino Mega 2560 и 3.2'' TFT дисплея (контроллер ILI9481).
 Автор: Порохня Дмитрий, spywarrior@gmail.com

 Прошивка свободна для НЕКОММЕРЧЕСКОГО ИСПОЛЬЗОВАНИЯ!
 
 ВСЕ НАСТРОЙКИ ПРОШИВКИ - В ФАЙЛЕ CONFIG.H (ВТОРАЯ ВКЛАДКА СЛЕВА) !!!
 
 В ПАПКЕ libraries архива с проектом - используемые в проекте библиотеки, их надо установить в среду Arduino IDE, даже если такая же библиотека
 уже стоит (UTFT, например - используется изменённый!!!)

 В ПАПКЕ Schematic - СХЕМА СОЕДИНЕНИЙ ДЛЯ ПРОГРАММЫ sPlan.
 
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
#ifdef USE_KEYBOARD
  #include "MPR121.h"
  bool hasDataFromKeyboard = false;
  MPR121 keyboard;
  void keyboardInterrupt()
  {
    hasDataFromKeyboard = true;
  }

  // преобразуем нажатые площадки в их привязанные номера
  int8_t getKeyCode(uint8_t pressed)
  {
    switch(pressed)
    {
      case KEY_1: return 1;
      case KEY_2: return 2;
      case KEY_3: return 3;
      case KEY_4: return 4;
      case KEY_5: return 5;
      case KEY_6: return 6;
      case KEY_7: return 7;
      case KEY_8: return 8;
      case KEY_9: return 9;      
      case KEY_0: return 0;
      case KEY_DELETE: return DELETE_KEY;
      case KEY_BACKSPACE: return BACKSPACE_KEY;
    } // switch
    
    return -1;
  }
#endif // USE_KEYBOARD
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void screenAction(AbstractHALScreen* screen)
{
   // какое-то действие на экране произошло.
   // тут просто сбрасываем таймер ничегонеделанья.
   screenIdleTimer = millis();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#if (EEPROM_USED_MEMORY != EEPROM_BUILTIN) || defined(USE_KEYBOARD)
void resetI2C(uint8_t sclPin, uint8_t sdaPin)
{
  pinMode(sdaPin, OUTPUT);
  digitalWrite(sdaPin,HIGH);
  pinMode(sclPin,OUTPUT);
  
  for(uint8_t i=0;i<10;i++) // Send NACK signal
  {
    digitalWrite(sclPin,HIGH);
    delayMicroseconds(5);
    digitalWrite(sclPin,LOW);
    delayMicroseconds(5);   
  }

  // Send STOP signal
  digitalWrite(sdaPin,LOW);
  delayMicroseconds(5);
  digitalWrite(sclPin,HIGH);
  delayMicroseconds(2);
  digitalWrite(sdaPin,HIGH);
  delayMicroseconds(2);
  
  
  pinMode(sclPin,INPUT);   
  pinMode(sdaPin,INPUT);   
}
#endif
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setup() 
{
  
   #if (EEPROM_USED_MEMORY != EEPROM_BUILTIN) || defined(USE_KEYBOARD)
      resetI2C(SCL, SDA);
      Wire.begin();
   #endif
  
  MemInit();

  Serial.begin(SERIAL_SPEED);

   #ifdef USE_KEYBOARD
   
     pinMode(KEYBOARD_IRQ_PIN,INPUT);
     digitalWrite(KEYBOARD_IRQ_PIN,HIGH); // включаем подтяжку к питанию
     attachInterrupt(digitalPinToInterrupt(KEYBOARD_IRQ_PIN),keyboardInterrupt,FALLING);
     if(!keyboard.begin(KEYBOARD_ADDRESS))
     {
        DBGLN(F("CAN'T INIT KEYBOARD!!!"));
     }
   #endif // USE_KEYBOARD
  
  
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

  DBGLN(F("Add steps screen...")); 
  Screen.addScreen(StepsScreen::create()); // добавляем экран шагания
  DBGLN(F("Steps screen added.")); 

  DBGLN(F("Add settings screen...")); 
  Screen.addScreen(TuneScreen::create()); // добавляем экран настроек
  DBGLN(F("Settings screen added.")); 

  DBGLN(F("Add motor setup screen...")); 
  Screen.addScreen(MotorSetupScreen::create()); // добавляем экран настроек мотора
  DBGLN(F("Motor setup screen added.")); 

  DBGLN(F("Add microstep setup screen...")); 
  Screen.addScreen(MicrostepScreen::create()); // добавляем экран настроек делителя шага
  DBGLN(F("Microstep screen added.")); 

  DBGLN(F("Add reduction setup screen...")); 
  Screen.addScreen(ReductionScreen::create()); // добавляем экран настроек редукции
  DBGLN(F("Reduction screen added.")); 

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
    Events.raise(NULL,EncoderPositionChanged, &changes);
  }

  encoderButton.update();
  if(encoderButton.isClicked())
  {
    Events.raise(NULL,EncoderButtonClicked,NULL);
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
    Events.raise(NULL,ButtonStateChanged, &p);
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

  #ifdef USE_KEYBOARD
  if(hasDataFromKeyboard)
  {
    hasDataFromKeyboard = false;

    uint16_t touched = keyboard.touched();
    for (uint8_t i=0; i<12; i++) 
    {
      if(touched & (1 << i))
      {
        int8_t keyCode = getKeyCode(i);
        if(keyCode != -1)
        {
          Events.raise(NULL,KeyboardEvent, &keyCode);
          break;
        }
      }
    } // for

  }
  #endif
  
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

