#include "main.h"
// Noi khai bao hang so
#define     LED     PORTD
#define     ON      1
#define     OFF     0

#define     INIT_SYSTEM        255
#define     ENTER_PASSWORD     1
#define     CHECK_PASSWORD     2
#define     UNLOCK_DOOR        3
#define     WRONG_PASSWORD     4

#define PORTD_OUT   PORTD
#define TRISD_OUT   TRISD

#define PORTA_OUT   PORTA
#define TRISA_OUT   TRISA

#define PORTB_OUT   PORTB
#define TRISB_OUT   TRISB

#define PORTE_OUT   PORTE
#define TRISE_OUT   TRISE

void init_output(void);
// Noi khai bao bien toan cuc
unsigned char arrayMapOfOutput [8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
unsigned char statusOutput[8] = {0,0,0,0,0,0,0,0};
unsigned char decode_led[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99,0x92,0x82,0xf8,0x80,0x90};
// Khai bao cac ham co ban IO
void init_system(void);
void delay_ms(int value);
void OpenOutput(int index);
void CloseOutput(int index);
void TestOutput(void);
void ReverseOutput(int index);
unsigned char isButtonMotorOn();
unsigned char isButtonMotorOff();
void MotorOn();
void MotorOff();
void BaiTap_Motor();
void Test_KeyMatrix();
//Chuong trinh Password Door
unsigned char arrayMapOfNumber [16] = {1,2,3,'A',4,5,6,'B',
                                       7,8,9,'C','*',0,'E','D'};
unsigned char arrayMapOfPassword [5][4]= {
  {1,2,3,4},
  {2,7,8,9},
  {3,3,3,3},
  {4,8,6,8},
  {5,'A','B','C'},
};
unsigned char arrayPassword[4];
unsigned char statusPassword = INIT_SYSTEM;
unsigned char indexOfNumber = 0;
unsigned char timeDelay = 0;

void App_PasswordDoor();
unsigned char CheckPassword();
unsigned char isButtonNumber();
unsigned char numberValue;
unsigned char isButtonEnter();
void UnlockDoor();
void LockDoor();
// Den giao thong
void Phase1_GreenOn();
void Phase1_GreenOff();
void Phase1_YellowOn();
void Phase1_YellowOff();
void Phase1_RedOn();
void Phase1_RedOff();

void Phase2_GreenOn();
void Phase2_GreenOff();
void Phase2_YellowOn();
void Phase2_YellowOff();
void Phase2_RedOn();
void Phase2_RedOff();

#define     INIT_SYSTEM         255
#define     SET_TIME            254
#define     STAND_BY            253
#define     MANUAL_CONTROL      252
#define     PHASE1_GREEN        0
#define     PHASE1_YELLOW       1
#define     PHASE2_GREEN        2
#define     PHASE2_YELLOW       3
#define     WAIT                4
#define     SET_PHASE1_GREEN    5
#define     SET_PHASE1_YELLOW   6
#define     SET_PHASE2_GREEN    7
#define     SET_PHASE2_YELLOW   8
#define     START               9

#define     P1G_P2R             10
#define     P1Y_P2R             11
#define     P1R_P2G             12
#define     P1R_P2Y             13


unsigned char statusOfLight = INIT_SYSTEM;
unsigned char timeOfGreenPhase1 = 0;
unsigned char timeOfYellowPhase1 = 0;
unsigned char timeOfRedPhase1 = 0;
unsigned char timeOfGreenPhase2 = 0;
unsigned char timeOfYellowPhase2 = 0;
unsigned char timeOfRedPhase2 = 0;

unsigned char _timeOfGreenPhase1 = 0;
unsigned char _timeOfYellowPhase1 = 0;
unsigned char _timeOfRedPhase1 = 0;
unsigned char _timeOfGreenPhase2 = 0;
unsigned char _timeOfYellowPhase2 = 0;
unsigned char _timeOfRedPhase2 = 0;

unsigned char timeOfLight = 0;
unsigned char cntOfLight = 0;
unsigned char numberValue = 0;
unsigned char arrayInput[2];

unsigned char timeOfPhase1 = 0;
unsigned char timeOfPhase2 = 0;

unsigned char standByStatus = 0;
unsigned char previousStatus = 0;

void AppTrafficLight();
void UartDataReceiveProcess();
void UartDataReceiveProcess_ElectronicDeviceControl();
unsigned char code(int number);
void scan1(unsigned char number);
void scan2(unsigned char number);
void showLedPhase1();
void showLedPhase2();
void turnOffLedPhase1();
void turnOffLedPhase2();

////////////////////////////////////////////////////////////////////
//Hien thuc cac chuong trinh con, ham, module, function duoi cho nay
////////////////////////////////////////////////////////////////////

int cnt=0;
unsigned char flag_1s=0;

void main(void)
{
	unsigned int k = 0;
	init_system();
    delay_ms(1000);
	while (1)
	{
        while (!flag_timer3);
        flag_timer3 = 0;
        scan_key_matrix(); // 8 button
        AppTrafficLight();
        showLedPhase1();
        showLedPhase2();
        DisplayLcdScreenOld(); //Output process 14ms vs 10ms with no timer
            
	}
}
// Hien thuc cac module co ban cua chuong trinh
void delay_ms(int value)
{
	int i,j;
	for(i=0;i<value;i++)
		for(j=0;j<238;j++);
}

void init_output(void)
{
   TRISD_OUT = 0x00;
   PORTD_OUT = 0x00;
   TRISA_OUT = 0x00;
   PORTA_OUT = 0x00;
   TRISE_OUT = 0x00;
   PORTE_OUT = 0x00;
}

void init_system(void)
{
    init_output();
    lcd_init();
    init_key_matrix();
    init_interrupt();
    lcd_clear();
    LcdClearS();
    delay_ms(500);
    init_timer0(4695);      //dinh thoi 1ms sai so 1%
    init_timer3(46950);     //dinh thoi 10ms
    SetTimer0_ms(2);
    SetTimer3_ms(50);       //Chu ky thuc hien viec xu ly input,proccess,output
    //PORTAbits.RA0 = 1;
}

void OpenOutputA(int index)
{
	if (index >= 0 && index <= 7)
	{
		PORTA_OUT = PORTA_OUT | arrayMapOfOutput[index];
	}
}

void OpenOutputE(int index)
{
	if (index >= 0 && index <= 3)
	{
		PORTE_OUT = PORTE_OUT | arrayMapOfOutput[index];
	}
}

void OpenOutput(int index)
{
	if (index >= 0 && index <= 7)
	{
		PORTD_OUT = PORTD_OUT | arrayMapOfOutput[index];
	}
}

void CloseOutput(int index)
{
	if (index >= 0 && index <= 7)
	{
		PORTD_OUT = PORTD_OUT & ~arrayMapOfOutput[index];
	}
}

void CloseOutputA(int index) {
    if (index >= 0 && index <= 7) {
        PORTA_OUT = PORTA_OUT & ~arrayMapOfOutput[index];
    }
}

void CloseOutputE(int index) {
    if (index >= 0 && index <= 3) {
        PORTE_OUT = PORTE_OUT & ~arrayMapOfOutput[index];
    }
}

void ReverseOutput(int index)
{
    if (statusOutput[index]  == ON)
    {
        CloseOutput(index);
        statusOutput[index] = OFF;
    }
    else
    {
        OpenOutput(index);
        statusOutput[index] = ON;
    }
}

void TestOutput(void)
{
	int k;
	for (k=0;k<14 ;k++ )
	{
		OpenOutput(k);
		delay_ms(500);
		CloseOutput(k);
		delay_ms(500);
	}
}

void Phase1_GreenOn()
{
    OpenOutputE(0);
}
void Phase1_GreenOff()
{
    CloseOutputE(0);
}

void Phase1_YellowOn()
{
    OpenOutputA(2);
}
void Phase1_YellowOff()
{
    CloseOutputA(2);
}

void Phase1_RedOn()
{
    OpenOutput(7);
}
void Phase1_RedOff()
{
    CloseOutput(7);
}

void Phase2_GreenOn()
{
    OpenOutputA(1);
}
void Phase2_GreenOff()
{
    CloseOutputA(1);
}

void Phase2_YellowOn()
{
    OpenOutputA(3);
}
void Phase2_YellowOff()
{
    CloseOutputA(3);
}

void Phase2_RedOn()
{
    OpenOutput(6);
}
void Phase2_RedOff()
{
    CloseOutput(6);
}

void Init_flag() {
    LcdPrintStringS(0,0,"SET TIME");
    LcdPrintStringS(0,13,"(*)");
    LcdPrintStringS(1,0,"START");
    LcdPrintStringS(1,13,"(#)");
}

void set_time() {
    
}

int isButtonSetTime() {
    if (key_code[12] == 1) {
        return 1;
    }
    else return 0;
}

int isManualControl() {
    return (key_code[7]==1) ? 1:0;
}

unsigned char isButtonNumber() {
    unsigned char i;
    for (i=0; i<16; i++) {
        if (key_code[i]==1) {
            numberValue = arrayMapOfNumber[i];
            return 1;
        }
    }
    return 0;
}

void clear_screen(int _case) {
    unsigned char i;
    if (_case==2) {
        LcdPrintStringS(0,0,"                ");
        LcdPrintStringS(1,0,"                ");
    }
    if (_case==0) {
        LcdPrintStringS(0,0,"                ");
    }
    if (_case==1) {
        LcdPrintStringS(1,0,"                ");
    }
}

int isComfirmButton() {
    return (key_code[14]==1)?1:0;
}

int isStandByButton() {
    return (key_code[3]==1)?1:0;
}

unsigned char isSetTime=0;
unsigned char manual_control_status = 0;

void AppTrafficLight()
{
    //cntOfLight = (cntOfLight + 1)%20;
    //if (cntOfLight == 0)
    //    timeOfLight --;
    cnt++; flag_1s=0;
    if (cnt==17) {
        cnt=0;
        flag_1s=1;
    }
    switch (statusOfLight)
    {
        case INIT_SYSTEM:
            clear_screen(2);
            turnOffLedPhase1();
            turnOffLedPhase2();
            Init_flag();
            if (isButtonSetTime()) {
                statusOfLight = SET_TIME;
            }
            if (isComfirmButton()) {
                clear_screen(2);
              
                statusOfLight = START;
            }
            break;
        case SET_TIME:
            statusOfLight = SET_PHASE1_GREEN;
            indexOfNumber=0;
            clear_screen(2);
            break;
        case SET_PHASE1_GREEN:
            LcdPrintStringS(0,0,"PHASE1_GREEN:");
            timeDelay++;
            if (indexOfNumber==0) {
                clear_screen(1);
                LcdPrintStringS(1,0,"(10-99)");
                LcdPrintStringS(1,14,"__");
            }
            if (indexOfNumber==1) {
                clear_screen(1);
                LcdPrintStringS(1,0,"(10-99)");
                LcdPrintNumS(1,14,arrayInput[0]);
                LcdPrintStringS(1,15,"_");
            }
            if (isButtonNumber()) {
                int temp = indexOfNumber+1;
                if (temp<3) {
                    LcdPrintNumS(1,indexOfNumber+14,numberValue);
                arrayInput[indexOfNumber]=numberValue;
                }
                indexOfNumber++;
            }
            //if (timeDelay > 200) statusOfLight = INIT_SYSTEM;
            if (isComfirmButton()) {
                if (indexOfNumber==1) timeOfGreenPhase1 = 10;
                else timeOfGreenPhase1=arrayInput[0]*10 + arrayInput[1];
                _timeOfGreenPhase1 = timeOfGreenPhase1;
                indexOfNumber=0;
                statusOfLight = SET_PHASE1_YELLOW;
                clear_screen(2);
            }
            break;
        case SET_PHASE1_YELLOW:
            LcdPrintStringS(0,0,"PHASE1_YELLOW:");
            timeDelay++;
            if (indexOfNumber==0) {
                clear_screen(1);
                LcdPrintStringS(1,0,"(1-9)");
                LcdPrintCharS(1,15,'_');
            }
            if (isButtonNumber()) {
                int temp = indexOfNumber+1;
                if (temp<2) {
                    LcdPrintNumS(1,indexOfNumber+15,numberValue);
                    arrayInput[indexOfNumber]=numberValue;
                } 
                indexOfNumber++;
                timeDelay=0;
            }
            //if (timeDelay > 200) statusOfLight = INIT_SYSTEM;
            if (isComfirmButton()) {
                indexOfNumber=0;
                timeOfYellowPhase1 = arrayInput[0];
                _timeOfYellowPhase1 = timeOfYellowPhase1;
                timeOfRedPhase2 = timeOfGreenPhase1+timeOfYellowPhase1;
                _timeOfRedPhase2 = timeOfRedPhase2;
                clear_screen(2);
                statusOfLight = SET_PHASE2_GREEN;
            }
            break;
        case SET_PHASE2_GREEN:
            LcdPrintStringS(0,0,"PHASE2_GREEN:");
            timeDelay++;
            if (indexOfNumber==0) {
                clear_screen(1);
                LcdPrintStringS(1,0,"(10-99)");
                LcdPrintStringS(1,14,"__");
            }
            if (indexOfNumber==1) {
                clear_screen(1);
                LcdPrintStringS(1,0,"(10-99)");
                LcdPrintNumS(1,14,arrayInput[0]);
                LcdPrintStringS(1,15,"_");
            }
            if (isButtonNumber()) {
                int temp = indexOfNumber+1;
                if (temp<3) {
                    LcdPrintNumS(1,indexOfNumber+14,numberValue);
                    arrayInput[indexOfNumber]=numberValue;
                }
                
                indexOfNumber++;
            }
            //if (timeDelay > 200) statusOfLight = INIT_SYSTEM;
            if (isComfirmButton()) {
                indexOfNumber=0;
                timeOfGreenPhase2 = arrayInput[0]*10 + arrayInput[1];
                _timeOfGreenPhase2 = timeOfGreenPhase2;
                statusOfLight = SET_PHASE2_YELLOW;
                clear_screen(2);
            }
            break;
        case SET_PHASE2_YELLOW:
            LcdPrintStringS(0,0,"PHASE2_YELLOW:");
            timeDelay++;
            if (indexOfNumber==0) {
                clear_screen(1);
                LcdPrintStringS(1,0,"(1-9)");
                LcdPrintCharS(1,15,'_');
            }
            if (isButtonNumber()) {
                int temp = indexOfNumber+1;
                if (temp<2) {
                    LcdPrintNumS(1,indexOfNumber+15,numberValue);
                arrayInput[indexOfNumber]=numberValue;
                }
                indexOfNumber++;
                timeDelay=0;
            }
            //if (timeDelay > 200) statusOfLight = INIT_SYSTEM;
            if (isComfirmButton()) {
                indexOfNumber=0;
                timeOfYellowPhase2 = arrayInput[0];
                _timeOfYellowPhase2 = timeOfYellowPhase2;
                timeOfRedPhase1 = timeOfGreenPhase2+timeOfYellowPhase2;
                _timeOfRedPhase1 = timeOfRedPhase1;
                clear_screen(2);
                isSetTime=1;
                statusOfLight = INIT_SYSTEM;
            }
            break;   
        case START:
            if (isSetTime==0) {
                clear_screen(2);
                LcdPrintStringS(0,0,"TIME NOT ALREADY");
                LcdPrintStringS(1,0,"PRESS #");
                if (isComfirmButton()) {
                    statusOfLight = INIT_SYSTEM;
                }
            }
            else {
                statusOfLight = P1G_P2R;
                clear_screen(2);
            }
            break;
        case P1G_P2R:
            clear_screen(0);
            Phase1_GreenOn();
            Phase2_RedOn();
            timeOfPhase1 = timeOfGreenPhase1;
            timeOfPhase2 = timeOfRedPhase2;
            showLedPhase1();
            showLedPhase2();
            LcdPrintStringS(0,0,"P1: GREEN");
            if (timeOfGreenPhase1>=10) {
                LcdPrintNumS(0,14,timeOfGreenPhase1);
            }
            else {
                LcdPrintNumS(0,15,timeOfGreenPhase1);
            }
            clear_screen(1);
            LcdPrintStringS(1,0,"P2: RED");
            if (timeOfRedPhase2>=10) {
                LcdPrintNumS(1,14,timeOfRedPhase2);
            }
            else {
                LcdPrintNumS(1,15,timeOfRedPhase2);
            }
            if (flag_1s) {
                timeOfGreenPhase1--;
                timeOfRedPhase2--;
            }
            if (isStandByButton()) {
                Phase1_GreenOff();
                Phase2_RedOff();
                previousStatus = P1G_P2R;
                statusOfLight = STAND_BY;
            }
            if (isButtonSetTime()) {
                Phase1_GreenOff();
                Phase2_RedOff();
                statusOfLight = SET_TIME;
            }
            if (isManualControl()) {
                previousStatus = P1G_P2R;
                Phase1_GreenOff();
                Phase2_RedOff();
                statusOfLight = MANUAL_CONTROL;
            }
            if (timeOfGreenPhase1==0) {
                clear_screen(2);
                Phase1_GreenOff();
                statusOfLight = P1Y_P2R;
            }
            break;
        case P1Y_P2R:
            clear_screen(0);
            Phase1_YellowOn();
            Phase2_RedOn();
            timeOfPhase1 = timeOfYellowPhase1;
            timeOfPhase2 = timeOfRedPhase2;
            showLedPhase1();
            showLedPhase2();
            LcdPrintStringS(0,0,"P1: YELLOW");
            if (timeOfYellowPhase1>=10) {
                LcdPrintNumS(0,14,timeOfYellowPhase1);
            }
            else {
                LcdPrintNumS(0,15,timeOfYellowPhase1);
            }
            clear_screen(1);
            LcdPrintStringS(1,0,"P2: RED");
            if (timeOfRedPhase2>=10) {
                LcdPrintNumS(1,14,timeOfRedPhase2);
            }
            else {
                LcdPrintNumS(1,15,timeOfRedPhase2);
            }
            
            if (flag_1s) {
                timeOfYellowPhase1--;
                timeOfRedPhase2--;
            }
              if (isStandByButton()) {
                Phase1_YellowOff();
                Phase2_RedOff();
                previousStatus = P1Y_P2R;
                statusOfLight = STAND_BY;
            }
            if (isButtonSetTime()) {
                Phase1_YellowOff();
                Phase2_RedOff();
                statusOfLight = SET_TIME;
            }
            if (isManualControl()) {
                previousStatus = P1Y_P2R;
                Phase1_YellowOff();
                Phase2_RedOff();
                statusOfLight = MANUAL_CONTROL;
            }
            if (timeOfYellowPhase1 == 0 || timeOfRedPhase2==0) {
                clear_screen(2);
                timeOfGreenPhase1 = _timeOfGreenPhase1;
                timeOfYellowPhase1 = _timeOfYellowPhase1;
                timeOfRedPhase2 = _timeOfRedPhase2;
                Phase1_YellowOff();
                Phase2_RedOff();
                statusOfLight = P1R_P2G;
            }
            break;
        case P1R_P2G:
            clear_screen(0);
            Phase1_RedOn();
            Phase2_GreenOn();
            timeOfPhase1 = timeOfRedPhase1;
            timeOfPhase2 = timeOfGreenPhase2;
            showLedPhase1();
            showLedPhase2();
            LcdPrintStringS(0,0,"P1: RED");
            if (timeOfRedPhase1>=10) {
                LcdPrintNumS(0,14,timeOfRedPhase1);
            }
            else {
                LcdPrintNumS(0,15,timeOfRedPhase1);
            }
            clear_screen(1);
            LcdPrintStringS(1,0,"P2: GREEN");
            if (timeOfGreenPhase2>=10) {
                LcdPrintNumS(1,14,timeOfGreenPhase2);
            }
            else {
                LcdPrintNumS(1,15,timeOfGreenPhase2);
            }
            if (flag_1s) {
                timeOfRedPhase1--;
                timeOfGreenPhase2--;
            }
              if (isStandByButton()) {
                Phase1_RedOff();
                Phase2_GreenOff();
                previousStatus = P1R_P2G;
                statusOfLight = STAND_BY;
            }
            if (isButtonSetTime()) {
                Phase1_RedOff();
                Phase2_GreenOff();
                statusOfLight = SET_TIME;
            }
            if (isManualControl()) {
                previousStatus = P1R_P2G;
                Phase1_RedOff();
                Phase2_GreenOff();
                statusOfLight = MANUAL_CONTROL;
            }
            if (timeOfGreenPhase2==0) {
                clear_screen(2);
                Phase2_GreenOff();
                statusOfLight = P1R_P2Y;
            }
            break;
        case P1R_P2Y:
            clear_screen(0);
            Phase1_RedOn();
            Phase2_YellowOn();
            timeOfPhase1 = timeOfRedPhase1;
            timeOfPhase2 = timeOfYellowPhase2;
            showLedPhase1();
            showLedPhase2();
            LcdPrintStringS(0,0,"P1: RED");
            if (timeOfRedPhase1>=10) {
                LcdPrintNumS(0,14,timeOfRedPhase1);
            }
            else {
                LcdPrintNumS(0,15,timeOfRedPhase1);
            }
            clear_screen(1);
            LcdPrintStringS(1,0,"P2: YELLOW");
            if (timeOfYellowPhase2>=10) {
                LcdPrintNumS(1,14,timeOfYellowPhase2);
            }
            else {
                LcdPrintNumS(1,15,timeOfYellowPhase2);
            }
            if (flag_1s) {
                timeOfRedPhase1--;
                timeOfYellowPhase2--;
            }
            if (isStandByButton()) {
                Phase1_RedOff();
                Phase2_YellowOff();
                previousStatus = P1G_P2R;
                statusOfLight = STAND_BY;
            }
            if (isButtonSetTime()) {
                Phase1_RedOff();
                Phase2_YellowOff();
                statusOfLight = SET_TIME;
            }
            if (isManualControl()) {
                previousStatus = P1R_P2Y;
                Phase1_RedOff();
                Phase2_YellowOff();
                statusOfLight = MANUAL_CONTROL;
            }
            if (timeOfYellowPhase2 == 0 || timeOfRedPhase1==0) {
                clear_screen(2);
                timeOfGreenPhase2 = _timeOfGreenPhase2;
                timeOfYellowPhase2 = _timeOfYellowPhase2;
                timeOfRedPhase1 = _timeOfRedPhase1;
                Phase1_RedOff();
                Phase2_YellowOff();
                statusOfLight = START;
            }
            break;
        case STAND_BY:
            clear_screen(2);
            LcdPrintStringS(0,0,"STAND BY");
            if (flag_1s) {
                if (standByStatus==0) {
                    Phase2_YellowOn();
                    Phase1_YellowOff();
                    standByStatus = 1;
                }
                else {
                    Phase2_YellowOff();
                    Phase1_YellowOn();
                    standByStatus = 0;
                }
            }
            if (isComfirmButton()) {
                Phase1_YellowOff();
                Phase2_YellowOff();
                statusOfLight = previousStatus;
            }
            break;
        case MANUAL_CONTROL:
            if (manual_control_status==0) {
                Phase1_GreenOn();
                Phase2_RedOn();
                if (isManualControl()) {
                    Phase1_GreenOff();
                    manual_control_status = 1;
                }
            }
            else if (manual_control_status == 1) {
                Phase1_YellowOn();
                if (flag_1s) {
                    Phase1_YellowOff();
                    Phase2_RedOff();
                    manual_control_status = 2;
                }
            }
            else if (manual_control_status == 2){
                Phase1_RedOn();
                Phase2_GreenOn();
                if (isManualControl()) {
                    Phase2_GreenOff();
                    manual_control_status = 3;
                }
            }
            else {
                Phase2_YellowOn();
                if (flag_1s) {
                    Phase2_YellowOff();
                    Phase1_RedOff();
                    manual_control_status = 0;
                }
            }
            if (isComfirmButton()) {
                Phase1_GreenOff();
                Phase1_RedOff();
                Phase2_GreenOff();
                Phase2_RedOff();
                statusOfLight = previousStatus;
            }
            break;
    }
}



void scan1(unsigned char number)
{
    int i, temp;
    for (i=0; i<8; i++) {
        temp = number;
        temp =temp & 0x80;
        if (temp == 0x80) OpenOutput(0);
        else CloseOutput(0);
        number *= 2;
        CloseOutput(1);
        OpenOutput(1);
    }
}

void scan2(unsigned char number)
{
    int i, temp;
    for (i=0; i<8; i++) {
        temp = number;
        temp =temp & 0x80;
        if (temp == 0x80) OpenOutput(3);
        else CloseOutput(3);
        number *= 2;
        CloseOutput(4);
        OpenOutput(4);
    }
}

unsigned char code(int number) {
    switch (number) {
        case 0:
            return 0xc0;
        case 1:
            return 0xf9;
        case 2:
            return 0xa4;
        case 3:
            return 0xb0;
        case 4:
            return 0x99;
        case 5:
            return 0xa92;
        case 6:
            return 0xa82;
        case 7:
            return 0xf8;
        case 8:
            return 0x80;
        case 9:
            return 0x90;
    }
    return 0xff;
}

void showLedPhase1() {
    int number = timeOfPhase1;
    int donvi, chuc;
    donvi = number%10;
    scan1(code(donvi));
    chuc = number/10;
    scan1(code(chuc));
    CloseOutput(2);
    OpenOutput(2);
    //delay_ms(1000);
}

void showLedPhase2() {
    int number = timeOfPhase2;
    int donvi, chuc;
    donvi = number%10;
    scan2(code(donvi));
    chuc = number/10;
    scan2(code(chuc));
    CloseOutput(5);
    OpenOutput(5);
    //delay_ms(1000);
}

void turnOffLedPhase1() {
    scan1(code(10));
    CloseOutput(2);
    OpenOutput(2);
}

void turnOffLedPhase2() {
    scan2(code(10));
    CloseOutput(5);
    OpenOutput(5);
}