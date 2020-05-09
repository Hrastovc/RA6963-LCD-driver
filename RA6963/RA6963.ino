#include "pinsDef.h"
#include "RA6963defines.h"

#define FALSE  (0)
#define TRUE   (1)

#define DATA   (0)
#define AUTO   (0)
#define CMD    (1)
#define STATUS (1)

#define READ   (0)
#define WRITE  (1)

#define FILL   (0)
#define BITMAP (1)

void setup()
{
  /* set output pins */
  WR_DDR   |= (1 << WR);
  RD_DDR   |= (1 << RD);
  CE_DDR   |= (1 << CE);
  CD_DDR   |= (1 << CD);
  RST_DDR  |= (1 << RST);
  FS1_DDR  |= (1 << FS1);
  /* set pins to HIGH */
  RD_PORT  |= (1 << RD);
  WR_PORT  |= (1 << WR);
  CE_PORT  |= (1 << CE);
  CD_PORT  |= (1 << CD);
  /* set pins to LOW */
  FS1_PORT &= ~(1 << FS1);
  
  /* Reset LCD IC */
  RST_PORT &= ~(1 << RST);
  delay(50);
  RST_PORT |=  (1 << RST);
  delay(50);
  /* LCD init */
  R6963(WRITE, CMD, 0x00, 0x00, SetControlWord | GraphicHomeAddress, 2);
  R6963(WRITE, CMD, 0x1E, 0x00, SetControlWord | GraphicArea, 2);
  R6963(WRITE, CMD, 0x00, 0x00, ModeSet, 0);
  R6963(WRITE, CMD, 0x00, 0x00, DisplayMode | GraphicOn, 0);
  R6963(WRITE, CMD, 0x00, 0x00, RegistersSetting | AddressPointer, 2);
  R6963(WRITE, DATA, FILL, 0x00, DataAutoReadWrite, 3840);
}

void loop()
{
  R6963(WRITE, CMD, 0x00, 0x00, RegistersSetting | AddressPointer, 2);
  for(uint16_t i = 0; i < 3840; i++)
  {
    R6963(WRITE, CMD, 0x00, (0x01 << (7-((i/30)%8))), DataReadWrite | WriteIncrementADP, 1);
  }
  delay(1000);
  
  R6963(WRITE, CMD, 0x00, 0x00, RegistersSetting | AddressPointer, 2);
  for(uint16_t i = 0; i < 3840; i++)
  {
    R6963(WRITE, CMD, 0x00, (0x80 << (7-((i/30)%8))), DataReadWrite | WriteIncrementADP, 1);
  }
  delay(1000);
  
  //R6963(WRITE, CMD, 0x00, 0x00, RegistersSetting | AddressPointer, 2);
  //R6963(WRITE, DATA, FILL, 0xFF, DataAutoReadWrite, 3840);
  //R6963(WRITE, CMD, 0x00, 0x00, RegistersSetting | AddressPointer, 2);
  //R6963(WRITE, DATA, FILL, 0x00, DataAutoReadWrite, 3840);
}

/************************************************************************************//**
** \brief     R6963
** \param     rdWr Indicates reading or writing, WRITE or READ.
** \param     cmdData Indicates data or command operatiom.
** \param     data0 First data byte.
** \param     data1 Second data byte.
** \param     cmd Command to send.
** \param     dataCount Number of bytes to send.
** \return    True if successful, false otherwise.
**
****************************************************************************************/
uint8_t R6963(uint8_t rdWr, uint8_t cmdData, uint8_t data0, uint16_t data1, uint8_t cmd,
              uint16_t dataCount)
{
  uint8_t command[3];
  /* Write command to RA6963. */
  if(rdWr == WRITE & cmdData == CMD)
  {
    /* Fill array with first data byte */
    command[0] = data0;
    /* In case only one data byte is to be writen put it in the command[0]. */
    if(dataCount) command[dataCount - 1] = data1;
    /* Fill array with command */
    command[dataCount] = cmd;
    /* Call R6963sendCmd and return status */
    return R6963sendCmd(command, dataCount);
  }
  /* Write data to RA6963 */
  else if(cmdData == DATA)
  {
    /* Call R6963auto and return status */
    return R6963auto(rdWr, data0, (uint8_t *)data1, dataCount);
  }
  /* Parameters invalid */
  return FALSE;
} /*** end of R6963 ***/

