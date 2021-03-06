/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

#if defined(BUILD_LK)
#include <platform/mt_gpio.h>
#include <platform/mt_pmic.h>
#else
#include <mach/mt_gpio.h>
#include <mach/mt_pm_ldo.h>
#include <mach/upmu_common.h>
#endif

#if !defined(BUILD_LK)
#include <linux/string.h>
#endif
#include "lcm_drv.h"

#if defined(BUILD_LK)
	#define LCM_DEBUG  printf
	#define LCM_FUNC_TRACE() printf("huyl [uboot] %s\n",__func__)
#else
	#define LCM_DEBUG  printk
	#define LCM_FUNC_TRACE() printk("huyl [kernel] %s\n",__func__)
#endif
// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  (720)
#define FRAME_HEIGHT (1280)

#define LCM_OTM1283_ID			(0x1283)

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

static unsigned int lcm_esd_test = FALSE;      ///only for ESD test

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))

#define REGFLAG_DELAY             							0XFE
#define REGFLAG_END_OF_TABLE      							0x100   // END OF REGISTERS MARKER

// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	        lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)											lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)   				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)


struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};

static void lcm_init_power(void)
{
}


static void lcm_suspend_power(void)
{
}

static void lcm_resume_power(void)
{
}


static struct LCM_setting_table lcm_initialization_setting[] = {

    {0x00,  1,  {0x00}},
    {0xff,  3,  {0x12,0x83,0x01}},        //EXTC

    {0x00,  1,  {0x80}},                  //Orise mode enable
    {0xff,  2,  {0x12,0x83}},

    {0x00,  1,  {0x92}},
    {0xFF,  2,  {0x30,0x02}},             //MIPI 4Lanes

    //-------------------- power setting -----------------------------------//
    {0x00,  1,  {0xc6}},                  //debounce
    {0xB0,  1,  {0x03}},

    {0x00,  1,  {0xB9}},                  //MIPI driving capability
    {0xB0,  1,  {0x51}},

    {0x00,  1,  {0x80}},                  //TCON Setting
    {0xC0,  9,  {0xc0,0x00,0x64,0x00,0x0f,0x11,0x00,0x64,0x0f,0x11}},

    {0x00,  1,  {0x90}},                  //Panel Timing Setting
    {0xc0,  6,  {0x00,0x55,0x00,0x01,0x00,0x04}},

    {0x00,  1,  {0xa4}},                  //source pre.
    {0xc0,  1,  {0x00}},

    {0x00,  1,  {0xb3}},                  //Interval Scan Frame: 0 frame, column inversion
    {0xc0,  2,  {0x00,0x50}},             //50 column, 00 1dot

    {0x00,  1,  {0x81}},                  //frame rate:60Hz
    {0xc1,  1,  {0x66}},


    {0x00,  1,  {0xa0}},
    {0xc1,  1,  {0x02}},

    /*
      {0x00,  1,  {0xa2}},
      {0xc1,  1,  {0x08}},

      {0x00,  1,  {0xa4}},
      {0xc1,  1,  {0xf0}}, */

    {0x00,  1,  {0x80}},                  //source to ground
    {0xc4,  1,  {0x30}},

    {0x00,  1,  {0x81}},                  //source bias 0.75uA
    {0xc4,  1,  {0x82}},

    {0x00,  1,  {0x82}},                  //flash-orise add
    {0xc4,  1,  {0x02}},

    {0x00,  1,  {0x87}},                  //source precharge to ground
    {0xc4,  1,  {0x18}},

    {0x00,  1,  {0x8A}},                  //source to ground
    {0xc4,  1,  {0x40}},

    {0x00,  1,  {0x8b}},                  //source to ground
    {0xc4,  1,  {0x00}},

    {0x00,  1,  {0x90}},                  //clock delay for data latch
    {0xc4,  1,  {0x52}},                  //0x49

    {0x00,  1,  {0xa0}},                  //dcdc setting (PFM Fre)
    {0xC4, 14,  {0x05,0x10,0x06,0x02,0x05,0x15,0x10,0x05,0x10,0x07,
                 0x02,0x05,0x15,0x10}},

    {0x00,  1,  {0xb0}},                  //clamp voltage setting
    {0xc4,  2,  {0x00,0x00}},             //VSP and VSN Change (5.6V,-5.6V)

    {0x00,  1,  {0x90}},                  //3xVPNL
    {0xc5,  1,  {0x50}},

