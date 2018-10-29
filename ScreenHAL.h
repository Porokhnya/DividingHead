#ifndef _UTFTMENU_H
#define _UTFTMENU_H
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "CONFIG.h"
#include "CoreArray.h"
#include "Events.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class HalDC;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#if DISPLAY_USED == DISPLAY_TFT
#include <UTFT.h>
#else
  #error "Unsupported display!"
#endif  
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#if DISPLAY_USED == DISPLAY_TFT

  #if defined (__arm__)
    #define READ_FONT_BYTE(x) font[(x)]  
  #elif defined(__AVR__)  
    #define READ_FONT_BYTE(x) pgm_read_byte(&(font[(x)]))  
  #endif
  
#else
  #error "Unsupported display!"
#endif  
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#if DISPLAY_USED == DISPLAY_TFT

  typedef UTFT HalDCDescriptor;
  typedef uint16_t COLORTYPE;
  typedef uint8_t FONT_TYPE;
  extern FONT_TYPE BigRusFont[];               // какой шрифт используем
  extern FONT_TYPE SmallRusFont[];             // какой шрифт используем
  extern FONT_TYPE Various_Symbols_16x32[];    // какой шрифт используем
  extern FONT_TYPE Various_Symbols_32x32[];    // какой шрифт используем
  extern FONT_TYPE AdditionalLettersFont[];
  extern FONT_TYPE SevenSeg_XXXL_Num[];
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  #define BUTTON_HEIGHT 40
  #define BUTTON_X_OFFSET 10
  #define BUTTON_Y_OFFSET 10
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  #ifdef CONTRAST_COLOR_SCHEME
  
    #define SCREEN_BACK_COLOR VGA_BLACK          // цвет фона
    #define SCREEN_TEXT_COLOR VGA_WHITE          // цвет шрифта
    #define BUTTON_COLORS VGA_WHITE, VGA_GRAY,VGA_SILVER,VGA_GRAY,VGA_BLACK
    #define BUTTON_ACTIVE_FONT_COLOR VGA_WHITE
    #define BUTTON_ACTIVE_BACK_COLOR VGA_BLUE
    #define BUTTON_INACTIVE_FONT_COLOR VGA_WHITE
    #define BUTTON_INACTIVE_BACK_COLOR VGA_BLACK
    #define STEPPER_STATUS_COLOR VGA_YELLOW
    #define DIGITS_FONT_COLOR VGA_YELLOW
    #define DIGITS_LABEL_FONT_COLOR VGA_SILVER
    #define DIGITS_SELECTED_FONT_COLOR VGA_WHITE
    #define DIGITS_SELECTED_BACK_COLOR VGA_BLUE
    
  #else
    #define SCREEN_BACK_COLOR VGA_WHITE          // цвет фона
    #define SCREEN_TEXT_COLOR VGA_BLACK          // цвет шрифта  
    #define BUTTON_COLORS VGA_BLACK, VGA_GRAY,VGA_SILVER,VGA_GRAY,VGA_WHITE
    #define BUTTON_ACTIVE_FONT_COLOR VGA_WHITE
    #define BUTTON_ACTIVE_BACK_COLOR VGA_BLUE
    #define BUTTON_INACTIVE_FONT_COLOR VGA_BLACK
    #define BUTTON_INACTIVE_BACK_COLOR VGA_WHITE
    #define STEPPER_STATUS_COLOR VGA_RED
    #define DIGITS_FONT_COLOR VGA_BLUE
    #define DIGITS_LABEL_FONT_COLOR VGA_RED
    #define DIGITS_SELECTED_FONT_COLOR VGA_WHITE
    #define DIGITS_SELECTED_BACK_COLOR VGA_BLUE
    

  #endif
  #define SCREEN_SMALL_FONT SmallRusFont       // маленький шрифт
  #define SCREEN_BIG_FONT BigRusFont       // маленький шрифт
  #define SCREEN_SYMBOL_FONT Various_Symbols_16x32  // символьный шрифт
  #define SCREEN_ORIENTATION  LANDSCAPE         // ориентация экрана горизонтальная
      
#else
  #error "Unsupported display!"
