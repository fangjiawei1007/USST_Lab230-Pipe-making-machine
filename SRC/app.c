#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

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

#include "includes.h"
extern INTERALMEMORY g_InteralMemory;

//extern GLOBALVAR g_GlobalVar;

//extern INT16U PreScrNum;


extern  int InPulseCount[7];		//记录6路编码器输入的脉冲数

extern int OutPulse_PreSet[4];	// 3路脉冲输出的  脉冲预设值
extern volatile unsigned int OutPulse_Counter[4];	// 3路脉冲输出的  当前输出脉冲数

///////////添加的变量///////////////


volatile unsigned char init_zero_check;
volatile U8 init_zero_start;  //开始回零标志位



///////////英文版//////////
unsigned char flag_english=0,model_work;
volatile unsigned char  backward_flag;  //跟踪到位标志位
volatile unsigned char forward_flag;
volatile unsigned int postion_cut_pulse,position_back_pulse,position_dingchang_pulse;
volatile unsigned char knife_finish_flag,knife_start_flag,sebiao_start_flag;
volatile unsigned char start_flag;
unsigned char change_flag=1;
volatile unsigned char init_status=0;
unsigned char start_times;
unsigned char back_finish_flag=0;	//返回时，未检测到零位信号标志位
///////////////////////////////////////////////////////////////
//////	初始化     //////////////////////////////////////////
//////	每次上电App_Init 只执行一次	 //////////////////
//////////////////////////////////////////////////////////////
void App_Init(void)
{
	int i ;
	
	X_In_Init();
	Y_Out_Init();
	Pulse_In_Init();
	Pulse_Out_Init();
	parameter_init();
	
	signal1 = Get_X_Value(2);	//上电获得B相信号为之后滤波使用,好像也没啥用
//	DA0_10_Init();

	// 脉冲输入初始化
	for(i=0; i<7; i++)
	{
		InPulseCount[i] = 0;
	}
	
	big_value_init();
  	fn_Button_init(&Lang_Chinese_kb,&Lang_English_kb,&flag_english);
	parameter_input();
	
}
///////////////////////////////////////////////////////////////
//////	应用主程序     //////////////////////////////////////////
//////	系统每个循环会调用一次App_Operation	 //////////////////
//////////////////////////////////////////////////////////////
void App_Operation(void)
{  		
	//static int back_pulse_counter=0;
	/**
		按键互斥，每次上电执行一次

		powered by 方佳伟
	**/
	if (change_flag==1){
		fn_Button_init(&Lang_Chinese_kb,&Lang_English_kb,&flag_english);
		fn_Button_init(&model_SeBiao_kb,&model_DingChang_kb,&model_work);
		change_flag=0;
	}
  	
	/**
		色标模式打开，model_display_b使用了多状态指示灯控件;
		并且打开X11的中断，定长模式下关闭X11中断

		powered by 方佳伟
	**/
  	if( model_work == model_sebiao && sebiao_status==0){					//操作模式显示
		model_display_b = model_sebiao;
		InPulse11_Start();
	}
	else if(model_work == model_DingChang && sebiao_status==1){
		model_display_b = model_DingChang;
		InPulse11_Stop();
	}
	
	/**
		选择语言Lang_select == language select

		powered by 方佳伟
	**/
  	if(Lang_select_confirm_b==1)
  	{
  	   Lang_select_confirm_b = 0;  	   
  	   if( flag_english == Lang_Chinese_Version )
  	   		g_InteralMemory.Word[100]=5; 	
  	   else if( flag_english == Lang_English_Version )
  	   		g_InteralMemory.Word[100]=55;   
  	}
	
	
	if (return_b==1)
	{
		if( flag_english == Lang_Chinese_Version )
  	   		g_InteralMemory.Word[100]=5; 	
  	   else if( flag_english == Lang_English_Version )
  	   		g_InteralMemory.Word[100]=55;  
	}
	
	
	/**
		整机停下之后需要不停扫描此函数来判断是否进行参数的修改。
		powered by 方佳伟
	**/	
	if (forward_flag==0&&backward_flag==0||start_b==0)
	{
		parameter_input();
	}
	
	/************************************************************
		上电回零点判断,首先看中断函数 InPulse10 零位传感器;
		init_zero_start，init_zero_check为全局变量，初始化为0
	*************************************************************/
 	
	/*此处保证上电的时候start_button不能够被按下*/
	if (init_zero_start==1)
		start_b=0;
 	if (init_zero_check==0 && start_b==0 || init_status==0){
		init_zero_start=1;
		start_flag=0;
		//零点处 传感器--外部挡住输出低电平灯亮1 （内部0） ；  不挡住输出高电平灯灭0 （内部1）
		
		fn_init_proc();
		
	}
	/************************************************************/
	
	/************************************************************
							点动控制
	*************************************************************/
	fn_jog();
	fn_warning();

	/************************************************************
							位置显示
	*************************************************************/
	if (pulse_get_num>=0)
		current_knife_position_kw=pulse_get_num*mm_per_pulse;
	else
		current_knife_position_kw=0;
	
	
	/************************************************************
							正常启动
	*************************************************************/
	if((start_b==1) && (jog_status[0]==0)&& (jog_status[1]==0) &&init_zero_check==1){  // 启动
		start_times=1;
		
		//托板返回	/**切刀动作完成之后托板返回backward == 1**/	
		if(backward_flag==1){
			for (;pulse_send_num>0;pulse_send_num--){//
				
				/**当托板返回到达了之后，不发脉冲直接break**/
				if (backward_flag==0)
					break;
				
				/**托板返回时加减速**/
				else if (pulse_send_num>(pulse_send_num_record-50)){
					rGPBDAT |=(1<<1);
					Delay(2500+back_fre_kw);
					rGPBDAT &=(~(1<<1));
					Delay(2500+back_fre_kw);
				}
				else if (pulse_send_num>(pulse_send_num_record-150)){
					rGPBDAT |=(1<<1);
					Delay(1500+back_fre_kw);
					rGPBDAT &=(~(1<<1));
					Delay(1500+back_fre_kw);
				}
				else if (pulse_send_num>150){
					rGPBDAT |=(1<<1);
					Delay(500+back_fre_kw);
					rGPBDAT &=(~(1<<1));
					Delay(500+back_fre_kw);
				}
				else if (pulse_send_num>50){
					rGPBDAT |=(1<<1);
					Delay(1500+back_fre_kw);
					rGPBDAT &=(~(1<<1));
					Delay(1500+back_fre_kw);
				}
				else if (pulse_send_num>20){
					rGPBDAT |=(1<<1);
					Delay(3500+back_fre_kw);
					rGPBDAT &=(~(1<<1));
					Delay(3500+back_fre_kw);
				}
				else{
					rGPBDAT |=(1<<1);
					Delay(200000);
					rGPBDAT &=(~(1<<1));
					Delay(200000);
				}
			}
			
			/**正常的流程为此处回到零点之后，进入X10中断程序**/
			
			/**此处是为了防止零位传感器出问题导致无法进入中断函数进行清零，换向操作**/
			if (pulse_send_num<=0){	//2017-2-24
				if (back_finish_flag != 1){	//2017-2-24
					backward_flag=0;
					backward_status=0;
					pulse_get_num=0;
					
					Set_Y_Value (6,turn_right);  //刀-----右方向
					Set_Y_Value (5,turn_right);  //刀-----右方向
					
				}
				
			}
		}
		
		//切刀动作	/**切刀动作开始 forward == 1, backward == 0**/	
		if (knife_start_flag==knife_start){
			 for(;pulse_knife_record>0;pulse_knife_record--){
				/**切刀加减速发脉冲，所以外部使用返回频率以及旋转刀频率的时候数值越大越慢(Delay())**/
				 if(pulse_knife_record>(knife_pulse_num_kw-50)){
					Set_Y_Value(2,1);
					Delay(1500+(knife_fre_kw*10));
					Set_Y_Value(2,0);
					Delay(1500+(knife_fre_kw*10));
				}
				else if(pulse_knife_record>50){
					Set_Y_Value(2,1);
					Delay(500+(knife_fre_kw*10));
					Set_Y_Value(2,0);
					Delay(500+(knife_fre_kw*10));
				}
				else{
					Set_Y_Value(2,1);
					Delay(1500+(knife_fre_kw*10));
					Set_Y_Value(2,0);
					Delay(1500+(knife_fre_kw*10));
				}
			}
			knife_finish_flag=knife_finish;
		} 
		
		/**完成切管子动作之后，切刀停止标志位置位，总产量++，签到产量++**/
		if(knife_start_flag==knife_start && knife_finish_flag==knife_finish) 
		{
			zongchanliang_kw++;
			if (zongchanliang_kw>=10000){
				zongchanliang_upper_kw++;
				zongchanliang_kw=0;
			}
			if(qiandao_b==1)
			{
				dangbanchanliang_kw++;
				if (dangbanchanliang_kw>=10000)
				{
					dangbanchanliang_upper_kw++;
					dangbanchanliang_kw=0;
				}
			}	
			knife_start_flag=knife_stop;	
		}
	}
	
	/**制管机没有暂停，只有停止，停止了之后需要回零继续开始**/
	else if (start_b==0 && start_times==1){
		PulseOut_1_Stop();
		PulseOut_2_Stop();
		init_zero_check=0;
		init_flag_cover=0;
		init_flag_uncover=0;
		start_flag=0;
		start_times=0;
	}
}

