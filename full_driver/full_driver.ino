#include "driver/ledc.h" 
#include "driver/mcpwm.h" 
#include <control.h>
#include <WiFi.h>
#include <WiFiUdp.h>


// WiFi network name and password:
const char * networkName = "your_wifi_name";
const char * networkPswd = "your_wifi_password";


const char * udpAddress = "192.168.XX.XXX";
const int udpPort = 3333;


boolean connected = false;
char incomingPacket[255];

WiFiUDP udp;


const int pwm2 = 41;
const int dirPin21 = 39;
const int dirPin22 = 40;
const int pwm1 = 35;
const int dirPin11 = 37;
const int dirPin12 = 36;


controlStruct_t controlData;
LevelSensorStruct_t LevelSensorData;
controlStruct_t* getcontrolData(){
    return &controlData;
}
uint8_t rx_array[47]= {0x00};
uint8_t state = 2;
uint8_t spd = 50;



 
void setup(){
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, pwm1);
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, pwm2); 

  mcpwm_config_t pwm_config;
  pwm_config.frequency = 200;    
  pwm_config.cmpr_a = 50;
  pwm_config.cmpr_b = 50;        
  pwm_config.counter_mode = MCPWM_UP_COUNTER;
  pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);   //Configure PWM0A & PWM0B with above settings
  mcpwm_init(MCPWM_UNIT_1, MCPWM_TIMER_1, &pwm_config);   //Configure PWM0A & PWM0B with above settings
  
  
  mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);
  mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A,0);
  mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B,0);
  pinMode(dirPin11, OUTPUT);
  pinMode(dirPin12, OUTPUT);
  digitalWrite(dirPin11, LOW);
  digitalWrite(dirPin12, HIGH);
  pinMode(dirPin21, OUTPUT);
  pinMode(dirPin22, OUTPUT);
  digitalWrite(dirPin21, LOW);
  digitalWrite(dirPin22, HIGH);


  Serial.begin(115200);
  Serial.println("Connecting...");
  connectToWiFi(networkName, networkPswd);


  Serial2.begin(115200, SERIAL_8N1, 18, 17);
  Serial2.onReceive(serialCallback);
  congtrolGlobalInit();
  controlData.set_cur.float_temp = 0.0f;
  controlData.control_mode = MODE_CURRENT;
  controlData.set_motor_flag = MODE_CLOSE;
  // wiredSendData();
}
 
