
#include "posture_ctrl.h"

void gait(	GaitParams params,LegGain gait_gains,
            float leg0_offset, float leg1_offset,float leg2_offset, float leg3_offset,
            float leg0_direction, float leg1_direction,float leg2_direction, float leg3_direction);
void gait_detached(	GaitParams params_left,GaitParams params_right,
                    float leg0_offset, float leg1_offset,float leg2_offset, float leg3_offset,
                    float leg0_direction, float leg1_direction,float leg2_direction, float leg3_direction);
void CoupledMoveLeg(float t, GaitParams params,float gait_offset, float leg_direction, int LegId);
void CycloidTrajectory (float t, GaitParams params, float gait_offset) ;
void SinTrajectory (float t, GaitParams params, float gait_offset) ;
void CartesianToTheta(float leg_direction) ;
void SetCoupledPosition(int LegId);
void CommandAllLegs(LegGain gains);
bool IsValidLegGain( LegGain gains);
bool IsValidGaitParams( GaitParams params);
void ChangeTheGainsOfPD(LegGain gains);

void StartJump(float start_time_s);
void TrajectoryJump(float t, float launchTime, float stanceHeight, float downAMP) ;
void ExecuteJump();

void StartPosToMiddlePos (void);
void MiddlePosToEndPos (void);
float x, y, theta1, theta2, testtheta;

enum States state = STOP;

extern TaskHandle_t MotorControlTask_Handler;

//初始化步态参数
GaitParams gait_params = {17.3,12.0,4.0,0.00,0.25,1};
//初始化步态增益
LegGain gait_gains = {15.5, 0.00, 25.0, 0.00};
LegGain state_gait_gains[] = {
    //{kp_spd, kd_spd, kp_pos, kd_pos}
    {15.5, 0.00, 25.0, 0.00}, // STOP
    {15.5, 0.00, 25.0, 0.00}, // PRONK
    {15.5, 0.00, 25.0, 0.00},	// TROT
    {15.5, 0.00, 25.0, 0.00}, // PACE
    {15.5, 0.00, 25.0, 0.00}, // BOUND

    {15.5, 0.00, 25.0, 0.00}, // WALK AHEAD
    {15.5, 0.00, 25.0, 0.00}, // WALK BACK
    {15.5, 0.00, 25.0, 0.00}, // WALK LEFT
    {15.5, 0.00, 25.0, 0.00}, // WALK RIGHT
    {15.5, 0.00, 25.0, 0.00}, // ROTATE LEFT
    {15.5, 0.00, 25.0, 0.00}, // ROTATE RIGHT
    {15.5, 0.00, 25.0, 0.00}, // JUMP
};
//设定每一种步态的具体参数
GaitParams state_gait_params[] = {
    //{stance_height, step_length, up_amp, down_amp, flight_percent, freq}  cm
    //{步高, 步长, (上)抬腿高, (下)下压腿高度, 飞行时间占比, 频率}	单位 cm
    {NAN, NAN, NAN, NAN, NAN, NAN}, // STOP
		{NAN, NAN, NAN, NAN, NAN, NAN}, // RELASE
    {16.0, 0.00, 0.00, 5.00, 0.75, 1.0}, // PRONK		//单拍步态 四足跳跃
		
    {16.0, 15.0, 5.00, 2.00, 0.25, 2.3}, // TROT		//双拍步态 对角小跑
		
    {17.3, 15.0, 6.00, 4.00, 0.35, 2.0}, // PACE		//双拍步态 同侧溜步
    {17.3, 0.00, 5.00, 4.00, 0.35, 2.5}, // BOUND		//双拍步态 跳跑

    {20.3, 16.0, 8.00, 0.00, 0.20, 2.2}, // WALK AHEAD
    {15.0, 6.00, 4.00, 0.00, 0.25, 0.5}, // WALK BACK
		
    {17.3, 10.0, 6.00, 0.00, 0.35, 2.0}, // WALK LEFT
    {17.3, 10.0, 6.00, 0.00, 0.35, 2.0}, // WALK RIGHT
		
    {17.3, 12.0, 5.00, 0.00, 0.25, 2.3}, // ROTATE LEFT
    {17.3, 12.0, 5.00, 0.00, 0.25, 2.3}, // ROTATE RIGHT
    {NAN, NAN, NAN, NAN, NAN, NAN}, // JUMP
		{NAN, NAN, NAN, NAN, NAN, NAN}, // START
		{NAN, NAN, NAN, NAN, NAN, NAN} // END
};

