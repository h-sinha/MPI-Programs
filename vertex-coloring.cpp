// Luby Jones algorithm

#include <bits/stdc++.h>
#include <mpi.h>
#include <unistd.h>
using namespace std;

int main(int argc, char *argv[])
{
	int rank, size;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int graph[101][101];
	int color[101];
	int weight[101];
	int data[3];
	int N, M;

	memset(graph, 0, sizeof(graph));
	memset(color, -1, sizeof(color));

	if(rank == 0)
	{
		
		cin >> N >> M;
		
		int u, v;
		for (int i = 0; i < M; ++i)
		{
			cin >> u >> v;
			graph[u][v] = 1;
			graph[v][u] = 1;
		}

		int max_deg = 0;
		int cur_count = 0;
		for (int i = 1; i <= N; ++i)
		{
			cur_count = 0;
			for (int j = 1; j <= N; ++j)
			{
				cur_count += graph[i][j];
			}
			max_deg = max(max_deg, cur_count);
		}

		int max_col = max_deg + 1;
		cout << max_col << endl;
		int vertex_per_proc = N / size;
		int vertex_last_proc = N % size + N / size;
		// random seed
		srand(time(0)); 
		
		// Assign random weights
		for (int i = 1; i <= N; ++i)
		{
			weight[i] = rand();
		}

		// send number of vertices
		MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
		
		// send randomly assigned weights
		MPI_Bcast(&weight, N + 1, MPI_INT, 0, MPI_COMM_WORLD);

		// send graph
		for (int i = 1; i <= N; ++i)
		{
			MPI_Bcast(&graph[i], N + 1, MPI_INT, 0, MPI_COMM_WORLD);
		}
		int l = 1 + vertex_per_proc;
		for (int i = 1; i < size; ++i)
		{
			cur_count = vertex_per_proc;
			if(i == size - 1)cur_count = vertex_last_proc;
			
			data[0] = l;
			data[1] = l + cur_count - 1;
			data[2] = max_col;
			// send range of vertices and max allowed color
			MPI_Send(&data, 3, MPI_INT, i, i, MPI_COMM_WORLD);

			l += cur_count;
		}
		// for process 0
		data[0] = 1;
		data[1] = 1 + vertex_per_proc - 1;
		data[2] = max_col;
	}
	if(rank)
	{
		// receive number of vertices
		MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

		// receive randomly assigned weights
		MPI_Bcast(&weight, N + 1, MPI_INT, 0, MPI_COMM_WORLD);

		// receive graph
		for (int i = 1; i <= N; ++i)
		{
			MPI_Bcast(&graph[i], N + 1, MPI_INT, 0, MPI_COMM_WORLD);
		}

		// receive range of vertices and max allowed color
		MPI_Recv(&data, 3, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	int assign = 0, it = 0;
	while(it <= 2*N)
	{
		it++;
		assign = 1;
		for (int v = data[0]; v <= data[1]; ++v)
		{
			if(color[v] == -1)
			{
				assign = 1;
				for (int i = 1; i <= N; ++i)
				{
					if(graph[v][i] && color[i] == -1 && weight[i] > weight[v])
					{
						assign = 0;
						break;
					}
				}
				if(assign)
				{
					set<int>forbidden;
					for (int i = 1; i <= N; ++i)
					{
						if(graph[v][i] && color[i] != -1)
						{
							forbidden.insert(color[i]);
						}
					}
					for (int i = 1; i <= data[2]; ++i)
					{
						if(forbidden.find(i) == forbidden.end())
						{
							color[v] = i;
							break;
						}
					}
				}
			}
		}
		
		MPI_Bcast(&color, N + 1, MPI_INT, rank, MPI_COMM_WORLD);

		for (int i = 0; i < size; ++i)
		{
			if(i != rank)
			{
				int recv_col[101];
				MPI_Bcast(&recv_col, N + 1, MPI_INT, i, MPI_COMM_WORLD);
				for (int j = 1; j <= N; ++j)
				{
					if(color[j] == -1 && recv_col[j] != -1)
					{
						color[j] = recv_col[j];
					}
				}
			}
		}
	}
	if(rank)MPI_Send(color, N + 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
	else
	{
		// receive color from all processes
		for(int i = 1; i < size; i++)
		{
			MPI_Recv(&color, N + 1, MPI_INT, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}

		int max_col = 0;
		for (int i = 1; i <= N; ++i)
		{
			max_col = max(max_col, color[i]);
			for (int j = 1; j <= N; ++j)
			{
				if(graph[i][j])
				{
					// cout << i <<" "<<j <<" "<<color[i]<<" "<<color[j]<<endl;
					assert(color[i] != color[j]);
				}
			}
		}

		cout << max_col << "\n";
		for (int i = 1; i <= N; ++i)
		{
			cout << color[i] << " ";
		}
		cout << "\n";
	}
	MPI_Finalize();
	return 0;
}