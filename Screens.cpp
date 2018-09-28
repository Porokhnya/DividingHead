//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Screens.h"
#include "CONFIG.h"
#include "Settings.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MainScreen* StartScreen = NULL;        
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
            DBGLN(F("Step screen!"));     
          }
          break;

          case ROTATION_BUTTON:
          {
            DBGLN(F("Rotation screen!"));
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

  if(showTime && (millis() - showTime > 5000))
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
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RotationScreen::onActivate()
{
  // мы активизируемся
  rotationSpeed = Settings.getRotationSpeed();
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RotationScreen::onEvent(Event event, void* param)
{
  if(!isActive())
    return;  

    switch(event)
    {
      case EncoderPositionChanged: // смена позиции энкодера
      {
        int changes = *((int*) param);
        if(changes != 0)
        {
            Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.
            rotationSpeed += changes;
            
            if(rotationSpeed < 1)
              rotationSpeed = 1;
            if(rotationSpeed > 100)
              rotationSpeed = 100;
              
            wantRedrawRotationSpeed = true;
        }
      }
      break; // EncoderPositionChanged

      case EncoderButtonClicked: // кликнута кнопка энкодера
      {
        // переключаемся на стартовый экран
        DBGLN(F("Back to main screen!"));
        Screen.switchToScreen(StartScreen);
        Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.
      }
      break; // EncoderButtonClicked
      
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

    if(wantRedrawRotationSpeed)
    {
      wantRedrawRotationSpeed = false;
      drawRotationSpeed(hal,50);
    } // if

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
   
   String strToDraw = F("СКОРОСТЬ ВРАЩЕНИЯ:");
   int len = hal->print(strToDraw.c_str(),0,0,0,true);
   int left = (screenWidth - fontWidth*len)/2;
   hal->print(strToDraw.c_str(),left,top);

   top += fontHeight + vSpacing;
   top += drawRotationSpeed(hal,50);


   
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
    hal->setColor(SCREEN_BACK_COLOR);
    hal->fillRect(left - fontWidth, top, left + fontWidth*4,top+fontHeight);
   }

   lastRotationSpeedLength = len;
   
   hal->setColor(VGA_BLUE);
   hal->print(strToDraw.c_str(),left,top);   

   return fontHeight + 10;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------