/**
*	最低walk {12.0, 15.0, 1.8, 0.00, 0.50, 1.0}
*	效果不错{14.0, 12.0, 3.5, 0.00, 0.50, 1.0},
*	{16.0, 12.00, 5.0, 0.00, 0.20, 2.0}, // WALK		ahead 不错
*	{17.3, 12.00, 4.0, 0.00, 0.35, 2.5}, // WALK		ahead
* 对角小跑步态不错速度和稳定都非常好
*/

GaitParams gait_params_1 = {16.0, 16.00, 5.0, 0.00, 0.50, 2.0};
GaitParams gait_params_2 = {16.0, 16.00, 5.0, 0.00, 0.20, 2.0};


/*******************************************************************************************
	*@ 任务名称：void PostureControl_task(void *pvParameters)
	*@ 功能：姿态控制
	*@ 备注：选择状态
 *******************************************************************************************/
void PostureControl_task(void *pvParameters)
{



    for(;;)
    {
        GaitParams gait_params = state_gait_params[state];
        // LegGain gait_gains =state_gait_gains[state] ;
        switch(state) {
        case STOP:		//停止


            x=0;
            y = 17.3205081;
            CartesianToTheta(1.0);
            CommandAllLegs(gait_gains);

            break;
        case REALSE:		// 释放 什么都不做
            break;
            // vTaskDelay(10);
            //printf("\r\n x=%f  y=%f  theta1=%f  theta2=%f   he=%f ",x,y,theta1,theta2,theta1+theta2);
            break;
        case PRONK:		//单拍步态 四足跳跃
            gait(gait_params, gait_gains, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0);
            break;
        case TROT:		//双拍步态 对角小跑
            gait(gait_params, gait_gains, 0.0, 0.5, 0.5, 0.0, 1.0, 1.0, 1.0, 1.0);
            break;
        case PACE:		//双拍步态 同侧溜步
            gait(gait_params, gait_gains, 0.5, 0.0, 0.5, 0.0, 1.0, 1.0, 1.0, 1.0);
            break;
        case BOUND:		//双拍步态 跳跑
            gait(gait_params, gait_gains, 0.0, 0.0, 0.5, 0.5, 1.0, 1.0, 1.0, 1.0);
            break;
        case WALK_AHEAD:		//前进
            gait(gait_params, gait_gains, 0.0, 0.5, 0.75, 0.25, 1.0, 1.0, 1.0, 1.0);
            break;
        case WALK_BACK:		//后退
            gait(gait_params, gait_gains, 0.00, 0.5, 0.75, 0.25, -1.0, -1.0, -1.0, -1.0);
            break;
        case WALK_LEFT:		//向左偏走
            gait_detached(gait_params_1, gait_params_2, 0.0, 0.5, 0.75, 0.25, 1.0, 1.0, 1.0, 1.0);
            break;
        case WALK_RIGHT:		//向右偏走
            gait_detached(gait_params_2, gait_params_1, 0.0, 0.5, 0.75, 0.25, 1.0, 1.0, 1.0, 1.0);
            break;
        case ROTAT_LEFT:		//原地左转
            gait(gait_params, gait_gains, 0.0, 0.5, 0.5, 0.00, -1.0, 1.0, -1.0, 1.0);
            break;
        case ROTAT_RIGHT:		//原地右转
            gait(gait_params, gait_gains, 0.0, 0.5, 0.5, 0.00, 1.0, -1.0, 1.0, -1.0);
            break;
        case JUMP:		//跳跃
            ExecuteJump();
            break;
        case START:		//
            StartPosToMiddlePos();
            break;
				 case END:		//
            MiddlePosToEndPos();
            break;
				 
        }
        vTaskDelay(1);
    }
}

/**
* NAME: void gait_detached
* FUNCTION : 左右分离的腿部增益函数
*/
void gait_detached(	GaitParams params_left,GaitParams params_right,
                    float leg0_offset, float leg1_offset,float leg2_offset, float leg3_offset,
                    float leg0_direction, float leg1_direction,float leg2_direction, float leg3_direction) {

    float t = times/1000.0;

    //printf("\r\n t=%f",t);
    // const float leg0_direction = 1.0;
    CoupledMoveLeg( t, params_left, leg0_offset, leg0_direction, 0);

    // const float leg1_direction = 1.0;
    CoupledMoveLeg( t, params_right, leg1_offset, leg1_direction, 1);

    // const float leg2_direction = 1.0;
    CoupledMoveLeg( t, params_left, leg2_offset, leg2_direction, 2);

    //  const float leg3_direction = 1.0;
    CoupledMoveLeg( t, params_right, leg3_offset, leg3_direction, 3);


}