void loop(){
  Lemon_data(rx_array);
  
  if(connected){
//    Send a packet
//    udp.beginPacket(udpAddress,udpPort);
//    udp.printf("Seconds since boot: %lu", millis()/1000);
//    udp.endPacket();
    int Data_length=udp.parsePacket();  //获取接收的数据的长度
    
    if(Data_length)  //如果有数据那么Data_length不为0，无数据Data_length为0
    {
      int len = udp.read(incomingPacket, 255);  //读取数据，将数据保存在数组incomingPacket中
      if (len > 0)  //为了避免获取的数据后面乱码做的判断
      {
        incomingPacket[len] = 0;
      }
      if(len==3)
      switch (incomingPacket[2])
      {
        case 0x00:
          state=0;
          controlData.set_cur.float_temp=0.03f;
          wiredSendData();
          digitalWrite(dirPin11, HIGH);
          digitalWrite(dirPin12, LOW);
          digitalWrite(dirPin21, LOW);
          digitalWrite(dirPin22, HIGH);
          mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A,40);
          mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B,30);
          break;
        case 0x01:
          state=0;
          controlData.set_cur.float_temp=0.03f;
          wiredSendData();
          digitalWrite(dirPin11, HIGH);
          digitalWrite(dirPin12, LOW);
          digitalWrite(dirPin21, LOW);
          digitalWrite(dirPin22, HIGH);
          mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A,50);
          mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B,40);
          break;
        case 0x02:
          state=0;
          controlData.set_cur.float_temp=0.03f;
          wiredSendData();
          digitalWrite(dirPin11, HIGH);
          digitalWrite(dirPin12, LOW);
          digitalWrite(dirPin21, LOW);
          digitalWrite(dirPin22, HIGH);
          mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A,80);
          mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B,70);
          break;
        case 0x03:
          state=0;
          controlData.set_cur.float_temp=0.03f;
          wiredSendData();
          digitalWrite(dirPin11, HIGH);
          digitalWrite(dirPin12, LOW);
          digitalWrite(dirPin21, LOW);
          digitalWrite(dirPin22, HIGH);
          mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A,90);
          mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B,80);
          break;
        case 0x04:
          state=1;
          controlData.set_cur.float_temp=0.2f;
          wiredSendData();
          digitalWrite(dirPin11, HIGH);
          digitalWrite(dirPin12, LOW);
          digitalWrite(dirPin21, LOW);
          digitalWrite(dirPin22, HIGH);
          mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A,50);
          mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B,0);
          break;
        case 0x05:
          state=2;
          controlData.set_cur.float_temp=1.2f;
          wiredSendData();
          mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A,0);
          mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B,0);
          digitalWrite(dirPin11, LOW);
          digitalWrite(dirPin12, LOW);
          digitalWrite(dirPin21, LOW);
          digitalWrite(dirPin22, LOW);
          break;
        case 0x06:
          state=2;
          controlData.set_cur.float_temp=0.0f;
          wiredSendData();
          mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A,0);
          mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B,0);
          digitalWrite(dirPin11, LOW);
          digitalWrite(dirPin12, LOW);
          digitalWrite(dirPin21, LOW);
          digitalWrite(dirPin22, LOW);
          break;
        default:
          break;
      }
      spd = (uint8_t)incomingPacket[1];
      Serial.println(spd);
      udp.beginPacket(udp.remoteIP(),udp.remotePort());  //准备发送数据到目标IP和目标端口
      udp.print("receive data:");  //将数据receive data:放入发送的缓冲区
      udp.println(incomingPacket);  //将接收到的数据放入发送的缓冲区
      udp.endPacket();  //向目标IP目标端口发送数据
    }
  }
  int count = int(LevelSensorData.pos_estimate_.float_temp+0.1f);
  float error = (LevelSensorData.pos_estimate_.float_temp+0.1f-count);
  // Serial.print(LevelSensorData.pos_estimate_.float_temp);
  // Serial.print(";");
  // Serial.println(error);
  // Serial.println(spd);
  if(state==0)
  {
    int inspd = spd-10-(error*70);
    if(inspd>=spd-10)
    {
      inspd=spd-10;
    }
    if(inspd<=0)
    {
      inspd=0;
    }
    int outspd = spd+(error*70);
    if(outspd>=100)
    {
      outspd=100;
    }
    if(outspd<=spd)
    {
      outspd=spd;
    }
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A,out
    
    spd);
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B,inspd);
  }
}


void connectToWiFi(const char * ssid, const char * pwd){
  Serial.println("Connecting to WiFi network: " + String(ssid));

  // delete old config
  WiFi.disconnect(true);
  //register event handler
  WiFi.onEvent(WiFiEvent);
  
  //Initiate connection
  WiFi.begin(ssid, pwd);

  Serial.println("Waiting for WIFI connection...");
}


void WiFiEvent(WiFiEvent_t event){
    switch(event) {
      case ARDUINO_EVENT_WIFI_STA_GOT_IP:
          //When connected set 
          Serial.print("WiFi connected! IP address: ");
          Serial.println(WiFi.localIP());  
          //initializes the UDP state
          //This initializes the transfer buffer
          udp.begin(WiFi.localIP(),udpPort);
          connected = true;
          break;
      case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
          Serial.println("WiFi lost connection");
          connected = false;
          break;
      default: break;
    }
}


void serialCallback() 
{
  uint8_t index_ptr = 0;
  while (Serial2.available()) 
  {
    rx_array[index_ptr] = (uint8_t)Serial2.read();
    index_ptr++;
  }
}


