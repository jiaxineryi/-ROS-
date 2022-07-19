#ifndef __SERIAL_W_R
#define __SERIAL_W_R

#include "ros/ros.h"
#include <iostream>
#include <serial/serial.h>
#include <geometry_msgs/Twist.h>


struct SendData
{
    uint8_t send_array[11];
    float linearX;
    float linearY;
    float angleZ;
}sd;

class serial_w_r
{
    public:
    serial_w_r();
    ~serial_w_r();


};

#endif