/************************************************************************************//**
** \brief     R6963auto
** \param     rdWr Indicates reading or writing, WRITE or READ.
** \param     p Indicates if data parameter is pointer or not.
** \param     data Pointer to the start of bitmap.
** \param     byteCount Number ob bytes to send.
** \return    True if successful, false otherwise.
**
****************************************************************************************/
uint8_t R6963auto(uint8_t rdWr, uint8_t p, uint8_t *data, uint16_t byteCount)
{
  uint8_t cmd;
  /* wait for status */
  if(!R6963waitStatus(STA1 | STA0, 1000)) return FALSE;
  /* write data */
  if(rdWr == WRITE)
  {
    /* write command */
    cmd = DataAutoReadWrite | DataAutoWrite;
    if(!R6963rdWrData(WRITE, CMD, &cmd)) return FALSE;
    /* wait for status */
    if(!R6963waitStatus(STA3, 1000)) return FALSE;
  }
  /* read data */
  else if(rdWr == READ)
  {
    /* write command */
    cmd = DataAutoReadWrite | DataAutoRead;
    if(!R6963rdWrData(WRITE, CMD, &cmd)) return FALSE;
    /* wait for status */ 
    if(!R6963waitStatus(STA2, 1000)) return FALSE;
  }
  else
  {
    /* Mode not supported. */
    return FALSE;
  }
  /* counter */
  uint16_t i = 0;
  /* write/read all the data bytes */
  do
  {
    /* write data */
    if(rdWr == WRITE)
    {
      if(p == FILL)
      {
        /* write data */
        uint8_t color = (uint16_t)data;
        if(!R6963rdWrData(WRITE, DATA, &color)) return FALSE;
        i++;
      }
      else if(p == BITMAP)
      {
        /* write data */
        if(!R6963rdWrData(WRITE, DATA, (data + (i++)))) return FALSE;
      }
      else
      {
        /* Mode not supported. */
        return FALSE;
      }
      /* wait for status */
      if(!R6963waitStatus(STA3, 1000)) return FALSE;
    }
    /* read data */
    else if(rdWr == READ)
    {
      /* read data */
      if(!R6963rdWrData(READ, DATA, (data + (i++)))) return FALSE;
      /* wait for status */
      if(!R6963waitStatus(STA2, 1000)) return FALSE;
    }
    else
    {
      /* Mode not supported. */
      return FALSE;
    }
  }
  while(i < byteCount);
  /* write command */
  cmd = DataAutoReadWrite | AutoReset;
  if(!R6963rdWrData(WRITE, CMD, &cmd)) return FALSE;
  /* return true */
  return TRUE;
} /*** end of R6963auto ***/

/************************************************************************************//**
** \brief     R6963sendCmd
** \param     data Pointer to the variable with data to send.
** \param     byteCount Number of byte to send.
** \return    True if successful, false otherwise.
**
****************************************************************************************/
uint8_t R6963sendCmd(uint8_t *data, uint8_t byteCount)
{
  /* constrain byteCount */
  if(byteCount > 2) return FALSE;
  /* write all the data bytes */
  for(uint8_t i = 0; i < byteCount; i++)
  {
    /* wait for status */
    if(!R6963waitStatus(STA1 | STA0, 1000)) return FALSE;
    /* write data */
    if(!R6963rdWrData(WRITE, DATA, (data + i))) return FALSE;
  }
  /* wait for status */
  if(!R6963waitStatus(STA1 | STA0, 1000)) return FALSE;
  /* write command */
  if(!R6963rdWrData(WRITE, CMD, (data + byteCount))) return FALSE;
  /* return true */
  return TRUE;
} /*** end of R6963sendCmd ***/