uint8_t index_ptr = 0;
void wiredSendData(void){ 
	uint8_t tx_array[23];
//	uint8_t index_ptr = 0;
//	uint8_t index = 0;
	//帧头，长度4，1kHz
	tx_array[index_ptr++] = MAIN_CONTROL_BEGIN;
	tx_array[index_ptr++] = MAIN_CONTROL_ADDRESS;
	tx_array[index_ptr++] = 0x00;
	tx_array[index_ptr++] = 0x00;
	

	tx_array[index_ptr++] = getcontrolData()->save_config_flag;
	
	tx_array[index_ptr++] = getcontrolData()->clear_error_flag;
	

	
	tx_array[index_ptr++] = (uint8_t)getcontrolData()->control_mode;
	
	tx_array[index_ptr++] = (uint8_t)getcontrolData()->set_motor_flag;
	
	tx_array[index_ptr++] = (uint8_t)getcontrolData()->set_vel_cur_limit_flag;
	
	
	
	for(int index = 0; index < 4; index++){ 
		tx_array[index_ptr++] =  getcontrolData()->vel_limit.u8_temp[index];
	}			
	
	for(int index = 0; index < 4; index++){ 
		tx_array[index_ptr++] = getcontrolData()->current_limit.u8_temp[index];
	}			
	
	//显示当前使用的控制模式
	switch(getcontrolData()->control_mode){
		
		case MODE_CURRENT:
		case MODE_CURRENT_RAMP:
 
			//32位数据，串口只能发8位数据，故进行四次for循环
			for(int index = 0; index < 4; index++){ 
				tx_array[index_ptr++] = getcontrolData()->set_cur.u8_temp[index];
			}			
				
            break;  
		
		case VELOCITY:
		case VELOCITY_RAMP:
 
				//32位数据，串口只能发8位数据，故进行四次for循环
				for(int index = 0; index < 4; index++){ 
					tx_array[index_ptr++] = getcontrolData()->set_vel.u8_temp[index];
				}	
		
            break;  
		
		case MODE_POSITION:
		case MODE_POSITION_TRAP:
 
				//32位数据，串口只能发8位数据，故进行四次for循环
				for(int index = 0; index < 4; index++){ 
					tx_array[index_ptr++] = getcontrolData()->set_pos.u8_temp[index];
				}			
		
		
            break;
 			
		default:break;
		} 
	
	//填装校验位
	tx_array[2] = 0x17;
	Append_CRC8_Check_Sum(tx_array, 4);
	Append_CRC16_Check_Sum(tx_array, tx_array[2]);
  Serial2.write(tx_array, sizeof(tx_array));
	// HAL_UART_Transmit(&huart1, tx_array, sizeof(tx_array),10);
	index_ptr = 0;
	for(int i=0;i<24;i++){
		tx_array[i] = 0;
	};
}





