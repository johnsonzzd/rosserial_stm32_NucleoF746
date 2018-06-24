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
 	 http://wiki.ros.org/rosserial_mbed/Tutorials/

   	1st terminal. roscore
	2nd terminal. rosrun rosserial_python serial_node.py _port:=/dev/ttyACM0 _baud:=57600
	3rd terminal. rostopic list 、 rostopic echo /chatter

	rostopic pub toggle_led std_msgs/Empty --once

	rosrun tf tf_echo odom base_link,	or,  rosrun tf view_frames
 *
 */

#include "main.h"
#include "stm32f7xx_hal.h"
#include "ringbuffer.h"

#include "ros.h"
#include "ros/time.h"
#include "std_msgs/String.h"
#include "std_msgs/Empty.h"
#include "tf/transform_broadcaster.h"

extern UART_HandleTypeDef huart3;

const static uint16_t rbuflen = 1024;
uint8_t RxBuffer[rbuflen];
struct ringbuffer rb;

void messageCb(const std_msgs::Empty& toggle_msg)
{
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}

ros::NodeHandle nh;
std_msgs::String str_msg;
ros::Publisher chatter("chatter", &str_msg);
ros::Subscriber<std_msgs::Empty> sub("toggle_led", &messageCb);

geometry_msgs::TransformStamped t;
tf::TransformBroadcaster broadcaster;

char base_link[] = "/base_link";
char odom[] = "/odom";

char hello[] = "test from STM32!";

//在C中如何调用C++函数：将函数用extern "C"声明
extern "C" void USART_RX_Callback()
{
	ringbuffer_putchar(&rb, huart3.Instance->RDR);
}


extern "C" void setup()
{
	ringbuffer_init(&rb, RxBuffer, rbuflen);
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);

	// Initialize ROS
	nh.initNode();
	nh.advertise(chatter);
	nh.subscribe(sub);
	broadcaster.init(nh);
}

extern "C" void loop()
{
	static int last_time=0,last_time1=0;

	if (HAL_GetTick() - last_time > 1000) {
		HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
		str_msg.data = hello;
		chatter.publish(&str_msg);
		last_time = HAL_GetTick();
	}

	if (HAL_GetTick() - last_time > 10) {
		t.header.frame_id = odom;
		t.child_frame_id = base_link;
		t.transform.translation.x = 1.0;
		t.transform.rotation.x = 0.0;
		t.transform.rotation.y = 0.0;
		t.transform.rotation.z = 0.0;
		t.transform.rotation.w = 1.0;
		t.header.stamp = nh.now();
		broadcaster.sendTransform(t);
	}

	nh.spinOnce();
}







