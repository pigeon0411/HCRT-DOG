#include "combinations.h"

//bool optoelec_switch[2];

/**
*��϶��� ��������д��
*�У�		JUMP		��ʼ����		��������		Խ��		�����߼�
*/
bool GROUND_SELECT=BLUE_GROUNG;  //������ѡ��

void StepOver(void)
{
    StepOver_one_leg(0);  //��0
    vTaskDelay(150);

    StepOver_one_leg(1);	//��1
    vTaskDelay(150);

    state=REALSE;				//�ͷ�
    vTaskDelay(150);

    x = 18*sin(40*PI/180);		//ǰ�����ȱ���һ��СС��վ���� ��һ��
    y = 18*cos(40*PI/180);
    CartesianToTheta(1.0);
    SetCoupledPosition(0);
    SetCoupledPosition(1);
    vTaskDelay(100);

    _leg_active[0]=NO;		//�ر�ǰ������
    _leg_active[1]=NO;
    state=TEST4;				//ǰ���������
    vTaskDelay(300);
    _leg_active[0]=YES;		//��ǰ������
    _leg_active[1]=YES;
    vTaskDelay(3000);

    state=TEST6;				//С�鲽
    vTaskDelay(1500);

    state=REALSE; 			//�ͷ�

    StepOver_one_leg(2);	//��2
    vTaskDelay(400);

    StepOver_one_leg(3);	//��3
    vTaskDelay(400);

    state=REALSE;		//�ͷ�
    vTaskDelay(500);

    x = 18.6*sin(28*PI/180);		//�������� Сվ ��ǰ��
    y = 18.6*cos(28*PI/180);
    CartesianToTheta(1.0);
    SetCoupledPosition(2);
    SetCoupledPosition(3);
    vTaskDelay(100);

    _leg_active[2]=NO;		//�رպ�������
    _leg_active[3]=NO;
    state=TEST5; //ǰ������ �����������
    vTaskDelay(1000);

    _leg_active[2]=YES;		//�򿪺�������
    _leg_active[3]=YES;
    vTaskDelay(2000);

    state=REALSE;		//�ͷ�
    vTaskDelay(250);

    state=TROT;		    //С������ɳ��

    vTaskDelay(250);

}


