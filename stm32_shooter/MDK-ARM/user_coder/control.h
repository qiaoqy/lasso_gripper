#include "main.h"
#include "stdint.h"
#include "stm32f1xx_hal.h"
#include "driver_crc.h"
////////////////////////////////////////
#define MAIN_CONTROL_BEGIN				0x3F
#define MAIN_CONTROL_ADDRESS			0x10

#define LEVEL_FOC_BEGIN					0xAD
#define LEVEL_FOC_ADDRESS				0x40
///////////////////////////////////////

///////////////////////////////////////////
//电机控制状态
typedef enum {
	
	MODE_NULL	= 0x1A,  //无状态
	MODE_RELAX	= 0x1B,  //失能
	MODE_CLOSE = 0x6D,   //闭环
	MODE_CALL = 0x63,   //校准
			
}AxisStateStruct_t;

//电机控制模式
typedef enum {

	MODE_CURRENT		= 1,//电流控制模式——直接控制
	MODE_CURRENT_RAMP	= 2,//电流控制模式——梯形
	VELOCITY		= 3,//速度控制模式——直接控制
	VELOCITY_RAMP	= 4,//速度控制模式——梯形
	MODE_POSITION		= 5,//位置控制模式——直接控制
	MODE_POSITION_TRAP	= 6,//位置控制模式——梯形
			
}ControlModeStruct_t;



//联合体用于转换数据
typedef union{
	u8 		u8_temp[4];
	float float_temp;
	s32 	s32_temp;
	u32		u32_temp;
} formatTrans32Struct_t;

typedef union{
	u8 		u8_temp[2];
	s16 	s16_temp;
	u16		u16_temp;	
} formatTrans16Struct_t;


typedef struct{
	
	uint8_t control_mode;
	uint8_t motor_mode;
	formatTrans32Struct_t Vbus_Cur;

	formatTrans32Struct_t pos_estimate_;
	formatTrans32Struct_t vel_estimate_;	
	formatTrans32Struct_t cur_estimate_;	
	
	formatTrans32Struct_t current_limit;
	formatTrans32Struct_t vel_limit;
	formatTrans32Struct_t count_in_cpr_;	
	
	formatTrans32Struct_t motor_phase_resistance;
	formatTrans32Struct_t motor_phase_inductance;	
	
	uint8_t mErrorCode;
	
	uint8_t	save_config_flag;
	uint8_t clear_error_flag;
	uint8_t set_vel_cur_limit_flag ;
} LevelSensorStruct_t;

typedef struct {
	uint8_t dataInitFlag; //没用到
	uint32_t loops; //没用到
	
	uint8_t	save_config_flag; //保存电机参数标志位
	uint8_t	clear_error_flag;	//清除电机错误标志位
	AxisStateStruct_t	set_motor_flag; //设置电机状态
	ControlModeStruct_t control_mode;	//电机控制模式	
	formatTrans32Struct_t set_pos;	//设置电机位置
	formatTrans32Struct_t set_vel;	//设置电机速度
	formatTrans32Struct_t set_cur;	//设置电机电流		
	formatTrans32Struct_t vel_limit;	//设置电机速度限制
	formatTrans32Struct_t current_limit; //设置电机电流限制		
	uint8_t set_vel_cur_limit_flag; //设置电机速度/电流限制Flag
	
} controlStruct_t;	
////////////////////////////////////////////////////

void Lemon_data (uint8_t *Data);
void wiredSendData(void);
void congtrolGlobalInit(void);
