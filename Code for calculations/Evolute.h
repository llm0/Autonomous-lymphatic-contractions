#include "Common.h"
void input();
void Init();
int ReadData(char* cc);
void InitAllStatic();
double Collide(int x,int y,int Order);
void Stream();
void Caculate();
void FileOpen();
void FileSave();
void DataBak(int iNormal = 1);
void DataShow();
void FindLink();
int Check(long lNum);
void Move();
void Remesh();
void OutLet();
void InitShear();
void Adjust();
void MoveCenter();
void test();
void BoundPress(int iy1,int iy2);
void ReactionNO();
void DeffusionNO();
void ShearStress();
int BetweenValve(int i,int j);
void SaveDensity(int k);
void TestDiscloseInit();
double Flux();
void SaveNO();
void SaveForAnimate();
void TranPara(parameter* aa);
//void checkt(int *i, fstream* ff, LARGE_INTEGER* ticks);
void ChangePress();
void TranferValve(Valve_str* valve);
void CommunicateLattice();
void TobeSame();
void CommunicateMem();
void SetDataSize(int* size, int* s5);
int FluidNode(int i, int j);
void BounCollide();
void PorousCollide();
int IsPorous(int i, int j);
void ActivePorous();
void InsertNonActivePorous();
void PartialBounceback(int iPosition);
void IncreasPress();
void Average(int ix1, int ix2);
//void CommunicateAll();
//double Curvebound(int x,int y,int Order,double data);
