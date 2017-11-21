

void App_Init(void);



void App_Operation(void);
void big_value_init(void);

void clear_all (void) ;

/// 1 意味着 挡住传感器输出低电平   Get_X_Value(num)=1 ，num口与GND接通
/// 0 则相反
#define	Lang_Chinese_Version				0
#define	Lang_English_Version				1
#define	model_sebiao						0
#define	model_DingChang						1
#define cover								0    //挡住传感器   1
#define uncover 							1  //未挡住传感器 0  
#define	knife_finish						1
#define	knife_unfinish						0
#define	knife_start							1
#define	knife_stop							0
#define	sebiaojiadingchang					1
#define	sebiaowudingchang					0



#define bmq_xianshu_kw				g_InteralMemory.KeepWord[21]   //编码器线数
#define bmq_D_kw					g_InteralMemory.KeepWord[22]	 //编码器外圈周长 3.14*35---直径为35mm

#define	start_b						g_InteralMemory.Bit[0]

#define	left_jog_b					g_InteralMemory.Bit[1]
#define	right_jog_b					g_InteralMemory.Bit[2]
#define	model_display_b				g_InteralMemory.Bit[6]
#define	Lang_select_confirm_b		g_InteralMemory.Bit[7]
#define	knife_jog_b					g_InteralMemory.Bit[8]
#define	return_b					g_InteralMemory.Bit[9]

#define	qiandao_b					g_InteralMemory.KeepBit[10]

#define	Lang_Chinese_kb_num			15
#define	Lang_English_kb_num			13

#define Lang_Chinese_kb				g_InteralMemory.KeepBit[15]
#define	Lang_English_kb				g_InteralMemory.KeepBit[13]

#define	model_SeBiao_kb				g_InteralMemory.KeepBit[11]
#define	model_DingChang_kb			g_InteralMemory.KeepBit[12]

#define turn_right   				g_InteralMemory.KeepBit[20]   			//右移动 
#define turn_left 	 				((~g_InteralMemory.KeepBit[20])&0x1)    //左移动  

#define	position_cut_kw				g_InteralMemory.KeepWord[0]
#define	back_fre_kw					g_InteralMemory.KeepWord[3]
#define	knife_fre_kw				g_InteralMemory.KeepWord[4]
#define	position_back_kw			g_InteralMemory.KeepWord[5]
#define	current_knife_position_kw	g_InteralMemory.KeepWord[6]
#define	dingchangchangdu_kw			g_InteralMemory.KeepWord[7]
#define	forward_speed_fac_kw		g_InteralMemory.KeepWord[8]
#define	knife_round_num_kw			g_InteralMemory.KeepWord[19]
#define	knife_pulse_num_kw			g_InteralMemory.KeepWord[20]
#define	zongchanliang_kw			g_InteralMemory.KeepWord[24]
#define zongchanliang_upper_kw		g_InteralMemory.KeepWord[25]
#define	dangbanchanliang_kw			g_InteralMemory.KeepWord[26]
#define	dangbanchanliang_upper_kw	g_InteralMemory.KeepWord[27]

#define	init_fre_kw					g_InteralMemory.KeepWord[1]
#define	jog_fre_kw					g_InteralMemory.KeepWord[10]






#define	test1_b						g_InteralMemory.Bit[4]
#define	test1_w						g_InteralMemory.Word[10]
#define	test2_w						g_InteralMemory.Word[11]