void Lemon_data (uint8_t *Data){
  // LevelFocDataRead(Data);
  if(Data[0] == LEVEL_FOC_BEGIN && (Data[1] & LEVEL_FOC_ADDRESS) )
  {
    // LevelFocDataRead(Data);
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
}


void congtrolGlobalInit(void){
    
 
	//控制初始化
	//保存电机参数flag
	controlData.save_config_flag = 0;
	
	//保存电机参数flag	
	controlData.clear_error_flag = 0;
	
	//设置目标电流
	controlData.set_cur.float_temp = 0.0f;
	
	//设置目标位置	
	controlData.set_pos.float_temp = 0.0f;
	
	//设置目标速度
	controlData.set_vel.float_temp = 0.0f;
		
	//设置电机速度限制
	controlData.vel_limit.float_temp = 30.0f;
	
	//设置电机电流限制
	controlData.current_limit.float_temp = 1.2f;
	
	//第五位 电机控制模式	
	/*
	MODE_CURRENT		= 1,//电流控制模式——直接控制
	MODE_CURRENT_RAMP	= 2,//电流控制模式——梯形
	VELOCITY		= 3,//速度控制模式——直接控制
	VELOCITY_RAMP	= 4,//速度控制模式——梯形
	MODE_POSITION		= 5,//位置控制模式——直接控制
	MODE_POSITION_TRAP	= 6,//位置控制模式——梯形
	*/
	controlData.control_mode = MODE_CURRENT;
	
	//设置电机状态
	/*
	MODE_NULL	= 0x1A,  //无状态
	MODE_RELAX	= 0x1B,  //失能
	MODE_CLOSE = 0x6D,   //闭环
	MODE_CALL = 0x63,   //校准	
	
	MODE_NULL 无状态的情况下才可以设置电机各种参数，比如保存电机参数
	*/
	
	controlData.set_motor_flag = MODE_NULL;

	//设置电机的速度/电流限制flag
	controlData.set_vel_cur_limit_flag = 1;	
	
	wiredSendData();
	
	
}


//读取柠檬FOC邮票孔数据
static void LevelFocDataRead(uint8_t *rx_array){
	uint8_t index_ptr = 4;
	uint8_t index = 0;
	
	//第四位 错误信息
	LevelSensorData.mErrorCode = rx_array[index_ptr++];
	
	//第五位 电机控制模式	
	/*
	MODE_CURRENT		= 1,//电流控制模式——直接控制
	MODE_CURRENT_RAMP	= 2,//电流控制模式——梯形
	VELOCITY		= 3,//速度控制模式——直接控制
	VELOCITY_RAMP	= 4,//速度控制模式——梯形
	MODE_POSITION		= 5,//位置控制模式——直接控制
	MODE_POSITION_TRAP	= 6,//位置控制模式——梯形
	*/

	LevelSensorData.control_mode = rx_array[index_ptr++];
	
	//第六位 电机状态
	// CONTROL_MODE_RELAX	= 0 失能
	// CONTROL_MODE_CLOSE	= 1 闭环
	// CONTROL_MODE_CAIL	= 2 校准
	// CONTROL_MODE_ANTI	= 3 抗齿槽距校准
	LevelSensorData.motor_mode = rx_array[index_ptr++]; 
	
		//第七位 -> 第十位 电压值
	for(index=0;index < 4;index++)
		LevelSensorData.Vbus_Cur.u8_temp[index] = rx_array[index_ptr++];	
	
	//第十一位 -> 第十四位 实际电流
	for(index=0;index < 4;index++)
		LevelSensorData.cur_estimate_.u8_temp[index] = rx_array[index_ptr++];	

	//第十五位 -> 第十八位 实际位置
	for(index=0;index < 4;index++)
		LevelSensorData.pos_estimate_.u8_temp[index] = rx_array[index_ptr++];	
	
	//第十九位 -> 第二十二位 实际速度
	for(index=0;index < 4;index++)
		LevelSensorData.vel_estimate_.u8_temp[index] = rx_array[index_ptr++];		
	
	//第二十三位 -> 第二十六位 电流限制
	for(index=0;index < 4;index++)
		LevelSensorData.current_limit.u8_temp[index] = rx_array[index_ptr++];	
	
	//第二十七位 -> 第三十位 速度限制
	for(index=0;index < 4;index++)
		LevelSensorData.vel_limit.u8_temp[index] = rx_array[index_ptr++];		
	
	//第三十一位 -> 第三十四位 编码器cpr绝对值 (单圈：0-16384)
	for(index=0;index < 4;index++)
		LevelSensorData.count_in_cpr_.u8_temp[index] = rx_array[index_ptr++];		
	
	//第三十五位 -> 第三十八位 电机电阻
	for(index = 0; index < 4; index++)
		LevelSensorData.motor_phase_resistance.u8_temp[index] = rx_array[index_ptr++];			

	//第三十九位 -> 第四十二位 电机电感
	for(index = 0; index < 4; index++)
		LevelSensorData.motor_phase_inductance.u8_temp[index] = rx_array[index_ptr++];			
	
	//第四十三位 参数保存的flag
	LevelSensorData.save_config_flag =  rx_array[index_ptr++];		
	
	//第四十四位 使用电流/速度限制的flag
	LevelSensorData.set_vel_cur_limit_flag = rx_array[index_ptr++];	
	
	//第四十五位 清除电机错误的flag
	LevelSensorData.clear_error_flag = rx_array[index_ptr++];		
	
	
}//主机读取从机发过来的数据


//crc8 generator polynomial:G(x)=x8+x5+x4+1
static const unsigned char CRC8_INIT = 0xff;
static const unsigned char CRC8_TAB[256] = {
  0x00, 0x5e, 0xbc, 0xe2, 0x61, 0x3f, 0xdd, 0x83, 0xc2, 0x9c, 0x7e, 0x20, 0xa3, 0xfd, 0x1f, 0x41,
  0x9d, 0xc3, 0x21, 0x7f, 0xfc, 0xa2, 0x40, 0x1e, 0x5f, 0x01, 0xe3, 0xbd, 0x3e, 0x60, 0x82, 0xdc,
  0x23, 0x7d, 0x9f, 0xc1, 0x42, 0x1c, 0xfe, 0xa0, 0xe1, 0xbf, 0x5d, 0x03, 0x80, 0xde, 0x3c, 0x62,
  0xbe, 0xe0, 0x02, 0x5c, 0xdf, 0x81, 0x63, 0x3d, 0x7c, 0x22, 0xc0, 0x9e, 0x1d, 0x43, 0xa1, 0xff,
  0x46, 0x18, 0xfa, 0xa4, 0x27, 0x79, 0x9b, 0xc5, 0x84, 0xda, 0x38, 0x66, 0xe5, 0xbb, 0x59, 0x07,
  0xdb, 0x85, 0x67, 0x39, 0xba, 0xe4, 0x06, 0x58, 0x19, 0x47, 0xa5, 0xfb, 0x78, 0x26, 0xc4, 0x9a,
  0x65, 0x3b, 0xd9, 0x87, 0x04, 0x5a, 0xb8, 0xe6, 0xa7, 0xf9, 0x1b, 0x45, 0xc6, 0x98, 0x7a, 0x24,
  0xf8, 0xa6, 0x44, 0x1a, 0x99, 0xc7, 0x25, 0x7b, 0x3a, 0x64, 0x86, 0xd8, 0x5b, 0x05, 0xe7, 0xb9,
  0x8c, 0xd2, 0x30, 0x6e, 0xed, 0xb3, 0x51, 0x0f, 0x4e, 0x10, 0xf2, 0xac, 0x2f, 0x71, 0x93, 0xcd,
  0x11, 0x4f, 0xad, 0xf3, 0x70, 0x2e, 0xcc, 0x92, 0xd3, 0x8d, 0x6f, 0x31, 0xb2, 0xec, 0x0e, 0x50,
  0xaf, 0xf1, 0x13, 0x4d, 0xce, 0x90, 0x72, 0x2c, 0x6d, 0x33, 0xd1, 0x8f, 0x0c, 0x52, 0xb0, 0xee,
  0x32, 0x6c, 0x8e, 0xd0, 0x53, 0x0d, 0xef, 0xb1, 0xf0, 0xae, 0x4c, 0x12, 0x91, 0xcf, 0x2d, 0x73,
  0xca, 0x94, 0x76, 0x28, 0xab, 0xf5, 0x17, 0x49, 0x08, 0x56, 0xb4, 0xea, 0x69, 0x37, 0xd5, 0x8b,
  0x57, 0x09, 0xeb, 0xb5, 0x36, 0x68, 0x8a, 0xd4, 0x95, 0xcb, 0x29, 0x77, 0xf4, 0xaa, 0x48, 0x16,
  0xe9, 0xb7, 0x55, 0x0b, 0x88, 0xd6, 0x34, 0x6a, 0x2b, 0x75, 0x97, 0xc9, 0x4a, 0x14, 0xf6, 0xa8,
  0x74, 0x2a, 0xc8, 0x96, 0x15, 0x4b, 0xa9, 0xf7, 0xb6, 0xe8, 0x0a, 0x54, 0xd7, 0x89, 0x6b, 0x35,
};


unsigned char Get_CRC8_Check_Sum(unsigned char *pchMessage,unsigned int dwLength,unsigned char ucCRC8){
  unsigned char ucIndex;
  while (dwLength--){
    ucIndex = ucCRC8^(*pchMessage++);
    ucCRC8 = CRC8_TAB[ucIndex];
  }
  return(ucCRC8);
}


/*
** Descriptions: CRC8 Verify function
** Input: Data to Verify,Stream length = Data + checksum
** Output: True or False (CRC Verify Result)
*/
unsigned int Verify_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength){
  unsigned char ucExpected = 0;
  if ((pchMessage == 0) || (dwLength <= 2)) 
    return 0;
  ucExpected = Get_CRC8_Check_Sum (pchMessage, dwLength-1, CRC8_INIT);
  return ( ucExpected == pchMessage[dwLength-1] );
}