void clear_all ()   ///报错 要调用他  ////////清零复位函数
{
	 
	start_b=0;//启动按钮 关闭
	forward_flag=0;
	backward_flag=0;
	backward_status=0;
	start_flag=1;
	init_flag_cover=0;
	init_flag_uncover=0;
	// InPulse1_Stop();
	// InPulse10_Stop();
	// InPulse11_Stop();
	//InPulse12_Stop(); 
	//InPulse13_Stop();
	init_zero_check=0;
	current_knife_position_kw =0; 
}
/***********************************************************************************************
*函数名 ： void big_value_init(void)
*函数功能描述 ： 上电判断是否为大值(不可能值)，若为大值，则将其初始化为小值以防止事故发生。
*函数功能逻辑：  简单判断
*函数参数 ： void
*函数返回值 ： void
*作者 ：王德铭
*函数创建日期 ： 2016
*函数修改日期 ： 2017
*修改人 ：方佳伟
*修改原因 ： 修改注释
*版本 ： V2.2.0
*历史版本 ： 无
***********************************************************************************************/
void big_value_init(void)
{
	
	if ( position_cut_kw > 60000 ) //切割点位置
		position_cut_kw = 23;
	
	if ( init_fre_kw > 60000 ) //找零点频率
		init_fre_kw = 500;
	/* if ( back_fre_kw > 10000 ) //返回速度
		back_fre_kw = 100; */

	if ( knife_fre_kw > 60000 ) //旋转刀频率
		knife_fre_kw = 3000;

	if ( position_back_kw > 60000 ) //返回点位置
		position_back_kw = 63;
  
	current_knife_position_kw = 0;			//刀具位置

	 
	if ( jog_fre_kw > 60000 ) //点动频率
		jog_fre_kw = 500;
 
	if ( knife_pulse_num_kw > 60000 ) //旋转刀发的脉冲
		knife_pulse_num_kw = 250;
		
	if ( bmq_xianshu_kw > 60000 )//编码器的线数
		bmq_xianshu_kw = 2000;
		
	if ( bmq_D_kw > 60000 )//滚筒直径
		bmq_D_kw = 360;
	//////////////////////		



}


