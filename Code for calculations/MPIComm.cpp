#include "MPIComm.h"
void MPIInitComm()
{
	int i;
	int iSizeLattice, iSizeMem;
	MPI_Datatype old_type_mem[2];
	int blocklens_mem[2];
	MPI_Aint indices_mem[2];
	int blocklens0[HEIGHT];
	MPI_Aint indices0[HEIGHT];
	MPI_Datatype old_type0[HEIGHT];
	int blocklens[3];
	MPI_Aint indices[3];
	MPI_Datatype old_type[3];
	//int blocklens_all[2];
	//MPI_Aint indices_all[2];
	//MPI_Datatype old_type_all[2];


	//开始第一纵队的数据定位
	for (i = 0; i<HEIGHT; i++)
	{
		blocklens0[i] = 16;
		old_type0[i] = MPI_DOUBLE;
		MPI_Address(&(lattice[iEnd_Row - 4][i].m_iSol), &indices0[i]);//格子边界重复方式,因为每段占两格
		if (i != 0)
		{
			indices0[i] -= indices0[0];
		}
	}
	indices0[0] = 0;
	MPI_Type_struct(HEIGHT, blocklens0, indices0, old_type0, &Lattice_data_type0);
	MPI_Type_commit(&Lattice_data_type0);//end send end,send shared data down to right

	//结束第一纵队的数据定位
	//开始三列数据定位
	for (i = 0; i<3; i++)
	{
		blocklens[i] = 1;
		old_type[i] = Lattice_data_type0;
		MPI_Address(&(lattice[iEnd_Row - 4 + i][0].m_iSol), &indices[i]);
		if (i != 0)
		{
			indices[i] -= indices[0];
		}
	}
	indices[0] = 0;
	MPI_Type_struct(3, blocklens, indices, old_type, &Lattice_data_type);
	MPI_Type_commit(&Lattice_data_type);//end send end,send shared data down to right
	//MPI_Type_free(&Lattice_data_type0);
	//结束三列数据定位

	//开始膜数据定位
	blocklens_mem[0] = 23; blocklens_mem[1] = 23;
	old_type_mem[0] = MPI_DOUBLE; old_type_mem[1] = MPI_DOUBLE;
	MPI_Address(&(LymVessle.m_MemUp.m_segment[iEnd_Mem - 1].m_bPassTh), &indices_mem[0]);//膜不重复方式，顺连
	MPI_Address(&(LymVessle.m_MemDown.m_segment[iEnd_Mem - 1].m_bPassTh), &indices_mem[1]);
	indices_mem[1] -= indices_mem[0];
	indices_mem[0] = 0;
	////结束膜数据定位
	MPI_Type_struct(2, blocklens_mem, indices_mem, old_type_mem, &Mem_data_type);
	MPI_Type_commit(&Mem_data_type);//end send end,send shared data down to right
	blocklens_mem[0] = 32;
	old_type_mem[0] = MPI_DOUBLE; indices_mem[0] = 0;
	MPI_Type_struct(1, blocklens_mem, indices_mem, old_type_mem, &One_Mem_data_type);
	MPI_Type_commit(&One_Mem_data_type);
	blocklens_mem[0] = 34;
	old_type_mem[0] = MPI_DOUBLE; indices_mem[0] = 0;
	MPI_Type_struct(1, blocklens_mem, indices_mem, old_type_mem, &One_Valve_Mem_data_type);
	MPI_Type_commit(&One_Valve_Mem_data_type);

	MPI_Pack_size(3, Lattice_data_type, MPI_COMM_WORLD, &iSizeLattice);
	MPI_Pack_size(2, Mem_data_type, MPI_COMM_WORLD, &iSizeMem);
	iBufferSize = iSizeLattice + iSizeMem + MPI_BSEND_OVERHEAD;
	SendBuffer = new char[iBufferSize];
	if (SendBuffer == NULL) printf("Can't apply buffer\n");
	MPI_Buffer_attach(SendBuffer, iBufferSize);
}
