#include"includes.h"

unsigned int init_flag_cover,init_flag_uncover;
unsigned int init_delay_flag;
short  jog_status[2]={0};
unsigned char error_warning;
INT16U previous_fre[3];
float	mm_per_pulse;
INT16U bmq_D,bmq_xianshu,position_cut,position_back,dingchangchangdu,forward_speed_fac;

/***********************************************************************************************
*函数名 ： parameter_init(void)
*函数功能描述 ： 初始化参数，内存擦除之后只会执行一次，之后上电均不会继续进入该函数。
*函数功能逻辑：  init_flag_kb类型为kb，那么nand被擦除之后，上电第一次就会把init_flag_kb置1，
			    那么之后上电均不会进入该函数。
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
void parameter_init(void){
	if (init_flag_kb!=1){
		position_cut_kw=10;
		init_fre_kw=500;
		pulse_get_total_num=0;
		back_fre_kw=10000;
		position_back_kw=55;
		current_knife_position_kw=0;
		dingchangchangdu_kw=150;
		forward_speed_fac_kw=7000;
		jog_fre_kw=500;
		turn_right=1;
		knife_fre_kw=5000;
		bmq_xianshu_kw=2000;
		bmq_D_kw=3400;
		zongchanliang_kw=0;
		zongchanliang_upper_kw=0;
		dangbanchanliang_kw=0;
		dangbanchanliang_upper_kw=0;
		gangdaigengzong_kb = 0;
		gangdaixishu_kw = 500;
		init_flag_kb=1;
	}
}

/***********************************************************************************************
*函数名 ： parameter_input(void)
*函数功能描述 ： 主要根据输入的参数计算脉冲数，以及跟踪系数k的赋值
*函数功能逻辑：  bmq_D——编码器直径;bmq_xianshu——编码器线数;mm_per_pulse——每个脉冲多少mm,
				之后根据各个段的长度计算各个阶段所需要的脉冲数,上电的时候进行一次判断是
				否在上次关机之前有修改，如果有修改就重新进行计算。
				k_motor——即跟踪系数k值
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
void parameter_input(void){
	
	/**
		bmq_D——编码器直径
		bmq_xianshu——编码器线数
		mm_per_pulse——每个脉冲多少mm

		之后根据各个段的长度计算各个阶段所需要的脉冲数
	**/
	if (bmq_D!=bmq_D_kw || bmq_xianshu!=bmq_xianshu_kw){
		mm_per_pulse=bmq_D_kw/100.0*PI/bmq_xianshu_kw;
		position_back_pulse=position_back_kw/mm_per_pulse;
		postion_cut_pulse=position_cut_kw/mm_per_pulse;
		position_dingchang_pulse=dingchangchangdu_kw/mm_per_pulse;
		bmq_D=bmq_D_kw;
		bmq_xianshu=bmq_xianshu_kw;
	}
	
	
	if (position_cut!=position_cut_kw){
		postion_cut_pulse=position_cut_kw/mm_per_pulse;
		position_cut=position_cut_kw;
	}
	
	
	if (position_back!=position_back_kw){
		position_back_pulse=position_back_kw/mm_per_pulse;
		position_back=position_back_kw;
	}
	
	
	if (dingchangchangdu!=dingchangchangdu_kw){
		position_dingchang_pulse=dingchangchangdu_kw/mm_per_pulse;
		dingchangchangdu=dingchangchangdu_kw;
	}
	
	
	if (forward_speed_fac!=forward_speed_fac_kw){
		forward_speed_fac=forward_speed_fac_kw;
		k_motor=forward_speed_fac_kw;  
	}
}
/***********************************************************************************************
*函数名 ：	fn_Button_Hushuo(unsigned char * button0,unsigned char * button1,unsigned char * status_record)
*函数功能描述 ： 两个按键互斥调用的子函数
*函数功能逻辑：  请自行查看，复杂但是直接调用即可，无需改变。
*函数参数 ： unsigned char * button0：第一个按钮的地址;unsigned char * button1：第二个按钮的地址;
			unsigned char * status_record：按钮记录的地址：status_record变量设置为0即可。
*函数返回值 ： void
*作者 ：王德铭
*函数创建日期 ： 2016
*函数修改日期 ： 2017
*修改人 ：方佳伟
*修改原因 ： 修改注释
*版本 ： V2.2.0
*历史版本 ： 无
***********************************************************************************************/

