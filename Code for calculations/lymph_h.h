//��Ҫ�������: �ܰ͹���:-1;��֯:-2;�߽�:0



#ifndef FLAG_H
	#define FLAG_H
#define BAKTIME 2000//2400
	#define BUFFER_LENGTH 500
	#define W0 0.4444444444444444444444444 //(4.0/9.0)
	#define W1 0.1111111111111111111111111 //(1.0/9.0)
	#define W2 0.02777777777777777777777778 //(1.0/36.0)
	#define SAVETIME 3000
	#define DELTA_W 3 //������ݽ�����
	#define DELTA_M 1 //Ĥ������
	#define WIDTH_E  471//�ܿ�
	#define WIDTH (WIDTH_E-DELTA_W*2)//�����
	#define SEGNUM_E 235//��Ĥ����(n-1)/2
	#define SEGNUM (SEGNUM_E-DELTA_M*2)//����Ĥ����
	#define HEIGHT 66//106//102//250������ż�������������ڸ����ǣ�������Ĥ�رղ���
	#define CENTER ((HEIGHT-1)/2.0)
	#define QUICK 1
	#define EPSILON 1e-12
	#define BOUDNUM 20000
	#define LINKNUM (WIDTH*16) //6000
	#define H1 20//int((HEIGHT-6)/5.0*2.0)//40
	#define H2 45//int((HEIGHT-6)/5.0*3.0+5)//65
	#define PX1 (16+3)//int(WIDTH / 30.0) + 4; //4:2int(WIDTH / 30.0) + 3;//��Ĥ��Ӵ��պ�Ϊ��
	#define  PX2 (72+3)//int(WIDTH / 5.0);
	#define LEFTVALVE 37
	#define RIGHTVALVE 153//152
	#define LIMITCA 200//0.98//0.98
	#define LIMITNO 200
	#define VALSEGNUM int(1.0*SEGNUM/(WIDTH-PX2)*(H2-H1)*2)
	#define BEGINMEM 1
	#define LIMIT 5
#endif