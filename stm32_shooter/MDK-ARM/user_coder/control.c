///////include///////

#include "control.h"

/////////////////////
extern UART_HandleTypeDef huart1;
//////
controlStruct_t controlData;
LevelSensorStruct_t LevelSensorData;
controlStruct_t* getcontrolData(){
    return &controlData;
}





void wiredSendData(void){ 
	uint8_t array[64];
	uint8_t index_ptr = 0;
	uint8_t index = 0;
	//帧头，长度4，1kHz
	array[index_ptr++] = MAIN_CONTROL_BEGIN;
	array[index_ptr++] = MAIN_CONTROL_ADDRESS;
	array[index_ptr++] = 0x00;
	array[index_ptr++] = 0x00;
	

	array[index_ptr++] = getcontrolData()->save_config_flag;
	
	array[index_ptr++] = getcontrolData()->clear_error_flag;
	

	
	array[index_ptr++] = (uint8_t)getcontrolData()->control_mode;
	
	array[index_ptr++] = (uint8_t)getcontrolData()->set_motor_flag;
	
	array[index_ptr++] = (uint8_t)getcontrolData()->set_vel_cur_limit_flag;
	
	
	
	for(index = 0; index < 4; index++){ 
		array[index_ptr++] =  getcontrolData()->vel_limit.u8_temp[index];
	}			
	
	for(index = 0; index < 4; index++){ 
		array[index_ptr++] = getcontrolData()->current_limit.u8_temp[index];
	}			
	
	//显示当前使用的控制模式
	switch(getcontrolData()->control_mode){
		
		case MODE_CURRENT:
		case MODE_CURRENT_RAMP:
 
			//32位数据，串口只能发8位数据，故进行四次for循环
			for(index = 0; index < 4; index++){ 
				array[index_ptr++] = getcontrolData()->set_cur.u8_temp[index];
			}			
				
            break;  
		
		case VELOCITY:
		case VELOCITY_RAMP:
 
				//32位数据，串口只能发8位数据，故进行四次for循环
				for(index = 0; index < 4; index++){ 
					array[index_ptr++] = getcontrolData()->set_vel.u8_temp[index];
				}	
		
            break;  
		
		case MODE_POSITION:
		case MODE_POSITION_TRAP:
 
				//32位数据，串口只能发8位数据，故进行四次for循环
				for(index = 0; index < 4; index++){ 
					array[index_ptr++] = getcontrolData()->set_pos.u8_temp[index];
				}			
		
		
            break;
 			
		default:break;
		} 
	
	//填装校验位
	array[2] = index_ptr + 2;
	Append_CRC8_Check_Sum(array, 4);
	Append_CRC16_Check_Sum(array, array[2]);
	HAL_UART_Transmit_DMA(&huart1, array, sizeof(array));
}
//读取柠檬FOC邮票孔数据
static void LevelFocDataRead(uint8_t *array){
	uint8_t index_ptr = 4;
	uint8_t index = 0;
	
	//第四位 错误信息
	LevelSensorData.mErrorCode = array[index_ptr++];
	
	//第五位 电机控制模式	
	/*
	MODE_CURRENT		= 1,//电流控制模式——直接控制
	MODE_CURRENT_RAMP	= 2,//电流控制模式——梯形
	VELOCITY		= 3,//速度控制模式——直接控制
	VELOCITY_RAMP	= 4,//速度控制模式——梯形
	MODE_POSITION		= 5,//位置控制模式——直接控制
	MODE_POSITION_TRAP	= 6,//位置控制模式——梯形
	*/

	LevelSensorData.control_mode = array[index_ptr++];
	
	//第六位 电机状态
	// CONTROL_MODE_RELAX	= 0 失能
	// CONTROL_MODE_CLOSE	= 1 闭环
	// CONTROL_MODE_CAIL	= 2 校准
	// CONTROL_MODE_ANTI	= 3 抗齿槽距校准
	LevelSensorData.motor_mode = array[index_ptr++]; 
	
		//第七位 -> 第十位 电压值
	for(index=0;index < 4;index++)
		LevelSensorData.Vbus_Cur.u8_temp[index] = array[index_ptr++];	
	
	//第十一位 -> 第十四位 实际电流
	for(index=0;index < 4;index++)
		LevelSensorData.cur_estimate_.u8_temp[index] = array[index_ptr++];	

	//第十五位 -> 第十八位 实际位置
	for(index=0;index < 4;index++)
		LevelSensorData.pos_estimate_.u8_temp[index] = array[index_ptr++];	
	
	//第十九位 -> 第二十二位 实际速度
	for(index=0;index < 4;index++)
		LevelSensorData.vel_estimate_.u8_temp[index] = array[index_ptr++];		
	
	//第二十三位 -> 第二十六位 电流限制
	for(index=0;index < 4;index++)
		LevelSensorData.current_limit.u8_temp[index] = array[index_ptr++];	
	
	//第二十七位 -> 第三十位 速度限制
	for(index=0;index < 4;index++)
		LevelSensorData.vel_limit.u8_temp[index] = array[index_ptr++];		
	
	//第三十一位 -> 第三十四位 编码器cpr绝对值 (单圈：0-16384)
	for(index=0;index < 4;index++)
		LevelSensorData.count_in_cpr_.u8_temp[index] = array[index_ptr++];		
	
	//第三十五位 -> 第三十八位 电机电阻
	for(index = 0; index < 4; index++)
		LevelSensorData.motor_phase_resistance.u8_temp[index] = array[index_ptr++];			

	//第三十九位 -> 第四十二位 电机电感
	for(index = 0; index < 4; index++)
		LevelSensorData.motor_phase_inductance.u8_temp[index] = array[index_ptr++];			
	
	//第四十三位 参数保存的flag
	LevelSensorData.save_config_flag =  array[index_ptr++];		
	
	//第四十四位 使用电流/速度限制的flag
	LevelSensorData.set_vel_cur_limit_flag = array[index_ptr++];	
	
	//第四十五位 清除电机错误的flag
	LevelSensorData.clear_error_flag = array[index_ptr++];		
	
	
}//主机读取从机发过来的数据

void Lemon_data (uint8_t *Data){

		if(Data[0] == LEVEL_FOC_BEGIN && (Data[1] & LEVEL_FOC_ADDRESS) )
			{
				if(!Verify_CRC8_Check_Sum(Data, 4) && !Verify_CRC16_Check_Sum(Data, Data[2])){					
					// 校验失败则不导入数据
					//暂时不进行操作
				}
				else{
					//读取水平电机数据
//					rx_data_count = 0;
					LevelFocDataRead(Data);
				}
			}
};
