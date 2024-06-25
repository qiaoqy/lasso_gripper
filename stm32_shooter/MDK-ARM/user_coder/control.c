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
	//֡ͷ������4��1kHz
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
	
	//��ʾ��ǰʹ�õĿ���ģʽ
	switch(getcontrolData()->control_mode){
		
		case MODE_CURRENT:
		case MODE_CURRENT_RAMP:
 
			//32λ���ݣ�����ֻ�ܷ�8λ���ݣ��ʽ����Ĵ�forѭ��
			for(index = 0; index < 4; index++){ 
				array[index_ptr++] = getcontrolData()->set_cur.u8_temp[index];
			}			
				
            break;  
		
		case VELOCITY:
		case VELOCITY_RAMP:
 
				//32λ���ݣ�����ֻ�ܷ�8λ���ݣ��ʽ����Ĵ�forѭ��
				for(index = 0; index < 4; index++){ 
					array[index_ptr++] = getcontrolData()->set_vel.u8_temp[index];
				}	
		
            break;  
		
		case MODE_POSITION:
		case MODE_POSITION_TRAP:
 
				//32λ���ݣ�����ֻ�ܷ�8λ���ݣ��ʽ����Ĵ�forѭ��
				for(index = 0; index < 4; index++){ 
					array[index_ptr++] = getcontrolData()->set_pos.u8_temp[index];
				}			
		
		
            break;
 			
		default:break;
		} 
	
	//��װУ��λ
	array[2] = index_ptr + 2;
	Append_CRC8_Check_Sum(array, 4);
	Append_CRC16_Check_Sum(array, array[2]);
	HAL_UART_Transmit_DMA(&huart1, array, sizeof(array));
}
//��ȡ����FOC��Ʊ������
static void LevelFocDataRead(uint8_t *array){
	uint8_t index_ptr = 4;
	uint8_t index = 0;
	
	//����λ ������Ϣ
	LevelSensorData.mErrorCode = array[index_ptr++];
	
	//����λ �������ģʽ	
	/*
	MODE_CURRENT		= 1,//��������ģʽ����ֱ�ӿ���
	MODE_CURRENT_RAMP	= 2,//��������ģʽ��������
	VELOCITY		= 3,//�ٶȿ���ģʽ����ֱ�ӿ���
	VELOCITY_RAMP	= 4,//�ٶȿ���ģʽ��������
	MODE_POSITION		= 5,//λ�ÿ���ģʽ����ֱ�ӿ���
	MODE_POSITION_TRAP	= 6,//λ�ÿ���ģʽ��������
	*/

	LevelSensorData.control_mode = array[index_ptr++];
	
	//����λ ���״̬
	// CONTROL_MODE_RELAX	= 0 ʧ��
	// CONTROL_MODE_CLOSE	= 1 �ջ�
	// CONTROL_MODE_CAIL	= 2 У׼
	// CONTROL_MODE_ANTI	= 3 ���ݲ۾�У׼
	LevelSensorData.motor_mode = array[index_ptr++]; 
	
		//����λ -> ��ʮλ ��ѹֵ
	for(index=0;index < 4;index++)
		LevelSensorData.Vbus_Cur.u8_temp[index] = array[index_ptr++];	
	
	//��ʮһλ -> ��ʮ��λ ʵ�ʵ���
	for(index=0;index < 4;index++)
		LevelSensorData.cur_estimate_.u8_temp[index] = array[index_ptr++];	

	//��ʮ��λ -> ��ʮ��λ ʵ��λ��
	for(index=0;index < 4;index++)
		LevelSensorData.pos_estimate_.u8_temp[index] = array[index_ptr++];	
	
	//��ʮ��λ -> �ڶ�ʮ��λ ʵ���ٶ�
	for(index=0;index < 4;index++)
		LevelSensorData.vel_estimate_.u8_temp[index] = array[index_ptr++];		
	
	//�ڶ�ʮ��λ -> �ڶ�ʮ��λ ��������
	for(index=0;index < 4;index++)
		LevelSensorData.current_limit.u8_temp[index] = array[index_ptr++];	
	
	//�ڶ�ʮ��λ -> ����ʮλ �ٶ�����
	for(index=0;index < 4;index++)
		LevelSensorData.vel_limit.u8_temp[index] = array[index_ptr++];		
	
	//����ʮһλ -> ����ʮ��λ ������cpr����ֵ (��Ȧ��0-16384)
	for(index=0;index < 4;index++)
		LevelSensorData.count_in_cpr_.u8_temp[index] = array[index_ptr++];		
	
	//����ʮ��λ -> ����ʮ��λ �������
	for(index = 0; index < 4; index++)
		LevelSensorData.motor_phase_resistance.u8_temp[index] = array[index_ptr++];			

	//����ʮ��λ -> ����ʮ��λ ������
	for(index = 0; index < 4; index++)
		LevelSensorData.motor_phase_inductance.u8_temp[index] = array[index_ptr++];			
	
	//����ʮ��λ ���������flag
	LevelSensorData.save_config_flag =  array[index_ptr++];		
	
	//����ʮ��λ ʹ�õ���/�ٶ����Ƶ�flag
	LevelSensorData.set_vel_cur_limit_flag = array[index_ptr++];	
	
	//����ʮ��λ �����������flag
	LevelSensorData.clear_error_flag = array[index_ptr++];		
	
	
}//������ȡ�ӻ�������������

void Lemon_data (uint8_t *Data){

		if(Data[0] == LEVEL_FOC_BEGIN && (Data[1] & LEVEL_FOC_ADDRESS) )
			{
				if(!Verify_CRC8_Check_Sum(Data, 4) && !Verify_CRC16_Check_Sum(Data, Data[2])){					
					// У��ʧ���򲻵�������
					//��ʱ�����в���
				}
				else{
					//��ȡˮƽ�������
//					rx_data_count = 0;
					LevelFocDataRead(Data);
				}
			}
};
