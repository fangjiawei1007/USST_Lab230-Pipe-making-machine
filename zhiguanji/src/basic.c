#include"includes.h"

unsigned int init_flag_cover,init_flag_uncover;
unsigned int init_delay_flag;
short  jog_status[2]={0};
unsigned char error_warning;
INT16U previous_fre[3];
float	mm_per_pulse;
INT16U bmq_D,bmq_xianshu,position_cut,position_back,dingchangchangdu,forward_speed_fac;

/***********************************************************************************************
*������ �� parameter_init(void)
*������������ �� ��ʼ���������ڴ����֮��ֻ��ִ��һ�Σ�֮���ϵ�������������ú�����
*���������߼���  init_flag_kb����Ϊkb����ônand������֮���ϵ��һ�ξͻ��init_flag_kb��1��
			    ��ô֮���ϵ���������ú�����
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
*������ �� parameter_input(void)
*������������ �� ��Ҫ��������Ĳ����������������Լ�����ϵ��k�ĸ�ֵ
*���������߼���  bmq_D����������ֱ��;bmq_xianshu��������������;mm_per_pulse����ÿ���������mm,
				֮����ݸ����εĳ��ȼ�������׶�����Ҫ��������,�ϵ��ʱ�����һ���ж���
				�����ϴιػ�֮ǰ���޸ģ�������޸ľ����½��м��㡣
				k_motor����������ϵ��kֵ
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
void parameter_input(void){
	
	/**
		bmq_D����������ֱ��
		bmq_xianshu��������������
		mm_per_pulse����ÿ���������mm

		֮����ݸ����εĳ��ȼ�������׶�����Ҫ��������
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
*������ ��	fn_Button_Hushuo(unsigned char * button0,unsigned char * button1,unsigned char * status_record)
*������������ �� ��������������õ��Ӻ���
*���������߼���  �����в鿴�����ӵ���ֱ�ӵ��ü��ɣ�����ı䡣
*�������� �� unsigned char * button0����һ����ť�ĵ�ַ;unsigned char * button1���ڶ�����ť�ĵ�ַ;
			unsigned char * status_record����ť��¼�ĵ�ַ��status_record��������Ϊ0���ɡ�
*��������ֵ �� void
*���� ��������
*������������ �� 2016
*�����޸����� �� 2017
*�޸��� ������ΰ
*�޸�ԭ�� �� �޸�ע��
*�汾 �� V2.2.0
*��ʷ�汾 �� ��
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
*������ ��	fn_Button_init(unsigned char * button0,unsigned char * button1,unsigned char * status_record)
*������������ �� ������������
*���������߼���  �����в鿴�����ӵ���ֱ�ӵ��ü��ɣ�����ı䡣
*�������� �� unsigned char * button0����һ����ť�ĵ�ַ;unsigned char * button1���ڶ�����ť�ĵ�ַ;
			unsigned char * status_record����ť��¼�ĵ�ַ��status_record��������Ϊ0���ɡ�
*��������ֵ �� void
*���� ��������
*������������ �� 2016
*�����޸����� �� 2017
*�޸��� ������ΰ
*�޸�ԭ�� �� �޸�ע��
*�汾 �� V2.2.0
*��ʷ�汾 �� ��
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
*������ ��	fn_init_proc(void)
*������������ �� �ϵ�/�������֮���а����ȷ��
*���������߼���  ��ס���ʱ��Ӧ�������ƶ����Ƴ�������λ�ã�
				δ��ס���ʱ��ֱ�ӻ���㡣
				����init_delay_flag���жϺ���__irq PulseOut_1_Process(void)�н���++
*�������� ��void
*��������ֵ �� void
*���� ��������
*������������ �� 2016
*�����޸����� �� 2017
*�޸��� ������ΰ
*�޸�ԭ�� �� �޸�ע��
*�汾 �� V2.2.0
*��ʷ�汾 �� ��
***********************************************************************************************/
void fn_init_proc(void){
	if ((init_flag_cover==0)&& (Get_X_Value(10)==cover)  )  // ��ס���ʱ��Ӧ�������ƶ����Ƴ�������λ��
	{	
		init_flag_cover=1;
		init_delay_flag=0;
		Set_Y_Value(6,turn_right);  //��---�ҷ��� ---- 0Ϊ����ߵ�ƽ---���� Y6/Y5����һ,
		Set_Y_Value(5,turn_right);  //��---�ҷ��� ---- 0Ϊ����ߵ�ƽ---���� Y6
		PulseOut_1_Start(init_fre_kw,-1);//Y1����----�ŷ�	
	}
	if ( (init_flag_uncover==0)&&(Get_X_Value(10)==uncover) )
	{	
		/*if()��ʽ���浲ס���ʱ�������ƶ���ʱ���Ƴ�֮��������*/
		if (init_flag_cover==1 && init_delay_flag>=100)
		{
			init_flag_uncover=1;
			init_delay_flag=0;
			Set_Y_Value(6,turn_left);   // ��---����---- 1Ϊ����͵�ƽ
			Set_Y_Value(5,turn_left);   // ��---����---- 1Ϊ����͵�ƽ
			PulseOut_1_Start(init_fre_kw,-1); // Y1����
		}
		else if(init_flag_cover!=1){
			init_flag_cover=1;
			init_flag_uncover=1;
			init_delay_flag=0;
			Set_Y_Value(6,turn_left);   // ��---����---- 1Ϊ����͵�ƽ
			Set_Y_Value(5,turn_left);   // ��---����---- 1Ϊ����͵�ƽ
			PulseOut_1_Start(init_fre_kw,-1); // Y1����
		}
	}
}

