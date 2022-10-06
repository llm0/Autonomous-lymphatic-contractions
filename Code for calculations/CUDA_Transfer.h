#include "Common.h"
#include "math.h"
using namespace std;
extern "C" int CudaCollide();
extern "C" int CudaReactionNO(Valve_str * valve);
extern "C" int CudaCalculate();
extern "C" int CudaStream();
//extern "C" int CudaStress(Segment memup[SEGNUM], Segment memdown[SEGNUM]);
extern "C" int CudaDiffusionNO();
extern "C" int CudaFindLink(int * linknum, Segment* memup, Segment* memdown, parameter* p);
extern "C" int InitCudaParameter(int gpuid,parameter* InitData, Lattice la[WIDTH_E][HEIGHT], Segment memup[SEGNUM_E],
	Segment memdown[SEGNUM_E], double pNO[WIDTH_E][HEIGHT], Link* plink,int iMyID);
extern "C" double GetValue();//����̽���Կ��ϵ�����
extern "C" void freeMem();