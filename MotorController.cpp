//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "MotorController.h"
#include "CONFIG.h"
#include "TimerOne.h"
#include "Settings.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MotorControllerClass MotorController;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void  timerInterrupt() 
{
  MotorController.update(); // управвление двигателем
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MotorControllerClass::MotorControllerClass()
{
  workMode = stepperIdle;
  started = false;
  remainingSteps = 0;

  driver = new StepperDriver(STEP_PIN, DIR_PIN, EN_PIN);
  driver->setHoldOnStop(true); // удерживаем ток на обмотках после остановки
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MotorControllerClass::init()
{  
  Events.subscribe(this);

  Timer1.initialize(computeTimerInterval(Settings.getRotationSpeed()));  // инициализация таймера 1
  Timer1.attachInterrupt(timerInterrupt);  // задаем обработчик прерываний
  Timer1.stop(); // останавливаем таймер, он нам не нужен в режиме простоя
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float MotorControllerClass::getStepsPerRevolution()
{
  float stepsPerRevolution = Settings.getStepsPerRevolution();
  stepsPerRevolution *= Settings.getDivider();
  stepsPerRevolution /= Settings.getGearReduction();
  stepsPerRevolution *= Settings.getMotorReduction();

  return stepsPerRevolution;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint32_t MotorControllerClass::computeTimerInterval(uint8_t percentsOfMaxSpeed)
{
  // высчитываем интервал между тиками таймера
  
  float stepsPerRevolution = getStepsPerRevolution();

  DBG(F("Computed stepsPerRevolution = "));
  DBGLN(stepsPerRevolution);

  uint32_t stepsForMaxSpeed = stepsPerRevolution*MAX_SPEED;

  DBG(F("Computed stepsForMaxSpeed = "));
  DBGLN(stepsForMaxSpeed);

  float timerInterval = 60000000.0/stepsForMaxSpeed;
  
  DBG(F("Max speed timerInterval = "));
  DBGLN(timerInterval);

  timerInterval = (100.0/percentsOfMaxSpeed)*timerInterval;

  DBG(F("timerInterval for "));
  DBG(percentsOfMaxSpeed);
  DBG(F("% is "));
  DBGLN(timerInterval);


  return uint32_t(timerInterval);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MotorControllerClass::stop()
{
  if(!started)
    return;
    
  DBGLN(F("STOP STEPPER!!!"));
  remainingSteps = driver->stepsRemaining();
  driver->stop();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MotorControllerClass::rotate()
{  
  int steps = 10000;
  
  if(!rotationSettings.ccw)
    steps = -steps;

  remainingSteps = 0;
  driver->step(steps);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MotorControllerClass::onEvent(Event event, void* param)
{
    switch(event)
    {
      #ifdef USE_KEYBOARD
      case KeyboardEvent:
      break;
      #endif
      
      case StepperWorkDone: // движок остановился
      {
        
      }
      break; // StepperWorkDone

      case ButtonStateChanged: // изменилось состояние кнопки
      {
        
      }
      break; // ButtonStateChanged

      case EncoderPositionChanged: // смена позиции энкодера
      {
      
      }
      break; // EncoderPositionChanged

      case EncoderButtonClicked: // кликнута кнопка энкодера
      {
        
      }
      break; // EncoderButtonClicked

      case StepsRequested: // запросили запустить или остановить шагание на определённое кол-во шагов
      {
        StepsEventParam* p = (StepsEventParam*) param;
        workMode = stepperStep;
        DBG(F("STEP STEPPER with steps="));
        DBGLN(p->steps);
         
        if(p->start)
        {
          // запускаем мотор
          started = true;
          
          int32_t steps = p->steps;
          
          if(!p->ccw)
            steps = -steps;

          remainingSteps = 0;
          driver->step(steps);
            
          Timer1.setPeriod(computeTimerInterval(p->speed));
          Timer1.start();
          
        }
        else
        {
          // останавливаем мотор
          stepperDone();
        }
      }
      break; // StepsRequested 
      
      case RotationRequested: // запросили запустить или остановить вращение шаговика
      {
        workMode = stepperRotation;
        
        RotationEventParam* p = (RotationEventParam*) param;
        rotationSettings = *p;
        
        if(rotationSettings.start)
        {
          // запускаем мотор
          started = true;
          
          DBGLN(F("ROTATE STEPPER with rotationSettings!!!"));
          rotate();
          
          Timer1.setPeriod(computeTimerInterval(rotationSettings.speed));
          Timer1.start();

        }
        else
        {
          // останавливаем мотор
          stepperDone();
        }

      }
      break; // RotationRequested
      
    } // switch  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MotorControllerClass::stepperDone()
{
  if(!started)
    return;
  
    workMode = stepperIdle;
    Timer1.stop();
    stop();
    started = false;
    
    DBGLN(F("STEPPER DONE, stop it, stop Timer1."));

    Events.raise(this,StepperWorkDone,NULL);          
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MotorControllerClass::update()
{
    switch(workMode)
    {
      case stepperIdle:
        return;

      case stepperRotation: // постоянно перезапускаем счётчик шагов, чтобы мотор крутился, пока нажата кнопка
      {
        rotate();
      }
      break; // stepperRotation

      case stepperStep:
      {
        
      }
      break; // stepperStep

      
    } // switch
    
    driver->update();

    if(driver->isOnIdle())
    {
      // мотор закончил шагать!
      stepperDone();
    }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// StepperDriver
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
StepperDriver::StepperDriver(uint8_t pinStep, uint8_t pinDir, uint8_t pinEn) 
{

  // перегрузка номеров выводов
  stepPin = pinStep;
  dirPin = pinDir;
  enPin = pinEn;
  
  pinMode(stepPin, OUTPUT); digitalWrite(stepPin, LOW);
  pinMode(dirPin, OUTPUT); digitalWrite(dirPin, LOW);
  pinMode(enPin, OUTPUT); digitalWrite(enPin, HIGH);
  
  remainingSteps = 0;
  holdOnStop = true;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool StepperDriver::isOnIdle()
{
  return stepsRemaining() == 0;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void  StepperDriver::update() 
{    
  if(remainingSteps == 0) // закончили работу
  {
    return; 
  }
    if (remainingSteps > 0) 
      remainingSteps--; // вращение против часовой стрелки   
    else
      remainingSteps++; // вращение по часовой стрелке           

  if(remainingSteps != 0 ) 
  {
    // сделать шаг
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(10);  // 10 мкс
    digitalWrite(stepPin, LOW);        
  }
  else 
  {
    if ( !holdOnStop ) 
      digitalWrite(enPin, HIGH);    
  }          
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// инициирует поворот двигателя на заданное число шагов
void  StepperDriver::step(int32_t steps) 
{

  // блокировка
  if ( (steps == 0) && !holdOnStop ) 
    digitalWrite(enPin, HIGH);
  else 
    digitalWrite(enPin, LOW);

  // направление вращения    
  if ( steps < 0 ) 
    digitalWrite(dirPin, LOW);
  else 
    digitalWrite(dirPin, HIGH);

  noInterrupts();
  remainingSteps = steps;
  interrupts();  
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void  StepperDriver::setHoldOnStop(boolean holdOn)  
{  
  holdOnStop = holdOn;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int32_t StepperDriver::stepsRemaining()  
{
  int32_t remaining;
  
  noInterrupts();
    remaining = remainingSteps;
  interrupts();
  
  return abs(remaining);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------


