// Matrix Multiplication

#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

int main(int argc, char *argv[])
{
    int N;
    cin >> N;

    int a[N][N], b[N][N], c[N][N], sum = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            cin >> a[i][j];

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            cin >> b[i][j];

    int a_temp[N][N], c_temp[N][N];

    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Scatter(a, N * N / size, MPI_INT, a_temp, N * N / size, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Bcast(b, N * N, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            sum += a_temp[j] * b[j][i];
        }
        c_temp[i] = sum;
        sum = 0;
    }

    MPI_Gather(c_temp, N * N / size, MPI_INT, c, N * N / size, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                cout << c[i][j] << " ";
            }
            cout << endl;
        }
    }

    MPI_Finalize();

    return 0;
}