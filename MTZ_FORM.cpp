

#include "MTZ_FORM.h"


#define print_solution_X
//#define print_solution_U
//#define write_lp
//#define print_sol_callback
//#define solve_lp

#define EPSILON_TOLL 1e-4

/*****************************************************************/
int CPXPUBLIC mycutcallback_MODEL(CPXCENVptr env,void *cbdata,int wherefrom,void *cbhandle,int *useraction_p)
/*****************************************************************/
{

	(*useraction_p)=CPX_CALLBACK_DEFAULT;

	data *TSP_instance=(data *) cbhandle;

	int num_variables=TSP_instance->n_cities*TSP_instance->n_cities+TSP_instance->n_cities;

	TSP_instance->status=CPXgetcallbacknodex(env,cbdata,wherefrom,TSP_instance->x,0,num_variables-1);
	if(TSP_instance->status!=0)
	{
		printf("cannot get the x\n");
		exit(-1);
	}

#ifdef print_sol_callback
	cout << "\n\nVARIABLES X\n";
	for(int j=0; j<TSP_instance->n_cities; j++)
	{
		for(int k=0; k<TSP_instance->n_cities; k++)
		{
			printf("%.3f ",TSP_instance->x[position_x(TSP_instance,j,k)]);
		}
		cout << endl;
	}
	cout << endl;

	cout << "\n\nVARIABLES U\n";
	for(int j=0; j<TSP_instance->n_cities; j++)
	{
		cout << TSP_instance->x[position_u(TSP_instance,j)] << "\t";
	}

	TSP_instance->status=CPXgetcallbacknodeobjval(env,cbdata,wherefrom,&TSP_instance->objval);
	if(TSP_instance->status!=0)
	{
		printf("error CPXgetcallbacknodeobjval\n");
		exit(-1);
	}
	cout << endl;

#endif

	//////////////////////////////////////////////////////////////////////////////////////////////////
	SEP_set_objective_function_cplex(TSP_instance);
	//////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////////
	TSP_instance->objval_SEP=SEP_solve_cplex(TSP_instance);
	//////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////////
	if(TSP_instance->objval_SEP < 1 - EPSILON_TOLL)
	{

		TSP_instance->nzcnt=0;
		int size_subset=0;

		for(int j=0; j<TSP_instance->n_cities; j++)
		{
			if(TSP_instance->x_SEP[position_w(TSP_instance,j)]>0.5)
			{
				size_subset++;
			}
		}

//		cout << "\nFOUND A VIOLATED CUT (INTEGER SOL)\t" <<  TSP_instance->objval_SEP << "\tsubset of size\t" << size_subset << "\n";
//		cin.get();

		TSP_instance->cut_1++;

		TSP_instance->cut_SEP_RHS=size_subset-1;

		TSP_instance->nzcnt=0;

		for(int j=0; j<TSP_instance->n_cities; j++)
		{
			for(int k=0; k<TSP_instance->n_cities; k++)
			{
				if(j==k){continue;}
				if
				(
						TSP_instance->x_SEP[position_w(TSP_instance,j)] > 0.5
						&&
						TSP_instance->x_SEP[position_w(TSP_instance,k)] > 0.5
				)
				{
					TSP_instance->cut_SEP_rmatind[TSP_instance->nzcnt]=position_x(TSP_instance,j,k);
					TSP_instance->cut_SEP_rmatval[TSP_instance->nzcnt]=1.0;
					TSP_instance->nzcnt++;
				}
			}
		}

		//TSP_instance->status=CPXcutcallbackaddlocal (env,cbdata,wherefrom,TSP_instance->nzcnt,TSP_instance->cut_SEP_RHS,'L',TSP_instance->cut_SEP_rmatind,TSP_instance->cut_SEP_rmatval);
		TSP_instance->status=CPXcutcallbackadd (env,cbdata,wherefrom,TSP_instance->nzcnt,TSP_instance->cut_SEP_RHS,'L',TSP_instance->cut_SEP_rmatind,TSP_instance->cut_SEP_rmatval,0);
		//TSP_instance->status=CPXcutcallbackadd (env,cbdata,wherefrom,TSP_instance->nzcnt,TSP_instance->cut_SEP_RHS,'L',TSP_instance->cut_SEP_rmatind,TSP_instance->cut_SEP_rmatval,1);

		if(TSP_instance->status!=0)
		{
			printf("CPXcutcallbackadd\n");
			exit(-1);
		}

		(*useraction_p)=CPX_CALLBACK_SET;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////



	return 0;
}

/*****************************************************************/
int CPXPUBLIC myusercutcallback_MODEL(CPXCENVptr env,void *cbdata,int wherefrom,void *cbhandle,int *useraction_p)
/*****************************************************************/
{

	(*useraction_p)=CPX_CALLBACK_DEFAULT;

	data *TSP_instance=(data *) cbhandle;

	int num_variables=TSP_instance->n_cities*TSP_instance->n_cities+TSP_instance->n_cities;

	TSP_instance->status=CPXgetcallbacknodex(env,cbdata,wherefrom,TSP_instance->x,0,num_variables-1);
	if(TSP_instance->status!=0)
	{
		printf("cannot get the x\n");
		exit(-1);
	}

#ifdef print_sol_callback
	cout << "\n\nVARIABLES X\n";
	for(int j=0; j<TSP_instance->n_cities; j++)
	{
		for(int k=0; k<TSP_instance->n_cities; k++)
		{
			printf("%.3f ",TSP_instance->x[position_x(TSP_instance,j,k)]);
		}
		cout << endl;
	}
	cout << endl;

	cout << "\n\nVARIABLES U\n";
	for(int j=0; j<TSP_instance->n_cities; j++)
	{
		cout << TSP_instance->x[position_u(TSP_instance,j)] << "\t";
	}

	TSP_instance->status=CPXgetcallbacknodeobjval(env,cbdata,wherefrom,&TSP_instance->objval);
	if(TSP_instance->status!=0)
	{
		printf("error CPXgetcallbacknodeobjval\n");
		exit(-1);
	}
	cout << endl;

#endif

	//////////////////////////////////////////////////////////////////////////////////////////////////
	SEP_set_objective_function_cplex(TSP_instance);
	//////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////////
	SEP_solve_cplex(TSP_instance);
	//////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////////
	if(TSP_instance->objval_SEP < 1 - EPSILON_TOLL)
	{

		TSP_instance->nzcnt=0;
		int size_subset=0;

		for(int j=0; j<TSP_instance->n_cities; j++)
		{
			if(TSP_instance->x_SEP[position_w(TSP_instance,j)]>0.5)
			{
				size_subset++;
			}
		}

//		cout << "\nFOUND A VIOLATED CUT (FRACT SOL)\t" <<  TSP_instance->objval_SEP << "\tsubset of size\t" << size_subset << "\n";
//		cin.get();

		TSP_instance->cut_2++;


		TSP_instance->cut_SEP_RHS=size_subset-1;

		TSP_instance->nzcnt=0;

		for(int j=0; j<TSP_instance->n_cities; j++)
		{
			for(int k=0; k<TSP_instance->n_cities; k++)
			{
				if(j==k){continue;}
				if
				(
						TSP_instance->x_SEP[position_w(TSP_instance,j)] >0.5
						&&
						TSP_instance->x_SEP[position_w(TSP_instance,k)]>0.5
				)
				{
					TSP_instance->cut_SEP_rmatind[TSP_instance->nzcnt]=position_x(TSP_instance,j,k);
					TSP_instance->cut_SEP_rmatval[TSP_instance->nzcnt]=1.0;
					TSP_instance->nzcnt++;
				}
			}
		}

		//TSP_instance->status=CPXcutcallbackaddlocal (env,cbdata,wherefrom,TSP_instance->nzcnt,TSP_instance->cut_SEP_RHS,'L',TSP_instance->cut_SEP_rmatind,TSP_instance->cut_SEP_rmatval);
		TSP_instance->status=CPXcutcallbackadd (env,cbdata,wherefrom,TSP_instance->nzcnt,TSP_instance->cut_SEP_RHS,'L',TSP_instance->cut_SEP_rmatind,TSP_instance->cut_SEP_rmatval,0);
		//TSP_instance->status=CPXcutcallbackadd (env,cbdata,wherefrom,TSP_instance->nzcnt,TSP_instance->cut_SEP_RHS,'L',TSP_instance->cut_SEP_rmatind,TSP_instance->cut_SEP_rmatval,1);

		if(TSP_instance->status!=0)
		{
			printf("CPXcutcallbackadd\n");
			exit(-1);
		}

		(*useraction_p)=CPX_CALLBACK_SET;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////



	return 0;
}

/*****************************************************************/
int position_x(data *TSP_instance,int j,int k)
/*****************************************************************/
{
	return TSP_instance->n_cities*j+k;
}

/*****************************************************************/
int position_u(data *TSP_instance,int j)
/*****************************************************************/
{
	return TSP_instance->n_cities*TSP_instance->n_cities+j;
}


/***********************************************************************************/
void MTZ_load_cplex(data *TSP_instance)
/***********************************************************************************/
{

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// * setting the CPLEX environment

	//opening the environment
	TSP_instance->env=CPXopenCPLEX(&(TSP_instance->status));
	if(TSP_instance->status!=0)
	{
		printf("cannot open CPLEX environment\n");
		exit(-1);
	}

	//opening the pointer to the problem
	TSP_instance->lp=CPXcreateprob(TSP_instance->env,&(TSP_instance->status),"BPP");
	if(TSP_instance->status!=0)
	{
		printf("cannot create problem\n");
		exit(-1);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// * creating the variables *
	TSP_instance->ccnt=TSP_instance->n_cities*TSP_instance->n_cities+TSP_instance->n_cities;
	TSP_instance->obj=(double*) calloc(TSP_instance->ccnt,sizeof(double));
	TSP_instance->lb=(double*) calloc(TSP_instance->ccnt,sizeof(double));
	TSP_instance->ub=(double*) calloc(TSP_instance->ccnt,sizeof(double));
	TSP_instance->xctype=(char*) calloc(TSP_instance->ccnt,sizeof(char));


	TSP_instance->colname=(char**) calloc(TSP_instance->ccnt,sizeof(char*));
	for(int i=0;i<TSP_instance->ccnt;i++){TSP_instance->colname[i]=(char*) calloc(2000,sizeof(char));}

	int counter=0;
	for(int j=0; j<TSP_instance->n_cities; j++)
	{
		for(int k=0; k<TSP_instance->n_cities; k++)
		{

			TSP_instance->obj[counter]=TSP_instance->DISTANCE_MATRIX[j][k];
			TSP_instance->lb[counter]=0.0;
			TSP_instance->ub[counter]=1.0;

			if(j==k){TSP_instance->ub[counter]=0.0;}

			TSP_instance->xctype[counter]='B';
			//cout << "POSITION X\t" << position_x(TSP_instance,j,k) << endl;
			sprintf(TSP_instance->colname[counter], "x%d.%d",j+1,k+1);
			counter++;
		}
	}

	for(int j=0; j<TSP_instance->n_cities; j++)
	{

		TSP_instance->obj[counter]=0.0;

		TSP_instance->lb[counter]=1.0;
		TSP_instance->ub[counter]=TSP_instance->n_cities-1;


		TSP_instance->xctype[counter]='C';
		//cout << "POSITION U\t" << position_u(TSP_instance,j) << endl;
		sprintf(TSP_instance->colname[counter], "u%d",j+1);
		counter++;
	}

	TSP_instance->status=CPXnewcols(TSP_instance->env,TSP_instance->lp,TSP_instance->ccnt,TSP_instance->obj,TSP_instance->lb,TSP_instance->ub,TSP_instance->xctype,TSP_instance->colname);
	if(TSP_instance->status!=0)
	{
		printf("error in CPXnewcols\n");
		exit(-1);
	}

	free(TSP_instance->obj);
	free(TSP_instance->lb);
	free(TSP_instance->ub);
	free(TSP_instance->xctype);

	for(int i=0;i<TSP_instance->ccnt;i++){free(TSP_instance->colname[i]);}
	free(TSP_instance->colname);


	// * setting the objective function in the minimization form
	CPXchgobjsen(TSP_instance->env,TSP_instance->lp,CPX_MIN);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//ONE OUTGOING ARC PER VERTEX
	for(int j=0; j<TSP_instance->n_cities; j++)
	{

		TSP_instance->rcnt=1;
		TSP_instance->nzcnt=TSP_instance->n_cities;
		TSP_instance->rhs=(double*) calloc(TSP_instance->rcnt,sizeof(double));
		TSP_instance->sense=(char*) calloc(TSP_instance->rcnt,sizeof(double));

		TSP_instance->rhs[0]=1.0;
		TSP_instance->sense[0]='E';


		TSP_instance->rmatbeg=(int*) calloc(TSP_instance->rcnt,sizeof(int));
		TSP_instance->rmatind=(int*) calloc(TSP_instance->nzcnt,sizeof(int));
		TSP_instance->rmatval=(double*) calloc(TSP_instance->nzcnt,sizeof(double));

		for(int k=0; k<TSP_instance->n_cities; k++)
		{
			TSP_instance->rmatval[k]=1.0;
			TSP_instance->rmatind[k]=position_x(TSP_instance,j,k);
		}

		TSP_instance->rmatbeg[0]=0;

		TSP_instance->status=CPXaddrows(TSP_instance->env,TSP_instance->lp,0,TSP_instance->rcnt,TSP_instance->nzcnt,TSP_instance->rhs,TSP_instance->sense,TSP_instance->rmatbeg,TSP_instance->rmatind,TSP_instance->rmatval,NULL,NULL);
		if(TSP_instance->status!=0)
		{
			printf("error in CPXaddrows\n");
			exit(-1);
		}

		free(TSP_instance->rmatbeg);
		free(TSP_instance->rmatval);
		free(TSP_instance->rmatind);
		free(TSP_instance->rhs);
		free(TSP_instance->sense);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//ONE INGOING ARC PER VERTEX
	for(int j=0; j<TSP_instance->n_cities; j++)
	{

		TSP_instance->rcnt=1;
		TSP_instance->nzcnt=TSP_instance->n_cities;
		TSP_instance->rhs=(double*) calloc(TSP_instance->rcnt,sizeof(double));
		TSP_instance->sense=(char*) calloc(TSP_instance->rcnt,sizeof(double));

		TSP_instance->rhs[0]=1.0;
		TSP_instance->sense[0]='E';


		TSP_instance->rmatbeg=(int*) calloc(TSP_instance->rcnt,sizeof(int));
		TSP_instance->rmatind=(int*) calloc(TSP_instance->nzcnt,sizeof(int));
		TSP_instance->rmatval=(double*) calloc(TSP_instance->nzcnt,sizeof(double));

		for(int k=0; k<TSP_instance->n_cities; k++)
		{
			TSP_instance->rmatval[k]=1.0;
			TSP_instance->rmatind[k]=position_x(TSP_instance,k,j);
		}

		TSP_instance->rmatbeg[0]=0;

		TSP_instance->status=CPXaddrows(TSP_instance->env,TSP_instance->lp,0,TSP_instance->rcnt,TSP_instance->nzcnt,TSP_instance->rhs,TSP_instance->sense,TSP_instance->rmatbeg,TSP_instance->rmatind,TSP_instance->rmatval,NULL,NULL);
		if(TSP_instance->status!=0)
		{
			printf("error in CPXaddrows\n");
			exit(-1);
		}

		free(TSP_instance->rmatbeg);
		free(TSP_instance->rmatval);
		free(TSP_instance->rmatind);
		free(TSP_instance->rhs);
		free(TSP_instance->sense);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////


	//ORDERING CONSTRAINTS

	if(TSP_instance->option==1)
	{
		for(int j=1; j<TSP_instance->n_cities; j++)
		{
			for(int k=1; k<TSP_instance->n_cities; k++)
			{

				if(j==k){continue;}

				TSP_instance->rcnt=1;
				TSP_instance->nzcnt=3;
				TSP_instance->rhs=(double*) calloc(TSP_instance->rcnt,sizeof(double));
				TSP_instance->sense=(char*) calloc(TSP_instance->rcnt,sizeof(double));

				TSP_instance->rhs[0]=TSP_instance->n_cities-2;
				TSP_instance->sense[0]='L';


				TSP_instance->rmatbeg=(int*) calloc(TSP_instance->rcnt,sizeof(int));
				TSP_instance->rmatind=(int*) calloc(TSP_instance->nzcnt,sizeof(int));
				TSP_instance->rmatval=(double*) calloc(TSP_instance->nzcnt,sizeof(double));

				TSP_instance->rmatval[0]=1.0;
				TSP_instance->rmatind[0]=position_u(TSP_instance,j);

				TSP_instance->rmatval[1]=-1.0;
				TSP_instance->rmatind[1]=position_u(TSP_instance,k);

				TSP_instance->rmatval[2]=TSP_instance->n_cities-1;
				TSP_instance->rmatind[2]=position_x(TSP_instance,j,k);


				TSP_instance->rmatbeg[0]=0;

				TSP_instance->status=CPXaddrows(TSP_instance->env,TSP_instance->lp,0,TSP_instance->rcnt,TSP_instance->nzcnt,TSP_instance->rhs,TSP_instance->sense,TSP_instance->rmatbeg,TSP_instance->rmatind,TSP_instance->rmatval,NULL,NULL);
				if(TSP_instance->status!=0)
				{
					printf("error in CPXaddrows\n");
					exit(-1);
				}

				free(TSP_instance->rmatbeg);
				free(TSP_instance->rmatval);
				free(TSP_instance->rmatind);
				free(TSP_instance->rhs);
				free(TSP_instance->sense);
			}
		}
	}
	if(TSP_instance->option==2)
	{
		for(int j=1; j<TSP_instance->n_cities; j++)
		{
			for(int k=1; k<TSP_instance->n_cities; k++)
			{

				if(j==k){continue;}

				TSP_instance->rcnt=1;
				TSP_instance->nzcnt=4;
				TSP_instance->rhs=(double*) calloc(TSP_instance->rcnt,sizeof(double));
				TSP_instance->sense=(char*) calloc(TSP_instance->rcnt,sizeof(double));

				TSP_instance->rhs[0]=TSP_instance->n_cities-2;
				TSP_instance->sense[0]='L';


				TSP_instance->rmatbeg=(int*) calloc(TSP_instance->rcnt,sizeof(int));
				TSP_instance->rmatind=(int*) calloc(TSP_instance->nzcnt,sizeof(int));
				TSP_instance->rmatval=(double*) calloc(TSP_instance->nzcnt,sizeof(double));

				TSP_instance->rmatval[0]=1.0;
				TSP_instance->rmatind[0]=position_u(TSP_instance,j);

				TSP_instance->rmatval[1]=-1.0;
				TSP_instance->rmatind[1]=position_u(TSP_instance,k);

				TSP_instance->rmatval[2]=TSP_instance->n_cities-1;
				TSP_instance->rmatind[2]=position_x(TSP_instance,j,k);

				TSP_instance->rmatval[3]=TSP_instance->n_cities-3;
				TSP_instance->rmatind[3]=position_x(TSP_instance,k,j);


				TSP_instance->rmatbeg[0]=0;

				TSP_instance->status=CPXaddrows(TSP_instance->env,TSP_instance->lp,0,TSP_instance->rcnt,TSP_instance->nzcnt,TSP_instance->rhs,TSP_instance->sense,TSP_instance->rmatbeg,TSP_instance->rmatind,TSP_instance->rmatval,NULL,NULL);
				if(TSP_instance->status!=0)
				{
					printf("error in CPXaddrows\n");
					exit(-1);
				}

				free(TSP_instance->rmatbeg);
				free(TSP_instance->rmatval);
				free(TSP_instance->rmatind);
				free(TSP_instance->rhs);
				free(TSP_instance->sense);
			}
		}
	}



	/////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef write_lp
	// * writing the created ILP model on a file *
	TSP_instance->status=CPXwriteprob(TSP_instance->env,TSP_instance->lp,"MTZ.lp",NULL);
	if(TSP_instance->status!=0)
	{
		printf("error in CPXwriteprob\n");
		exit(-1);
	}
#endif
	/////////////////////////////////////////////////////////////////////////////////////////////////////////




}

/***********************************************************************************/
void MTZ_free_cplex(data *TSP_instance)
/***********************************************************************************/
{


	TSP_instance->status=CPXfreeprob(TSP_instance->env,&(TSP_instance->lp));
	if(TSP_instance->status!=0) {printf("error in CPXfreeprob\n");exit(-1);}

	TSP_instance->status=CPXcloseCPLEX(&(TSP_instance->env));
	if(TSP_instance->status!=0) {printf("error in CPXcloseCPLEX\n");exit(-1);}

}


/***********************************************************************************/
void MTZ_solve_LP(data *TSP_instance)
/***********************************************************************************/
{

	///////////////////////////////////////////////////////////////////////////////////
	/* linear programming relaxation*/

	CPXsetintparam (TSP_instance->env, CPX_PARAM_SCRIND, CPX_OFF);

	double solution_time_lp=0;

	TSP_instance->status = CPXchgprobtype (TSP_instance->env, TSP_instance->lp, CPXPROB_LP);

	clock_t time_start_lp=clock();
	TSP_instance->status=CPXlpopt(TSP_instance->env,TSP_instance->lp);
	if(TSP_instance->status!=0)
	{
		printf("err_FILEor in CPXlpopt slave solve\n");
		exit(-1);
	}

	clock_t time_end_lp=clock();

	solution_time_lp=(double)(time_end_lp-time_start_lp)/(double)CLOCKS_PER_SEC;



	TSP_instance->status=CPXgetobjval(TSP_instance->env,TSP_instance->lp,&TSP_instance->cplex_lp);
	if(TSP_instance->status!=0)
	{
		printf("error in CPXgetmipobjval\n");
		exit(-1);
	}

	printf("\n\nLP solution value ->\t\%f",TSP_instance->cplex_lp);

	TSP_instance->status=CPXgetmipx(TSP_instance->env,TSP_instance->lp,TSP_instance->x,0,TSP_instance->n_cities*TSP_instance->n_cities+TSP_instance->n_cities-1);
	if(TSP_instance->status!=0)
	{
		printf("error in CPXgetmipx\n");
		exit(-1);
	}

#ifdef print_solution_X
	cout << "\n\nVARIABLES X\n";
	for(int j=0; j<TSP_instance->n_cities; j++)
	{
		for(int k=0; k<TSP_instance->n_cities; k++)
		{
			printf("%.3f ",TSP_instance->x[position_x(TSP_instance,j,k)]);
			//			cout << TSP_instance->x[position_x(TSP_instance,j,k)] << "\t";
		}
		cout << endl;
	}
	cout << endl;
#endif

	cout << "\n\nVARIABLES U\n";
	for(int j=0; j<TSP_instance->n_cities; j++)
	{
		cout << TSP_instance->x[position_u(TSP_instance,j)] << "\t";
	}

}

/***********************************************************************************/
int MTZ_solve_cplex(data *TSP_instance)
/***********************************************************************************/
{



	CPXsetintparam (TSP_instance->env, CPX_PARAM_SCRIND, CPX_ON);

	TSP_instance->number_of_CPU=1;


	// * Set number of CPU*
	TSP_instance->status = CPXsetintparam (TSP_instance->env, CPX_PARAM_THREADS, TSP_instance->number_of_CPU);
	if (TSP_instance->status)
	{
		printf ("error for CPX_PARAM_EPRHS\n");
	}

	// * Set time limit *
	TSP_instance->status = CPXsetdblparam (TSP_instance->env, CPX_PARAM_TILIM,TSP_instance->timeLimit);
	if (TSP_instance->status)
	{
		printf ("error for CPX_PARAM_EPRHS\n");
	}

	///////////////////////////////
	SEP_load_cplex(TSP_instance);
	///////////////////////////////

	TSP_instance->x=(double*) calloc(TSP_instance->n_cities*TSP_instance->n_cities+TSP_instance->n_cities,sizeof(double));

	///////////////////////////////////////////////////////////////////////////////////
	// * solving the MIP model
	clock_t time_start=clock();

	if(TSP_instance->cut_integer==1 || TSP_instance->cut_fractional==1)
	{
		CPXsetintparam(TSP_instance->env, CPX_PARAM_MIPCBREDLP, CPX_OFF);// let MIP callbacks work on the original model
		CPXsetintparam(TSP_instance->env, CPX_PARAM_PRELINEAR, CPX_OFF); // assure linear mappings between the presolved and original models
		CPXsetintparam(TSP_instance->env, CPX_PARAM_REDUCE, CPX_PREREDUCE_PRIMALONLY);
	}

	if(TSP_instance->cut_integer==1 )
	{
		cout << "******INTEGER SEPARATION******\n\n";
		TSP_instance->status = CPXsetlazyconstraintcallbackfunc(TSP_instance->env,mycutcallback_MODEL,TSP_instance);
		if (TSP_instance->status)
		{
			printf ("error for CPXsetlazyconstraintcallbackfunc\n");
		}
	}

	if(TSP_instance->cut_fractional==1)
	{
		cout << "******FRACTIONAL SEPARATION******\n\n";
		TSP_instance->status = CPXsetusercutcallbackfunc(TSP_instance->env,myusercutcallback_MODEL,TSP_instance);
		if (TSP_instance->status)
		{
			printf ("error for CPXsetuserconstraintcallbackfunc\n");
		}
	}


	TSP_instance->status=CPXmipopt(TSP_instance->env,TSP_instance->lp);
	if(TSP_instance->status!=0)
	{
		printf("error in CPXmipopt\n");
		exit(-1);
	}

	clock_t time_end=clock();
	double solution_time=(double)(time_end-time_start)/(double)CLOCKS_PER_SEC;

	///////////////////////////////////////////////////////////////////////////////////


	// * getting the solution

	TSP_instance->status=CPXgetmipx(TSP_instance->env,TSP_instance->lp,TSP_instance->x,0,TSP_instance->n_cities*TSP_instance->n_cities+TSP_instance->n_cities-1);
	if(TSP_instance->status!=0)
	{
		printf("error in CPXgetmipx\n");
		exit(-1);
	}




#ifdef print_solution_X
	cout << "\n\nVARIABLES X\n";
	for(int j=0; j<TSP_instance->n_cities; j++)
	{
		for(int k=0; k<TSP_instance->n_cities; k++)
		{
			printf("%d ",(int)(TSP_instance->x[position_x(TSP_instance,j,k)]+0.5));
			//			cout << TSP_instance->x[position_x(TSP_instance,j,k)] << "\t";
		}
		cout << endl;
	}
	cout << endl;
#endif

	cout << "\n\nVARIABLES U\n";
	for(int j=0; j<TSP_instance->n_cities; j++)
	{
		cout << TSP_instance->x[position_u(TSP_instance,j)] << "\t";
	}

	TSP_instance->status=CPXgetmipobjval(TSP_instance->env,TSP_instance->lp,&(TSP_instance->objval));
	if(TSP_instance->status!=0)
	{
		printf("error in CPXgetmipobjval\n");
		exit(-1);
	}

	printf("\n\nMIP solution value ->\t\%f",TSP_instance->objval);
	printf("\n\nMIP solution time ->\t\%f",solution_time);


	TSP_instance->status=CPXgetbestobjval(TSP_instance->env,TSP_instance->lp,&(TSP_instance->bestobjval));
	if(TSP_instance->status!=0)
	{
		printf("error in CPXgetbestobjval\n");
		exit(-1);
	}

	TSP_instance->lpstat=CPXgetstat(TSP_instance->env,TSP_instance->lp);
	TSP_instance->nodecount = CPXgetnodecnt(TSP_instance->env, TSP_instance->lp);

	cout << "\n\ncut_1\t" << TSP_instance->cut_1 << endl;
	cout << "cut_2\t" << TSP_instance->cut_2 << endl;


	///////////////////////////////////////////////////////////////////////////////////

	TSP_instance->cur_numcols=CPXgetnumcols(TSP_instance->env,TSP_instance->lp);
	TSP_instance->cur_numrows=CPXgetnumrows(TSP_instance->env,TSP_instance->lp);

	cout << endl;
	printf("\nnumcols\t%d\n",TSP_instance->cur_numcols);
	printf("\nnumrows\t%d\n",TSP_instance->cur_numrows);


	ofstream compact_file;
	compact_file.open("info_MTZ_MODEL.txt", ios::app);
	compact_file << fixed
			<< TSP_instance->istname << "\t"
			<< TSP_instance->n_cities << "\t"

			<< TSP_instance->objval << "\t"
			<< TSP_instance->bestobjval << "\t"
			<< solution_time << "\t"

			<< TSP_instance->lpstat << "\t"
			<< TSP_instance->nodecount << "\t"

			<<  TSP_instance->cur_numcols<< "\t"
			<<  TSP_instance->cur_numrows<< "\t"

			<< endl;
	compact_file.close();



	/////////////////////////////////////////////////////////////////////////////////
#ifdef solve_lp
	MTZ_solve_LP(TSP_instance);
#endif
	/////////////////////////////////////////////////////////////////////////////////

	free(TSP_instance->x);

	///////////////////////////////7
	SEP_free_cplex(TSP_instance);
	///////////////////////////////7


	return TSP_instance->objval;

}