/***********************************************************************************************
*������ ��	fn_jog(void)
*������������ �� �㶯����
*���������߼���  �㶯���Ƶ�ʱ��ʹ��PWM�������壬��Ϊ�����¼λ�ã��㶯����֮�������ó�Ϊ��ͨģʽ��
				�˴�set_pwm()/set_normal()ʹ�õľ�Ϊֱ�Ӳ����Ĵ���
*�������� ��void
*��������ֵ �� void
*���� ��������
*������������ �� 2016
*�����޸����� �� 2017
*�޸��� ������ΰ
*�޸�ԭ�� �� �޸�ע��
*�汾 �� V2.2.0
*��ʷ�汾 �� ��
***********************************************************************************************/
void fn_jog(void){
	if(start_b == 0)
	{	
		if  (left_jog_b==1) 
		{
			Set_Y_Value(6,turn_left);  //��---���� ---- 1Ϊ����ߵ�ƽ---���� Y6
			Set_Y_Value(5,turn_left);  //��---���� ---- 1Ϊ����ߵ�ƽ---���� Y6
		}
		else if  (right_jog_b==1) 
		{ 
			Set_Y_Value(6,turn_right);  //��---�ҷ��� ---- 0Ϊ����ߵ�ƽ---���� Y6
			Set_Y_Value(5,turn_right);  //��---�ҷ��� ---- 0Ϊ����ߵ�ƽ---���� Y6
		}
		if (jog_status[0]==0 && (left_jog_b==1 || right_jog_b==1))
		{
			set_pwm();
			PulseOut_1_Start(jog_fre_kw,-1);//Y1����----�ŷ�
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
	//���ص��ԭ�����Ҫ�����и���ԭ�����
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
	rGPBCON = tmp | (0x1<<2) ;	//Y1��ͨ  */
}
void set_knife_pwm(void){
	DWORD tmp;
	tmp = rGPBCON & (~(0x3<< 4)) ;
	rGPBCON = tmp | (0x2<<4) ;	//set GPB1 as TOUT	
}
void set_knife_normal(void){
	DWORD tmp;
	tmp = rGPBCON & (~(0x3<< 4));
	rGPBCON = tmp | (0x1<<4) ;	//Y1��ͨ  */
}


/***********************************************************************************************
*������ ��	next_start(void)
*������������ �� ��һ�ο�ʼ
*���������߼���  ���һ����������֮����һ�ο�ʼ��־λ����������Ҫ��Ϊforward_flag=1;
			   �˴���ǰ����־λ��1��
*�������� ��void
*��������ֵ �� void
*���� ��������
*������������ �� 2016
*�����޸����� �� 2017
*�޸��� ������ΰ
*�޸�ԭ�� �� �޸�ע��
*�汾 �� V2.2.0
*��ʷ�汾 �� ��
***********************************************************************************************/
void next_start(void){
	PulseOut_1_Stop(); 
	knife_finish_flag=knife_unfinish;
	Set_Y_Value(6,turn_right);  //��-----�ҷ���
	Set_Y_Value(5,turn_right);  //��-----�ҷ���
				//���ƫ�������
	pulse_get_total_num=0;
	pulse_send_num=0;
	motor_fac=0;
	forward_flag=1;				//��ǰ�˶���ʼ�ı�־λ
	
	start_flag=0;
}