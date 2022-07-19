#include "serial_w_r.h"

using namespace std;


uint8_t check_num(uint8_t count)
{
    uint8_t check_num=0;
    for(uint8_t i=0;i<count;i++)
    {
        check_num = check_num^sd.send_array[i];
    }
    return check_num;
}

void cmd_vel_callback(const geometry_msgs::Twist& msg)
{
    //打印接收到的消息
    ROS_INFO("%f,%f,%f",msg.linear.x,msg.linear.y,msg.angular.z);

    //通过串口发送到小车
    short  transition;  //intermediate variable //中间变量
    
    sd.send_array[0]=0x7B; //frame head 0x7B //帧头0X7B
    sd.send_array[1] = 0; //set aside //预留位
    sd.send_array[2] = 0; //set aside //预留位

    //The target velocity of the X-axis of the robot
    //机器人x轴的目标线速度
    transition=0;
    transition = msg.linear.x*1000; //将浮点数放大一千倍，简化传输
    sd.send_array[4] = transition;     //取数据的低8位
    sd.send_array[3] = transition>>8;  //取数据的高8位

    //The target velocity of the Y-axis of the robot
    //机器人y轴的目标线速度
    transition=0;
    transition = msg.linear.y*1000;
    sd.send_array[6] = transition;
    sd.send_array[5] = transition>>8;

    //The target angular velocity of the robot's Z axis
    //机器人z轴的目标角速度
    transition=0;
    transition = msg.angular.z*1000;
    sd.send_array[8] = transition;
    sd.send_array[7] = transition>>8;

    sd.send_array[9]=check_num(9); //For the BCC check bits, see the Check_Sum function //BCC校验位，规则参见Check_Sum函数
    sd.send_array[10]=0x7D; //frame tail 0x7D //帧尾0X7D

    serial::Serial ser;
    ser.setPort("/dev/ttyUSB0");
    ser.setBaudrate(115200);
    serial::Timeout to = serial::Timeout::simpleTimeout(100);
    ser.setTimeout(to);

    try
    {
        ser.open();
    }
    catch(serial::IOException& e)
    {
        ROS_ERROR_STREAM("Unable to open port!");
    }
    if(ser.isOpen())
    {
        ROS_INFO_STREAM("Open port successfully!");
    }
    
    try
    {
        ser.write(sd.send_array,sizeof (sd.send_array)); //Sends data to the downloader via serial port //通过串口向下位机发送数据 
    }
    catch (serial::IOException& e)   
    {
        ROS_ERROR_STREAM("Unable to send data through serial port"); //If sending data fails, an error message is printed //如果发送数据失败，打印错误信息
    }    

}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "serial_port");
    ros::NodeHandle n;

    ros::Subscriber cmd_vel_sub = n.subscribe("cmd_vel",100,cmd_vel_callback);

    ros::spin();
    return 0;
}



