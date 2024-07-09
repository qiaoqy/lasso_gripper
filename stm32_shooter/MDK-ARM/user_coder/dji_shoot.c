#include "dji_shoot.h"
#include "main.h"
#include "control.h"
extern TIM_HandleTypeDef htim2;
extern controlStruct_t controlData;
void pc_receive(uint8_t *Data)
{
	unsigned char checksum = ~(Data[0] + Data[1] + Data[2]);
	if (checksum == Data[3])
	{
		if (Data[2])
		{
			shooter_launch();
		}
		else
		{
			shooter_recycle();
		}
	}
}


uint8_t tx_buf_to_pc[4];
void pc_transmit(uint8_t instruction)
{
	uint8_t package1 = 0xff;
	uint8_t package2 = 0xff;
	uint8_t checksum = ~(package1 + package2 + instruction);
		
	tx_buf_to_pc[0] = package1;
	tx_buf_to_pc[1] = package2;
	tx_buf_to_pc[2] = instruction;///0 for close, 1 for shoot
	tx_buf_to_pc[3] = checksum;
}
void shooter_launch(void)
{
	__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1, 146);
	__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_3, 146);
	__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2, 154);
	__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_4, 154);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);
	
	controlData.set_cur.float_temp = 0.05f;
}

void shooter_recycle(void)
{
	__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1, 154);
	__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_3, 154);
	__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2, 146);
	__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_4, 146);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);
	
	controlData.set_cur.float_temp = -0.05f;
}


