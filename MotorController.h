#pragma once
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Events.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
  stepperIdle, // ничего не делаем
  stepperRotation, // вращаемся
  stepperStep, // шагаем на указанное кол-во шагов
  
} StepperWorkMode;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class StepperDriver 
{

  public:
    StepperDriver(uint8_t pinStep, uint8_t pinDir, uint8_t pinEn);
    
    void  update();  // обновление внутреннего состояния
    
    void  setHoldOnStop(boolean holdOn);  // задает режим удержания
    
    void  step(int32_t steps);  // инициирует поворот двигателя на заданное число шагов (0 - останавливает двигатель)
    void  stop() { step(0); } // остановка двигателя
    
    int32_t stepsRemaining();  // чтение оставшихся шагов
    
    bool isOnIdle(); // возвращает true, если мотор никуда не движется

    private:
      int32_t remainingSteps;        // оставшееся число шагов 
      bool holdOnStop;  // признак фиксации положения при остановке
      uint8_t stepPin, dirPin, enPin;
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class MotorControllerClass : public IEventSubscriber
{
  public:
    MotorControllerClass();
    virtual void onEvent(Event event, void* param);
    void init();
    void update();

    int32_t getRemainingSteps(){ return remainingSteps; }
    float getStepsPerRevolution();
    bool isOnIdle() { return driver->isOnIdle(); }

  private:
    StepperWorkMode workMode;
    RotationEventParam rotationSettings;

    int32_t remainingSteps;

    void stop(); // останавливаем мотор
    void rotate(); // вращаем мотор

    StepperDriver* driver;

    uint32_t computeTimerInterval(uint8_t percentsOfMaxSpeed);

    void stepperDone();
    bool started;
    
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern MotorControllerClass MotorController;