    {0x00,  1,  {0x91}},
    {0xC5,  2,  {0x76,0x50}},             //VGH=14.5 VGL=-13.5  54 50

    {0x00,  1,  {0x94}},                  //2xVPNL
    {0xc5,  1,  {0x66}},

    {0x00,  1,  {0xb0}},
    {0xc5,  2,  {0x04,0x38}},             //VDD_18V=1.7V, LVDSVDD=1.55V  0xb8

    {0x00,  1,  {0xb4}},                  //VGLO1/2 Pull low setting
    {0xc5,  1,  {0xc0}},                  //d[7] vglo1 d[6] vglo2 => 0: pull vss, 1: pull vgl

    {0x00,  1,  {0xbb}},                  //LVD voltage level setting
    {0xc5,  1,  {0x80}},

    {0x00,  1,  {0xb5}},
    {0xc5,  6,  {0x3b,0xed,0x30,0x3b,0xed,0x30}},

    {0x00,  1,  {0xb1}},                  //LED PWM fre.
    {0xc6,  1,  {0x05}},

    //-------------------------------------- Power IC-----------------------------------------
    {0x00,  1,  {0x90}},                  //Mode-3
    {0xf5,  4,  {0x02,0x11,0x02,0x11}},

    {0x00,  1,  {0xb2}},                  //VGLO1
    {0xf5,  2,  {0x00,0x00}},

    {0x00,  1,  {0xb4}},                  //VGLO1_S
    {0xf5,  2,  {0x00,0x00}},

    {0x00,  1,  {0xb6}},                  //VGLO2
    {0xf5,  2,  {0x00,0x00}},

    {0x00,  1,  {0xb8}},                  //VGLO2_S
    {0xf5,  2,  {0x00,0x00}},

    {0x00,  1,  {0x94}},                  //VCL on
    {0xF5,  1,  {0x02}},

    {0x00,  1,  {0xBA}},                  //VSP on
    {0xF5,  1,  {0x03}},

    {0x00,  1,  {0xC3}},                  //GVDD OP
    {0xF5,  1,  {0x81}},

    {0x00,  1,  {0xB1}},
    {0xc6,  1,  {0x05}},                  //PWM rate set 22KHZ

    //-------------------- control setting ---------------------------------------------------//
    {0x00,  1,  {0x00}},                  //ID1
    {0xd0,  1,  {0x40}},

    {0x00,  1,  {0x00}},                  //ID2, ID3
    {0xd1,  2,  {0x00,0x00}},