/************************************************************************************//**
** \brief     R6963waitStatus
** \param     mask Status register mask.
** \param     timeout Time to wait for status in ms.
** \return    True if successful, false otherwise.
**
****************************************************************************************/
uint8_t R6963waitStatus(uint8_t mask, uint16_t timeout)
{
  uint8_t statReg;
  /* convert time to count */
  //TODO
  /* poll status */
  do
  {
    /* read status register */
    if(!R6963rdWrData(READ, STATUS, &statReg)) return FALSE;
    /* wait a bit */
    //TODO
  }
  while(!((statReg & mask) == mask));
  /* return status */
  if(!((statReg & mask) == mask)) return FALSE;
  /* return TRUE */
  return TRUE;
} /*** end of R6963waitStatus ***/

/************************************************************************************//**
** \brief     R6963rdWrData
** \param     rdWr Indicates reading or writing, WRITE or READ.
** \param     cdState State of a CD pin.
** \param     data Pointer to the data variable to read or write.
** \return    True if successful, false otherwise.
**
****************************************************************************************/
uint8_t R6963rdWrData(uint8_t rdWr, uint8_t cdState, uint8_t *data)
{
  /* write data */
  if(rdWr == WRITE)
  {
    /* Data bus direction */
    if(!R6963dataBusDir(OUTPUT)) return FALSE;
    /* set data bus */
    if(!R6963setDataBus(*data)) return FALSE;
  }
  /* read data */
  else if(rdWr == READ)
  {
    /* Data bus direction */
    if(!R6963dataBusDir(INPUT)) return FALSE;
  }
  else
  {
    /* Mode not supported. */
    return FALSE;
  }
  /* Register select; Command/Data write */
  if(!R6963cd(cdState)) return FALSE;
  /* WR or RD take low */
  if(!R6963wr(~rdWr)) return FALSE;
  if(!R6963rd( rdWr)) return FALSE;
  /* CE pulsed low for greater than 150ns */
  if(!R6963ce(LOW)) return FALSE;
  /* wait a least 150ns */
  //asm("nop");
  //asm("nop");
  //asm("nop");
  if(rdWr == READ)
  {
    /* read data bus */
    if(!R6963readDataBus(data)) return FALSE;
  }
  /* CE take high */
  if(!R6963ce(HIGH)) return FALSE;
  /* return TRUE */
  return TRUE;
} /*** end of R6963rdWrData ***/

/************************************************************************************//**
** \brief     R6963cd
** \param     state State of the CD pin, 0 or 1.
** \return    True if successful, false otherwise.
**
****************************************************************************************/
uint8_t R6963cd(uint8_t state)
{
  /* set pin to the state. */
  CD_PORT = (CD_PORT & ~(1 << CD)) | ((state & 0x01) << CD);
  /* return TRUE */
  return TRUE;
} /*** end of R6963cd ***/

/************************************************************************************//**
** \brief     R6963wr
** \param     state State of the WR pin, 0 or 1.
** \return    True if successful, false otherwise.
**
****************************************************************************************/
uint8_t R6963wr(uint8_t state)
{
  /* set pin to the state. */
  WR_PORT = (WR_PORT & ~(1 << WR)) | ((state & 0x01) << WR);
  /* return TRUE */
  return TRUE;
} /*** end of R6963wr ***/

/************************************************************************************//**
** \brief     R6963rd
** \param     state State of the RD pin, 0 or 1.
** \return    True if successful, false otherwise.
**
****************************************************************************************/
uint8_t R6963rd(uint8_t state)
{
  /* set pin to the state. */
  RD_PORT = (RD_PORT & ~(1 << RD)) | ((state & 0x01) << RD);
  /* return TRUE */
  return TRUE;
} /*** end of R6963rd ***/

