//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Screens.h"
#include "CONFIG.h"
#include "Settings.h"
#include "MotorController.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MainScreen* StartScreen = NULL;        
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void drawBackButton(HalDC* hal, bool active)
{
 int screenWidth = hal->getScreenWidth();
 int screenHeight = hal->getScreenHeight();
 int buttonWidth = screenWidth - BUTTON_X_OFFSET*2;

  int top = screenHeight - BUTTON_HEIGHT - BUTTON_Y_OFFSET;
  int left = BUTTON_X_OFFSET;

  
  hal->setColor(active ? VGA_BLUE : VGA_WHITE);
  hal->fillRoundRect(left,top,left + buttonWidth, top + BUTTON_HEIGHT);

  if(!active)
  {
    hal->setColor(VGA_SILVER);
    hal->drawRoundRect(left,top,left + buttonWidth, top + BUTTON_HEIGHT);
  }

  hal->setFont(SCREEN_BIG_FONT);
  hal->setColor(active ? VGA_WHITE : VGA_BLACK);
  hal->setBackColor(active ? VGA_BLUE : VGA_WHITE);
  
  int fontWidth = hal->getFontWidth(SCREEN_BIG_FONT);
  int fontHeight = hal->getFontHeight(SCREEN_BIG_FONT);

  String strToDraw;
  strToDraw = F("< НАЗАД");
  
  int len = hal->print(strToDraw.c_str(),0,0,0,true);
  hal->print(strToDraw.c_str(),(screenWidth - fontWidth*len)/2,top + (BUTTON_HEIGHT-fontHeight)/2);  
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void drawStepperStatus(HalDC* hal,bool active)
{
  hal->setFont(Various_Symbols_32x32);
  int fontWidth = hal->getFontWidth(Various_Symbols_32x32);
  int screenWidth = hal->getScreenWidth();
  int offset = 5;
  
  if(active)
  {
    hal->setBackColor(SCREEN_BACK_COLOR);
    hal->setColor(VGA_BLUE);
    hal->print("_",screenWidth-fontWidth-offset,offset);
  }
  else
  {
    hal->setColor(SCREEN_BACK_COLOR);
    int left = screenWidth-fontWidth-offset;
    hal->fillRect(left,offset,left+fontWidth,offset+fontWidth);
  }
    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setButtonActive(UTFT_Buttons_Rus* bb, int bID)
{
  bb->setButtonBackColor(bID, VGA_BLUE);
  bb->setButtonFontColor(bID, VGA_WHITE);  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setButtonInactive(UTFT_Buttons_Rus* bb, int bID)
{
  bb->setButtonBackColor(bID, VGA_WHITE);
  bb->setButtonFontColor(bID, VGA_BLACK);  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MainScreen::MainScreen() : AbstractHALScreen()
{
  StartScreen = this;
  buttons = new UTFT_Buttons_Rus(Screen.getUTFT());
  buttons->setTextFont(SCREEN_BIG_FONT);
  buttons->setButtonColors(BUTTON_COLORS);

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MainScreen::~MainScreen()
{
  delete buttons;  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainScreen::onDeactivate()
{
  // станем неактивными
  Settings.setLastSelectedMenu(lastActiveButton);
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainScreen::onActivate()
{
  // мы активизируемся
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainScreen::onEvent(Event event, void* param)
{
  if(!isActive())
    return;  

    switch(event)
    {
      case StepsRequested: // запросили шагать на определённое кол-во шагов
      {
        
      }
      break; // StepsRequested
      
      case StepperWorkDone: // движок остановился
      {
        
      }
      break; // StepperWorkDone

      case RotationRequested: // запросили вращение
      {
        
      }
      break; // RotationRequested

      case ButtonStateChanged: // изменилось состояние кнопки
      {
        
      }
      break; // ButtonStateChanged
    
      case EncoderPositionChanged: // смена позиции энкодера
      {
        int changes = *((int*) param);
        if(changes != 0)
        {
          int requested = lastActiveButton;
          requested += changes;
          if(requested < 0)
            requested = buttonList.size()-1;

          if(requested >= buttonList.size())
            requested = 0;

          setButtonInactive(buttons,lastActiveButton);

          buttons->drawButton(lastActiveButton);

          lastActiveButton = requested;
          setButtonActive(buttons,lastActiveButton);
            
          buttons->drawButton(lastActiveButton);

          Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.
        }
      }
      break; // EncoderPositionChanged

      case EncoderButtonClicked: // кликнута кнопка энкодера
      {
        switch(lastActiveButton)
        {
          case DIVIDE_DEGREE_BUTTON:
          {
            DBGLN(F("Divide by degrees screen!"));     
          }
          break;

          case DIVIDE_PARTS_BUTTON:
          {
            DBGLN(F("Divide by parts screen!"));     
          }
          break;

          case STEP_BUTTON:
          {
            DBGLN(F("Switch to steps screen!"));
            Screen.switchToScreen(Steps);     
          }
          break;

          case ROTATION_BUTTON:
          {
            DBGLN(F("Switch to rotation screen!"));
            Screen.switchToScreen(Rotation);     
          }
          break;

          case SETTINGS_BUTTON:
          {
            DBGLN(F("Settings screen!"));     
          }
          break;

        } // switch

        Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.

      }
      break; // EncoderButtonClicked
      
    } // switch    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainScreen::doSetup(HalDC* hal)
{
  int screenWidth = hal->getScreenWidth();
  int buttonWidth = screenWidth - BUTTON_X_OFFSET*2;

  int top = BUTTON_Y_OFFSET;
  int btn = buttons->addButton(BUTTON_X_OFFSET,top,buttonWidth,BUTTON_HEIGHT,"ДЕЛЕНИЕ ПО ГРАДУСАМ");

  buttonList.push_back(btn);

  top += BUTTON_HEIGHT + BUTTON_Y_OFFSET;
  btn = buttons->addButton(BUTTON_X_OFFSET,top,buttonWidth,BUTTON_HEIGHT,"ДЕЛЕНИЕ ПО ЧАСТЯМ");

  buttonList.push_back(btn);

  top += BUTTON_HEIGHT + BUTTON_Y_OFFSET;
  btn = buttons->addButton(BUTTON_X_OFFSET,top,buttonWidth,BUTTON_HEIGHT,"ШАГАНИЕ");

  buttonList.push_back(btn);

  top += BUTTON_HEIGHT + BUTTON_Y_OFFSET;
  btn = buttons->addButton(BUTTON_X_OFFSET,top,buttonWidth,BUTTON_HEIGHT,"ВРАЩЕНИЕ");

  buttonList.push_back(btn);

  top += BUTTON_HEIGHT + BUTTON_Y_OFFSET;
  btn = buttons->addButton(BUTTON_X_OFFSET,top,buttonWidth,BUTTON_HEIGHT,"НАСТРОЙКИ");

  buttonList.push_back(btn);

  lastActiveButton = Settings.getLastSelectedMenu();
  setButtonActive(buttons,lastActiveButton);

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainScreen::doUpdate(HalDC* hal)
{
  if(!isActive())
    return;

}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainScreen::drawGUI(HalDC* hal)
{
  if(!isActive())
  return;
  
  buttons->drawButtons();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainScreen::doDraw(HalDC* hal)
{
   hal->clearScreen();

   // тут отрисовка текущего состояния
   drawGUI(hal);

   hal->updateDisplay();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// SplashScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SplashScreen* Splash = NULL;        
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SplashScreen::SplashScreen() : AbstractHALScreen()
{
  Splash = this;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SplashScreen::onDeactivate()
{
  // станем неактивными
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SplashScreen::onActivate()
{
  // мы активизируемся
  showTime = 0;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SplashScreen::doSetup(HalDC* hal)
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SplashScreen::doUpdate(HalDC* hal)
{
  if(!isActive())
    return;

  if(showTime && (millis() - showTime > 3000))
  {
    DBGLN(F("Splash done, switch to main screen!"));
    Screen.switchToScreen(StartScreen);
  }
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SplashScreen::doDraw(HalDC* hal)
{
   hal->clearScreen();

   // тут отрисовка текущего состояния
   DBGLN(F("Show splash screen!"));

   extern unsigned short splash[];
   int splashWidth = 100;
   int splashHeight = 83;
   int screenWidth = hal->getScreenWidth();
   
   hal->drawBitmap((screenWidth - splashWidth)/2, 10, splashWidth, splashHeight, (unsigned int*)splash);

   hal->setFont(SCREEN_BIG_FONT);
   int fontWidth = hal->getFontWidth(SCREEN_BIG_FONT);
   int fontHeight = hal->getFontHeight(SCREEN_BIG_FONT);
   hal->setBackColor(VGA_WHITE);
   hal->setColor(VGA_RED);

   int top = splashHeight + 20;
   
   String strToDraw = F("ДЕЛИТЕЛЬНАЯ ГОЛОВКА");
   int len = hal->print(strToDraw.c_str(),0,0,0,true);
   hal->print(strToDraw.c_str(),(screenWidth - fontWidth*len)/2,top);

    hal->setColor(VGA_BLACK);

   top += fontHeight + 10;
   strToDraw = F("v. 0.1a");
   len = hal->print(strToDraw.c_str(),0,0,0,true);
   hal->print(strToDraw.c_str(),(screenWidth - fontWidth*len)/2,top);

   top += fontHeight*2 + 10;
   strToDraw = F("(c) Порохня Дмитрий");
   len = hal->print(strToDraw.c_str(),0,0,0,true);
   hal->print(strToDraw.c_str(),(screenWidth - fontWidth*len)/2,top);

   top += fontHeight + 10;
   strToDraw = F("2018-2019");
   len = hal->print(strToDraw.c_str(),0,0,0,true);
   hal->print(strToDraw.c_str(),(screenWidth - fontWidth*len)/2,top);

   hal->setColor(VGA_BLUE);
   top += fontHeight*2 + 10;
   strToDraw = F("spywarrior@gmail.com");
   len = hal->print(strToDraw.c_str(),0,0,0,true);
   hal->print(strToDraw.c_str(),(screenWidth - fontWidth*len)/2,top);

   /*
   hal->setColor(VGA_RED);
   hal->setFont(SevenSegNumFontMDS);
   hal->print("1234",5,5);
   */
   
   hal->updateDisplay();

   showTime = millis();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// RotationScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
RotationScreen* Rotation = NULL;        
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
RotationScreen::RotationScreen() : AbstractHALScreen()
{
  Rotation = this;
  wantRedrawRotationSpeed = false;
  lastRotationSpeedLength = 0;
  isInWork = false;
  ccwButtonPressed = false;
  cwButtonPressed = false;
  speedSelected = true;
  wantDrawStepperStatus = false;

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
RotationScreen::~RotationScreen()
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RotationScreen::onDeactivate()
{
  // станем неактивными
  Settings.setRotationSpeed(rotationSpeed);
  wantRedrawRotationSpeed = false;
  wantRedrawBackButton = false;
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RotationScreen::onActivate()
{
  // мы активизируемся
  rotationSpeed = Settings.getRotationSpeed();
  speedSelected = true;
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RotationScreen::startRotate(bool ccw)
{
  if(Settings.getRotationSpeed() != rotationSpeed)
    Settings.setRotationSpeed(rotationSpeed);
    
  isInWork = true;

  RotationEventParam rr;
  rr.start = true;
  rr.ccw = ccw;
  rr.speed = rotationSpeed;
  
  #ifdef _DEBUG
  if(ccw)
  {
    DBGLN(F("START ROTATE CCW !"));
  }
  else
  {
    DBGLN(F("START ROTATE CW !"));
    
  }
  #endif

  Events.raise(RotationRequested, &rr);
  wantDrawStepperStatus = true;

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RotationScreen::stopRotate(bool ccw)
{
  isInWork = false;    

  RotationEventParam rr;
  rr.start = false;
  rr.ccw = ccw;
  rr.speed = 0;  

  #ifdef _DEBUG
  if(ccw)
  {
    DBGLN(F("STOP ROTATE CCW !"));
  }
  else
  {
    DBGLN(F("STOP ROTATE CW !"));
    
  }
  #endif

  Events.raise(RotationRequested, &rr);
  wantDrawStepperStatus = true;

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RotationScreen::onEvent(Event event, void* param)
{
  if(!isActive())
    return;  

    switch(event)
    {
      case StepsRequested: // запросили шагать на определённое кол-во шагов
      {
        
      }
      break; // StepsRequested
      
      case StepperWorkDone: // движок остановился
      {
        
      }
      break; // StepperWorkDone

      case RotationRequested: // запросили вращение
      {
        
      }
      break; // RotationRequested
            
      case EncoderPositionChanged: // смена позиции энкодера
      {
        int changes = *((int*) param);
        if(changes != 0)
        {
            Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.
            if(speedSelected)
            {
              rotationSpeed += changes;
              
              if(rotationSpeed < 1)
                rotationSpeed = 1;
              if(rotationSpeed > 100)
                rotationSpeed = 100;
                
              wantRedrawRotationSpeed = true;
            }
            else
            {
              // переключаемся на стартовый экран
              DBGLN(F("Back to main screen!"));
              Screen.switchToScreen(StartScreen);              
            }
        }
      }
      break; // EncoderPositionChanged

      case EncoderButtonClicked: // кликнута кнопка энкодера
      {
        speedSelected = !speedSelected;
        wantRedrawBackButton = true;
        wantRedrawRotationSpeed = true;
        
        Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.
      }
      break; // EncoderButtonClicked


      case ButtonStateChanged: // состояние какой-либо кнопки изменилось
      {
        Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.
        ButtonEventParam* p = (ButtonEventParam*) param;
        
        switch(p->button)
        {
          case LEFT_BUTTON:
          {
            if(!cwButtonPressed)
            {
                bool isPressedNow = p->state & BUTTON_PRESSED;
                if(isPressedNow)
                {
                 ccwButtonPressed = true;
                 startRotate(true);
                }
                else
                {              
                  if(p->state & BUTTON_CLICKED)
                  {
                   ccwButtonPressed = false;
                   stopRotate(true);
                  }
                }
            }
          }
          break;

          case RIGHT_BUTTON:
          {
            if(!ccwButtonPressed)
            {
                bool isPressedNow = p->state & BUTTON_PRESSED;
                if(isPressedNow)
                {
                 cwButtonPressed = true;
                 startRotate(false);
                }
                else
                {              
                  if(p->state & BUTTON_CLICKED)
                  {
                   cwButtonPressed = false;
                   stopRotate(false);
                  }
                }
            }
          }
          break;
          
        } // switch
      }
      break; // ButtonStateChanged
      
    } // switch    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RotationScreen::doSetup(HalDC* hal)
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RotationScreen::doUpdate(HalDC* hal)
{
  if(!isActive())
    return;

    if(isInWork)
       Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.

    if(wantRedrawRotationSpeed)
    {
      wantRedrawRotationSpeed = false;
      drawRotationSpeed(hal,80);
    } // if

    if(wantRedrawBackButton)
    {
      wantRedrawBackButton = false;
      drawBackButton(hal,!speedSelected);
    }

    if(wantDrawStepperStatus)
    {
      wantDrawStepperStatus = false;
      drawStepperStatus(hal,isInWork);
    }

}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RotationScreen::doDraw(HalDC* hal)
{
   hal->clearScreen();

   // тут отрисовка текущего состояния
   drawGUI(hal);

   hal->updateDisplay();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RotationScreen::drawGUI(HalDC* hal)
{
  int screenWidth = hal->getScreenWidth();
   
   hal->setFont(SCREEN_BIG_FONT);
   int fontWidth = hal->getFontWidth(SCREEN_BIG_FONT);
   int fontHeight = hal->getFontHeight(SCREEN_BIG_FONT);
   hal->setBackColor(SCREEN_BACK_COLOR);
   hal->setColor(SCREEN_TEXT_COLOR);  

  int top = 20;
  int vSpacing = 10;
   
   String strToDraw = F("СКОРОСТЬ ВРАЩЕНИЯ");
   int len = hal->print(strToDraw.c_str(),0,0,0,true);
   int left = (screenWidth - fontWidth*len)/2;
   hal->print(strToDraw.c_str(),left,top);

   top =  drawRotationSpeed(hal,80);
   top += 20;

   hal->setFont(SCREEN_BIG_FONT);
   fontWidth = hal->getFontWidth(SCREEN_BIG_FONT);
   fontHeight = hal->getFontHeight(SCREEN_BIG_FONT);
   hal->setBackColor(SCREEN_BACK_COLOR);
   hal->setColor(SCREEN_TEXT_COLOR);  


   strToDraw = F("Зажатие < и > - работа.");
   len = hal->print(strToDraw.c_str(),0,0,0,true);
   left = (screenWidth - fontWidth*len)/2;
   hal->print(strToDraw.c_str(),left,top);
   top += fontHeight + vSpacing;

   strToDraw = F("Отпускание < и > - стоп.");
   len = hal->print(strToDraw.c_str(),0,0,0,true);
   left = (screenWidth - fontWidth*len)/2;
   hal->print(strToDraw.c_str(),left,top);


   drawBackButton(hal,!speedSelected);
   drawStepperStatus(hal,isInWork);
   
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int RotationScreen::drawRotationSpeed(HalDC* hal, int top)
{
   int screenWidth = hal->getScreenWidth();
   hal->setFont(SevenSeg_XXXL_Num);
   int fontWidth = hal->getFontWidth(SevenSeg_XXXL_Num);
   int fontHeight = hal->getFontHeight(SevenSeg_XXXL_Num);

    
   String strToDraw;
   strToDraw = rotationSpeed;
   int len = hal->print(strToDraw.c_str(),0,0,0,true);
   int left = (screenWidth - fontWidth*len)/2;

   if(lastRotationSpeedLength && lastRotationSpeedLength != len)
   {
    hal->setColor(speedSelected ? SCREEN_BACK_COLOR : VGA_BLUE);
    hal->fillRect(left - fontWidth, top, left + fontWidth*4,top+fontHeight);
   }

   lastRotationSpeedLength = len;
   
   hal->setColor(speedSelected? SCREEN_BACK_COLOR : VGA_BLUE);
   hal->setBackColor(speedSelected? VGA_BLUE : SCREEN_BACK_COLOR);
   hal->print(strToDraw.c_str(),left,top);   

   return (top + fontHeight + 10);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// StepsScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
StepsScreen* Steps = NULL;        
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
StepsScreen::StepsScreen() : AbstractHALScreen()
{
  Steps = this;
  wantRedrawRotationSpeed = false;
  lastRotationSpeedLength = 0;
  lastStepsLength = 0;
  isInWork = false;
  wantDrawStepperStatus = false;
  wantRedrawSteps = false;
  steps = 200;

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
StepsScreen::~StepsScreen()
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void StepsScreen::onDeactivate()
{
  // станем неактивными
  Settings.setRotationSpeed(rotationSpeed);
  Settings.setSteps(steps);
  wantRedrawRotationSpeed = false;
  wantRedrawBackButton = false;
  wantRedrawSteps = false;
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void StepsScreen::onActivate()
{
  // мы активизируемся
  rotationSpeed = Settings.getRotationSpeed();
  steps = Settings.getSteps();
  selectedMenu = 0; // выбираем кол-во шагов
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void StepsScreen::onEvent(Event event, void* param)
{
  if(!isActive())
    return;  

    switch(event)
    {
      case StepsRequested: // запросили шагать на определённое кол-во шагов
      {
        
      }
      break; // StepsRequested
      
      case StepperWorkDone: // движок остановился
      {
        #ifdef _DEBUG
          int32_t thisSteps = steps;
          thisSteps -= MotorController.getRemainingSteps();
          DBG(thisSteps);
          DBGLN(F(" STEPS DONE!"));
        #endif
        
        if(isInWork)
        {
          isInWork = false;
          wantDrawStepperStatus = true;
        }
      }
      break; // StepperWorkDone

      case RotationRequested: // запросили вращение
      {
        
      }
      break; // RotationRequested
            
      case EncoderPositionChanged: // смена позиции энкодера
      {
        int changes = *((int*) param);
        if(changes != 0)
        {
            Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.
            switch(selectedMenu)
            {
              case 0: // выбрано кол-во шагов
              {
                steps += changes;
                if(steps < 1)
                  steps = 1;
                  
                if(steps > 999)
                  steps = 999;

                wantRedrawSteps = true;
              }
              break;

              case 1: // выбрана скорость
              {
                 rotationSpeed += changes;
              
                if(rotationSpeed < 1)
                  rotationSpeed = 1;
                if(rotationSpeed > 100)
                  rotationSpeed = 100;
                  
                wantRedrawRotationSpeed = true;
              }
              break;

              case 2: // выбрана кнопка "назад"
              {
                // переключаемся на стартовый экран
                DBGLN(F("Back to main screen!"));
                Screen.switchToScreen(StartScreen);    
              }
              break;
              
            } // switch
            
        }
      }
      break; // EncoderPositionChanged

      case EncoderButtonClicked: // кликнута кнопка энкодера
      {

        int8_t lastSelMenu = selectedMenu;
        selectedMenu++;
        if(selectedMenu > 2)
          selectedMenu = 0;

          switch(lastSelMenu)
          {
            case 0: wantRedrawSteps = true; break;
            case 1: wantRedrawRotationSpeed = true; break;
            case 2: wantRedrawBackButton = true; break;
          }

          switch(selectedMenu)
          {
            case 0: wantRedrawSteps = true; break;
            case 1: wantRedrawRotationSpeed = true; break;
            case 2: wantRedrawBackButton = true; break;
          }
        
        Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.
      }
      break; // EncoderButtonClicked


      case ButtonStateChanged: // состояние какой-либо кнопки изменилось
      {
        Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.
        ButtonEventParam* p = (ButtonEventParam*) param;

        if(p->state & BUTTON_CLICKED)
        {
          // кнопка кликнута
          if(isInWork)
          {
            // работаем ещё, надо остановить мотор
            DBGLN(F("Stop motor prematurely!!!"));
            stopSteps(p->button == LEFT_BUTTON);
          }
          else              
          {
            // режим простоя, запускаем шагание
            startSteps(p->button == LEFT_BUTTON);
          }
          
        } // if(p->state & BUTTON_CLICKED)   
        
        
      }
      break; // ButtonStateChanged
      
    } // switch    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void StepsScreen::doSetup(HalDC* hal)
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void StepsScreen::doUpdate(HalDC* hal)
{
  if(!isActive())
    return;

    if(isInWork)
       Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.

    if(wantRedrawRotationSpeed)
    {
      wantRedrawRotationSpeed = false;
      drawRotationSpeed(hal,80);
    } // if

    if(wantRedrawSteps)
    {
      wantRedrawSteps = false;
      drawSteps(hal,80);
    }

    if(wantRedrawBackButton)
    {
      wantRedrawBackButton = false;
      drawBackButton(hal,selectedMenu == 2);
    }

    if(wantDrawStepperStatus)
    {
      wantDrawStepperStatus = false;
      drawStepperStatus(hal,isInWork);
    }

}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void StepsScreen::doDraw(HalDC* hal)
{
   hal->clearScreen();

   // тут отрисовка текущего состояния
   drawGUI(hal);

   hal->updateDisplay();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void StepsScreen::drawGUI(HalDC* hal)
{
  int screenWidth = hal->getScreenWidth();
   
   hal->setFont(SCREEN_BIG_FONT);
   int fontWidth = hal->getFontWidth(SCREEN_BIG_FONT);
   int fontHeight = hal->getFontHeight(SCREEN_BIG_FONT);
   hal->setBackColor(SCREEN_BACK_COLOR);
   hal->setColor(SCREEN_TEXT_COLOR);  

  int top = 20;
  int vSpacing = 10;
   
   String strToDraw = F("РЕЖИМ ШАГАНИЯ");
   int len = hal->print(strToDraw.c_str(),0,0,0,true);
   int left = (screenWidth - fontWidth*len)/2;
   hal->print(strToDraw.c_str(),left,top);


   hal->setColor(VGA_RED);
   strToDraw = F("шаги");
   hal->print(strToDraw.c_str(),40,80-fontHeight-6);
   strToDraw = F("скорость");
   len = hal->print(strToDraw.c_str(),0,0,0,true);
   hal->print(strToDraw.c_str(),screenWidth - 40 - len*fontWidth,80-fontHeight-6);

   drawSteps(hal,80);
   top =  drawRotationSpeed(hal,80);
   top += 20;

   hal->setFont(SCREEN_BIG_FONT);
   fontWidth = hal->getFontWidth(SCREEN_BIG_FONT);
   fontHeight = hal->getFontHeight(SCREEN_BIG_FONT);
   hal->setBackColor(SCREEN_BACK_COLOR);
   hal->setColor(SCREEN_TEXT_COLOR);  


   strToDraw = F("Клик < и > - работа.");
   len = hal->print(strToDraw.c_str(),0,0,0,true);
   left = (screenWidth - fontWidth*len)/2;
   hal->print(strToDraw.c_str(),left,top);
   top += fontHeight + vSpacing;

   strToDraw = F("Повторно < и > - стоп.");
   len = hal->print(strToDraw.c_str(),0,0,0,true);
   left = (screenWidth - fontWidth*len)/2;
   hal->print(strToDraw.c_str(),left,top);


   drawBackButton(hal,selectedMenu == 2);
   drawStepperStatus(hal,isInWork);
   
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int StepsScreen::drawRotationSpeed(HalDC* hal, int top)
{
   int screenWidth = hal->getScreenWidth();
   hal->setFont(SevenSeg_XXXL_Num);
   int fontWidth = hal->getFontWidth(SevenSeg_XXXL_Num);
   int fontHeight = hal->getFontHeight(SevenSeg_XXXL_Num);

    
   String strToDraw;
   strToDraw = rotationSpeed;
   int len = hal->print(strToDraw.c_str(),0,0,0,true);
   int left = (screenWidth - fontWidth*len) - 40;

   if(lastRotationSpeedLength && lastRotationSpeedLength != len)
   {
      if(len < lastRotationSpeedLength) // уменьшили разрядность числа
      {
        hal->setColor(selectedMenu == 1 ? SCREEN_BACK_COLOR : VGA_BLUE);
        int offset = left-fontWidth*(lastRotationSpeedLength-len);
        hal->fillRect(offset, top, offset + fontWidth,top+fontHeight);
      }
   }

   lastRotationSpeedLength = len;
   
   hal->setColor(selectedMenu == 1 ? SCREEN_BACK_COLOR : VGA_BLUE);
   hal->setBackColor(selectedMenu == 1 ? VGA_BLUE : SCREEN_BACK_COLOR);
   hal->print(strToDraw.c_str(),left,top);   

   return (top + fontHeight + 10);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void StepsScreen::drawSteps(HalDC* hal, int top)
{
   hal->setFont(SevenSeg_XXXL_Num);
   int fontWidth = hal->getFontWidth(SevenSeg_XXXL_Num);
   int fontHeight = hal->getFontHeight(SevenSeg_XXXL_Num);

    
   String strToDraw;
   strToDraw = steps;
   int len = hal->print(strToDraw.c_str(),0,0,0,true);
   int left = 40;

   if(lastStepsLength && lastStepsLength != len)
   {    
      if(len < lastStepsLength) // уменьшили разрядность числа
      {
        hal->setColor(selectedMenu == 0 ? SCREEN_BACK_COLOR : VGA_BLUE);
        int offset = left+fontWidth*(3 - (lastStepsLength-len));
        hal->fillRect(offset, top, offset + fontWidth,top+fontHeight);
      }
   }

   lastStepsLength = len;
   
   hal->setColor(selectedMenu == 0 ? SCREEN_BACK_COLOR : VGA_BLUE);
   hal->setBackColor(selectedMenu == 0 ? VGA_BLUE : SCREEN_BACK_COLOR);
   hal->print(strToDraw.c_str(),left,top);   

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void StepsScreen::startSteps(bool ccw)
{
  if(Settings.getRotationSpeed() != rotationSpeed)
    Settings.setRotationSpeed(rotationSpeed);

  if(Settings.getSteps() != steps)
    Settings.setSteps(steps);
    
  isInWork = true;

  StepsEventParam rr;
  rr.start = true;
  rr.ccw = ccw;
  rr.speed = rotationSpeed;
  rr.steps = steps;
  
  #ifdef _DEBUG
  if(ccw)
  {
    DBGLN(F("START STEPS CCW !"));
  }
  else
  {
    DBGLN(F("START STEPS CW !"));
    
  }
  #endif

  Events.raise(StepsRequested, &rr);
  wantDrawStepperStatus = true;

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void StepsScreen::stopSteps(bool ccw)
{
  if(!isInWork)
    return;
    
  isInWork = false;    

  StepsEventParam rr;
  rr.start = false;
  rr.ccw = ccw;
  rr.speed = 0;
  rr.steps = 0;  

  #ifdef _DEBUG
  if(ccw)
  {
    DBGLN(F("STOP STEPS CCW !"));
  }
  else
  {
    DBGLN(F("STOP STEPS CW !"));
    
  }
  #endif

  Events.raise(StepsRequested, &rr);
  wantDrawStepperStatus = true;

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------