void fn_Button_Hushuo(unsigned char * button0,unsigned char * button1,unsigned char * status_record){
	if (*status_record==0){
		if (*button1==1)
		{
			*button0=0;
			*status_record=1;
		}
	}
	else{
		if (*button0==1){
			*button1=0;
			*status_record=0;
		}
	}
}

/***********************************************************************************************
*函数名 ：	fn_Button_init(unsigned char * button0,unsigned char * button1,unsigned char * status_record)
*函数功能描述 ： 两个按键互斥
*函数功能逻辑：  请自行查看，复杂但是直接调用即可，无需改变。
*函数参数 ： unsigned char * button0：第一个按钮的地址;unsigned char * button1：第二个按钮的地址;
			unsigned char * status_record：按钮记录的地址：status_record变量设置为0即可。
*函数返回值 ： void
*作者 ：王德铭
*函数创建日期 ： 2016
*函数修改日期 ： 2017
*修改人 ：方佳伟
*修改原因 ： 修改注释
*版本 ： V2.2.0
*历史版本 ： 无
***********************************************************************************************/
void fn_Button_init(unsigned char * button0,unsigned char * button1,unsigned char * status_record){
	if(*button0==1 || *button1==1){
		fn_Button_Hushuo(button0,button1,status_record);
	}
	else{
		*button0=1;
		*button1=0;
		*status_record=0;
	}
}

/***********************************************************************************************
*函数名 ：	fn_init_proc(void)
*函数功能描述 ： 上电/工作完成之后，托板零点确认
*函数功能逻辑：  挡住零点时，应先向右移动，移出传感器位置；
				未挡住零点时候，直接回零点。
				参数init_delay_flag在中断函数__irq PulseOut_1_Process(void)中进行++
*函数参数 ：void
*函数返回值 ： void
*作者 ：王德铭
*函数创建日期 ： 2016
*函数修改日期 ： 2017
*修改人 ：方佳伟
*修改原因 ： 修改注释
*版本 ： V2.2.0
*历史版本 ： 无
***********************************************************************************************/
void fn_init_proc(void){
	if ((init_flag_cover==0)&& (Get_X_Value(10)==cover)  )  // 挡住零点时，应先向右移动，移出传感器位置
	{	
		init_flag_cover=1;
		init_delay_flag=0;
		Set_Y_Value(6,turn_right);  //刀---右方向 ---- 0为输出高电平---灯灭 Y6/Y5择其一,
		Set_Y_Value(5,turn_right);  //刀---右方向 ---- 0为输出高电平---灯灭 Y6
		PulseOut_1_Start(init_fre_kw,-1);//Y1脉冲----伺服	
	}
	if ( (init_flag_uncover==0)&&(Get_X_Value(10)==uncover) )
	{	
		/*if()中式上面挡住零点时，往右移动的时候移出之后再往回*/
		if (init_flag_cover==1 && init_delay_flag>=100)
		{
			init_flag_uncover=1;
			init_delay_flag=0;
			Set_Y_Value(6,turn_left);   // 刀---左方向---- 1为输出低电平
			Set_Y_Value(5,turn_left);   // 刀---左方向---- 1为输出低电平
			PulseOut_1_Start(init_fre_kw,-1); // Y1脉冲
		}
		else if(init_flag_cover!=1){
			init_flag_cover=1;
			init_flag_uncover=1;
			init_delay_flag=0;
			Set_Y_Value(6,turn_left);   // 刀---左方向---- 1为输出低电平
			Set_Y_Value(5,turn_left);   // 刀---左方向---- 1为输出低电平
			PulseOut_1_Start(init_fre_kw,-1); // Y1脉冲
		}
	}
}

