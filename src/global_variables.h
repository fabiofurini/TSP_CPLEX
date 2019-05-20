#ifndef VARIABLE_local_HEADER
#define VARIABLE_local_HEADER


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <string.h>
#include <errno.h>
#include <sstream>
#include <vector>
#include <algorithm>
#include <set>
#include <iomanip> // for precision


////////////////////////////////////////////////////////////////////////////////////////////////////
#include </Users/fabiofurini/Applications/IBM/ILOG/CPLEX_Studio126/cplex/include/ilcplex/cplex.h>
//#include </home/fabio/ILOG/CPLEX_Studio_AcademicResearch127/cplex/include/ilcplex/cplex.h>
////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace std;


typedef struct data
{
	double timeLimit;
	int number_of_CPU;
	int n_input;

	int algo;
	int option;

	int cut_integer;
	int cut_fractional;

	int ** DISTANCE_MATRIX;
	int n_cities;

	char *istname;

	int status,ccnt,rcnt,nzcnt,lpstat,nodecount,cur_numrows,cur_numcols;
	int* rmatbeg,*rmatind,*cmatbeg, *cmatind,*indobj;
	double *rmatval,*cmatval,*rngval,*x,*x_SEP,*pi,*obj, *lb, *ub,*rhs,coef_p,objval,objval_SEP,bestobjval,*valobj;
	char *xctype,*sense;
	char **rowname,**colname;

	double cplex_lp;

	CPXENVptr env,env_SEP;
	CPXLPptr  lp,lp_SEP;

	double cut_SEP_RHS;
	int *cut_SEP_rmatind;
	double *cut_SEP_rmatval;

	int cut_1;
	int cut_2;


} data;



#endif
