//重要标记声明: 淋巴管内:-1;组织:-2;边界:0



#ifndef FLAG_H
	#define DELTA_W 3 //宽度数据交换区
	#define DELTA_M 1 //膜交换区
	#define WIDTH_E  471//总宽
	#define SEGNUM_E 235//总膜段数
    #define SHORT_W
	#define BAKTIME 2400
	#define BUFFER_LENGTH 256
	#define W0 0.4444444444444444444444444 //(4.0/9.0)
	#define W1 0.1111111111111111111111111 //(1.0/9.0)
	#define W2 0.02777777777777777777777778 //(1.0/36.0)
	#define SAVETIME 3000
	#ifdef SHORT_W
		#define WIDTH (WIDTH_E-DELTA_W*2)//计算宽
		#define SEGNUM (SEGNUM_E-DELTA_M*2)//计算膜段数	
	#else 
		#define WIDTH 713//(421+72 )//453,473,493,513,553,593, 633, 673, 713
                                    //20, 40 ,60 ,80 ,120,160 ,200, 240, 280
		#define SEGNUM (201+186+40) //13, 26 ,40 ,53 ,80 ,106, 133,159,  186
	#endif
	#define HEIGHT 66//106//102//250必须是奇数，避免中线在格子是，两个瓣膜关闭不严
	#define QUICK 1
	#define EPSILON 1e-12
	#define BEGINSHEAR 0//400
	#define BOUDNUM 20000
	#define SIGMA 0.01
	#define CLJ 0.01
	#define LINKNUM 6000
	#define PX1 (16+3)//int(WIDTH / 30.0) + 4; //4:2int(WIDTH / 30.0) + 3;//与膜相接处刚好为洞
	#define  PX2 (72+3)//int(WIDTH / 5.0);
	#define H1 20//int((HEIGHT-6)/5.0*2.0)
	#define H2 45//int((HEIGHT-6)/5.0*3.0+5)
	//#define SEGNUM 201//201//601 //201
	//#define SEGNUM 241
	#define LIMITSEG 10
	#define TOTALTIME  1200  
	#define VALSEGNUM int(1.0*SEGNUM/(WIDTH-PX2)*(H2-H1)*2)
	#define SPACE 20 
#define LEFTVALVE 37
#define RIGHTVALVE 153//152
	#define BEGINROW 3
	#define THRESHOLD 0.1
#endif