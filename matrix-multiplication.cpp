// Matrix Multiplication

#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

void multiply(int **c, int **a, int **b, int n)
{
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			for (int k = 0; k < n; ++k)
			{
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}
}
int** allocate_matrix(int N)
{
	int **a = new int *[N];
	int* p = (int*)malloc(sizeof(int*) * N * N);

	for(int i = 0; i < N; ++i)
	{
		a[i] = &(p[i * N]);
		for(int j = 0; j < N; ++j)
		{
			a[i][j] = 0;
		}
	}
	return a;
}
void print_mat(int ** a, int N)
{
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			cout << a[i][j] <<" ";
		}
		cout << "\n";
	}

}
void test(int **a, int **b, int **c, int N)
{
	int **d = allocate_matrix(N);
	multiply(d, a, b, N);
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			assert(c[i][j] == d[i][j]);
		}
	}
}
int main(int argc, char *argv[])
{
	int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int N;
	int **a, **b, **c;
	int left, right, up, down;
    int sqrt_size;

    if(rank == 0)
    {
	    cin >> N;

	    a = allocate_matrix(N);
	    b = allocate_matrix(N);
	    c = allocate_matrix(N);
	    for (int i = 0; i < N; i++)
	        for (int j = 0; j < N; j++)
	            cin >> a[i][j];

	    for (int i = 0; i < N; i++)
	        for (int j = 0; j < N; j++)
	            cin >> b[i][j];

	    sqrt_size = sqrt(size);
    }
	MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&sqrt_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Comm comm;

	int Np = N / sqrt_size;
	int **ta = allocate_matrix(Np);
	int **tb = allocate_matrix(Np);
	int **tc = allocate_matrix(Np);

	int dims[] = {sqrt_size, sqrt_size};
	int period[] = {1, 1};
	int coord[2];
	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, period, 1, &comm);
	    
	int mat_size[] = {N, N};
	int block_size[] = {Np, Np};
	int start_idx[2] = {0, 0};
	MPI_Datatype type, subarray_type;
	MPI_Type_create_subarray(2, mat_size, block_size, start_idx, MPI_ORDER_C, MPI_INT, &type);
	MPI_Type_create_resized(type, 0, Np * sizeof(int), &subarray_type);
	MPI_Type_commit(&subarray_type);
		
	int send_counts[size], displ[size];
	if (rank == 0) 
	{
		for (int i = 0; i < size; i++)
		{
			send_counts[i] = 1;
			displ[i] = (i / sqrt_size) * (Np * sqrt_size) + i % sqrt_size;
		}
		MPI_Scatterv(&(a[0][0]), send_counts, displ, subarray_type, &(ta[0][0]), (N * N) / (size), MPI_INT, 0, comm);
		MPI_Scatterv(&(b[0][0]), send_counts, displ, subarray_type, &(tb[0][0]), (N * N) / (size), MPI_INT, 0, comm);
	}
	else
	{
		MPI_Scatterv(NULL, NULL, NULL, subarray_type, &(ta[0][0]), (N * N) / (size), MPI_INT, 0, comm);
		MPI_Scatterv(NULL, NULL, NULL, subarray_type, &(tb[0][0]), (N * N) / (size), MPI_INT, 0, comm);
	}
	
	// determine proc coord in cartesian topology
	MPI_Cart_coords(comm, rank, 2, coord);
	// initial skew
	MPI_Cart_shift(comm, 1, coord[0], &left, &right);
	MPI_Sendrecv_replace(&(ta[0][0]), Np * Np, MPI_INT, left, 1, right, 1, comm, MPI_STATUS_IGNORE);
	MPI_Cart_shift(comm, 0, coord[1], &up, &down);
	MPI_Sendrecv_replace(&(tb[0][0]), Np * Np, MPI_INT, up, 1, down, 1, comm, MPI_STATUS_IGNORE);
	for (int i = 0; i < sqrt_size; i++) 
	{
		multiply(tc, ta, tb, Np);
		// Shift a once left and b once up
		MPI_Cart_shift(comm, 1, 1, &left, &right);
		MPI_Cart_shift(comm, 0, 1, &up, &down);
		MPI_Sendrecv_replace(&(ta[0][0]), Np * Np, MPI_INT, left, 1, right, 1, comm, MPI_STATUS_IGNORE);
		MPI_Sendrecv_replace(&(tb[0][0]), Np * Np, MPI_INT, up, 1, down, 1, comm, MPI_STATUS_IGNORE);
	}
	
	if(rank == 0)
	{
 		MPI_Gatherv(&(tc[0][0]), (N * N) / size, MPI_INT, &(c[0][0]), send_counts, displ, subarray_type, 0, comm);
   	}
   	else
   	{
 		MPI_Gatherv(&(tc[0][0]), (N * N) / size, MPI_INT, NULL, NULL, NULL, subarray_type, 0, comm);
   	}
   	MPI_Barrier(MPI_COMM_WORLD);
 	if(rank == 0)
  	{
  		test(a, b, c, N);
  		print_mat(c, N);
  	}
    MPI_Finalize();
    return 0;
}