void StepOver_one_leg(int LegId)
{

    //------�ĸ�����ȫ��̧�����-----//

    if(LegId==0||LegId==2)
    {
        x = 0;
        y = 28;
        CartesianToTheta(1.0);
        CommandAllLegs_v();
        vTaskDelay(600);
    }
    else if(LegId==1||LegId==3)
    {
        vTaskDelay(200);
    }

    //--------�Խ��������--------//
    int rev_legid, rev_legid1;
    if(LegId==0) {
        rev_legid=3 ;
        y = 25;
    }
    else if(LegId==1) {
        x = 0;
        y = 28;
        CartesianToTheta(1.0);
        SetCoupledPosition(3);
        vTaskDelay(300);

        rev_legid=2 ;
        y = 25 ;
    }

    else if(LegId==2) {
        rev_legid=1 ;
        y = 25;
    }
    else if(LegId==3) {
        rev_legid=0 ;
        y = 16;
    }
    x = 0;
    CartesianToTheta(1.0);
    SetCoupledPosition(rev_legid);
    vTaskDelay(300);

    //----ֻ��leg3��ʱ������----ǰ�����Ƚ���ƽ��״̬---//

    if(LegId==3)
    {

        if(GROUND_SELECT==RED_GROUNG) {  //�������ж� �쳡

            x = -18*sin(17.2*PI/180);
            y = 18*cos(17.2*PI/180);
            CartesianToTheta(1.0);
            SetCoupledPosition(0);

            x = -21*sin(15*PI/180);
            y = 21*cos(15*PI/180);
            CartesianToTheta(1.0);
            SetCoupledPosition(1);

            x = 22*sin(10*PI/180);
            y = 22*cos(40*PI/180)-4;		//�߶ȱ���
            CartesianToTheta(1.0);
            SetCoupledPosition(2);

            vTaskDelay(1500);
        }
        else if(GROUND_SELECT==BLUE_GROUNG) {

            x = -18*sin(17.2*PI/180);
            y = 18*cos(17.2*PI/180);
            CartesianToTheta(1.0);
            SetCoupledPosition(0);

            x = -21*sin(15*PI/180);
            y = 21*cos(15*PI/180);
            CartesianToTheta(1.0);
            SetCoupledPosition(1);

            x = 22*sin(10*PI/180);
            y = 22*cos(40*PI/180)-4;		//�߶ȱ���
            CartesianToTheta(1.0);
            SetCoupledPosition(2);

            vTaskDelay(1300);
        }

    }

    //--------��������--------//
    if(LegId==3)
    {
        x = -10*sin(80*PI/180);
        y = 10*cos(80*PI/180);
        CartesianToTheta(1.0);
        SetCoupledPosition(LegId);
        vTaskDelay(600);
    }
    else
    {

//        x = -10*sin(60*PI/180);
//        y = 10*cos(60*PI/180);
//        CartesianToTheta(1.0);
//        SetCoupledPosition(LegId);
//        vTaskDelay(600);

    }

    //-------�������ǰ----------//
    x = 10*sin(60*PI/180);
    y = 10*cos(60*PI/180);
    CartesianToTheta(1.0);
    SetCoupledPosition(LegId);
    vTaskDelay(400);

    //--------���״̬��ǰ--------//
    x = 26*sin(80*PI/180);
    y = 26*cos(80*PI/180);
    CartesianToTheta(1.0);
    SetCoupledPosition(LegId);
    vTaskDelay(400);

    //--------���״̬���´���--------//
    if(LegId==0||LegId==1)
    {
        x = 29*sin(34*PI/180);
        y = 29*cos(34*PI/180);
    }
    else if(LegId==2)
    {
        x = 22*sin(40*PI/180);
        y = 22*cos(40*PI/180);
    }
    else if(LegId==3)
    {
        x = 20*sin(50*PI/180);
        y = 20*cos(50*PI/180);
    }
    CartesianToTheta(1.0);
    SetCoupledPosition(LegId);
    vTaskDelay(400);

}



void CrossTheLine(void)
{

    state=REALSE;

    CrossTheLine_one_leg(0); //��0
    vTaskDelay(150);

    CrossTheLine_one_leg(1);	//��1
    vTaskDelay(150);

    now_time=times;
    state=TEST1;  //------------��ǰ���ú��ȽӴ�����
    vTaskDelay(1900);
    state=REALSE;

    CrossTheLine_one_leg(2);		//��2
    vTaskDelay(150);

    CrossTheLine_one_leg(3);		//��3
    vTaskDelay(150);

    //**********�м�����**********//
    now_time=times;
    state=TEST1;

    OpenMvInspect(openmv_Red);  //��⵽��ɫ֮��ʼ���ڶ�������

    IndicateLED_On;

    state=STOP;
    state=REALSE;

    //**********END**********//

    CrossTheLine_one_leg(0);
    vTaskDelay(200);

    IndicateLED_Off;

    CrossTheLine_one_leg(1);
    vTaskDelay(200);

    now_time=times;
    state=TEST1;  //�����Ӳ�̬
    vTaskDelay(1900);
    state=REALSE;

    CrossTheLine_one_leg(2);
    vTaskDelay(200);

    CrossTheLine_one_leg(3);
    vTaskDelay(200);
}

