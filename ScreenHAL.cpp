#include "ScreenHAL.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#if DISPLAY_USED == DISPLAY_TFT
const uint8_t utf8_rus_charmap[] PROGMEM = {'A',128,'B',129,130,'E',131,132,133,134,135,'K',136,'M','H','O',137,'P','C','T',138,139,'X',140,141,
142,143,144,145,146,147,148,149,'a',150,151,152,153,'e',154,155,156,157,158,159,160,161,162,'o',163,'p','c',164,'y',165,'x',166,167,168,169,170,
171,172,173,174,175};
#endif
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AbstractHALScreen::AbstractHALScreen()
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AbstractHALScreen::~AbstractHALScreen()
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AbstractHALScreen::setup(HalDC* hal)
{
  // тут общие для всех классов настройки
  doSetup(hal); 
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AbstractHALScreen::update(HalDC* hal)
{
  if(isActive())
  {
      doUpdate(hal);
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AbstractHALScreen::draw(HalDC* hal)
{
  if(isActive())
  {
    doDraw(hal);    
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// HalDC
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HalDC::HalDC()
{
  currentScreenIndex = -1;
  requestedToActiveScreen = NULL;
  requestedToActiveScreenIndex = -1;
  on_action = NULL;
  halDCDescriptor = NULL;
  curFont = NULL;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HalDC::notifyAction(AbstractHALScreen* screen)
{
  if(on_action)
    on_action(screen);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HalDC::addScreen(AbstractHALScreen* screen)
{
  screen->setup(this);
  screens.push_back(screen);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HalDC::initHAL()
{
  //Тут инициализация/переинициализация дисплея
  #if DISPLAY_USED == DISPLAY_TFT

      halDCDescriptor->InitLCD(SCREEN_ORIENTATION);
    
      #if DISPLAY_INIT_DELAY > 0
        delay(DISPLAY_INIT_DELAY);
      #endif
      
      setBackColor(SCREEN_BACK_COLOR);
      setFont(SCREEN_SMALL_FONT);
            
	 #else
    #error "Unsupported display!"
  #endif  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HalDC::setup()
{
  //создание библиотеки для экрана

  #if DISPLAY_USED == DISPLAY_TFT  
    halDCDescriptor = new UTFT(TFT_MODEL,TFT_RS_PIN,TFT_WR_PIN,TFT_CS_PIN,TFT_RST_PIN);
  #else
    #error "Unsupported display!"
  #endif

 
  // инициализируем дисплей
  initHAL();

  // добавляем экран мессадж-бокса
  addScreen(MessageBoxScreen::create());

  Events.subscribe(this);
   
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HalDC::onEvent(Event event, void* param)
{
  AbstractHALScreen* a = getActiveScreen();
  if(a)
    a->onEvent(event,param);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HalDC::update()
{

  if(requestedToActiveScreen != NULL)
  {
    // попросили сделать экран активным
    AbstractHALScreen* screen = requestedToActiveScreen;
    currentScreenIndex = requestedToActiveScreenIndex;
    
    requestedToActiveScreen = NULL;
    
    screen->setActive(true);
    screen->onActivate();

    screen->update(this);
    screen->draw(this);
    
    return;
    
  } // if(requestedToActiveScreen != NULL)

  if(currentScreenIndex == -1) // ни разу не рисовали ещё ничего, исправляемся
  {
    if(screens.size())
     switchToScreen((unsigned int)0); // переключаемся на первый экран, если ещё ни разу не показали ничего     
  }

  if(currentScreenIndex == -1)
    return;

  // обновляем текущий экран
  AbstractHALScreen* currentScreen = screens[currentScreenIndex];
  currentScreen->update(this);
  
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AbstractHALScreen* HalDC::getActiveScreen()
{
  if(currentScreenIndex < 0 || !screens.size())
    return NULL;

  return screens[currentScreenIndex];
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HalDC::switchToScreen(AbstractHALScreen* screen)
{

  if(requestedToActiveScreen != NULL) // ждём переключения на новый экран
    return;
  
  if(currentScreenIndex > -1 && screens.size())
  {
     AbstractHALScreen* si = screens[currentScreenIndex];
     si->setActive(false);
     si->onDeactivate();
  }
  
  for(size_t i=0;i<screens.size();i++)
  {
    if(screens[i] == screen)
    {
      requestedToActiveScreen = screen;
      requestedToActiveScreenIndex = i;

      break;
    }
  }  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HalDC::switchToScreen(unsigned int screenIndex)
{
  if(screenIndex < screens.size())
      switchToScreen(screens[screenIndex]);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HalDC::drawBitmap(int x, int y, int w, int h, const unsigned int* bmp)
{
  #if DISPLAY_USED == DISPLAY_TFT
   halDCDescriptor->drawBitmap(x,y,w,h,bmp);
  #else
    #error "Unsupported display!"
  #endif    
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HalDC::setFont(FONT_TYPE* font)
{
  curFont = font;
  
  #if DISPLAY_USED == DISPLAY_TFT
   halDCDescriptor->setFont(font);
  #else
    #error "Unsupported display!"
  #endif    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
FONT_TYPE* HalDC::getFont()
{
  #if DISPLAY_USED == DISPLAY_TFT
    return halDCDescriptor->getFont();
  #else
    #error "Unsupported display!"
  #endif    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HalDC::clearScreen()
{
  #if DISPLAY_USED == DISPLAY_TFT
    halDCDescriptor->fillScr(SCREEN_BACK_COLOR);
  #else
    #error "Unsupported display!"
  #endif      
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HalDC::fillScreen(COLORTYPE color)
{
  #if DISPLAY_USED == DISPLAY_TFT
    halDCDescriptor->fillScr(color);
  #else
    #error "Unsupported display!"
  #endif    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HalDC::setBackColor(COLORTYPE color)
{
  #if DISPLAY_USED == DISPLAY_TFT
    halDCDescriptor->setBackColor(color);
  #else
    #error "Unsupported display!"
  #endif    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
COLORTYPE  HalDC::getBackColor()
{
  #if DISPLAY_USED == DISPLAY_TFT
    return halDCDescriptor->getBackColor();
  #else
    #error "Unsupported display!"
  #endif    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HalDC::setColor(COLORTYPE color)
{
  #if DISPLAY_USED == DISPLAY_TFT
    halDCDescriptor->setColor(color);
  #else
    #error "Unsupported display!"
  #endif    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
COLORTYPE  HalDC::getColor()
{
  #if DISPLAY_USED == DISPLAY_TFT
  return halDCDescriptor->getColor();
  #else
    #error "Unsupported display!"
  #endif  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void  HalDC::drawRect(int x1, int y1, int x2, int y2)
{
  #if DISPLAY_USED == DISPLAY_TFT
    halDCDescriptor->drawRect(x1,y1,x2,y2);
  #else
    #error "Unsupported display!"
  #endif    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void  HalDC::drawRoundRect(int x1, int y1, int x2, int y2)
{
  #if DISPLAY_USED == DISPLAY_TFT
    halDCDescriptor->drawRoundRect(x1,y1,x2,y2);
  #else
    #error "Unsupported display!"
  #endif    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void  HalDC::fillRect(int x1, int y1, int x2, int y2)
{
  #if DISPLAY_USED == DISPLAY_TFT
    halDCDescriptor->fillRect(x1,y1,x2,y2);
  #else
    #error "Unsupported display!"
  #endif    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void  HalDC::fillRoundRect(int x1, int y1, int x2, int y2)
{
  #if DISPLAY_USED == DISPLAY_TFT
    halDCDescriptor->fillRoundRect(x1,y1,x2,y2);
  #else
    #error "Unsupported display!"
  #endif    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void  HalDC::clrRoundRect(int x1, int y1, int x2, int y2)
{
  #if DISPLAY_USED == DISPLAY_TFT
    //halDCDescriptor->fillRoundRect(x1,y1,x2,y2);
  #else
    #error "Unsupported display!"
  #endif    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint16_t HalDC::getFontWidth(FONT_TYPE* font)
{
  #if DISPLAY_USED == DISPLAY_TFT    
    return READ_FONT_BYTE(0);
  #else
    #error "Unsupported display!"
  #endif    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint16_t HalDC::getFontHeight(FONT_TYPE* font)
{
  #if DISPLAY_USED == DISPLAY_TFT
    return READ_FONT_BYTE(1); 
  #else
    #error "Unsupported display!"
  #endif    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint16_t HalDC::getScreenWidth()
{
  #if DISPLAY_USED == DISPLAY_TFT
    return halDCDescriptor->getDisplayXSize(); 
  #else
    #error "Unsupported display!"
  #endif     
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint16_t HalDC::getScreenHeight()
{
  #if DISPLAY_USED == DISPLAY_TFT
    return halDCDescriptor->getDisplayYSize(); 
  #else
    #error "Unsupported display!"
  #endif     
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HalDC::updateDisplay()
{
  #if DISPLAY_USED == DISPLAY_TFT
  #else
    #error "Unsupported display!"
  #endif      
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int HalDC::print(const char* st,int x, int y, int deg, bool computeStringLengthOnly)
{
  #if DISPLAY_USED == DISPLAY_TFT
    return printTFT(st,x,y,deg,computeStringLengthOnly);
  #else
    #error "Unsupported display!"
  #endif    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#if DISPLAY_USED == DISPLAY_TFT
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int HalDC::printTFT(const char* st,int x, int y, int deg, bool computeStringLengthOnly)
{
    
 int stl, i;
  stl = strlen(st);

  if (halDCDescriptor->orient==PORTRAIT)
  {
    if (x==RIGHT) 
      x=(halDCDescriptor->disp_x_size+1)-(stl*halDCDescriptor->cfont.x_size);
  
    if (x==CENTER) 
      x=((halDCDescriptor->disp_x_size+1)-(stl*halDCDescriptor->cfont.x_size))/2;
  } 
  else 
  {
    if (x==RIGHT) 
      x=(halDCDescriptor->disp_y_size+1)-(stl*halDCDescriptor->cfont.x_size);
    
    if (x==CENTER) 
      x=((halDCDescriptor->disp_y_size+1)-(stl*halDCDescriptor->cfont.x_size))/2;
  }
  
  uint8_t utf_high_byte = 0;
  uint8_t ch, ch_pos = 0;
  
  for (i = 0; i < stl; i++) 
  {
    ch = st[i];
    
    if ( ch >= 128) 
    {
      if ( utf_high_byte == 0 && (ch ==0xD0 || ch == 0xD1)) 
      {
        utf_high_byte = ch;
        continue;
      } 
      else 
      {
        if ( utf_high_byte == 0xD0) 
        {
          if (ch == 0x81) 
          { //Ё
            ch = 6;
          } 
          else 
          {
            if(ch <= 0x95) 
            {
              ch -= 0x90;
            } 
            else if( ch < 0xB6)
            {
              ch -= (0x90 - 1);
            } 
            else 
            {
              ch -= (0x90 - 2);
            }
          }
          
          ch = pgm_read_byte((utf8_rus_charmap + ch));
        
        } 
        else if (utf_high_byte == 0xD1) 
        {
          if (ch == 0x91) 
          {//ё
            ch = 39;
          } 
          else 
          {
            ch -= 0x80;
            ch += 50;
          }
          
          ch = pgm_read_byte((utf8_rus_charmap + ch));
        }
        
        utf_high_byte = 0;
      }
    } 
    else 
    {
      utf_high_byte = 0;
    }
    

    if (deg==0) 
    {
      if(!computeStringLengthOnly)
        halDCDescriptor->printChar(ch, x + (ch_pos * (halDCDescriptor->cfont.x_size)), y);
    } 
    else 
    {
      if(!computeStringLengthOnly)
        halDCDescriptor->rotateChar(ch, x, y, ch_pos, deg);
    }
    ++ch_pos;
  } // for  

  return ch_pos;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#endif // DISPLAY_USED == DISPLAY_TFT
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HalDC Screen;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MessageBoxScreen* MessageBox;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MessageBoxScreen::MessageBoxScreen() : AbstractHALScreen()
{
  targetOkScreen = NULL;
  targetCancelScreen = NULL; 
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MessageBoxScreen::doSetup(HalDC* dc)
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MessageBoxScreen::doUpdate(HalDC* dc)
{
    // тут обновляем внутреннее состояние
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MessageBoxScreen::doDraw(HalDC* hal)
{
  hal->setFont(SCREEN_SMALL_FONT);
  
  uint8_t fontHeight = hal->getFontHeight(SCREEN_SMALL_FONT);
  uint8_t fontWidth = hal->getFontWidth(SCREEN_SMALL_FONT);
  int displayWidth = hal->getScreenWidth();
  int lineSpacing = 2; 
  int curX = 0;
  int curY = 0;
  
  for(size_t i=0;i<lines.size();i++)
  {
    int lineLength = hal->print(lines[i],curX,curY,0,true);
    curX = (displayWidth - lineLength*fontWidth)/2;    
    hal->print(lines[i],curX,curY);
    curY += fontHeight + lineSpacing;
  }

  hal->updateDisplay();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
void MessageBoxScreen::onButtonPressed(HalDC* hal, int pressedButton)
{
  switch(boxType)
  {
    case mbShow: // нажатие на любую кнопку ведёт к выходу из окна
      if(targetOkScreen)
        hal->switchToScreen(targetOkScreen);
    break; // mbShow

    case mbConfirm: // нажатие на кнопку 1 - ОК, нажатие на кнопку 2 - отмена
    {
      switch(pressedButton)
      {
        case BUTTON_1:
          if(targetOkScreen)
            hal->switchToScreen(targetOkScreen);
        break;

        case BUTTON_2:
          if(targetCancelScreen)
            hal->switchToScreen(targetCancelScreen);
        break;
        
      } // switch
    }
    break; // mbConfirm
    
  } // switch

}
*/
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MessageBoxScreen::show(Vector<const char*>& _lines, AbstractHALScreen* okTarget)
{
  lines = _lines;
  boxType = mbShow;
  targetOkScreen = okTarget;
  targetCancelScreen = NULL;

  Screen.switchToScreen(this);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MessageBoxScreen::confirm(Vector<const char*>& _lines, AbstractHALScreen* okTarget, AbstractHALScreen* cancelTarget)
{
  lines = _lines;
  boxType = mbConfirm;
  targetOkScreen = okTarget;
  targetCancelScreen = cancelTarget;

  Screen.switchToScreen(this);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AbstractHALScreen* MessageBoxScreen::create()
{
    MessageBox = new MessageBoxScreen();
    return MessageBox;  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

