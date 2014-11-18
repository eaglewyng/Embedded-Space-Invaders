/*****************************************************************************
* Filename:          C:\Users\superman\Contacts\Desktop\Team_PT\Lab7_try3/drivers/nes_c_controller_v1_00_a/src/nes_c_controller.h
* Version:           1.00.a
* Description:       nes_c_controller Driver Header File
* Date:              Tue Nov 18 13:51:50 2014 (by Create and Import Peripheral Wizard)
*****************************************************************************/

#ifndef NES_C_CONTROLLER_H
#define NES_C_CONTROLLER_H

/***************************** Include Files *******************************/

#include "xbasic_types.h"
#include "xstatus.h"
#include "xil_io.h"

/************************** Constant Definitions ***************************/


/**
 * User Logic Slave Space Offsets
 * -- SLV_REG0 : user logic slave module register 0
 * -- SLV_REG1 : user logic slave module register 1
 */
#define NES_C_CONTROLLER_USER_SLV_SPACE_OFFSET (0x00000000)
#define NES_C_CONTROLLER_SLV_REG0_OFFSET (NES_C_CONTROLLER_USER_SLV_SPACE_OFFSET + 0x00000000)
#define NES_C_CONTROLLER_SLV_REG1_OFFSET (NES_C_CONTROLLER_USER_SLV_SPACE_OFFSET + 0x00000004)

/**
 * Software Reset Space Register Offsets
 * -- RST : software reset register
 */
#define NES_C_CONTROLLER_SOFT_RST_SPACE_OFFSET (0x00000100)
#define NES_C_CONTROLLER_RST_REG_OFFSET (NES_C_CONTROLLER_SOFT_RST_SPACE_OFFSET + 0x00000000)

/**
 * Software Reset Masks
 * -- SOFT_RESET : software reset
 */
#define SOFT_RESET (0x0000000A)

#define ENABLE_INTERRUPT 0x1

/**************************** Type Definitions *****************************/


/***************** Macros (Inline Functions) Definitions *******************/

/**
 *
 * Write a value to a NES_C_CONTROLLER register. A 32 bit write is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is written.
 *
 * @param   BaseAddress is the base address of the NES_C_CONTROLLER device.
 * @param   RegOffset is the register offset from the base to write to.
 * @param   Data is the data written to the register.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void NES_C_CONTROLLER_mWriteReg(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Data)
 *
 */
#define NES_C_CONTROLLER_mWriteReg(BaseAddress, RegOffset, Data) \
 	Xil_Out32((BaseAddress) + (RegOffset), (Xuint32)(Data))

/**
 *
 * Read a value from a NES_C_CONTROLLER register. A 32 bit read is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is read from the register. The most significant data
 * will be read as 0.
 *
 * @param   BaseAddress is the base address of the NES_C_CONTROLLER device.
 * @param   RegOffset is the register offset from the base to write to.
 *
 * @return  Data is the data from the register.
 *
 * @note
 * C-style signature:
 * 	Xuint32 NES_C_CONTROLLER_mReadReg(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */
#define NES_C_CONTROLLER_mReadReg(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (RegOffset))


/**
 *
 * Write/Read 32 bit value to/from NES_C_CONTROLLER user logic slave registers.
 *
 * @param   BaseAddress is the base address of the NES_C_CONTROLLER device.
 * @param   RegOffset is the offset from the slave register to write to or read from.
 * @param   Value is the data written to the register.
 *
 * @return  Data is the data from the user logic slave register.
 *
 * @note
 * C-style signature:
 * 	void NES_C_CONTROLLER_mWriteSlaveRegn(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Value)
 * 	Xuint32 NES_C_CONTROLLER_mReadSlaveRegn(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */
#define NES_C_CONTROLLER_mWriteSlaveReg0(BaseAddress, RegOffset, Value) \
 	Xil_Out32((BaseAddress) + (NES_C_CONTROLLER_SLV_REG0_OFFSET) + (RegOffset), (Xuint32)(Value))
#define NES_C_CONTROLLER_mWriteSlaveReg1(BaseAddress, RegOffset, Value) \
 	Xil_Out32((BaseAddress) + (NES_C_CONTROLLER_SLV_REG1_OFFSET) + (RegOffset), (Xuint32)(Value))

#define NES_C_CONTROLLER_mReadSlaveReg0(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (NES_C_CONTROLLER_SLV_REG0_OFFSET) + (RegOffset))
#define NES_C_CONTROLLER_mReadSlaveReg1(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (NES_C_CONTROLLER_SLV_REG1_OFFSET) + (RegOffset))

/**
 *
 * Reset NES_C_CONTROLLER via software.
 *
 * @param   BaseAddress is the base address of the NES_C_CONTROLLER device.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void NES_C_CONTROLLER_mReset(Xuint32 BaseAddress)
 *
 */
#define NES_C_CONTROLLER_mReset(BaseAddress) \
 	Xil_Out32((BaseAddress)+(NES_C_CONTROLLER_RST_REG_OFFSET), SOFT_RESET)

/************************** Function Prototypes ****************************/


/**
 *
 * Run a self-test on the driver/device. Note this may be a destructive test if
 * resets of the device are performed.
 *
 * If the hardware system is not built correctly, this function may never
 * return to the caller.
 *
 * @param   baseaddr_p is the base address of the NES_C_CONTROLLER instance to be worked on.
 *
 * @return
 *
 *    - XST_SUCCESS   if all self-test code passed
 *    - XST_FAILURE   if any self-test code failed
 *
 * @note    Caching must be turned off for this function to work.
 * @note    Self test may fail if data memory and device are not on the same bus.
 *
 */
XStatus NES_C_CONTROLLER_SelfTest(void * baseaddr_p);
/**
*  Defines the number of registers available for read and write*/
#define TEST_AXI_LITE_USER_NUM_REG 2

 /*Bit positions for NES:
  * No buttons pressed: 0xFF
  * Bit 0: Button A (0xFE when only A is pressed)
  * Bit 1: Button B (0xFD when only B is pressed)
  * Bit 2: Select Button (0xFB when only Select is pressed)
  * Bit 3: Start Button (0xF7 when only Start is pressed)
  * Bit 4: Up Arrow (0xEF when only Up is pressed)
  * Bit 5: Down Arrow (0xDF when only Down is pressed)
  * Bit 6: Left Arrow (0xBF when only Left is pressed)
  * Bit 7: Right Arrow (0x7F when only Right is pressed)
  */

#define NO_BUTTONS_PRESSED 0xFF
#define A_MASK 0x01
#define B_MASK 0x02
#define SELECT_MASK 0x04
#define START_MASK 0x08
#define UP_MASK 0x10
#define DOWN_MASK 0x20
#define LEFT_MASK 0x40
#define RIGHT_MASK 0x80


#endif /** NES_C_CONTROLLER_H */