#endif  
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// абстрактный класс экрана
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class AbstractHALScreen
{
  public:

    void setup(HalDC* hal);
    void update(HalDC* hal);
    void draw(HalDC* hal);

    bool isActive() {return isActiveScreen; }
    void setActive(bool val){ isActiveScreen = val; }
  
    AbstractHALScreen();
    virtual ~AbstractHALScreen();

    // вызывается, когда переключаются на экран
    virtual void onActivate(){}

    // вызывается, когда экран становится неактивным
    virtual void onDeactivate() {}
    
    virtual void onEvent(Event event, void* param) {};

  protected:
    
    virtual void doSetup(HalDC* hal) = 0;
    virtual void doUpdate(HalDC* hal) = 0;
    virtual void doDraw(HalDC* hal) = 0;

    private:
      bool isActiveScreen;
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef Vector<AbstractHALScreen*> HALScreensList; // список экранов
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// класс-менеджер работы с экраном
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef void (*OnScreenAction)(AbstractHALScreen* screen);
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class HalDC : public IEventSubscriber
{

public:
  HalDC();

  void setup();
  void update();
  void initHAL();

  virtual void onEvent(Event event, void* param);


  #if DISPLAY_USED == DISPLAY_TFT
    UTFT* getUTFT() {return halDCDescriptor; }
  #endif

  void addScreen(AbstractHALScreen* screen);

  AbstractHALScreen* getActiveScreen();
  void onAction(OnScreenAction handler) {on_action = handler;}

  // сообщает, что произведено какое-то действие на экране
  void notifyAction(AbstractHALScreen* screen);

  void switchToScreen(AbstractHALScreen* screen);
  void switchToScreen(unsigned int screenIndex);

  // HARDWARE HAL
  int print(const char* str,int x, int y, int deg=0, bool computeStringLengthOnly=false);

  void setFont(FONT_TYPE* font);
  FONT_TYPE* getFont();
  void setBackColor(COLORTYPE color);
  COLORTYPE  getBackColor();
  void setColor(COLORTYPE color);
  COLORTYPE  getColor();
  void fillScreen(COLORTYPE color);
  void  drawRect(int x1, int y1, int x2, int y2);
  void  drawRoundRect(int x1, int y1, int x2, int y2);
  void  fillRect(int x1, int y1, int x2, int y2);
  void  fillRoundRect(int x1, int y1, int x2, int y2);
  void clrRoundRect(int x1, int y1, int x2, int y2);
  uint16_t getFontWidth(FONT_TYPE* font);
  uint16_t getFontHeight(FONT_TYPE* font);

  void drawBitmap(int x, int y, int w, int h, unsigned int* bmp);

  uint16_t getScreenWidth();
  uint16_t getScreenHeight();

  void clearScreen();
  void updateDisplay();  

private:

#if DISPLAY_USED == DISPLAY_TFT
  int printTFT(const char* str,int x, int y, int deg=0, bool computeStringLengthOnly=false);
#endif


  AbstractHALScreen* requestedToActiveScreen;
  int requestedToActiveScreenIndex;

  OnScreenAction on_action;
  
  HALScreensList screens;
  HalDCDescriptor* halDCDescriptor;
  int currentScreenIndex;
  
  FONT_TYPE* curFont;

  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern HalDC Screen;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
  mbShow,
  mbConfirm
  
} MessageBoxType;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class MessageBoxScreen : public AbstractHALScreen
{
  public:

  static AbstractHALScreen* create();

  void confirm(Vector<const char*>& lines, AbstractHALScreen* okTarget, AbstractHALScreen* cancelTarget);
  void show(Vector<const char*>& lines, AbstractHALScreen* okTarget);
    
protected:

    virtual void doSetup(HalDC* dc);
    virtual void doUpdate(HalDC* dc);
    virtual void doDraw(HalDC* dc);

private:
      MessageBoxScreen();

      AbstractHALScreen* targetOkScreen;
      AbstractHALScreen* targetCancelScreen;
      Vector<const char*> lines;

      MessageBoxType boxType;
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern MessageBoxScreen* MessageBox;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#endif
