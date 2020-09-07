#!/bin/bash
fn_test_status()
{
	case $1 in
		0)
			echo "Passed"
		;;
		*)
			echo "Failed"
			cat out.teststdin
			cat out.teststderr
			exit 1
		;;
	esac
}

mpic++ ../reciprocal-square-sum.cpp
for ((i=1;i<=10;i++)); do
	mpirun  -np $i  ./a.out <<< 1 > out.teststdin 2>out.teststderr
    fn_test_status $?
	mpirun  -np $i  ./a.out <<< 5 > out.teststdin 2>out.teststderr
    fn_test_status $?
	mpirun  -np $i  ./a.out <<< 98 > out.teststdin 2>out.teststderr
    fn_test_status $?
	mpirun  -np $i  ./a.out <<< 6 > out.teststdin 2>out.teststderr
    fn_test_status $?
	mpirun  -np $i  ./a.out <<< 20 > out.teststdin 2>out.teststderr
    fn_test_status $?
done
mpic++ ../matrix-multiplication.cpp
mpirun  -np 4  ./a.out < mat_test > out.teststdin 2>out.teststderr
fn_test_status $?
mpirun  -np 1  ./a.out < mat_test > out.teststdin 2>out.teststderr
fn_test_status $?
mpirun  -np 16  ./a.out < mat_test1 > out.teststdin 2>out.teststderr
fn_test_status $?
mpirun  -np 4  ./a.out < mat_test1 > out.teststdin 2>out.teststderr
fn_test_status $?
mpirun  -np 1  ./a.out < mat_test1 > out.teststdin 2>out.teststderr
fn_test_status $?
mpirun  -np 1  ./a.out < mat_test2 > out.teststdin 2>out.teststderr
fn_test_status $?
mpirun  -np 9  ./a.out < mat_test2 > out.teststdin 2>out.teststderr
for ((i=1;i<=20;i++)); do
	g++ graph_gen.cpp
	./a.out > gr
	mpic++ ../vertex-coloring.cpp
	for ((j=1;j<=10;j++)); do
		mpirun  -np $j  ./a.out < gr > out.teststdin 2>out.teststderr
	    fn_test_status $?
	done
done
