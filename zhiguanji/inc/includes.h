#ifndef _INCLUDES_H_
#define	_INCLUDES_H_
#include "def.h"
#include "2416addr.h"
#include "2416lib.h"
#include "option.h"
#include "LCD.h"
#include "mmu.h"
#include "Nand.h"
#include "..\inc\download.h"
#include "..\inc\systemconf.h"
#include "..\inc\systemmenu.h"
#include "..\inc\function.h"
#include "..\inc\jiema.h"
#include "..\inc\communication.h"
#include "..\inc\test.h"
#include "..\inc\Rtc.h"
#include "..\inc\encrypt.h"

#include "..\inc\app.h"
#include "..\inc\appBase.h"


#include"basic.h"

#define	servo_pulse_kw			10000

extern INTERALMEMORY g_InteralMemory;



extern unsigned char signal;
extern volatile unsigned char init_zero_check;
extern volatile U8 init_zero_start;  
extern unsigned char flag_english;
extern	unsigned int init_delay_flag;
extern	unsigned int init_flag_cover;
extern	unsigned int init_flag_uncover;
//extern	volatile int motor_fac;
extern	int k_motor;
extern	volatile unsigned char forward_flag;
extern	volatile unsigned char backward_flag;
extern	volatile unsigned int postion_cut_pulse;
extern	volatile unsigned int position_back_pulse;
extern	volatile unsigned char knife_finish_flag;
extern	volatile unsigned char knife_start_flag;
extern	volatile unsigned char backward_status;
extern	volatile int pulse_get_num;
extern	volatile int pulse_send_num;
extern	volatile unsigned int position_dingchang_pulse;
extern	volatile unsigned char sebiao_start_flag;
extern	volatile unsigned char start_flag;
extern	unsigned char change_flag;
extern 	unsigned char model_work;
extern	short  jog_status[2];
extern	volatile unsigned char init_status;
extern	float	mm_per_pulse;
extern	unsigned char sebiao_status;
//extern	volatile unsigned char error_times;
extern	volatile int pulse_send_num_record;
extern	volatile int pulse_knife_record;
extern	volatile int pulse_get_total_num;
extern	int motor_fac;
extern	unsigned char back_finish_flag;
extern unsigned char signal1;








#define	guanliyuan_login_b			g_InteralMemory.Bit[3]
#define	zongchangliang_qingling_b	g_InteralMemory.Bit[10]
#define	dangban_qingling_b			g_InteralMemory.Bit[11]
#define	next_sebiao_b				g_InteralMemory.Bit[12]







#endif