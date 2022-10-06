//#include "stdafx.h"
#include "Line.h"
#include "Common.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Line::Line(Vector p1,Vector p2)
{
	m_p1=p1;
	m_p2=p2;
	if(fabs(m_p2.m_dx-m_p1.m_dx)>1e-16)
	{
		m_dk=(m_p2.m_dy-m_p1.m_dy)/(m_p2.m_dx-m_p1.m_dx);
		if(m_dk>100)
		{
			m_dk_re=(m_p2.m_dx-m_p1.m_dx)/(m_p2.m_dy-m_p1.m_dy);
		}
	}
	else if(fabs(m_p2.m_dy-m_p1.m_dy)>1e-16)
	{
		m_dk_re=(m_p2.m_dx-m_p1.m_dx)/(m_p2.m_dy-m_p1.m_dy);
	}
	else
	{
		m_dk=0;
		m_dk_re=0;
	}
}

void Line::init()
{
	if(fabs(m_p2.m_dx-m_p1.m_dx)>1e-16)
	{
		m_dk=(m_p2.m_dy-m_p1.m_dy)/(m_p2.m_dx-m_p1.m_dx);
		m_dk_re=1e17;
		if(m_dk>100)
		{
			m_dk_re=(m_p2.m_dx-m_p1.m_dx)/(m_p2.m_dy-m_p1.m_dy);
		}
	}
	else if(fabs(m_p2.m_dy-m_p1.m_dy)>1e-16)//接近竖直线
	{
		m_dk_re=(m_p2.m_dx-m_p1.m_dx)/(m_p2.m_dy-m_p1.m_dy);
		m_dk=2e17;
	}
	else
	{
		m_dk=0;
		m_dk_re=0;
	}
}

Line::~Line()
{

}

Line& Line::operator=(const Line &v)
{
	m_p1 = v.m_p1;
	m_p2 = v.m_p2;
	m_dk = v.m_dk;
	return *this;
}

double Line::distance(Vector pp,Vector* pCross)
{
	double rr;
	double dx;
	double dy;
	if(m_dk<=100)
	{
		dx=(m_dk*(pp.m_dy-m_p1.m_dy)+m_dk*m_dk*m_p1.m_dx+pp.m_dx)/(1+m_dk*m_dk);
		dy=(m_dk*(pp.m_dx-m_p1.m_dx)+m_dk*m_dk*pp.m_dy+m_p1.m_dy)/(1+m_dk*m_dk);
	}
	else
	{
		dx=(m_dk*(pp.m_dy-m_p1.m_dy)+m_dk*m_dk*m_p1.m_dx+pp.m_dx)/(1+m_dk_re*m_dk_re);
		dy=(m_dk*(pp.m_dx-m_p1.m_dx)+m_dk*m_dk*pp.m_dy+m_p1.m_dy)/(1+m_dk*m_dk);
	}
	rr=sqrt((dy-pp.m_dy)*(dy-pp.m_dy)+(dx-pp.m_dx)*(dx-pp.m_dx));
	pCross->m_dx=dx;
	pCross->m_dy=dy;
	return rr;   
}