/*
** Descriptions: append CRC8 to the end of data
** Input: Data to CRC and append,Stream length = Data + checksum
** Output: True or False (CRC Verify Result)
*/
void Append_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength){
  unsigned char ucCRC = 0;
  if ((pchMessage == 0) || (dwLength <= 2)) return;
  ucCRC = Get_CRC8_Check_Sum ( (unsigned char *)pchMessage, dwLength-1, CRC8_INIT);
  pchMessage[dwLength-1] = ucCRC;
}


uint16_t CRC_INIT = 0xffff;
const uint16_t wCRC_Table[256] = {
  0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
  0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
  0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
  0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
  0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
  0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
  0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
  0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
  0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
  0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
  0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
  0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
  0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
  0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
  0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
  0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
  0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
  0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
  0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
  0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
  0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
  0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
  0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
  0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
  0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
  0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
  0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
  0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
  0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
  0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
  0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
  0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};


/*
** Descriptions: CRC16 checksum function
** Input: Data to check,Stream length, initialized checksum
** Output: CRC checksum
*/
uint16_t Get_CRC16_Check_Sum(uint8_t *pchMessage,uint32_t dwLength,uint16_t wCRC){
  uint8_t chData;
  if (pchMessage == NULL){
    return 0xFFFF;
  }
  while(dwLength--){
    chData = *pchMessage++;
    (wCRC) = ((uint16_t)(wCRC) >> 8) ^ wCRC_Table[((uint16_t)(wCRC) ^
    (uint16_t)(chData)) & 0x00ff];
  }
  return wCRC;
}


