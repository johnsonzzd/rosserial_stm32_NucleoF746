# rosserial_NucleoF746
https://answers.ros.org/question/277324/stm32-with-ros-rosserial-in-eclipse-how/
http://www.rosclub.cn/post-948.html
https://blog.csdn.net/wubaobao1993/article/details/70808959


(1)	使用CubeMX生成SW4STM工程，配置USART2串口（连接usb）可用
(2)	工程属性，Convert to C++
(3)	使用ros_lib库
	首先按照http://wiki.ros.org/rosserial_arduino/Tutorials上面的步骤配置好arduino IDE中的ros_lib，之后到~/sketchbook/libraries（默认目录）目录下复制ros_lib文件夹中的全部内容到建立的工程中。
	工程中新建source folder，选择ros_lib。
	在工程的配置中，C/C++中的include path中把ros _lib路径加进去。
(4)	核心文件
新建STM32Hardware.h，实现串口读写，保存在ros_lib路径下。并包含到ros.h中。
串口读写可以采用轮询、中断、DMA等多种方式。
(5)	测试
	1st terminal. roscore
	2nd terminal. rosrun rosserial_python serial_node.py _port:=/dev/ttyACM0 _baud:=57600
	3rd terminal. rostopic list 、 rostopic echo /chatter





