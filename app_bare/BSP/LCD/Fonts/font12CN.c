/**
  ******************************************************************************
  * @file    Font12.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-February-2014
  * @brief   This file provides text Font12 for STM32xx-EVAL's LCD driver. 
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "fonts.h"


// 
//  Font data for Courier New 12pt
// 

#if ENABLE_Font_12CN == 1
const CH_CN Font12CN_Table[] =
        {
/*--  ����:  ��  --*/
/*--  ΢���ź�12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x21   --*/
                {{"��"}, {
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1D, 0xC0, 0x1D, 0x80, 0x3B, 0xFF, 0x3B, 0x07,
                                                                                                                                0x3F, 0x77, 0x7E, 0x76, 0xF8, 0x70, 0xFB, 0xFE, 0xFB, 0xFE, 0x3F, 0x77, 0x3F, 0x77, 0x3E, 0x73,
                                0x38, 0x70, 0x38, 0x70, 0x3B, 0xE0, 0x00, 0x00, 0x00, 0x00}},

/*--  ����:  ��  --*/
/*--  ΢���ź�12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x21   --*/
                {{"��"}, {
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x73, 0xFF, 0x70, 0x0F, 0xFE, 0x1E,
                                                                                                                                0x7E, 0x3C, 0x6E, 0x38, 0xEE, 0x30, 0xEF, 0xFF, 0xFC, 0x30, 0x7C, 0x30, 0x38, 0x30, 0x3E, 0x30,
                                0x7E, 0x30, 0xE0, 0x30, 0xC1, 0xF0, 0x00, 0x00, 0x00, 0x00}},

/*--  ����:  ��  --*/
/*--  ΢���ź�12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x21   --*/
                {{"��"}, {
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0E, 0x30, 0x0E, 0x3F, 0xEE, 0x30, 0xEE,
                                                                                                                                0xFC, 0xFF, 0x76, 0xCE, 0x77, 0xFE, 0x7B, 0xFE, 0xFF, 0xFE, 0xF3, 0xDE, 0xF3, 0xCE, 0x37, 0xEE,
                                0x3E, 0x6E, 0x3C, 0x0E, 0x30, 0x3E, 0x00, 0x00, 0x00, 0x00}},

/*--  ����:  ݮ  --*/
/*--  ΢���ź�12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x21   --*/
                {{"ݮ"}, {
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x70, 0xFF, 0xFF, 0x3E, 0x70, 0x38, 0x00,
                                                                                                                                0x7F, 0xFF, 0xE0, 0x00, 0xFF, 0xFC, 0x3B, 0x8C, 0x39, 0xCC, 0xFF, 0xFF, 0x73, 0x9C, 0x71, 0xDC,
                                0x7F, 0xFF, 0x00, 0x1C, 0x01, 0xF8, 0x00, 0x00, 0x00, 0x00}},

