#include <stdio.h>
#include <mpi.h>

using namespace std;

int main()
{
	int rank, size;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	int graph[101][101];
	int color[101];
	int N, M;
	
	memset(graph, 0,sizeof(graph));
	memset(color, -1,sizeof(color));

	if(rank == 0)
	{
		cin >> N >> M;
		
		for (int i = 0; i < m; ++i)
		{
			cin >> u >> v;
			graph[u][v] = 1;
			graph[v][u] = 1;
		}



	}

	return 0;
}