#include "Settings.h"
#include "Memory.h"
//--------------------------------------------------------------------------------------------------------------------------------
SettingsClass Settings;
//--------------------------------------------------------------------------------------------------------------------------------
SettingsClass::SettingsClass()
{
  
}
//--------------------------------------------------------------------------------------------------------------------------------
uint8_t SettingsClass::read8(uint16_t address, uint8_t defaultVal)
{
  Value8 v;
  uint8_t* iter = (uint8_t*)&v;
  for(size_t i=0;i<sizeof(Value8);i++)
  {
    *iter++ = MemRead(address++);
  }

  if(!(v.h1 == SETT_HEADER1 && v.h2 == SETT_HEADER2))
    return defaultVal; 

   return v.value;
}
//--------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::write8(uint16_t address, uint8_t val)
{
  Value8 v;
  v.value = val;
  uint8_t* iter = (uint8_t*)&v;
  for(size_t i=0;i<sizeof(Value8);i++)
  {
    MemWrite(address++,*iter++);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------
uint16_t SettingsClass::read16(uint16_t address, uint16_t defaultVal)
{
  Value16 v;
  uint8_t* iter = (uint8_t*)&v;
  for(size_t i=0;i<sizeof(Value16);i++)
  {
    *iter++ = MemRead(address++);
  }

  if(!(v.h1 == SETT_HEADER1 && v.h2 == SETT_HEADER2))
    return defaultVal; 

   return v.value;
}
//--------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::write16(uint16_t address, uint16_t val)
{
  Value16 v;
  v.value = val;
  uint8_t* iter = (uint8_t*)&v;
  for(size_t i=0;i<sizeof(Value16);i++)
  {
    MemWrite(address++,*iter++);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------
uint32_t SettingsClass::read32(uint16_t address, uint32_t defaultVal)
{
  Value32 v;
  uint8_t* iter = (uint8_t*)&v;
  for(size_t i=0;i<sizeof(Value32);i++)
  {
    *iter++ = MemRead(address++);
  }

  if(!(v.h1 == SETT_HEADER1 && v.h2 == SETT_HEADER2))
    return defaultVal; 

   return v.value;
}
//--------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::write32(uint16_t address, uint32_t val)
{
  Value32 v;
  v.value = val;
  uint8_t* iter = (uint8_t*)&v;
  for(size_t i=0;i<sizeof(Value32);i++)
  {
    MemWrite(address++,*iter++);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------
uint8_t SettingsClass::getRotationSpeed()
{
  return read8(ROTATION_SPEED_STORE_ADDRESS,DEFAULT_ROTATION_SPEED);
}
//--------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::setRotationSpeed(uint8_t value)
{
  write8(ROTATION_SPEED_STORE_ADDRESS, value);
}
//--------------------------------------------------------------------------------------------------------------------------------
uint8_t SettingsClass::getLastSelectedMenu()
{
  return read8(LAST_SELECTED_MENU_STORE_ADDRESS,0);  
}
//--------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::setLastSelectedMenu(uint8_t value)
{
  write8(LAST_SELECTED_MENU_STORE_ADDRESS, value);  
}
//--------------------------------------------------------------------------------------------------------------------------------
uint8_t SettingsClass::getMotorReduction()
{
  return read8(REDUCTION_MOTOR_STORE_ADDRESS,REDUCTION_MOTOR);
}
//--------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::setMotorReduction(uint8_t value)
{
  write8(REDUCTION_MOTOR_STORE_ADDRESS, value);
}
//--------------------------------------------------------------------------------------------------------------------------------
uint8_t SettingsClass::getGearReduction()
{
  return read8(REDUCTION_GEAR_STORE_ADDRESS,REDUCTION_GEAR);
}
//--------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::setGearReduction(uint8_t value)
{
  write8(REDUCTION_GEAR_STORE_ADDRESS, value);
}
//--------------------------------------------------------------------------------------------------------------------------------
uint8_t SettingsClass::getDivider()
{
  return read8(MICROSTEP_DIVIDER_STORE_ADDRESS,MICROSTEP_DIVIDER);
}
//--------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::setDivider(uint8_t value)
{
  write8(MICROSTEP_DIVIDER_STORE_ADDRESS, value);
}
//--------------------------------------------------------------------------------------------------------------------------------
uint16_t SettingsClass::getStepsPerRevolution()
{
  return read16(STEPS_PER_REVOLUTION_STORE_ADDRESS,STEPS_PER_REVOLUTION);
}
//--------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::setStepsPerRevolution(uint16_t value)
{
  write16(STEPS_PER_REVOLUTION_STORE_ADDRESS, value);
}
//--------------------------------------------------------------------------------------------------------------------------------
uint16_t SettingsClass::getSteps()
{
  return read16(STEPS_STORE_ADDRESS,200);
}
//--------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::setSteps(uint16_t value)
{
  write16(STEPS_STORE_ADDRESS, value);
}
//--------------------------------------------------------------------------------------------------------------------------------
uint16_t SettingsClass::getNumOfDivisions()
{
  return read16(NUMDIVS_STORE_ADDRESS,NUMDIVS_DEFAULT_VAL);
}
//--------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::setNumOfDivisions(uint16_t value)
{
  write16(NUMDIVS_STORE_ADDRESS, value);
}
//--------------------------------------------------------------------------------------------------------------------------------