/**
* NAME: void gait(	GaitParams params,float leg0_offset, float leg1_offset,float leg2_offset, float leg3_offset)
* FUNCTION : 产生时间脉冲 设定每个腿的参数 调整腿的运行方向 进行补偿
*/
void gait(	GaitParams params,LegGain gains,
            float leg0_offset, float leg1_offset,float leg2_offset, float leg3_offset,
            float leg0_direction, float leg1_direction,float leg2_direction, float leg3_direction) {

    if (!IsValidGaitParams(params) || !IsValidLegGain(gains)) {
        return;
    }

    float t = times/1000.0;

    //printf("\r\n t=%f",t);

    // const float leg0_direction = 1.0;
    CoupledMoveLeg( t, params, leg0_offset, leg0_direction, 0);

    // const float leg1_direction = 1.0;
    CoupledMoveLeg( t, params, leg1_offset, leg1_direction, 1);

    // const float leg2_direction = 1.0;
    CoupledMoveLeg( t, params, leg2_offset, leg2_direction, 2);

    //  const float leg3_direction = 1.0;
    CoupledMoveLeg( t, params, leg3_offset, leg3_direction, 3);

    //改变PD
    // ChangeTheGainsOfPD(gains);
}

/**
* NAME: void CoupledMoveLeg(float t, GaitParams params,float gait_offset, float leg_direction, int LegId)
* FUNCTION : 驱动并联腿 传递参数
*/
void CoupledMoveLeg(float t, GaitParams params,float gait_offset, float leg_direction, int LegId)
{
    SinTrajectory(t, params, gait_offset);		//足端摆线轨迹生成器
    CartesianToTheta(leg_direction);		//笛卡尔坐标转换到伽马坐标
    SetCoupledPosition(LegId);		//发送数据给电机驱动函数
//  printf("\r\nt=%f x=%f  y=%f  theta1=%f  theta2=%f  legid=%d he%f",t,x,y,theta1,theta2,LegId,theta1+theta2);
}

/**
* NAME: SinTrajectory (float t,GaitParams params, float gaitOffset)
* FUNCTION : 正弦轨迹生成器
*/
void SinTrajectory (float t,GaitParams params, float gaitOffset) {
    static float p = 0;
    static float prev_t = 0;


    //x=y=0;
    float stanceHeight = params.stance_height;
    float downAMP = params.down_amp;
    float upAMP = params.up_amp;
    float flightPercent = params.flight_percent;
    float stepLength = params.step_length;
    float FREQ = params.freq;

    p += FREQ * (t - prev_t);
    prev_t = t;

    float gp = fmod((p+gaitOffset),1.0);
    if (gp <= flightPercent) {
        x = (gp/flightPercent)*stepLength - stepLength/2.0;
        y = -upAMP*sin(PI*gp/flightPercent) + stanceHeight;
    }
    else {
        float percentBack = (gp-flightPercent)/(1.0-flightPercent);
        x = -percentBack*stepLength + stepLength/2.0;
        y = downAMP*sin(PI*percentBack) + stanceHeight;
    }

    //  printf("\r\nt=%f x=%f y=%f",t,x,y);
}

/**
* NAME: void CycloidTrajectory (float t, GaitParams params, float gait_offset)
* FUNCTION : 摆线轨迹生成器
*/
//void CycloidTrajectory (float t, GaitParams params, float gait_offset)
//{
//    float S0 = params.step_length;
//    float H0 = params.stance_height;
//    float T = params.gait_cycle;
//    float Ty = params.swing_cycle;
//

