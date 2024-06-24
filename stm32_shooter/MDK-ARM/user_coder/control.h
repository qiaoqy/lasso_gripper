#include "main.h"
#include "stdint.h"
#include "stm32f10x.h"

////////////////////////////////////////
#define MAIN_CONTROL_BEGIN				0x3F
#define MAIN_CONTROL_ADDRESS			0x10

#define LEVEL_FOC_BEGIN					0xAD
#define LEVEL_FOC_ADDRESS				0x40
///////////////////////////////////////

///////////////////////////////////////////
//�������״̬
typedef enum {
	
	MODE_NULL	= 0x1A,  //��״̬
	MODE_RELAX	= 0x1B,  //ʧ��
	MODE_CLOSE = 0x6D,   //�ջ�
	MODE_CALL = 0x63,   //У׼
			
}AxisStateStruct_t;

//�������ģʽ
typedef enum {

	MODE_CURRENT		= 1,//��������ģʽ����ֱ�ӿ���
	MODE_CURRENT_RAMP	= 2,//��������ģʽ��������
	VELOCITY		= 3,//�ٶȿ���ģʽ����ֱ�ӿ���
	VELOCITY_RAMP	= 4,//�ٶȿ���ģʽ��������
	MODE_POSITION		= 5,//λ�ÿ���ģʽ����ֱ�ӿ���
	MODE_POSITION_TRAP	= 6,//λ�ÿ���ģʽ��������
			
}ControlModeStruct_t;



//����������ת������
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
	uint8_t dataInitFlag; //û�õ�
	uint32_t loops; //û�õ�
	
	uint8_t	save_config_flag; //������������־λ
	uint8_t	clear_error_flag;	//�����������־λ
	AxisStateStruct_t	set_motor_flag; //���õ��״̬
	ControlModeStruct_t control_mode;	//�������ģʽ	
	formatTrans32Struct_t set_pos;	//���õ��λ��
	formatTrans32Struct_t set_vel;	//���õ���ٶ�
	formatTrans32Struct_t set_cur;	//���õ������		
	formatTrans32Struct_t vel_limit;	//���õ���ٶ�����
	formatTrans32Struct_t current_limit; //���õ����������		
	uint8_t set_vel_cur_limit_flag; //���õ���ٶ�/��������Flag
	
} controlStruct_t;	
////////////////////////////////////////////////////

void Lemon_data (uint8_t Data);