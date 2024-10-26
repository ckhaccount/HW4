#include "Motor.h"
#include "main.h"
#include "vector"
M3508_Motor::M3508_Motor()
{
    ratio_=3591.0/187;
    angle_=0;
    delta_angle_=0;
    ecd_angle_=0;
    last_ecd_angle_=0;
    delta_ecd_angle_=0;
    rotate_speed_=0;
    current_=0;
    temp_=25;
}

float M3508_Motor::linearMapping(int in, int in_min, int in_max, float out_min, float out_max)
{
    float n=0.0;
    n=(out_max-out_min)/(in_max-in_min)*(in-in_min)+out_min;
    return (n);
}
void M3508_Motor::canRxMsgCallback(uint8_t rx_data[8])
{
    this->last_ecd_angle_=ecd_angle_;
    this->ecd_angle_=linearMapping((rx_data[0]<<8)|rx_data[1],0,8191,0,360);
    this->rotate_speed_=(rx_data[2]<<8)|rx_data[3];
    this->current_=linearMapping((rx_data[4]<<8)|rx_data[5]-16384,
    -16384,16384,-20,20);
    this->temp_=rx_data[6];
    if(ecd_angle_>last_ecd_angle_){
        this->delta_ecd_angle_=ecd_angle_-last_ecd_angle_;
    }
    else{
        this->delta_ecd_angle_=360+ecd_angle_-last_ecd_angle_;
    }
    this->delta_angle_=this->delta_ecd_angle_/this->ratio_;
    this->angle_+=this->delta_angle_;
}
std::array<float, 6> M3508_Motor::Get_motor_message() const
{
    return {ratio_, delta_angle_, delta_ecd_angle_, rotate_speed_, current_, temp_};
}