void CrossTheLine_one_leg(int LegId)
{

    //------�ĸ�����ȫ��̧�����-----//
    x = 0;
    y = 28;
    CartesianToTheta(1.0);
    CommandAllLegs_v();
    vTaskDelay(600);

    //--------�Խ��������--------//
    int rev_legid;
    if(LegId==0) rev_legid=3;
    else if(LegId==1) rev_legid=2;
    else if(LegId==2) rev_legid=1;
    else if(LegId==3) rev_legid=0;
    x = 0;
    y = 24.2;
    CartesianToTheta(1.0);
    SetCoupledPosition(rev_legid);
    vTaskDelay(200);

    //��ǰ�������µ�һ�±���ƽ��//
    x = 0;
    y = 29.5;
    CartesianToTheta(1.0);
    SetCoupledPosition(LegId);
    vTaskDelay(1000);

    //--------���������--------//
    x = -24*sin(80*PI/180);
    y = 24*cos(80*PI/180);
    CartesianToTheta(1.0);
    SetCoupledPosition(LegId);
    vTaskDelay(300);

    //--------��������ں���--------//
    x = -10*sin(80*PI/180);
    y = 10*cos(80*PI/180);
    CartesianToTheta(1.0);
    SetCoupledPosition(LegId);
    vTaskDelay(380);

    //--------�����״̬��ǰ--------//
    x = 10*sin(75*PI/180);
    y = 10*cos(75*PI/180);
    CartesianToTheta(1.0);
    SetCoupledPosition(LegId);
    vTaskDelay(380);

    //--------���쳤��ǰ--------//
    x = 24*sin(70*PI/180);
    y = 24*cos(70*PI/180);
    CartesianToTheta(1.0);
    SetCoupledPosition(LegId);
    vTaskDelay(300);

    //--------������´���--------//
    x = 29*sin(1*PI/180); //1��
    y = 29*cos(1*PI/180);
    CartesianToTheta(1.0);
    SetCoupledPosition(LegId);
    vTaskDelay(200);

    //------�ĸ��Ȼָ���ʼλ��-----//
    x = 0;
    y = 28;
    CartesianToTheta(1.0);
    CommandAllLegs_v();
    vTaskDelay(200);

}

void OpenMvInspect(int color)
{

    while(1)
    {
        vTaskDelay(100);
        if(openmvinfo.ActVal[2]==color)
        {
            vTaskDelay(100);
            if(openmvinfo.ActVal[2]==color)
            {
                vTaskDelay(100);
                if(openmvinfo.ActVal[2]==color)
                {
                    vTaskDelay(100);
                    if(openmvinfo.ActVal[2]==color)
                    {
                        vTaskDelay(100);
                        if(openmvinfo.ActVal[2]==color)
                            break;
                    }
                }

            }
        }
    }

}

float OpenMv_Line_Dec(void)
{

//	float dev_calc_ang;
//
//	dev_calc_ang = pid_calc(&pid_openmv_dev,openmvinfo.ActVal[0],);
//
//
//	return
}

void KeyToken_Test(void)
{
    while(1)
    {

        if(keyToken==0) {
            IndicateLED_On;
        }
        else if(keyToken==1) {
            IndicateLED_Off;
        }
        vTaskDelay(200);
    }

}
void Climbing_Comb(void)
{

    IndicateLED_On;
    //...����------------
    while(keyInf1!=0) //��һ�����-------�ȴ���翪��--�������³���-----
        vTaskDelay(500);

    IndicateLED_Off;

    LinearCorrection=Deny;
    LinearCorrection=climbing_correction;

    yaw_set=imuinfo.ActVal[0];//�趨��ǰ�Ƕ�Ϊǰ������
    now_time=times;
    climbing_offset_flag=YES;
    _climbing_offset_angle=15;
    state= CLIMBING;

    vTaskDelay(5000);  //��ʱ3000

    OpenMvInspect(openmv_Red);  //�ȴ���⵽��ɫ ɫ�� ����ͷ�Ѿ���⵽���������¶�

    IndicateLED_On;

    vTaskDelay(3300);  //��ʱ3000

    IndicateLED_Off;

    state_detached_params[CLIMBING].detached_params_0.stance_height+=2;
    state_detached_params[CLIMBING].detached_params_1.stance_height+=2;
    vTaskDelay(300);

    state_detached_params[CLIMBING].detached_params_0.stance_height+=2;
    state_detached_params[CLIMBING].detached_params_1.stance_height+=2;
    vTaskDelay(300);

    state_detached_params[CLIMBING].detached_params_0.stance_height+=2;
    state_detached_params[CLIMBING].detached_params_1.stance_height+=2;
    vTaskDelay(300);

    state_detached_params[CLIMBING].detached_params_0.stance_height+=2;
    state_detached_params[CLIMBING].detached_params_1.stance_height+=2;
    vTaskDelay(300);

    state_detached_params[CLIMBING].detached_params_0.stance_height+=2;
    state_detached_params[CLIMBING].detached_params_1.stance_height+=2;
    vTaskDelay(300);

    state= REALSE;
    vTaskDelay(300);

    x=0;
    y = 29;
    CartesianToTheta(1.0);
    LegGain gains = {17.5, 0.00, 30.0, 0.00};
    CommandAllLegs(gains);

    vTaskDelay(300);

    Servo3_OPEN;  //��λ�����

    Servo1_PEAK; //���һ��λ
    vTaskDelay(200);

    Servo2_PEAK_POS;//�������λ

    state= REALSE;

    vTaskDelay(400);

}