/*
** Descriptions: CRC16 Verify function
** Input: Data to Verify,Stream length = Data + checksum
** Output: True or False (CRC Verify Result)
*/
uint32_t Verify_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength){
  uint16_t wExpected = 0;
  if ((pchMessage == NULL) || (dwLength <= 2)){
    return 0;
  }
  wExpected = Get_CRC16_Check_Sum ( pchMessage, dwLength - 2, CRC_INIT);
  return ((wExpected & 0xff) == pchMessage[dwLength - 2] && ((wExpected >> 8) & 0xff) == pchMessage[dwLength - 1]);
}


/*
** Descriptions: append CRC16 to the end of data
** Input: Data to CRC and append,Stream length = Data + checksum
** Output: True or False (CRC Verify Result)
*/
void Append_CRC16_Check_Sum(uint8_t * pchMessage,uint32_t dwLength){
  uint16_t wCRC = 0;
  if ((pchMessage == NULL) || (dwLength <= 2)){
    return ;
  }
  wCRC = Get_CRC16_Check_Sum ( (uint8_t *)pchMessage, dwLength-2, CRC_INIT );
  pchMessage[dwLength-2] = (uint8_t)(wCRC & 0x00ff);
  pchMessage[dwLength-1] = (uint8_t)((wCRC >> 8)& 0x00ff);
}