    //-------------------- panel timing state control --------------------//
    {0x00,  1,  {0x80}},                  //panel timing state control
    {0xCB, 11,  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                 0x00}},

    {0x00,  1,  {0x90}},                  //panel timing state control
    {0xCB, 15,  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                 0x00,0xff,0x00,0xff,0x00}},

    {0x00,  1,  {0xa0}},                  //panel timing state control
    {0xCB, 15,  {0xff,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                 0x00,0x00,0x00,0x00,0x00}},

    {0x00,  1,  {0xb0}},                 //panel timing state control
    {0xCB, 15,  {0x00,0x00,0x00,0xff,0x00,0xff,0x00,0xff,0x00,0xff,
                 0x00,0x00,0x00,0x00,0x00}},

    {0x00,  1,  {0xc0}},                 //panel timing state control
    {0xCB, 15,  {0x00,0x00,0x00,0x00,0x05,0x05,0x00,0x00,0x05,0x00,
                 0x05,0x05,0x00,0x05,0x05}},

    {0x00,  1,  {0xd0}},                 //panel timing state control
    {0xCB, 15,  {0x05,0x05,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                 0x00,0x05,0x05,0x00,0x00}},

    {0x00,  1,  {0xe0}},                 //panel timing state control
    {0xCB, 14,  {0x05,0x00,0x05,0x05,0x00,0x05,0x05,0x05,0x05,0x05,
                 0x00,0x00,0x00,0x00}},

    {0x00,  1,  {0xf0}},                 //panel timing state control
    {0xCB, 11,  {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
                 0xff}},

    //-------------------- panel pad mapping control --------------------//
    {0x00,  1,  {0x80}},                 //panel pad mapping control
    {0xCC, 15,  {0x00,0x00,0x00,0x00,0x2E,0x2D,0x00,0x00,0x29,0x00,
                 0x2A,0x0c,0x00,0x0a,0x10}},

    {0x00,  1,  {0x90}},                 //panel pad mapping control
    {0xCC, 15,  {0x0e,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                 0x00,0x2E,0x2D,0x00,0x00}},

    {0x00,  1,  {0xa0}},                 //panel pad mapping control
    {0xCC, 14,  {0x29,0x00,0x2A,0x0b,0x00,0x09,0x0f,0x0d,0x01,0x03,
                 0x00,0x00,0x00,0x00}},

    {0x00,  1,  {0xb0}},                 //panel pad mapping control
    {0xCC, 15,  {0x00,0x00,0x00,0x00,0x2D,0x2E,0x00,0x00,0x29,0x00,
                 0x2A,0x0f,0x00,0x09,0x0b}},

    {0x00,  1,  {0xc0}},                 //panel pad mapping control
    {0xCC, 15,  {0x0d,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                 0x00,0x2D,0x2E,0x00,0x00}},

    {0x00,  1,  {0xd0}},                 //panel pad mapping control
    {0xCC, 15,  {0x29,0x00,0x2A,0x10,0x00,0x0a,0x0c,0x0e,0x04,0x02,
                 0x00,0x00,0x00,0x00}},

    //-------------------- panel timing setting --------------------//

    {0x00,  1,  {0x80}},                 //panel VST setting
    {0xCE, 12,  {0x8b,0x03,0x10,0x8a,0x03,0x10,0x89,0x03,0x10,0x88,
                 0x03,0x10}},

    {0x00,  1,  {0x90}},                 //panel VEND setting
    {0xCE, 14,  {0xf0,0x00,0x00,0xf0,0x00,0x00,0xf0,0x00,0x00,0xf0,
                 0x00,0x00,0x00,0x00}},

    {0x00,  1,  {0xa0}},                 //panel CLKA1/2 setting
    {0xCE, 14,  {0x38,0x07,0x84,0xfe,0x00,0x10,0x00,0x38,0x06,0x84,
                 0xff,0x00,0x10,0x00}},

    {0x00,  1,  {0xb0}},                 //panel CLKA3/4 setting
    {0xCE, 14,  {0x38,0x05,0x85,0x00,0x00,0x10,0x00,0x38,0x04,0x85,
                 0x01,0x00,0x10,0x00}},

    {0x00,  1,  {0xc0}},                 //panel CLKb1/2 setting
    {0xCE, 14,  {0x38,0x03,0x85,0x02,0x00,0x10,0x00,0x38,0x02,0x85,
                 0x03,0x00,0x10,0x00}},

    {0x00,  1,  {0xd0}},                 //panel CLKb3/4 setting
    {0xCE, 14,  {0x38,0x01,0x85,0x04,0x00,0x10,0x00,0x38,0x00,0x85,
                 0x05,0x00,0x10,0x00}},

    {0x00,  1,  {0x80}},                 //panel CLKc1/2 setting
    {0xCF, 14,  {0x70,0x00,0x00,0x10,0x00,0x00,0x00,0x70,0x00,0x00,
                 0x10,0x00,0x00,0x00}},

    {0x00,  1,  {0x90}},                 //panel CLKc3/4 setting
    {0xCF, 14,  {0x70,0x00,0x00,0x10,0x00,0x00,0x00,0x70,0x00,0x00,
                 0x10,0x00,0x00,0x00}},

    {0x00,  1,  {0xa0}},                 //panel CLKd1/2 setting
    {0xCF, 14,  {0x70,0x00,0x00,0x10,0x00,0x00,0x00,0x70,0x00,0x00,
                 0x10,0x00,0x00,0x00}},

    {0x00,  1,  {0xb0}},                 //panel CLKd3/4 setting
    {0xCF, 14,  {0x70,0x00,0x00,0x10,0x00,0x00,0x00,0x70,0x00,0x00,
                 0x10,0x00,0x00,0x00}},

    {0x00,  1,  {0xc0}},                 //panel ECLK setting
    {0xCF, 11,  {0x04,0x01,0x20,0x20,0x00,0x00,0x01,0x80,0x00,0x03,
                 0x08}},

    {0x00,  1,  {0x00}},                //GVDD=4.87V, NGVDD=-4.87V
    {0xd8,  2,  {0x9c,0x9c}},

    {0x00,  1,  {0x00}},
    {0xd9,  1,  {0x98}}, //8f  88 80  78

    {0x00,  1,  {0x00}},
    {0xE1, 16,  {0x04,0x0A,0x0E,0x0D,0x05,0x11,0x0C,0x0B,0x03,0x06,
                 0x08,0x06,0x0E,0x1D,0x1A,0x08}},

    {0x00,  1,  {0x00}},
    {0xE2, 16,  {0x04,0x09,0x0E,0x0D,0x06,0x11,0x0B,0x0A,0x02,0x07,
                 0x09,0x05,0x0E,0x1D,0x1A,0x08}},

    //********ce  begin********//
    {0x00,  1,  {0xa0}},
    {0xD6, 12,  {0x01,0xCD,0x01,0xCD,0x01,0xCD,0x01,0xCD,0x01,0xCD,
                 0x01,0xCD}},

    {0x00,  1,  {0xB0}},
    {0xD6, 12,  {0x01,0xCD,0x01,0xCD,0x01,0xCD,0x01,0xCD,0x01,0xCD,
                 0x01,0xCD}},

    {0x00,  1,  {0xC0}},
    {0xD6, 12,  {0x89,0x11,0x89,0x89,0x11,0x89,0x89,0x11,0x89,0x89,
                 0x11,0x89}},

    {0x00,  1,  {0xD0}},
    {0xD6,  6,  {0x89,0x11,0x89,0x89,0x11,0x89}},

    {0x00,  1,  {0xE0}},
    {0xD6, 12,  {0x44,0x11,0x44,0x44,0x11,0x44,0x44,0x11,0x44,0x44,
                 0x11,0x44}},

    {0x00,  1,  {0xF0}},
    {0xD6,  6,  {0x44,0x11,0x44,0x44,0x11,0x44}},

    {0x00,  1,  {0x90}},
    {0xD6,  1,  {0x80}},

    {0x00,  1,  {0x81}},
    {0xD6,  2,  {0x03,0xFF}},            //锐力化设定

    {0x00,  1,  {0x00}},
    {0x55,  1,  {0x80}},                 //CE - Low
    //*******ce end********//


    {0x00,  1,  {0x00}},                 //Orise mode disable
    {0xff,  3,  {0xff,0xff,0xff}},

    {0x35, 1,  {0X00}},

    {0x11, 1,  {0X00}},
    {REGFLAG_DELAY, 200, {}},

    // Display On
    {0x29, 1,  {0X00}},
    {REGFLAG_DELAY, 120, {}},
};
static struct LCM_setting_table lcm_sleep_out_setting[] = {
    // Sleep Out
	{0x11, 0, {0x00}},
    {REGFLAG_DELAY, 120, {}},

