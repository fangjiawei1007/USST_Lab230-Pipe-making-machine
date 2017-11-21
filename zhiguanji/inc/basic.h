#ifndef	_BASIC_H_
#define	_BASIC_H_

#define	init_flag_kb					g_InteralMemory.KeepBit[14]

#define	init_fre_no						0
#define	knife_fre_no					1
#define	back_fre_no						2

#define	gangdaigengzong_kb				g_InteralMemory.KeepBit[16]
#define	gangdaixishu_kw					g_InteralMemory.KeepWord[11]



void fn_Button_init(unsigned char * button0,unsigned char * button1,unsigned char * status_record);
void fn_init_proc(void);
void fre1_set(INT16U fre,unsigned char no);
void fre2_set(INT16U fre);
void parameter_init(void);
void set_pwm(void);
void set_normal(void);
void fn_jog(void);
void fn_warning(void);
void parameter_input(void);
void set_knife_pwm(void);
void set_knife_normal(void);
void next_start(void);






#endif