float testtheta,testtheta1,testtheta2;

float legs_point_offset = 0,front_leg_angle = 100,back_leg_angle =102,back_open_angle_offset = 0;

//float legs_point_offset = 3,front_leg_angle = 101,back_leg_angle =113,back_open_angle_offset = 0;


void StartPosToMiddlePos (void)
{
    state = REALSE;

    //----------------------------��СС���ſ���--------------------------
    for (int i = 0; i < 8; i++) {
        pid_reset_kpkd(&pid_pos[i], 22.0, 0);
        pid_reset_kpkd(&pid_spd[i], 15.5f, 0);
    }
    testtheta1=25.0;
    testtheta2=25.0;
    temp_pid.ref_agle[0]+=testtheta1*ReductionAndAngleRatio;
    temp_pid.ref_agle[1]+=testtheta1*ReductionAndAngleRatio;
    temp_pid.ref_agle[2]-=testtheta1*ReductionAndAngleRatio;
    temp_pid.ref_agle[3]-=testtheta1*ReductionAndAngleRatio;

    temp_pid.ref_agle[4]+=(testtheta2+0)*ReductionAndAngleRatio;
    temp_pid.ref_agle[5]+=(testtheta2+0)*ReductionAndAngleRatio;
    temp_pid.ref_agle[6]-=testtheta2*ReductionAndAngleRatio;
    temp_pid.ref_agle[7]-=testtheta2*ReductionAndAngleRatio;

    //���ȶ��������
//    temp_pid.ref_agle[4]-=(testtheta2+0)*ReductionAndAngleRatio;
//    temp_pid.ref_agle[5]-=(testtheta2+0)*ReductionAndAngleRatio;
//    temp_pid.ref_agle[6]+=testtheta2*ReductionAndAngleRatio;
//    temp_pid.ref_agle[7]+=testtheta2*ReductionAndAngleRatio;
//

    IsMotoReadyOrNot= IsReady;		//����������

    vTaskDelay(800);

//----------------------------����dao����--------------------------
    for (int i = 0; i < 8; i++) {
        pid_reset_kpkd(&pid_pos[i], 22.0, 0);
        pid_reset_kpkd(&pid_spd[i], 15.5f, 0);
    }
    testtheta1=front_leg_angle;
    testtheta2=back_leg_angle;
    temp_pid.ref_agle[0]-=testtheta1*ReductionAndAngleRatio;
    temp_pid.ref_agle[1]+=testtheta1*ReductionAndAngleRatio;
    temp_pid.ref_agle[2]-=testtheta1*ReductionAndAngleRatio;
    temp_pid.ref_agle[3]+=testtheta1*ReductionAndAngleRatio;

    temp_pid.ref_agle[4]-=testtheta2*ReductionAndAngleRatio;
    temp_pid.ref_agle[5]+=testtheta2*ReductionAndAngleRatio;
    temp_pid.ref_agle[6]-=(testtheta2-legs_point_offset)*ReductionAndAngleRatio;
    temp_pid.ref_agle[7]+=(testtheta2-legs_point_offset)*ReductionAndAngleRatio;

//���ȶ��������
//    temp_pid.ref_agle[4]-=testtheta2*ReductionAndAngleRatio;
//    temp_pid.ref_agle[5]+=testtheta2*ReductionAndAngleRatio;
//    temp_pid.ref_agle[6]-=(testtheta2-0)*ReductionAndAngleRatio;
//    temp_pid.ref_agle[7]+=(testtheta2-0)*ReductionAndAngleRatio;
    IsMotoReadyOrNot= IsReady;		//����������

    vTaskDelay(800);

//----------------------------����--------------------------
    for (int i = 0; i < 8; i++) {
        pid_reset_kpkd(&pid_pos[i], 25.0, 0);
        pid_reset_kpkd(&pid_spd[i], 15.5f, 0);
    }
    testtheta=60.0;
    temp_pid.ref_agle[0]+=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[1]+=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[2]-=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[3]-=testtheta*ReductionAndAngleRatio;

    temp_pid.ref_agle[4]+=(testtheta+back_open_angle_offset)*ReductionAndAngleRatio;
    temp_pid.ref_agle[5]+=(testtheta+back_open_angle_offset)*ReductionAndAngleRatio;
    temp_pid.ref_agle[6]-=(testtheta+back_open_angle_offset)*ReductionAndAngleRatio;
    temp_pid.ref_agle[7]-=(testtheta+back_open_angle_offset)*ReductionAndAngleRatio;

//���ȶ��������
//    temp_pid.ref_agle[4]-=testtheta*ReductionAndAngleRatio;
//    temp_pid.ref_agle[5]-=testtheta*ReductionAndAngleRatio;
//    temp_pid.ref_agle[6]+=testtheta*ReductionAndAngleRatio;
//    temp_pid.ref_agle[7]+=testtheta*ReductionAndAngleRatio;
    IsMotoReadyOrNot= IsReady;		//����������


    vTaskDelay(1000);

    // printf(" moto_chassis = %d   moto_chassis[1]  =%d    ref_agle %f\n",moto_chassis[0].total_angle,moto_chassis[7].total_angle,ref_agle[0]);
//----------------------------���õ�ǰ��Ϊ���--------------------------
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

    x=0;
    y = 17.3205081;

    CartesianToTheta(1.0);
    CommandAllLegs_v();

    state = REALSE;
//----------------------------�Ȳ�£--------------------------
    for (int i = 0; i < 8; i++) {
        pid_reset_kpkd(&pid_pos[i], 24.0, 0);
        pid_reset_kpkd(&pid_spd[i], 15.5f, 0);
    }
    testtheta=-85.0;
    temp_pid.ref_agle[0]+=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[1]+=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[2]-=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[3]-=testtheta*ReductionAndAngleRatio;

    temp_pid.ref_agle[4]+=(testtheta+back_open_angle_offset)*ReductionAndAngleRatio;
    temp_pid.ref_agle[5]+=(testtheta+back_open_angle_offset)*ReductionAndAngleRatio;
    temp_pid.ref_agle[6]-=(testtheta+back_open_angle_offset)*ReductionAndAngleRatio;
    temp_pid.ref_agle[7]-=(testtheta+back_open_angle_offset)*ReductionAndAngleRatio;

//���ȶ��������
//    temp_pid.ref_agle[4]-=(testtheta+0)*ReductionAndAngleRatio;
//    temp_pid.ref_agle[5]-=(testtheta+0)*ReductionAndAngleRatio;
//    temp_pid.ref_agle[6]+=testtheta*ReductionAndAngleRatio;
//    temp_pid.ref_agle[7]+=testtheta*ReductionAndAngleRatio;
    IsMotoReadyOrNot= IsReady;		//����������


    vTaskDelay(600);
//----------------------------��ȥ--------------------------
    for (int i = 0; i < 8; i++) {
        pid_reset_kpkd(&pid_pos[i], 22.0, 0);
        pid_reset_kpkd(&pid_spd[i], 15.5f, 0);
    }
    testtheta1=-front_leg_angle;
    testtheta2=-back_leg_angle;

    temp_pid.ref_agle[0]-=testtheta1*ReductionAndAngleRatio;
    temp_pid.ref_agle[1]+=testtheta1*ReductionAndAngleRatio;
    temp_pid.ref_agle[2]-=testtheta1*ReductionAndAngleRatio;
    temp_pid.ref_agle[3]+=testtheta1*ReductionAndAngleRatio;

    temp_pid.ref_agle[4]-=testtheta2*ReductionAndAngleRatio;
    temp_pid.ref_agle[5]+=testtheta2*ReductionAndAngleRatio;
    temp_pid.ref_agle[6]-=(testtheta2-legs_point_offset)*ReductionAndAngleRatio;
    temp_pid.ref_agle[7]+=(testtheta2-legs_point_offset)*ReductionAndAngleRatio;

//���ȶ��������
//    temp_pid.ref_agle[4]-=testtheta2*ReductionAndAngleRatio;
//    temp_pid.ref_agle[5]+=testtheta2*ReductionAndAngleRatio;
//    temp_pid.ref_agle[6]-=(testtheta2-0)*ReductionAndAngleRatio;
//    temp_pid.ref_agle[7]+=(testtheta2-0)*ReductionAndAngleRatio;
    IsMotoReadyOrNot= IsReady;		//����������

    vTaskDelay(400);

//----------------------------����0λ--------------------------
    memset(&moto_chassis,0,sizeof(moto_measure_t)*8);
    for(int i=0; i<8; i++)
        temp_pid.ref_agle[i]=ref_agle[i]=0;

    vTaskDelay(100);

    state = REALSE;

    printf("END Done.");

}