////		printf("\r\n t=%f",t);
//    if(t>=0&&t<=Ty)		//足端摆动相
//    {
//        x=S0/(2*PI)*( (2*PI*t/Ty)-sin((2*PI*t/Ty)) );
//        if(t>=0&&t<=Ty/2)
//        {
//            y=2*H0*( t/Ty - sin( 4*PI*t/Ty )/(4*PI) );
//        }
//        else if(t>=Ty/2&&t<=Ty)
//        {
//            y=-2*H0*( t/Ty - sin( 4*PI*t/Ty )/(4*PI) )+2*H0;
//        }
//    }
//    else if(t>=Ty&&t<=T)		//足端支撑相
//    {
//        x=S0 - S0/(2*PI)*( (2*PI*(t-Ty)/(T-Ty))-sin((2*PI*(t-Ty)/(T-Ty))) );
//        y=0;
//    }
//    else
//        vTaskDelay(10);

//    x-=6.0;//将0点设置在穿过电机中心的竖直线上
//    y+=10.0*sqrt(3.0);//电机y初始位置 表示两条腿为水平的时

//}

/**
* NAME: void CartesianToThetaGamma(float leg_direction)
* FUNCTION : 笛卡尔坐标转换到伽马坐标 也就是将theta转换成XY
*/
void CartesianToTheta(float leg_direction)
{
    float L=0;
    float N=0;
    double M=0;
    float A1=0;
    float A2=0;

    L=sqrt(		pow(x,2)	+		pow(y,2)	);
    if(L<9.8||L>29.8)		//限位保护
        vTaskSuspend(MotorControlTask_Handler);
    N=asin(x/L)*180.0/PI;
    M=acos(	(pow(L,2)+pow(L1,2)-pow(L2,2))/(2*L1*L)	)*180.0/PI;
    A1=M-N;

    A2=M+N;

    if(leg_direction==1.0) {
        theta2=(A1-90.0);
        theta1=(A2-90.0);
    } else if(leg_direction==-1.0) {
        theta1=(A1-90.0);
        theta2=(A2-90.0);
    }

    //	printf("\r\n x=%f  y=%f  theta1=%f  theta2=%f   he=%f   L=%f  M=%lf  N=%f   SIXI=%f",x,y,theta1,theta2,theta1+theta2,L,M,N,(	(pow(L,2)+pow(L1,2)-pow(L2,2))/(2*L1*L)	));
}

/**
* NAME: void SetCoupledPosition( int LegId)
* FUNCTION : 发送电机控制角度
*/
void SetCoupledPosition( int LegId)
{
    //限位保护
    if((theta1+theta2)>178||(theta1+theta2)<-178||theta1>150||theta1<-150||theta2>150||theta2<-150)
        vTaskSuspend(MotorControlTask_Handler);

    if(LegId==0)
    {
        temp_pid.ref_agle[1]=-theta1*ReductionAndAngleRatio;
        temp_pid.ref_agle[0]=-theta2*ReductionAndAngleRatio;
    }
    else if(LegId==1)
    {
        temp_pid.ref_agle[2]=theta1*ReductionAndAngleRatio;
        temp_pid.ref_agle[3]=theta2*ReductionAndAngleRatio;
    }
    else if(LegId==2)
    {
        temp_pid.ref_agle[5]=-theta1*ReductionAndAngleRatio;
        temp_pid.ref_agle[4]=-theta2*ReductionAndAngleRatio;
    }
    else if(LegId==3)
    {
        temp_pid.ref_agle[6]=theta1*ReductionAndAngleRatio;
        temp_pid.ref_agle[7]=theta2*ReductionAndAngleRatio;

    }

    IsMotoReadyOrNot= IsReady;		//数据填充完毕

}

/**
* NAME: void CommandAllLegs(void)
* FUNCTION : 控制所有电机
*/
void CommandAllLegs(LegGain gains)
{
    if (!IsValidLegGain(gains)) {
        return;
    }

    //限位保护
    if((theta1+theta2)>170||(theta1+theta2)<-170||theta1>140||theta1<-140||theta2>140||theta2<-140)
        vTaskSuspend(MotorControlTask_Handler);

    ChangeTheGainsOfPD(gains);

    SetCoupledPosition(0);
    SetCoupledPosition(1);
    SetCoupledPosition(2);
    SetCoupledPosition(3);
}

/**
 *
 * 检测步态增益是否正确
 * @param  gains LegGain to check
 * @return       True if valid gains, false if invalid
 */
