/*
 * my_main.cpp
 *
 *  Created on: 2018年6月24日
 *      Author: Johnson
 */
/*
 * my_main.cpp
 *
 *  Created on: 2018年6月22日
 *      Author: Johnson
 *
 *
   	1st terminal. roscore
	2nd terminal. rosrun rosserial_python serial_node.py _port:=/dev/ttyACM0 _baud:=57600
	3rd terminal. rostopic list 、 rostopic echo /chatter
 *
 */

#include "main.h"
#include "stm32f7xx_hal.h"
#include "ringbuffer.h"

#include "ros.h"
#include "std_msgs/String.h"

extern UART_HandleTypeDef huart3;

const static uint16_t rbuflen = 1024;
uint8_t RxBuffer[rbuflen];
struct ringbuffer rb;

ros::NodeHandle nh;
std_msgs::String str_msg;
ros::Publisher chatter("chatter", &str_msg);

char hello[] = "test from STM32!";

//在C中如何调用C++函数：将函数用extern "C"声明
extern "C" void USART_RX_Callback()
{
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

	ringbuffer_putchar(&rb, huart3.Instance->RDR);
}

extern "C" void setup()
{
	ringbuffer_init(&rb, RxBuffer, rbuflen);
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);

	// Initialize ROS
	nh.initNode();
	nh.advertise(chatter);
}

extern "C" void loop()
{
	HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
	//HAL_UART_Transmit(&huart2, (uint8_t *)hello, strlen(hello), HAL_MAX_DELAY);

	str_msg.data = hello;
	chatter.publish(&str_msg);
	nh.spinOnce();

	HAL_Delay(1000);
}







