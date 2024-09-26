#include <iostream>
#include <array>
#include <Python.h>
#include <memory>
#include "Header.h"
using namespace std;

int main() {
	auto arrNodesX = new long double[NConst];
	auto arrFx = new long double[NConst];
	auto arrFx2 = new long double[MConst+NConst];
	auto arrLagrange = new long double[MConst+NConst];
	auto arrEvalpoints = new long double[MConst+NConst]; //Сумма возникает из за того что требуется построить сетку из M точек, если не учитывать сумму, сетка получится из M-N точек, поскольку точки старого массива содержатся в новом
	try{
		checkErrorsFun(aConst, bConst, NConst, MConst, Epsilon);

		genRandomArrWithSpacing(arrNodesX, Epsilon, NConst);
		genRandomArrSealing(arrEvalpoints, arrNodesX, NConst, MConst+NConst, aConst, bConst);
	
		func(arrNodesX,arrFx, NConst);
		lagrange3(arrNodesX, arrFx, arrEvalpoints, arrLagrange, NConst, MConst+NConst);
		func(arrEvalpoints, arrFx2, MConst+NConst);//увеличиваем область определения функции для передачи в питон

		callPythonScript2(arrEvalpoints, arrFx2, arrLagrange, arrNodesX, arrFx, NConst, MConst+NConst);
	}
	catch(...){
		cout<< "Error" << endl;
	}
	delete [] arrNodesX;
	delete [] arrFx;
	delete [] arrFx2;
	delete [] arrLagrange;
	delete [] arrEvalpoints;
	return 0;
}