bool IsValidLegGain( LegGain gains) {
    // check for unstable gains
    bool bad =  gains.kp_spd < 0 || gains.kd_spd < 0 ||
                gains.kp_pos < 0 || gains.kd_pos < 0;
    if (bad) {
        printf("Invalid gains: <0");
        vTaskDelay(500);
        return false;
    }
    // check for instability / sensor noise amplification
    bad = bad || gains.kp_spd > 32 || gains.kd_spd > 1 ||
          gains.kp_pos > 32 || gains.kd_pos > 1;
    if (bad) {
        printf("Invalid gains: too high.");
        vTaskDelay(500);
        return false;
    }
    // check for underdamping -> instability
    bad = bad || (gains.kp_spd > 50 && gains.kd_spd < 0.1);
    bad = bad || (gains.kp_pos > 50 && gains.kd_pos < 0.1);
    if (bad) {
        printf("Invalid gains: underdamped");
        vTaskDelay(500);
        return false;
    }
    return true;
}
/**
 *
 * 检测步态参数是否正确
 * @param  gains LegGain to check
 * @return       True if valid gains, false if invalid
 */
bool IsValidGaitParams( GaitParams params) {
    const float maxL = 29.3;
    const float minL = 10.2;

    float stanceHeight = params.stance_height;
    float downAMP = params.down_amp;
    float upAMP = params.up_amp;
    float flightPercent = params.flight_percent;
    float stepLength = params.step_length;
    float FREQ = params.freq;

    if (stanceHeight + downAMP > maxL || sqrt(pow(stanceHeight, 2) + pow(stepLength / 2.0, 2)) > maxL) {
        printf("Gait overextends leg");
        vTaskDelay(500);
        return false;
    }
    if (stanceHeight - upAMP < minL) {
        printf("Gait underextends leg");
        vTaskDelay(500);
        return false;
    }

    if (flightPercent <= 0 || flightPercent > 1.0) {
        printf("Flight percent is invalid");
        vTaskDelay(500);
        return false;
    }

    if (FREQ < 0) {
        printf("Frequency cannot be negative");
        vTaskDelay(500);
        return false;
    }

    if (FREQ > 10.0) {
        printf("Frequency is too high (>10)");
        vTaskDelay(500);
        return false;
    }

    return true;
}
/**
 *改变腿部增益
 *调用了PID_reset_kpKd 函数
 */
void ChangeTheGainsOfPD(LegGain gains)
{
    static uint8_t count=0;
    if(count>=1) {

    } else {
        count++;
        for (int i = 0; i < 8; i++) {
            pid_reset_kpkd(&pid_pos[i],gains.kp_pos,gains.kd_pos);
            pid_reset_kpkd(&pid_spd[i],gains.kp_spd,gains.kd_spd);
        }
    }

}


// Privates
float start_time_ = 0.0f;

/**
 * Tell the position control thread to do the jump
 * @param start_time_s The timestamp of when the jump command was sent
 */
void StartJump(float start_time_s) {
    start_time_ = start_time_s;
    state = JUMP;
}

/**
* Linear increase in height for jump.
*/
void TrajectoryJump(float t, float launchTime, float stanceHeight, float downAMP) {
    //Need to check if n works
    float n = t/launchTime;
    x = 0;
    y = downAMP*n + stanceHeight;
    //y = downAMP*sin(PI/4 + PI/4*n) + stanceHeight;
}

/**
* Drives the ODrives in an open-loop, position-control sinTrajectory.
*/
void ExecuteJump() {
    // min radius = 0.8
    // max radius = 0.25

    // x= y= theta1= theta2=0.0;


    const float prep_time = 0.8f; // Duration before jumping [s]
    const float launch_time = 0.5f ; // Duration before retracting the leg [s]
    const float fall_time = 1.0f; //Duration after retracting leg to go back to normal behavior [s]

    const float stance_height = 14.0f; // Desired leg extension before the jump [cm]
    const float jump_extension = 24.9f; // Maximum leg extension in [cm]
    const float fall_extension = 13.0f; // Desired leg extension during fall [cm]


    float t = times/1000.0f - start_time_; // Seconds since jump was commanded

    if (t < prep_time) {
        x = 0;
        y = stance_height;
        CartesianToTheta(1.0);

        // Use gains with small stiffness and lots of damping
        LegGain gains = {15.5, 0.00, 25.0, 0.00};
        CommandAllLegs(gains);
        // Serial << "Prep: +" << t << "s, y: " << y;
    } else if (t >= prep_time && t < prep_time + launch_time) {
        x = 0;
        y = jump_extension;
        CartesianToTheta(1.0);

        // Use high stiffness and low damping to execute the jump
        LegGain gains = {15.5, 0.00, 25.0, 0.00};
        CommandAllLegs(gains);
        // Serial << "Jump: +" << t << "s, y: " << y;
    } else if (t >= prep_time + launch_time && t < prep_time + launch_time + fall_time) {
        x = 0;
        y = fall_extension;
        CartesianToTheta(1.0);

        // Use low stiffness and lots of damping to handle the fall
        LegGain gains = {15.5, 0.00, 25.0, 0.00};
        CommandAllLegs(gains);
        // Serial << "Retract: +" << t << "s, y: " << y;
    } else {
        state = STOP;
        printf("Jump Complete.");
    }

}

