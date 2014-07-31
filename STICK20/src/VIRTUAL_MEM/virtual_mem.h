/* This header file is part of the ATMEL AVR32-UC3-SoftwareFramework-1.6.0 Release */

/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file ******************************************************************
 *
 * \brief Management of the virtual memory.
 *
 * This file manages the virtual memory.
 *
 * - Compiler:           IAR EWAVR32 and GNU GCC for AVR32
 * - Supported devices:  All AVR32 devices with a USB module can be used.
 * - AppNote:
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support and FAQ: http://support.atmel.no/
 *
 ***************************************************************************/

/* Copyright (c) 2009 Atmel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an Atmel
 * AVR product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 *
 */
/*
* This file contains modifications done by Rudolf Boeddeker
* For the modifications applies:
*
* Author: Copyright (C) Rudolf Boeddeker  Date: 2012-08-18
*
* GPF Crypto Stick 2  is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* GPF Crypto Stick is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with GPF Crypto Stick. If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef _VIRTUAL_MEM_H_
#define _VIRTUAL_MEM_H_


#include "conf_access.h"

#if VIRTUAL_MEM == DISABLE
  #error virtual_mem.h is #included although VIRTUAL_MEM is disabled
#endif


#include "ctrl_access.h"


//_____ D E F I N I T I O N S ______________________________________________

//! FAT specification
#define ENTRY_SIZE          32    // Size of entry in bytes

//! Value of virtual PBR
#define VMEM_SECTOR_SIZE    512   // Sector = 512 bytes
#define VMEM_CLUSTER_SIZE   1     // Number of sectors per cluster
#define VMEM_RESERVED_SIZE  1     // Number of reserved sectors in the Reserved region of the volume starting at the first sector (FAT sector) of the volume
                                  // It must be 1 for FAT 12 and 16 and 32 for FAT 32
#define VMEM_NB_FATS        2     // Number of FATs, it must be 2 to support all systems
#define VMEM_NB_ROOT_ENTRY  16    // Count of 32-byte directory entries in the root directory (32 * this value = must be a multiple of sector size)
                                  // It must be 512 for FAT 12 and 16 and 0 for FAT 32
#define VMEM_MEDIA_TYPE     0xF8  // Media type (not removable=0xF8 or removable=0xF0), the standard value is 0xF8
#define VMEM_SIZE_FAT       1     // Number of sectors per FAT
// Physical structure of disk
#define VMEM_SECT_PER_TRACK 1     // Number of sectors per track
#define VMEM_NB_HEAD        1     // Number of heads

#define VMEM_NB_HIDDEN_SECT 0     // This field should always be zero on non-partitioned media
#define VMEN_DRIVE_NUMBER   0x80  // 0x00 for floppy disk; 0x80 for hard disk

// First sector of the virtual disk (this disk isn't partitioned, only 1 partion)
#define PBR_SECTOR          0
#define FAT_SECTOR          (PBR_SECTOR  + VMEM_RESERVED_SIZE)
#define ROOT_SECTOR         (FAT_SECTOR  + VMEM_NB_FATS * VMEM_SIZE_FAT)
#define FILE_SECTOR         (ROOT_SECTOR + VMEM_NB_ROOT_ENTRY * ENTRY_SIZE / VMEM_SECTOR_SIZE)  // 1 sector = size root dir

#define FILE_SIZE           16    // Number of available clusters

// Size of virtual mem
#define VMEM_NB_SECTOR      (FILE_SIZE * VMEM_CLUSTER_SIZE + FILE_SECTOR)


#if VMEM_NB_SECTOR > 0xFF
  #error Virtual memory volume size > 512 bytes * 0xFF
#endif


//---- CONTROL FUNCTIONS ----

Ctrl_status virtual_unit_state_e;

extern Ctrl_status  virtual_test_unit_ready(void);
extern void         set_virtual_unit_busy(void);
extern Ctrl_status  virtual_read_capacity(U32 *u32_nb_sector);
extern Bool         virtual_wr_protect(void);
extern Bool         virtual_removal(void);


//---- ACCESS DATA FUNCTIONS ----

// USB interface
#if ACCESS_USB == ENABLED
extern Ctrl_status  virtual_usb_read_10 (U32 addr, U16 nb_sector);
extern Ctrl_status  virtual_usb_write_10(U32 addr, U16 nb_sector);
#endif

// RAM interface
#if ACCESS_MEM_TO_RAM == ENABLED
extern Ctrl_status  virtual_mem_2_ram(U32 addr,       void *ram);
extern Ctrl_status  virtual_ram_2_mem(U32 addr, const void *ram);
#endif


#endif  // _VIRTUAL_MEM_H_