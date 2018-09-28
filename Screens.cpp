//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Screens.h"
#include "CONFIG.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MainScreen* StartScreen = NULL;        
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MainScreen::MainScreen() : AbstractHALScreen()
{
  StartScreen = this;
  buttons = new UTFT_Buttons_Rus(Screen.getUTFT());
  buttons->setTextFont(SCREEN_BIG_FONT);
  buttons->setButtonColors(VGA_BLACK, VGA_GRAY,VGA_SILVER,VGA_GRAY,VGA_WHITE);

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
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainScreen::doSetup(HalDC* hal)
{
  int screenWidth = hal->getScreenWidth();
  int buttonXOffset = 10;
  int buttonYOffset = 10;
  int buttonWidth = screenWidth - buttonXOffset*2;
  int buttonHeight = 40;

  int top = buttonYOffset;
  divideDegreeButton = buttons->addButton(buttonXOffset,top,buttonWidth,buttonHeight,"ДЕЛЕНИЕ ПО ГРАДУСАМ");
  lastActiveButton = divideDegreeButton;

  buttons->setButtonBackColor(divideDegreeButton, VGA_BLUE);
  buttons->setButtonFontColor(divideDegreeButton, VGA_WHITE);  

  top += buttonHeight + buttonYOffset;
  dividePartsButton = buttons->addButton(buttonXOffset,top,buttonWidth,buttonHeight,"ДЕЛЕНИЕ ПО ЧАСТЯМ");

  top += buttonHeight + buttonYOffset;
  jogButton = buttons->addButton(buttonXOffset,top,buttonWidth,buttonHeight,"ШАГАНИЕ");

  top += buttonHeight + buttonYOffset;
  rotationButton = buttons->addButton(buttonXOffset,top,buttonWidth,buttonHeight,"ВРАЩЕНИЕ");

  top += buttonHeight + buttonYOffset;
  settingsButton = buttons->addButton(buttonXOffset,top,buttonWidth,buttonHeight,"НАСТРОЙКИ");

  Events.subscribe(this);

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainScreen::doUpdate(HalDC* hal)
{
  if(!isActive())
    return;

}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainScreen::drawGUI()
{
  buttons->drawButtons();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainScreen::doDraw(HalDC* hal)
{
   hal->clearScreen();

   // тут отрисовка текущего состояния
   drawGUI();

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
   hal->fillScreen(VGA_WHITE);

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