void TrajectoryJump(float t, float launchTime, float stanceHeight, float downAMP) {
    //Need to check if n works
    float n = t/launchTime;
    x = 0;
    y = downAMP*n + stanceHeight;
    //y = downAMP*sin(PI/4 + PI/4*n) + stanceHeight;
}

float start_time_ = 0.0f;

void StartJump(float start_time_s) {
    start_time_ = start_time_s;
    state = JUMP;
}

void ExecuteJump() {
    // min radius = 0.8
    // max radius = 0.25

    const float prep_time = 0.8f; // ׼��ʱ�� [s]
    const float launch_time = 0.2f ; // ������ǰ�ĳ���ʱ�� [s]
    const float fall_time = 0.8f; //�����Ⱥ����ʱ��ָ�������Ϊ [s]

    const float stance_height = 22.0f; // ��Ծ֮ǰ�ȵĸ߶� [cm]
    const float jump_extension = 28.9f; // ������ȳ��� [cm]
    const float fall_extension = 20.0f; // ����ʱ�������ȳ� [cm]


    float t = times/1000.0f - start_time_/1000.0f; // ��Ծ��ʼ���ʱ��

    if (t < prep_time) {
        x = 0;
        y = stance_height;
        CartesianToTheta(1.0);
        // ʹ�øն�С������������
        LegGain gains = {15.5, 0.00, 25.0, 0.00};
        CommandAllLegs(gains);

    } else if (t >= prep_time && t < prep_time + launch_time) {

        x = -jump_extension*sin(20*PI/180);
        y = jump_extension*cos(20*PI/180);
//        x = 0;
//        y = jump_extension;
        CartesianToTheta(1.0);

        // ʹ�ø߸նȺ͵�����ִ����ת
        LegGain gains = {25.5, 0.00, 49.0, 0.00};
        CommandAllLegs(gains);

    } else if (t >= prep_time + launch_time && t < prep_time + launch_time + fall_time) {
        x = 0;
        y = fall_extension;
        CartesianToTheta(1.0);

        //ʹ�õ͸նȺʹ����������������½�
        LegGain gains = {5.5, 0.00, 21.0, 0.00};
        CommandAllLegs(gains);

    } else {
        state = STOP;
        printf("Jump Complete.");
    }

}