/***********************************************************************************************
*函数名 ：	fn_jog(void)
*函数功能描述 ： 点动控制
*函数功能逻辑：  点动控制的时候，使用PWM发送脉冲，因为无需记录位置，点动结束之后再设置成为普通模式。
				此处set_pwm()/set_normal()使用的均为直接操作寄存器
*函数参数 ：void
*函数返回值 ： void
*作者 ：王德铭
*函数创建日期 ： 2016
*函数修改日期 ： 2017
*修改人 ：方佳伟
*修改原因 ： 修改注释
*版本 ： V2.2.0
*历史版本 ： 无
***********************************************************************************************/
void fn_jog(void){
	if(start_b == 0)
	{	
		if  (left_jog_b==1) 
		{
			Set_Y_Value(6,turn_left);  //刀---左方向 ---- 1为输出高电平---灯亮 Y6
			Set_Y_Value(5,turn_left);  //刀---左方向 ---- 1为输出高电平---灯亮 Y6
		}
		else if  (right_jog_b==1) 
		{ 
			Set_Y_Value(6,turn_right);  //刀---右方向 ---- 0为输出高电平---灯灭 Y6
			Set_Y_Value(5,turn_right);  //刀---右方向 ---- 0为输出高电平---灯灭 Y6
		}
		if (jog_status[0]==0 && (left_jog_b==1 || right_jog_b==1))
		{
			set_pwm();
			PulseOut_1_Start(jog_fre_kw,-1);//Y1脉冲----伺服
			jog_status[0]=1; 
		}
		if (jog_status[1]==0 && knife_jog_b==1)
		{
			set_knife_pwm();
			PulseOut_2_Start(jog_fre_kw,-1);
			jog_status[1]=1; 
		}
	}
	if(jog_status[0]==1)
	{
		if ((left_jog_b==0) && (right_jog_b==0))
		{
			PulseOut_1_Stop();
			jog_status[0]=0;
			set_normal();
		}
	}
	if(jog_status[1]==1)
	{
		if (knife_jog_b==0)
		{
			PulseOut_2_Stop();
			jog_status[1]=0;
			set_knife_normal();
		}
	}
}

void fn_warning(void){
	if( position_cut_kw >= position_back_kw && error_warning==0)
	//返回点距原点距离要大于切割点距原点距离
	{
		g_InteralMemory.Word[PING_CTRL]=3;
		error_warning=1;
		//clear_all ();
		return;			
	}
	else if (position_cut_kw < position_back_kw)
		error_warning=0;
}
void fre1_set(INT16U fre,unsigned char no){
	if (previous_fre[no]!=fre && fre!=0)
	{
		rTCNTB1= 300300/fre;		
		rTCMPB1 = rTCNTB1/2;
		previous_fre[no]=fre;
	}
	
}
void fre2_set(INT16U fre){
	if(previous_fre[knife_fre_no]!=fre && fre!=0){
		rTCNTB2= 300300/fre;
		rTCMPB2 = rTCNTB2/2;
		previous_fre[knife_fre_no]=fre;
	}
	
}

void set_pwm(void){
	DWORD tmp;
	tmp = rGPBCON & (~(0x3<< 2)) ;
	rGPBCON = tmp | (0x2<<2) ;	//set GPB1 as TOUT	
}
void set_normal(void){
	DWORD tmp;
	tmp = rGPBCON & (~(0x3<< 2));
	rGPBCON = tmp | (0x1<<2) ;	//Y1普通  */
}
void set_knife_pwm(void){
	DWORD tmp;
	tmp = rGPBCON & (~(0x3<< 4)) ;
	rGPBCON = tmp | (0x2<<4) ;	//set GPB1 as TOUT	
}
void set_knife_normal(void){
	DWORD tmp;
	tmp = rGPBCON & (~(0x3<< 4));
	rGPBCON = tmp | (0x1<<4) ;	//Y1普通  */
}


/***********************************************************************************************
*函数名 ：	next_start(void)
*函数功能描述 ： 下一次开始
*函数功能逻辑：  完成一轮走切任务之后，再一次开始标志位，其中最重要的为forward_flag=1;
			   此处将前进标志位置1。
*函数参数 ：void
*函数返回值 ： void
*作者 ：王德铭
*函数创建日期 ： 2016
*函数修改日期 ： 2017
*修改人 ：方佳伟
*修改原因 ： 修改注释
*版本 ： V2.2.0
*历史版本 ： 无
***********************************************************************************************/
void next_start(void){
	PulseOut_1_Stop(); 
	knife_finish_flag=knife_unfinish;
	Set_Y_Value(6,turn_right);  //刀-----右方向
	Set_Y_Value(5,turn_right);  //刀-----右方向
				//清除偏差计数器
	pulse_get_total_num=0;
	pulse_send_num=0;
	motor_fac=0;
	forward_flag=1;				//往前运动开始的标志位
	
	start_flag=0;
}