    // Display ON
	{0x29, 0, {0x00}},
	{REGFLAG_DELAY, 10, {}},

	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_sleep_mode_in_setting[] = {
	// Display off sequence
	{0x28, 0, {0x00}},
	{REGFLAG_DELAY, 120, {}},

    // Sleep Mode On
	{0x10, 0, {0x00}},
	{REGFLAG_DELAY, 200, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};
static struct LCM_setting_table lcm_compare_id_setting[] = {
	// Display off sequence
	{0xF0,	5,	{0x55, 0xaa, 0x52,0x08,0x00}},
	{REGFLAG_DELAY, 10, {}},

	{REGFLAG_END_OF_TABLE, 0x00, {}}
};

//static int vcom = 0x7e;
static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
	unsigned int i;

    for(i = 0; i < count; i++) {

        unsigned cmd;
        cmd = table[i].cmd;

        switch (cmd) {

//            case  0xd9:
//                table[i].para_list[0] = vcom;
//				dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
//                vcom+=2;
//                break;
            case REGFLAG_DELAY :
                MDELAY(table[i].count);
                break;

            case REGFLAG_END_OF_TABLE :
                break;

            default:
				dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
				//MDELAY(2);
       	}
    }
}

// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}


static void lcm_get_params(LCM_PARAMS *params)
{
	memset(params, 0, sizeof(LCM_PARAMS));

	params->type = LCM_TYPE_DSI;

	params->width = FRAME_WIDTH;
	params->height = FRAME_HEIGHT;

	// enable tearing-free
	params->dbi.te_mode = LCM_DBI_TE_MODE_DISABLED;
	params->dbi.te_edge_polarity = LCM_POLARITY_RISING;

	params->dsi.mode   = SYNC_PULSE_VDO_MODE; //SYNC_PULSE_VDO_MODE;//BURST_VDO_MODE;

	// DSI
	/* Command mode setting */
	params->dsi.LANE_NUM = LCM_FOUR_LANE;
	//The following defined the fomat for data coming from LCD engine.
	params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
	params->dsi.data_format.trans_seq = LCM_DSI_TRANS_SEQ_MSB_FIRST;
	params->dsi.data_format.padding = LCM_DSI_PADDING_ON_LSB;
	params->dsi.data_format.format = LCM_DSI_FORMAT_RGB888;

	params->dsi.intermediat_buffer_num = 0;	//because DSI/DPI HW design change, this parameters should be 0 when video mode in MT658X; or memory leakage

	params->dsi.PS = LCM_PACKED_PS_24BIT_RGB888;
	params->dsi.word_count = 720 * 3;


    params->dsi.vertical_sync_active                = 4;
    params->dsi.vertical_backporch                  = 16;
    params->dsi.vertical_frontporch                 = 14;
    params->dsi.vertical_active_line                = FRAME_HEIGHT;
    params->dsi.horizontal_sync_active              = 10;
    params->dsi.horizontal_backporch                = 64;
    params->dsi.horizontal_frontporch               = 64;
   // params->dsi.horizontal_blanking_pixel              = 60;
    params->dsi.horizontal_active_pixel            = FRAME_WIDTH;

#ifdef VANZO_LCM_ESD_CHECK_SUPPORT
    params->dsi.esd_check_enable = 1;
    params->dsi.customization_esd_check_enable = 0;
    params->dsi.lcm_esd_check_table[0].cmd          = 0x0A;
    params->dsi.lcm_esd_check_table[0].count        = 1;
    params->dsi.lcm_esd_check_table[0].para_list[0] = 0x9C;
#endif
    params->dsi.ssc_disable = 1;  // ssc disable control (1: disable, 0: enable, default: 0)
    params->dsi.ssc_range = 4;  // ssc range control (1:min, 8:max, default: 4)
    // Bit rate calculation
    params->dsi.PLL_CLOCK=200;//227;//254(124 0k)//247  //220

}

static void lcm_init(void)
{
    SET_RESET_PIN(1);
	MDELAY(5);
    SET_RESET_PIN(0);
    MDELAY(20);
    SET_RESET_PIN(1);
    MDELAY(100);
	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}

static void lcm_suspend(void)
{
    SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(5);
	SET_RESET_PIN(1);
	MDELAY(20);
	push_table(lcm_sleep_mode_in_setting, sizeof(lcm_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
}

static void lcm_resume(void)
{
    lcm_init();
}

static unsigned int lcm_compare_id(void)
{
	unsigned int id0, id1, id2, id3, id4;
	unsigned char buffer[5];
	unsigned int array[5];

	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(50);
	SET_RESET_PIN(1);
	MDELAY(120);

	// Set Maximum return byte = 1
	array[0] = 0x00053700;
	dsi_set_cmdq(array, 1, 1);

	read_reg_v2(0xA1, buffer, 5);
	id0 = buffer[0];
	id1 = buffer[1];
	id2 = buffer[2];
	id3 = buffer[3];
	id4 = buffer[4];

#if defined(BUILD_LK)
	printf("%s, Module ID = {%x, %x, %x, %x, %x} \n", __func__, id0,
	       id1, id2, id3, id4);
#else
	printk("%s, Module ID = {%x, %x, %x, %x,%x} \n", __func__, id0,
	       id1, id2, id3, id4);
#endif

	return (LCM_OTM1283_ID == ((id2 << 8) | id3)) ? 1 : 0;
}

static unsigned int lcm_esd_check(void)
{
#ifndef BUILD_LK
    char  buffer[3];
    int   array[4];

	if(lcm_esd_test)
	{
		lcm_esd_test = FALSE;
		return TRUE;
	}

	array[0] = 0x00013700;
	dsi_set_cmdq(array, 1, 1);

	read_reg_v2(0x36, buffer, 1);
	if(buffer[0]==0x90)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
 #endif

}

static unsigned int lcm_esd_recover(void)
{
	lcm_init();
	lcm_resume();

	return TRUE;
}


LCM_DRIVER otm1283a_lg55_qiji_hd_lcm_drv =
{
    .name			= "otm1283a_lg55_qiji_hd",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	.compare_id     = lcm_compare_id,
    .init_power        = lcm_init_power,
    .resume_power = lcm_resume_power,
    .suspend_power = lcm_suspend_power,
	//.esd_check = lcm_esd_check,
	//.esd_recover = lcm_esd_recover,
    };
