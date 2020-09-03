// Calculate the sum of reciprocals of squares

#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

int main(int argc, char *argv[])
{
    int rank, num_process;

    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_process);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int elements_per_process, start_element, end_element;

    if (rank == 0)
    {
        int N;
        cin >> N;

        int i;
        elements_per_process = N / num_process;

        if (num_process > 1)
        {
            for (i = 1; i < num_process; i++)
            {
                start_element = i * elements_per_process + 1;
                end_element = start_element + elements_per_process - 1;
                if (end_element > N)
                {
                    end_element = N;
                }

                MPI_Send(&start_element, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                MPI_Send(&end_element, 1, MPI_INT, i, 0, MPI_COMM_WORLD);

                if (end_element == N)
                {
                    break;
                }
            }
        }

        long double sum = 0;
        for (long double i = 1; i <= elements_per_process; i++)
        {
            sum += 1 / (i * i);
        }

        long double temp;
        for (i = 1; i < num_process; i++)
        {
            MPI_Recv(&temp, 1, MPI_LONG_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            sum += temp;
        }

        cout << std::fixed;
        cout << std::setprecision(6);
        cout << sum << endl;
    }
    else
    {
        MPI_Recv(&start_element, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&end_element, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        long double partial_sum = 0;
        for (long double i = start_element; i <= end_element; i++)
        {
            partial_sum += 1 / (i * i);
        }

        MPI_Send(&partial_sum, 1, MPI_LONG_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}