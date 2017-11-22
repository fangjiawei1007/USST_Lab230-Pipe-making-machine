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


extern  int InPulseCount[7];		//��¼6·�����������������

extern int OutPulse_PreSet[4];	// 3·���������  ����Ԥ��ֵ
extern volatile unsigned int OutPulse_Counter[4];	// 3·���������  ��ǰ���������

///////////��ӵı���///////////////


volatile unsigned char init_zero_check;
volatile U8 init_zero_start;  //��ʼ�����־λ



///////////Ӣ�İ�//////////
unsigned char flag_english=0,model_work;
volatile unsigned char  backward_flag;  //���ٵ�λ��־λ
volatile unsigned char forward_flag;
volatile unsigned int postion_cut_pulse,position_back_pulse,position_dingchang_pulse;
volatile unsigned char knife_finish_flag,knife_start_flag,sebiao_start_flag;
volatile unsigned char start_flag;
unsigned char change_flag=1;
volatile unsigned char init_status=0;
unsigned char start_times;
unsigned char back_finish_flag=0;	//����ʱ��δ��⵽��λ�źű�־λ
///////////////////////////////////////////////////////////////
//////	��ʼ��     //////////////////////////////////////////
//////	ÿ���ϵ�App_Init ִֻ��һ��	 //////////////////
//////////////////////////////////////////////////////////////
void App_Init(void)
{
	int i ;
	
	X_In_Init();
	Y_Out_Init();
	Pulse_In_Init();
	Pulse_Out_Init();
	parameter_init();
	
	signal1 = Get_X_Value(2);	//�ϵ���B���ź�Ϊ֮���˲�ʹ��,����Ҳûɶ��
//	DA0_10_Init();

	// ���������ʼ��
	for(i=0; i<7; i++)
	{
		InPulseCount[i] = 0;
	}
	
	big_value_init();
  	fn_Button_init(&Lang_Chinese_kb,&Lang_English_kb,&flag_english);
	parameter_input();
	
}
///////////////////////////////////////////////////////////////
//////	Ӧ��������     //////////////////////////////////////////
//////	ϵͳÿ��ѭ�������һ��App_Operation	 //////////////////
//////////////////////////////////////////////////////////////
void App_Operation(void)
{  		
	//static int back_pulse_counter=0;
	/**
		�������⣬ÿ���ϵ�ִ��һ��

		powered by ����ΰ
	**/
	if (change_flag==1){
		fn_Button_init(&Lang_Chinese_kb,&Lang_English_kb,&flag_english);
		fn_Button_init(&model_SeBiao_kb,&model_DingChang_kb,&model_work);
		change_flag=0;
	}
  	
	/**
		ɫ��ģʽ�򿪣�model_display_bʹ���˶�״ָ̬ʾ�ƿؼ�;
		���Ҵ�X11���жϣ�����ģʽ�¹ر�X11�ж�

		powered by ����ΰ
	**/
  	if( model_work == model_sebiao && sebiao_status==0){					//����ģʽ��ʾ
		model_display_b = model_sebiao;
		InPulse11_Start();
	}
	else if(model_work == model_DingChang && sebiao_status==1){
		model_display_b = model_DingChang;
		InPulse11_Stop();
	}
	
	/**
		ѡ������Lang_select == language select

		powered by ����ΰ
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
		����ͣ��֮����Ҫ��ͣɨ��˺������ж��Ƿ���в������޸ġ�
		powered by ����ΰ
	**/	
	if (forward_flag==0&&backward_flag==0||start_b==0)
	{
		parameter_input();
	}
	
	/************************************************************
		�ϵ������ж�,���ȿ��жϺ��� InPulse10 ��λ������;
		init_zero_start��init_zero_checkΪȫ�ֱ�������ʼ��Ϊ0
	*************************************************************/
 	
	/*�˴���֤�ϵ��ʱ��start_button���ܹ�������*/
	if (init_zero_start==1)
		start_b=0;
 	if (init_zero_check==0 && start_b==0 || init_status==0){
		init_zero_start=1;
		start_flag=0;
		//��㴦 ������--�ⲿ��ס����͵�ƽ����1 ���ڲ�0�� ��  ����ס����ߵ�ƽ����0 ���ڲ�1��
		
		fn_init_proc();
		
	}
	/************************************************************/
	
	/************************************************************
							�㶯����
	*************************************************************/
	fn_jog();
	fn_warning();

	/************************************************************
							λ����ʾ
	*************************************************************/
	if (pulse_get_num>=0)
		current_knife_position_kw=pulse_get_num*mm_per_pulse;
	else
		current_knife_position_kw=0;
	
	
	/************************************************************
							��������
	*************************************************************/
	if((start_b==1) && (jog_status[0]==0)&& (jog_status[1]==0) &&init_zero_check==1){  // ����
		start_times=1;
		
		//�а巵��	/**�е��������֮���а巵��backward == 1**/	
		if(backward_flag==1){
			for (;pulse_send_num>0;pulse_send_num--){//
				
				/**���а巵�ص�����֮�󣬲�������ֱ��break**/
				if (backward_flag==0)
					break;
				
				/**�а巵��ʱ�Ӽ���**/
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
			
			/**����������Ϊ�˴��ص����֮�󣬽���X10�жϳ���**/
			
			/**�˴���Ϊ�˷�ֹ��λ�����������⵼���޷������жϺ����������㣬�������**/
			if (pulse_send_num<=0){	//2017-2-24
				if (back_finish_flag != 1){	//2017-2-24
					backward_flag=0;
					backward_status=0;
					pulse_get_num=0;
					
					Set_Y_Value (6,turn_right);  //��-----�ҷ���
					Set_Y_Value (5,turn_right);  //��-----�ҷ���
					
				}
				
			}
		}
		
		//�е�����	/**�е�������ʼ forward == 1, backward == 0**/	
		if (knife_start_flag==knife_start){
			 for(;pulse_knife_record>0;pulse_knife_record--){
				/**�е��Ӽ��ٷ����壬�����ⲿʹ�÷���Ƶ���Լ���ת��Ƶ�ʵ�ʱ����ֵԽ��Խ��(Delay())**/
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
		
		/**����й��Ӷ���֮���е�ֹͣ��־λ��λ���ܲ���++��ǩ������++**/
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
	
	/**�ƹܻ�û����ͣ��ֻ��ֹͣ��ֹͣ��֮����Ҫ���������ʼ**/
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

void clear_all ()   ///���� Ҫ������  ////////���㸴λ����
{
	 
	start_b=0;//������ť �ر�
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
*������ �� void big_value_init(void)
*������������ �� �ϵ��ж��Ƿ�Ϊ��ֵ(������ֵ)����Ϊ��ֵ�������ʼ��ΪСֵ�Է�ֹ�¹ʷ�����
*���������߼���  ���ж�
*�������� �� void
*��������ֵ �� void
*���� ��������
*������������ �� 2016
*�����޸����� �� 2017
*�޸��� ������ΰ
*�޸�ԭ�� �� �޸�ע��
*�汾 �� V2.2.0
*��ʷ�汾 �� ��
***********************************************************************************************/
void big_value_init(void)
{
	
	if ( position_cut_kw > 60000 ) //�и��λ��
		position_cut_kw = 23;
	
	if ( init_fre_kw > 60000 ) //�����Ƶ��
		init_fre_kw = 500;
	/* if ( back_fre_kw > 10000 ) //�����ٶ�
		back_fre_kw = 100; */

	if ( knife_fre_kw > 60000 ) //��ת��Ƶ��
		knife_fre_kw = 3000;

	if ( position_back_kw > 60000 ) //���ص�λ��
		position_back_kw = 63;
  
	current_knife_position_kw = 0;			//����λ��

	 
	if ( jog_fre_kw > 60000 ) //�㶯Ƶ��
		jog_fre_kw = 500;
 
	if ( knife_pulse_num_kw > 60000 ) //��ת����������
		knife_pulse_num_kw = 250;
		
	if ( bmq_xianshu_kw > 60000 )//������������
		bmq_xianshu_kw = 2000;
		
	if ( bmq_D_kw > 60000 )//��Ͳֱ��
		bmq_D_kw = 360;
	//////////////////////		



}