/*--  ����:  ��  --*/
/*--  ΢���ź�12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x21   --*/
                {{"��"}, {
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x1F, 0xFF, 0xF0, 0x3E, 0x00, 0x0E, 0x1F,
                                                                                                                                0xCF, 0xFB, 0xFF, 0xF8, 0x3F, 0xFF, 0x0F, 0xFF, 0x7F, 0xD8, 0x7F, 0xDC, 0x6F, 0xCE, 0xED, 0xFF,
                                0xFD, 0xF7, 0xF9, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},

/*--  ����:  a  --*/
/*--  ΢���ź�12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x21   --*/
                {{"a"}, {
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                                                                                0x3E, 0x00, 0x67, 0x00, 0x07, 0x80, 0x0F, 0x80, 0x7F, 0x80, 0xE3, 0x80, 0xE7, 0x80, 0xE7, 0x80,
                                0x7F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},

/*--  ����:  b  --*/
/*--  ΢���ź�12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x21   --*/
                {{"b"}, {
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x70, 0x00, 0x70, 0x00, 0x70, 0x00,
                                                                                                                                0x7F, 0x00, 0x7B, 0x80, 0x71, 0xC0, 0x71, 0xC0, 0x71, 0xC0, 0x71, 0xC0, 0x71, 0xC0, 0x7B, 0x80,
                                0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},

/*--  ����:  c  --*/
/*--  ΢���ź�12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x21   --*/
                {{"c"}, {
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                                                                                0x3F, 0x00, 0x73, 0x00, 0xF0, 0x00, 0xE0, 0x00, 0xE0, 0x00, 0xE0, 0x00, 0xF0, 0x00, 0x73, 0x00,
                                0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},

/*--  ����:  A  --*/
/*--  ΢���ź�12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x21   --*/
                {{"A"}, {
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x1F, 0x00, 0x1F, 0x00,
                                                                                                                                0x1F, 0x00, 0x3B, 0x80, 0x3B, 0x80, 0x71, 0x80, 0x7F, 0xC0, 0x71, 0xC0, 0xE0, 0xE0, 0xE0, 0xE0,
                                0xE0, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
                {{"��"},
                        {
                                0x00, 0x00, 0x08, 0x20, 0x49, 0x20, 0x6B, 0x20, 0x2A, 0x40, 0x08, 0x7E, 0xFF, 0x44, 0x18, 0x44, 0x3C, 0xC4, 0x6E, 0xC4, 0x8A, 0x4C, 0x10, 0x28, 0x10, 0x28, 0xFE, 0x28, 0x62, 0x38, 0x66, 0x10,
                                0x34, 0x38, 0x1C, 0x28, 0x1E, 0x4C, 0x63, 0xC4, 0xC1, 0x02,    //  [0], "��",Size:42, W16��H21, addr:0x0000
                        }},
                {{"��"},
                        {
                                0x00, 0x00, 0x20, 0x00, 0x23, 0xFE, 0x22, 0x02, 0x22, 0x02, 0xFA, 0x02, 0x23, 0xFE, 0x22, 0x12, 0x22, 0x10, 0x22, 0x10, 0x2B, 0xFF, 0x72, 0x10, 0xA2, 0x10, 0x22, 0x10, 0x23, 0xFE, 0x27, 0x02,
                                0x25, 0x02, 0x25, 0x02, 0x2D, 0xFE, 0xE9, 0x02, 0x00, 0x00,    //  [1], "��",Size:42, W16��H21, addr:0x002A
                        }},
                {{"��"},
                        {
                                0x00, 0x00, 0x00, 0x00, 0x7F, 0xFC, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x3F, 0xFC, 0x20, 0x04, 0x20, 0x00, 0x20, 0x00, 0x20, 0x01, 0x20, 0x01, 0x20, 0x01,
                                0x20, 0x01, 0x20, 0x02, 0x1F, 0xFE, 0x00, 0x00, 0x00, 0x00,    //  [2], "��",Size:42, W16��H21, addr:0x0054
                        }},
                {{"д"},
                        {
                                0x00, 0x00, 0x00, 0x00, 0x7F, 0xFE, 0x40, 0x02, 0x48, 0x02, 0x08, 0x00, 0x1F, 0xFC, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x1F, 0xFC, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0xFF, 0xF4, 0x00, 0x04,
                                0x00, 0x04, 0x00, 0x08, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00,    //  [3], "д",Size:42, W16��H21, addr:0x007E
                        }},
                {{"��"},
                        {
                                0x00, 0x00, 0x04, 0x00, 0x02, 0x00, 0x03, 0x00, 0x01, 0x00, 0x01, 0x80, 0x01, 0x80, 0x01, 0xC0, 0x01, 0x40, 0x01, 0x40, 0x03, 0x60, 0x02, 0x20, 0x02, 0x20, 0x04, 0x10, 0x04, 0x10, 0x08, 0x08,
                                0x18, 0x08, 0x10, 0x04, 0x20, 0x06, 0x40, 0x03, 0x00, 0x00,    //  [4], "��",Size:42, W16��H21, addr:0x00A8
                        }},
                {{"U"},
                        {
                                0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1C, 0x38, 0x0C, 0x30, 0x0F, 0xF0,
                                0x03, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    //  [5], "U",Size:42, W16��H21, addr:0x00D2
                        }},
                {{"��"},
                        {
                                0x01, 0x00, 0x01, 0x00, 0x1F, 0xFC, 0x11, 0x04, 0x11, 0x04, 0x10, 0x84, 0x7F, 0xFF, 0x10, 0x04, 0x12, 0x04, 0x31, 0x04, 0x20, 0xF8, 0x40, 0x00, 0x3F, 0xFC, 0x22, 0x44, 0x22, 0x44, 0x22, 0x44,
                                0x22, 0x44, 0x22, 0x44, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,    //  [6], "��",Size:42, W16��H21, addr:0x00FC
                        }}
        };

cFONT Font12CN = {
        Font12CN_Table,
        sizeof(Font12CN_Table) / sizeof(CH_CN),  /*size of table*/
        11, /* ASCII Width */
        16, /* Width */
        21, /* Height */
};
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/