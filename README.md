# MPI-Programs
## Reciprocal Sqaure Sum
Uses the numerical identity that the sum of the reciprocals of the squares of integers converges to ![equation](https://latex.codecogs.com/gif.latex?\frac{\pi^2}{6}) for estimating the numerical value of ![equation](https://latex.codecogs.com/gif.latex?\frac{\pi^2}{6}).
* **Input** - An integer N denoting the number of terms in the series that the program has to use.
* **Output** - A floating point number, denoting the estimated value of ![equation](https://latex.codecogs.com/gif.latex?\frac{\pi^2}{6}) rounded off to six decimal places.

## Vertex Coloring
Given an undirected graph G, finds a proper vertex coloring of the graph using Delta(G) + 1 colors or fewer. Delta(G) is the maximum degree of any vertex in G.
* **Input** - First line contains two integers N & M, denoting the number vertices and edges of the graph. Then, there are M lines, each of which contains 2 integers u and v, which means there is an edge in graph joining vertices u and v.
* **Output** - First line contains a single integer x denoting the number of colors used. Second line contains N integers (c1, c2, ..., cN), where ci represents color of vertex i. 1 ≤ ci ≤ x

## Matrix Multiplication
Cannon’s algorithm for matrix multiplication.
* **Input** - First line contains an integer N, denoting the dimensions of the both square matrix. Then, there are N lines, each of which contains N integers representing the first matrix. Then, another similar N lines representing the second matrix.
* **Output** - N lines, where each line contains N integers denoting the product of the input matrices.
* **Assumptions** - No of processes will be a perfect square and N will be divisible by the square root of Number of processes.