void StartPosToMiddlePos (void)
{
state = REALSE;


    for (int i = 0; i < 8; i++) {
        pid_reset_kpkd(&pid_pos[i], 45.0, 0);
        pid_reset_kpkd(&pid_spd[i], 15.5f, 0);
    }
    testtheta=95.0;
    temp_pid.ref_agle[0]-=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[1]+=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[2]-=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[3]+=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[4]-=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[5]+=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[6]-=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[7]+=testtheta*ReductionAndAngleRatio;
    IsMotoReadyOrNot= IsReady;		//数据填充完毕

    vTaskDelay(2000);



    for (int i = 0; i < 8; i++) {
        pid_reset_kpkd(&pid_pos[i], 28.0, 0);
        pid_reset_kpkd(&pid_spd[i], 15.5f, 0);
    }
    testtheta=-5.0;
    temp_pid.ref_agle[0]-=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[1]+=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[2]-=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[3]+=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[4]-=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[5]+=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[6]-=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[7]+=testtheta*ReductionAndAngleRatio;
    IsMotoReadyOrNot= IsReady;		//数据填充完毕

    vTaskDelay(1000);
		
		
    for (int i = 0; i < 8; i++) {
        pid_reset_kpkd(&pid_pos[i], 20.0, 0);
        pid_reset_kpkd(&pid_spd[i], 15.5f, 0);
    }
    testtheta=85.0;
    temp_pid.ref_agle[0]+=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[1]+=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[2]-=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[3]-=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[4]+=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[5]+=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[6]-=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[7]-=testtheta*ReductionAndAngleRatio;
    IsMotoReadyOrNot= IsReady;		//数据填充完毕


		vTaskDelay(1000);

   // printf(" moto_chassis = %d   moto_chassis[1]  =%d    ref_agle %f\n",moto_chassis[0].total_angle,moto_chassis[7].total_angle,ref_agle[0]);

    memset(&moto_chassis,0,sizeof(moto_measure_t)*8);

    for(int i=0; i<8; i++)
        temp_pid.ref_agle[i]=ref_agle[i]=0;

    vTaskDelay(100);
    //printf(" moto_chassis = %d   moto_chassis[1]  =%d  ref_agle %f\n",moto_chassis[0].total_angle,moto_chassis[7].total_angle,ref_agle[0]);

    state = REALSE;

    printf("Start Done.");


}

void MiddlePosToEndPos (void)
{
	state = REALSE;
	   for (int i = 0; i < 8; i++) {
        pid_reset_kpkd(&pid_pos[i], 24.0, 0);
        pid_reset_kpkd(&pid_spd[i], 15.5f, 0);
    }
    testtheta=-85.0;
    temp_pid.ref_agle[0]+=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[1]+=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[2]-=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[3]-=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[4]+=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[5]+=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[6]-=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[7]-=testtheta*ReductionAndAngleRatio;
    IsMotoReadyOrNot= IsReady;		//数据填充完毕


		vTaskDelay(1000);
		
		  for (int i = 0; i < 8; i++) {
        pid_reset_kpkd(&pid_pos[i], 15.0, 0);
        pid_reset_kpkd(&pid_spd[i], 10.5f, 0);
    }
    testtheta=-90.0;
    temp_pid.ref_agle[0]-=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[1]+=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[2]-=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[3]+=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[4]-=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[5]+=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[6]-=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[7]+=testtheta*ReductionAndAngleRatio;
    IsMotoReadyOrNot= IsReady;		//数据填充完毕

    vTaskDelay(1000);
	
		
		 memset(&moto_chassis,0,sizeof(moto_measure_t)*8);
    for(int i=0; i<8; i++)
        temp_pid.ref_agle[i]=ref_agle[i]=0;

    vTaskDelay(100);
		
 state = REALSE;

    printf("END Done.");

}