void Line::FindLink(int *BouNum,Boundary *Boud )
{
	int i,j,iPx,iPy;
	int ix1,iy1,ix2,iy2;
	int m;
	int l;
	int k;
	int bOK;
	Vector pp;
	l=*BouNum;      //count Boundary Number in fluid
	if(m_iType==-1||m_iType==-2)
	{
		if(m_p1.m_dx<m_p2.m_dx)
		{
			ix1=int(m_p1.m_dx);
			ix2=int(m_p2.m_dx+0.5);
		}
		else
		{
			ix1=int(m_p2.m_dx);
			ix2=int(m_p1.m_dx+0.5);
		}
		if(m_p1.m_dy<m_p2.m_dy)
		{
			iy1=int(m_p1.m_dy)+1;//竖直线上下各去一格，避免和横线重复
			iy2=int(m_p2.m_dy+0.5)-1;//竖直线上下各去一格，避免和横线重复
		}
		else
		{
			iy1=int(m_p2.m_dy)+1;
			iy2=int(m_p1.m_dy+0.5)-1;
		}
	}
	else if(m_iType==1||m_iType==2)
	{
		if(m_p1.m_dx<m_p2.m_dx)
		{
			ix1=int(m_p1.m_dx);
			ix2=int(m_p2.m_dx+0.5)-1;//横线右边去一格，避免和膜重复
		}
		else
		{
			ix1=int(m_p2.m_dx);
			ix2=int(m_p1.m_dx+0.5)-1;
		}
		if(m_p1.m_dy<m_p2.m_dy)
		{
			iy1=int(m_p1.m_dy);
			iy2=int(m_p2.m_dy+0.5);
		}
		else
		{
			iy1=int(m_p2.m_dy)-1;
			iy2=int(m_p1.m_dy+0.5);
		}
	}
	for(i=ix1;i<=ix2;i++)
	{
      for(j=iy1;j<=iy2;j++)		
	  {  
		k=0;   //count Link Number 
		if(m_iType==1||m_iType==2)
		{
		//	m=int((i-m_p1.m_dx)/m_iHole);
		//	if(m%2==1)
			if(!IsHole(i,int(m_p1.m_dx)))
			{
				continue;
			}
		}
		else if(m_iType==-1||m_iType==-2)
		{
			if(!IsHole(j,int(m_p1.m_dy)))
			{
				continue;
			}
		}//if(i==13&&j==46) Show(lattice[iPx][iPy].m_iSol,"ss");
        for(m=1;m<9;m++)	
		{	
			bOK=1;
           if(lattice[i][j].m_iSol==-1||lattice[i][j].m_iSol==0)//i,j fluid node
		   {            
			 iPx=i+iPrjx[m];
             iPy=j+iPrjy[m];
			 if ((iMyID>0 && iMyID<iNumProcs - 1 || !(iMyID == 0 && iPx < iBegin_Row || iMyID == iNumProcs - 1&&iPx > iEnd_Row - 1))
				 && (lattice[iPx][iPy].m_iSol == -2 && lattice[iPx][iPy].m_ipr!=2 || (lattice[i][j].m_iSol == 0
				 &&lattice[iPx][iPy].m_iSol==-1)||(lattice[i][j].m_iSol==0
				 &&lattice[iPx][iPy].m_iSol==-3)))//back node,solid 
			  {	          
                Boud[l].m_link[k].m_iDir=iRev[m];//流体指向固体,别搞错这里					  					
				Boud[l].m_link[k].m_iSolx=i;
				Boud[l].m_link[k].m_iSoly=j;
				Boud[l].m_link[k].m_iFlux=iPx; //found fluid node
				Boud[l].m_link[k].m_iFluy=iPy; //-1~n
				switch(m)
				{
				case 1:
				case 2:
					if(m_iType<0)
					{	
						if(IsCross(Vector(i,j),Vector(iPx,iPy),&pp))
						{
							Boud[l].m_link[k].m_bf=pp;
						}
						else
						{
							bOK=0;	
						}
					}
					else if(m_iType>0)
					{
						if(lattice[i][j].m_iSol>=0)
						{
							Boud[l].m_link[k].m_bf=Vector(i,j);
						}
						else
						{
							Boud[l].m_link[k].m_bf=Vector(iPx,iPy);
						}
					}
					else
					{
						bOK=0;
					}
					break;
				case 3:
				case 4:
					if(m_iType>0)
					{	
						if(IsCross(Vector(i,j),Vector(iPx,iPy),&pp))
						{
							Boud[l].m_link[k].m_bf=pp;
						}
						else
						{
							bOK=0;	
						}
					}
					else if(m_iType<0)
					{
						if(lattice[i][j].m_iSol>=0)
						{
							Boud[l].m_link[k].m_bf=Vector(i,j);
						}
						else
						{
							Boud[l].m_link[k].m_bf=Vector(iPx,iPy);
						}
					}
					else
					{
						bOK=0;
					}
					break;
				case 5:
				case 6:
					if(fabs(m_dk_re)>1e16)
					{
						Boud[l].m_link[k].m_bf.m_dx=(j-m_p1.m_dy+m_dk*m_p1.m_dx+i)/(1+m_dk);
						Boud[l].m_link[k].m_bf.m_dy=(j*m_dk+m_p1.m_dy-m_dk*(m_p1.m_dx-i))/(1+m_dk);
					}
					else
					{
						Boud[l].m_link[k].m_bf=Vector(i,j);//不相交，任意值
					}
					break;
				case 7:
				case 8:
					if(fabs(m_dk_re)>1e16)
					{
						Boud[l].m_link[k].m_bf.m_dx=(j-m_p1.m_dy+m_dk*m_p1.m_dx-i)/(m_dk-1);
						Boud[l].m_link[k].m_bf.m_dy = (m_dk*(j - i + m_p1.m_dx) - m_p1.m_dy) / (m_dk - 1);
					}
					else
					{
						Boud[l].m_link[k].m_bf=Vector(i,j);//不相交，任意值
					}
					break;
				default:
					;
				}		 //for switch			
				if(bOK&&Inside(Boud[l].m_link[k].m_bf,Vector(i,j),Vector(iPx,iPy)))
				{	
					Boud[l].m_link[k].m_dDlf=
						sqrt((Boud[l].m_link[k].m_bf.m_dx-i)*(Boud[l].m_link[k].m_bf.m_dx-i)
						+(Boud[l].m_link[k].m_bf.m_dy-j)*(Boud[l].m_link[k].m_bf.m_dy-j))/
						sqrt((iPx-i)*(iPx-i)+(iPy-j)*(iPy-j));													    				
					k++;
					if(k>=8) printf("链数超过8了");
				}
			 }	//for if					    	    
		   }    //for if				
		} //for m 	
		if(k>0)
		{
			Boud[l]. m_iLinNum=k;			
		    l++;
			if (l >= BOUDNUM) printf("边界数超过了");
		}
	  }//for j
	}//for i
    *BouNum=l; 
}

