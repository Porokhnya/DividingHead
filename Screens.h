#pragma once
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "ScreenHAL.h"
#include "UTFT_Buttons_Rus.h"
#include "Events.h"
#include "CoreArray.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// главный экран
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define DIVIDE_DEGREE_BUTTON 0
#define DIVIDE_PARTS_BUTTON 1
#define STEP_BUTTON 2
#define ROTATION_BUTTON 3
#define SETTINGS_BUTTON 4
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class MainScreen : public AbstractHALScreen
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
    void drawGUI(HalDC* hal);

    Vector<uint8_t> buttonList;

    int lastActiveButton;

  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern MainScreen* StartScreen;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define MOTOR_SETUP_BUTTON 0
#define MICROSTEP_MODE_BUTTON 1
#define REDUCTION_BUTTON 2
#define BACK_FROM_TUNE_BUTTON 3
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class TuneScreen : public AbstractHALScreen
{
  public:

  static AbstractHALScreen* create()
  {
    return new TuneScreen();
  }
  
   virtual void onActivate();
   virtual void onDeactivate();

   virtual void onEvent(Event event, void* param);


protected:
  
    virtual void doSetup(HalDC* hal);
    virtual void doUpdate(HalDC* hal);
    virtual void doDraw(HalDC* hal);

private:
    TuneScreen();
    ~TuneScreen();


    UTFT_Buttons_Rus* buttons;
    void drawGUI(HalDC* hal);

    Vector<uint8_t> buttonList;

    int lastActiveButton;

  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern TuneScreen* Tune;
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
class RotationScreen : public AbstractHALScreen
{
  public:

  static AbstractHALScreen* create()
  {
    return new RotationScreen();
  }
  
   virtual void onActivate();
   virtual void onDeactivate();

   virtual void onEvent(Event event, void* param);


protected:
  
    virtual void doSetup(HalDC* hal);
    virtual void doUpdate(HalDC* hal);
    virtual void doDraw(HalDC* hal);

private:
    RotationScreen();
    ~RotationScreen();

    void validate();

     uint16_t rotationSpeed;
     bool wantRedrawRotationSpeed;
     int lastRotationSpeedLength;
     void drawGUI(HalDC* hal);
     
     bool isInWork, ccwButtonPressed, cwButtonPressed;
     void startRotate(bool ccw);
     void stopRotate(bool ccw);

     bool wantDrawStepperStatus;
     
     int drawRotationSpeed(HalDC* hal, int top);
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern RotationScreen* Rotation;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class MotorSetupScreen : public AbstractHALScreen
{
  public:

  static AbstractHALScreen* create()
  {
    return new MotorSetupScreen();
  }
  
   virtual void onActivate();
   virtual void onDeactivate();

   virtual void onEvent(Event event, void* param);


protected:
  
    virtual void doSetup(HalDC* hal);
    virtual void doUpdate(HalDC* hal);
    virtual void doDraw(HalDC* hal);

private:
    MotorSetupScreen();
    ~MotorSetupScreen();

     uint16_t stepsPerRevolution;
     bool wantRedrawStepsPerRevolution;
     int lastStepsPerRevolutionLength;
     void drawGUI(HalDC* hal);

     void validate();
     
     int drawStepsPerRevolution(HalDC* hal, int top);
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern MotorSetupScreen* MotorSetup;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class MicrostepScreen : public AbstractHALScreen
{
  public:

  static AbstractHALScreen* create()
  {
    return new MicrostepScreen();
  }
  
   virtual void onActivate();
   virtual void onDeactivate();

   virtual void onEvent(Event event, void* param);


protected:
  
    virtual void doSetup(HalDC* hal);
    virtual void doUpdate(HalDC* hal);
    virtual void doDraw(HalDC* hal);

private:
    MicrostepScreen();
    ~MicrostepScreen();

     uint16_t setting;
     bool wantRedrawSetting;
     int lastSettingLength;
     void drawGUI(HalDC* hal);
     
     int drawSetting(HalDC* hal, int top);

     void validate();
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern MicrostepScreen* Microstep;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class ReductionScreen : public AbstractHALScreen
{
  public:

  static AbstractHALScreen* create()
  {
    return new ReductionScreen();
  }
  
   virtual void onActivate();
   virtual void onDeactivate();

   virtual void onEvent(Event event, void* param);

protected:
  
    virtual void doSetup(HalDC* hal);
    virtual void doUpdate(HalDC* hal);
    virtual void doDraw(HalDC* hal);

private:
    ReductionScreen();
    ~ReductionScreen();

     int8_t selectedMenu;
     uint16_t reductionMotor,reductionGear;
     bool wantRedrawReductionMotor, wantRedrawBackButton, wantRedrawReductionGear;
     int lastReductionMotorLength, lastReductionGearLength;
     void drawGUI(HalDC* hal);

     void drawReductions(HalDC* hal, int top);
     void validate();

};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern ReductionScreen* Reduction;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class StepsScreen : public AbstractHALScreen
{
  public:

  static AbstractHALScreen* create()
  {
    return new StepsScreen();
  }
  
   virtual void onActivate();
   virtual void onDeactivate();

   virtual void onEvent(Event event, void* param);


protected:
  
    virtual void doSetup(HalDC* hal);
    virtual void doUpdate(HalDC* hal);
    virtual void doDraw(HalDC* hal);

private:
    StepsScreen();
    ~StepsScreen();

     int8_t selectedMenu;
     uint16_t rotationSpeed;
     uint16_t steps;
     bool wantRedrawRotationSpeed, wantRedrawBackButton, wantRedrawSteps;
     int lastRotationSpeedLength, lastStepsLength;
     void drawGUI(HalDC* hal);
     
     bool isInWork;

     bool wantDrawStepperStatus;
     
     int drawRotationSpeed(HalDC* hal, int top);
     void drawSteps(HalDC* hal, int top);

     void startSteps(bool ccw);
     void stopSteps(bool ccw);

     void validate();
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern StepsScreen* Steps;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
  dtParts,
  dtDegrees
  
} DivideType;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class DivideScreen : public AbstractHALScreen
{
  public:

  static AbstractHALScreen* create(DivideType t)
  {
    return new DivideScreen(t);
  }
  
   virtual void onActivate();
   virtual void onDeactivate();

   virtual void onEvent(Event event, void* param);


protected:
  
    virtual void doSetup(HalDC* hal);
    virtual void doUpdate(HalDC* hal);
    virtual void doDraw(HalDC* hal);

private:
    DivideScreen(DivideType t);
    ~DivideScreen();

     DivideType workMode;

     int16_t totalNumOfDivisions;
     int16_t currentPosition;
     
     bool wantRedrawNumOfDivisions, wantRedrawCurrentPosition;
     int lastNumOfDivisionsLength, lastCurrentPositionLength;
     
     
     void drawGUI(HalDC* hal);
     
     bool isInWork;
     bool isCWRotation;

     bool wantDrawStepperStatus;
     
     void drawNumOfDivisions(HalDC* hal, int top);
     int drawCurrentPosition(HalDC* hal, int top);

     void startSteps(bool ccw);
     void stopSteps(bool ccw);

     void validate();
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern DivideScreen* DivideByParts;
extern DivideScreen* DivideByDegrees;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

