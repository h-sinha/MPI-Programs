// Calculate the sum of reciprocals of squares

#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

long double sequential(int N)
{
    long double sum = 0;
    for (long double i = 1; i <= N ; ++i)
    {
        sum += (1/(i*i));
    }
    return sum;
}
int main(int argc, char *argv[])
{
    int rank, size;

    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int elements_per_process, start_element, end_element;

    if (rank == 0)
    {
        int N;
        cin >> N;

        int i;
        if (N > size)
            elements_per_process = N / size;
        else
            elements_per_process = 1;

        if (size > 1)
        {
            if (N > size)
            {
                for (i = 1; i < size - 1; i++)
                {
                    start_element = i * elements_per_process + 1;
                    end_element = start_element + elements_per_process - 1;

                    MPI_Send(&start_element, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                    MPI_Send(&end_element, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                }

                start_element = (size - 1) * elements_per_process + 1;
                end_element = N;

                MPI_Send(&start_element, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                MPI_Send(&end_element, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
            else
            {
                for (int i = 1; i < N; i++)
                {
                    start_element = i + 1;
                    end_element = i + 1;
                    MPI_Send(&start_element, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                    MPI_Send(&end_element, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                }
                for (int i = N; i < size; i++)
                {
                    start_element = end_element + 1;
                    MPI_Send(&start_element, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                    MPI_Send(&end_element, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                }
            }
        }

        long double sum = 0;
        for (long double i = 1; i <= elements_per_process; i++)
        {
            sum += 1 / (i * i);
        }

        long double temp;
        for (i = 1; i < size; i++)
        {
            MPI_Recv(&temp, 1, MPI_LONG_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            sum += temp;
        }

        // assert(abs(sum - sequential(N))<=1e-10);

        cout << std::fixed;
        cout << std::setprecision(6);
        cout << sum << endl;
    }
    else
    {
        MPI_Recv(&start_element, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&end_element, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        long double partial_sum = 0;

        if (start_element <= end_element)
        {
            for (long double i = start_element; i <= end_element; i++)
            {
                partial_sum += 1 / (i * i);
            }
        }

        MPI_Send(&partial_sum, 1, MPI_LONG_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}