/************************************************************************************//**
** \brief     R6963ce
** \param     state State of the CE pin, 0 or 1.
** \return    True if successful, false otherwise.
**
****************************************************************************************/
uint8_t R6963ce(uint8_t state)
{
  /* set pin to the state. */
  CE_PORT = (CE_PORT & ~(1 << CE)) | ((state & 0x01) << CE);
  /* return TRUE */
  return TRUE;
} /*** end of R6963ce ***/

/************************************************************************************//**
** \brief     R6963setDataBus
** \param     data Data to set the bus to
** \return    True if successful, false otherwise.
**
****************************************************************************************/
uint8_t R6963setDataBus(uint8_t data)
{
  /* set data bus */
  D0_PORT = (D0_PORT & ~(1 << D0)) | (((data >> 0) & 0x01) << D0);
  D1_PORT = (D1_PORT & ~(1 << D1)) | (((data >> 1) & 0x01) << D1);
  D2_PORT = (D2_PORT & ~(1 << D2)) | (((data >> 2) & 0x01) << D2);
  D3_PORT = (D3_PORT & ~(1 << D3)) | (((data >> 3) & 0x01) << D3);
  D4_PORT = (D4_PORT & ~(1 << D4)) | (((data >> 4) & 0x01) << D4);
  D5_PORT = (D5_PORT & ~(1 << D5)) | (((data >> 5) & 0x01) << D5);
  D6_PORT = (D6_PORT & ~(1 << D6)) | (((data >> 6) & 0x01) << D6);
  D7_PORT = (D7_PORT & ~(1 << D7)) | (((data >> 7) & 0x01) << D7);
  /* return TRUE */
  return TRUE;
} /*** end of R6963setDataBus ***/

/************************************************************************************//**
** \brief     R6963readDataBus
** \param     data Pointer to the bus variable
** \return    True if successful, false otherwise.
**
****************************************************************************************/
uint8_t R6963readDataBus(uint8_t *data)
{
  *data = 0;
  /* read data bus */
  *data |= (((D0_PIN >> D0) & 0x01) << 0);
  *data |= (((D1_PIN >> D1) & 0x01) << 1);
  *data |= (((D2_PIN >> D2) & 0x01) << 2);
  *data |= (((D3_PIN >> D3) & 0x01) << 3);
  *data |= (((D4_PIN >> D4) & 0x01) << 4);
  *data |= (((D5_PIN >> D5) & 0x01) << 5);
  *data |= (((D6_PIN >> D6) & 0x01) << 6);
  *data |= (((D7_PIN >> D7) & 0x01) << 7);
  /* return TRUE */
  return TRUE;
} /*** end of R6963readDataBus ***/

/************************************************************************************//**
** \brief     R6963dataBusDir
** \param     dir Data bus direction, 0 or 1.
** \return    True if successful, false otherwise.
**
****************************************************************************************/
uint8_t R6963dataBusDir(uint8_t dir)
{
  /* set pins direction */
  D0_DDR = (D0_DDR & ~(1 << D0)) | ((dir & 0x01) << D0);
  D1_DDR = (D1_DDR & ~(1 << D1)) | ((dir & 0x01) << D1);
  D2_DDR = (D2_DDR & ~(1 << D2)) | ((dir & 0x01) << D2);
  D3_DDR = (D3_DDR & ~(1 << D3)) | ((dir & 0x01) << D3);
  D4_DDR = (D4_DDR & ~(1 << D4)) | ((dir & 0x01) << D4);
  D5_DDR = (D5_DDR & ~(1 << D5)) | ((dir & 0x01) << D5);
  D6_DDR = (D6_DDR & ~(1 << D6)) | ((dir & 0x01) << D6);
  D7_DDR = (D7_DDR & ~(1 << D7)) | ((dir & 0x01) << D7);
  /* return TRUE */
  return TRUE;
} /*** end of R6963dataBusDir ***/