int Line:: Inside(Vector pp,Vector p1,Vector p2)//交点要在两个点这间
{
	if((pp.m_dx<=p2.m_dx&&pp.m_dx>=p1.m_dx||pp.m_dx<=p1.m_dx&&pp.m_dx>=p2.m_dx)
		&&(pp.m_dy<=p2.m_dy&&pp.m_dy>=p1.m_dy||pp.m_dy<=p1.m_dy&&pp.m_dy>=p2.m_dy))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
/*void Line::Collide()
{
	int i,j,iDir;
	double dKappa;
	double dBFVelx,dBFVely;//Vector BFV;
	double dFluVelx,dFluVely;//Vector FV;
	double dDen;
	int iFlux,iFluy;//iVector pf;
	int isx,isy;//iVector ps;
//	dFlowOut=0;
//	dFlowIn=0;
//	iTotal=0;
	for(i=0;i<iBouNum;i++)
	{
		for(j=0;j<Boud[i].m_iLinNum;j++) //Link
		{
			iDir=Boud[i].m_link[j].m_iDir;
			iFlux=GetModx(Boud[i].m_link[j].m_iFlux);
			iFluy=GetMody(Boud[i].m_link[j].m_iFluy);
			isx=GetModx(Boud[i].m_link[j].m_iSolx);
			isy=GetModx(Boud[i].m_link[j].m_iSoly);
			dFluVelx=lattice[iFlux][iFluy].m_dvx;
			dFluVely=lattice[iFlux][iFluy].m_dvy;
			dDen=lattice[iFlux][iFluy].m_dDen;
			if(lattice[iFlux+iPrjx[iDir]]
					[iFluy+iPrjy[iDir]].m_iSol<=-6)
			{
				if(Boud[i].m_link[j].m_dDlf>=0.5)
				{
					dBFVelx=(Boud[i].m_link[j].m_dDlf-1)/
						Boud[i].m_link[j].m_dDlf*dFluVelx;
					dBFVely=(Boud[i].m_link[j].m_dDlf-1)/
						Boud[i].m_link[j].m_dDlf*dFluVely;
					dKappa=(2*Boud[i].m_link[j].m_dDlf-1)/dTau;
				}
				else
				{
				   dBFVelx=lattice[GetModx(iFlux+iPrjx[iDir])]
						[GetMody(iFluy+iPrjy[iDir])].m_dvx;
					dBFVely=lattice[GetModx(iFlux+iPrjx[iDir])]
						[GetMody(iFluy+iPrjy[iDir])].m_dvy;
					dKappa=(2*Boud[i].m_link[j].m_dDlf-1)/(dTau-2);	
				}
				lattice[isx][isy].m_df[iDir]=
					(1.0-dKappa)*lattice[iFlux][iFluy].
					m_df[iRev[iDir]]+dKappa*Lattice::feqs(iRev[iDir],dDen,
				dBFVelx,dBFVely,dFluVelx,dFluVely);
			   
			}//if <=-6
			else
			{
				lattice[isx][isy].m_df[iDir]=
					lattice[iFlux][iFluy].m_df[iRev[iDir]];
			}//else
			if(lattice[isx][isy].m_df[iDir]>
				1.1*Lattice::feq(iDir,dDensity,0,0))
			{ 
			
				lattice[isx][isy].m_df[iDir]=
					1.1*Lattice::feq(iDir,dDensity,0,0);
			}
			else if(lattice[isx][isy].m_df[iDir]
				<0.90*Lattice::feq(iDir,dDensity,0,0))
			{
				
				lattice[isx][isy].m_df[iDir]=
					0.9*Lattice::feq(iDir,dDensity,0,0);
			}
		//	dFlowOut+=lattice[iFlux][iFluy].m_df[iRev[iDir]];
		//	dFlowIn+=lattice[iFlux][iFluy].m_df[iDir];
		//	iTotal++;
		}//for j
	}//for i
}*/


int Line::IsCross(Vector l1, Vector l2, Vector *pp)
{
	double f1,f2,f3,f4;
	double a1,b1,c1,a2,b2,c2;
	Vector s1,s2;
	s1=m_p1;s2=m_p2;
	a1=s2.m_dy-s1.m_dy;
	b1=-(s2.m_dx-s1.m_dx);
	c1=-a1*s1.m_dx-b1*s1.m_dy;
	a2=l2.m_dy-l1.m_dy;
	b2=-(l2.m_dx-l1.m_dx);
	c2=-a2*l1.m_dx-b2*l1.m_dy;
	f1=(l1-s1)^(s2-s1);//s1,l1,s2
	f2=(l2-s1)^(s2-s1);//s1,l2,s2
	f3=(s2-l1)^(l2-l1);//l1,s2,l2
	f4=(s1-l1)^(l2-l1);//l1,s1,l2
	if(f1*f2<=0&&f3*f4<=0||f1==0)//s2上的不要
	{
		if(pp!=NULL)
		{
			if(a1*b2-a2*b1>0||a1*b2-a2*b1<0)
			{
				pp->m_dx=(c2*b1-c1*b2)/(a1*b2-a2*b1);
				pp->m_dy=(c1*a2-c2*a1)/(a1*b2-a2*b1);
				pp->m_dx=ErrorCorrect(pp->m_dx,l1.m_dx,l2.m_dx);
				pp->m_dy=ErrorCorrect(pp->m_dy,l1.m_dy,l2.m_dy);
			}
			else
			{
				if(lattice[int(l1.m_dx)][int(l1.m_dy)].m_iSol==-1)//两线重叠，直接认为交点在固体点上？？？
				{
					*pp=l1;
				}
				else
				{
					*pp=l2;
				}
			}
		}
		return 1;
	}
	else 
	{	
		return 0;
	}
}

int Line::IsHole(int ix,int ix0)
{
	int m;
	m=int(ix-ix0)%(m_iCycle);
	if(m>=0&&m<=m_iHole-1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void Line::recalculate(int i, int j)
{
	int k;
	lattice[i][j].m_dDen = 0;
	lattice[i][j].m_v.m_dx = 0;
	lattice[i][j].m_v.m_dy = 0;
	for (k = 0; k<9; k++)
	{
		lattice[i][j].m_dDen += lattice[i][j].m_df[k];
		lattice[i][j].m_v.m_dx += lattice[i][j].m_df[k]
			* iPrjx[k];
		lattice[i][j].m_v.m_dy += lattice[i][j].m_df[k]
			* iPrjy[k];
	}
}

void Line::Oneway()
{
	int i,j;
	double dFraction = 0.85;
	switch (m_iType)
	{
	case 1://up
		for (i = int(m_p1.m_dx); i <= int(m_p2.m_dx + 0.5); i++)
		{
			if (lattice[i][H2].m_iSol == -1)
			{
				if (lattice[i][H2].m_v.m_dy > 0)
				{
					lattice[i][H2].m_v.m_dy = 0;
					for (j = 0; j < 9; j++)
					{
						if (lattice[i + iPrjx[j]][H2 + iPrjy[j]].m_iSol < 0 && lattice[i - iPrjx[j]][H2 - iPrjy[j]].m_iSol < 0)//两头是流体
						{
							lattice[i][H2].m_df[j] = PartialBounce(lattice[i + iPrjx[j]][H2 + iPrjy[j]].m_df[iRev[j]]
								, lattice[i][H2].m_df[j], dFraction);
							//	, lattice[i - iPrjx[j]][H2 - iPrjy[j]].m_df[j], dFraction);
							//lattice[i][H2].m_df[j]=Lattice::feq(j, lattice[i][H2].m_dDen,
							//lattice[i][H2].m_v.m_dx, lattice[i][H2].m_v.m_dy);
						}
					}
				}
				else
				{
					for (j = 0; j < 9; j++)
					{
						if (lattice[i + iPrjx[j]][H2 + iPrjy[j]].m_iSol < 0 && lattice[i - iPrjx[j]][H2 - iPrjy[j]].m_iSol < 0)
						{
							lattice[i][H2].m_df[j] = PartialBounce(lattice[i + iPrjx[j]][H2 + iPrjy[j]].m_df[iRev[j]]
								, lattice[i][H2].m_df[j], dResistance);
						}
					}
				}
				recalculate(i, H2);
			}
		}
		break;
	case 2://down
		for (i = int(m_p1.m_dx); i <= int(m_p2.m_dx + 0.5); i++)
		{
			if (lattice[i][H1].m_iSol == -1)
			{
				if (lattice[i][H1].m_v.m_dy < 0)
				{
					lattice[i][H1].m_v.m_dy = 0;
					for (j = 0; j < 9; j++)
					{
						if (lattice[i + iPrjx[j]][H1 + iPrjy[j]].m_iSol < 0 && lattice[i - iPrjx[j]][H1 - iPrjy[j]].m_iSol < 0)
						{
							lattice[i][H1].m_df[j] = PartialBounce(lattice[i + iPrjx[j]][H1 + iPrjy[j]].m_df[iRev[j]],
								lattice[i][H1].m_df[j], dFraction);
						}
					}
				}
				else
				{
					for (j = 0; j < 9; j++)
					{
						if (lattice[i + iPrjx[j]][H1 + iPrjy[j]].m_iSol < 0 && lattice[i - iPrjx[j]][H1 - iPrjy[j]].m_iSol < 0)
						{
							lattice[i][H1].m_df[j] = PartialBounce(lattice[i + iPrjx[j]][H1 + iPrjy[j]].m_df[iRev[j]],
								lattice[i][H1].m_df[j], dResistance);
						}
					}
				}
				recalculate(i, H1);
			}
		}
		break;
	case -1://v
		for (i = H1; i <= int(H2 + 0.5); i++)
		{
			if (lattice[int(m_p1.m_dx)][i].m_iSol == -1)
			{
				if (lattice[int(m_p1.m_dx)][i].m_v.m_dx < 0)
				{
					lattice[int(m_p1.m_dx)][i].m_v.m_dx = 0;
					for (j = 0; j < 9; j++)
					{
						if (lattice[int(m_p1.m_dx) + iPrjx[j]][i + iPrjy[j]].m_iSol < 0 && lattice[int(m_p1.m_dx) - iPrjx[j]][i - iPrjy[j]].m_iSol)
						{
							lattice[int(m_p1.m_dx)][i].m_df[j] = PartialBounce(lattice[int(m_p1.m_dx) + iPrjx[j]][i + iPrjy[j]].m_df[iRev[j]],
								lattice[int(m_p1.m_dx)][i].m_df[j], dFraction);
						}
					}
				}
				else
				{
					for (j = 0; j < 9; j++)
					{
						if (lattice[int(m_p1.m_dx) + iPrjx[j]][i + iPrjy[j]].m_iSol < 0 && lattice[int(m_p1.m_dx) - iPrjx[j]][i - iPrjy[j]].m_iSol)
						{
							lattice[int(m_p1.m_dx)][i].m_df[j] = PartialBounce(lattice[int(m_p1.m_dx) + iPrjx[j]][i + iPrjy[j]].m_df[iRev[j]],
								lattice[int(m_p1.m_dx)][i].m_df[j], dResistance);
						}
					}
				}
				recalculate(int(m_p1.m_dx), i);
			}
		}
		break;
	default:
		;
	}
}

double Line::PartialBounce(double in, double out, double dFraction)
{
	return dFraction*in + (1 - dFraction)*out;
}

void Line::Pack(char* buffer, int  size, int* position)//4d+2i
{
	MPI_Pack((char*)&(m_p1), 6, MPI_DOUBLE, buffer, size, position, MPI_COMM_WORLD);
}

void Line::Unpack(char* buffer, int  size, int* position)
{
	MPI_Unpack(buffer, size, position, (char*)&(m_p1), 6, MPI_DOUBLE, MPI_COMM_WORLD);
}

MPI_File& operator<<(MPI_File& fs, Line& v)//4d+2i
{
	MPI_Status Status;
	MPI_File_write(fs, (char*)&(v.m_p1), sizeof(v.m_p1), MPI_BYTE, &Status);
	MPI_File_write(fs, (char*)&(v.m_p2), sizeof(v.m_p2), MPI_BYTE, &Status);
	MPI_File_write(fs, (char*)&(v.m_iHole), sizeof(v.m_iHole), MPI_BYTE, &Status);
	MPI_File_write(fs, (char*)&(v.m_iType), sizeof(v.m_iType), MPI_BYTE, &Status);
	return fs;
}

MPI_File& operator>>(MPI_File& fs, Line& v)
{
	MPI_Status Status;
	MPI_File_read(fs, (char*)&(v.m_p1), sizeof(v.m_p1), MPI_BYTE, &Status);
	MPI_File_read(fs, (char*)&(v.m_p2), sizeof(v.m_p2), MPI_BYTE, &Status);
	MPI_File_read(fs, (char*)&(v.m_iHole), sizeof(v.m_iHole), MPI_BYTE, &Status);
	MPI_File_read(fs, (char*)&(v.m_iType), sizeof(v.m_iType), MPI_BYTE, &Status);
	return fs;
}