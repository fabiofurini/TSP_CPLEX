

#include "SEP.h"


/*****************************************************************/
int position_z(data *TSP_instance,int j,int k)
/*****************************************************************/
{
	return TSP_instance->n_cities*j+k;
}

/*****************************************************************/
int position_w(data *TSP_instance,int j)
/*****************************************************************/
{
	return TSP_instance->n_cities*TSP_instance->n_cities+j;
}

/***********************************************************************************/
void SEP_load_cplex(data *TSP_instance)
/***********************************************************************************/
{

	//PREPARE indices of the Z variables to be changed according to the current solution in the callback
	TSP_instance->indobj=(int*) calloc(TSP_instance->n_cities*TSP_instance->n_cities,sizeof(int));
	TSP_instance->valobj=(double*) calloc(TSP_instance->n_cities*TSP_instance->n_cities,sizeof(double));
	TSP_instance->x_SEP=(double*) calloc(TSP_instance->n_cities*TSP_instance->n_cities+TSP_instance->n_cities,sizeof(double));

	TSP_instance->cut_SEP_rmatind=(int*) calloc(TSP_instance->n_cities*TSP_instance->n_cities,sizeof(int));
	TSP_instance->cut_SEP_rmatval=(double*) calloc(TSP_instance->n_cities*TSP_instance->n_cities,sizeof(double));

	TSP_instance->cut_1=0;
	TSP_instance->cut_2=0;

	int dummy=0;
	for(int j=0; j<TSP_instance->n_cities; j++)
	{
		for(int k=0; k<TSP_instance->n_cities; k++)
		{
			TSP_instance->indobj[dummy]=position_z(TSP_instance,j,k);
			TSP_instance->valobj[dummy]=0.0;
			dummy++;
		}
	}


	//opening the environment
	TSP_instance->env_SEP=CPXopenCPLEX(&(TSP_instance->status));
	if(TSP_instance->status!=0)
	{
		printf("cannot open CPLEX environment\n");
		exit(-1);
	}

	//opening the pointer to the problem
	TSP_instance->lp_SEP=CPXcreateprob(TSP_instance->env_SEP,&(TSP_instance->status),"SEP");
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

			TSP_instance->obj[counter]=0.0;
			TSP_instance->lb[counter]=0.0;
			TSP_instance->ub[counter]=1.0;

			if(j==k){TSP_instance->ub[counter]=0.0;}

			TSP_instance->xctype[counter]='B';
			//cout << "POSITION Z\t" << position_z(TSP_instance,j,k) << endl;
			sprintf(TSP_instance->colname[counter], "z%d.%d",j+1,k+1);
			counter++;
		}
	}

	for(int j=0; j<TSP_instance->n_cities; j++)
	{

		TSP_instance->obj[counter]=1.0;

		TSP_instance->lb[counter]=0.0;
		TSP_instance->ub[counter]=1.0;

		TSP_instance->xctype[counter]='B';
		//cout << "POSITION W\t" << position_w(TSP_instance,j) << endl;
		sprintf(TSP_instance->colname[counter], "w%d",j+1);
		counter++;
	}

	TSP_instance->status=CPXnewcols(TSP_instance->env_SEP,TSP_instance->lp_SEP,TSP_instance->ccnt,TSP_instance->obj,TSP_instance->lb,TSP_instance->ub,TSP_instance->xctype,TSP_instance->colname);
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
	CPXchgobjsen(TSP_instance->env_SEP,TSP_instance->lp_SEP,CPX_MIN);


	//AT LEAST TWO VERTICES
	TSP_instance->rcnt=1;
	TSP_instance->nzcnt=TSP_instance->n_cities;
	TSP_instance->rhs=(double*) calloc(TSP_instance->rcnt,sizeof(double));
	TSP_instance->sense=(char*) calloc(TSP_instance->rcnt,sizeof(double));

	TSP_instance->rhs[0]=2.0;
	TSP_instance->sense[0]='G';


	TSP_instance->rmatbeg=(int*) calloc(TSP_instance->rcnt,sizeof(int));
	TSP_instance->rmatind=(int*) calloc(TSP_instance->nzcnt,sizeof(int));
	TSP_instance->rmatval=(double*) calloc(TSP_instance->nzcnt,sizeof(double));

	for(int k=0; k<TSP_instance->n_cities; k++)
	{
		TSP_instance->rmatval[k]=1.0;
		TSP_instance->rmatind[k]=position_w(TSP_instance,k);
	}

	TSP_instance->rmatbeg[0]=0;

	TSP_instance->status=CPXaddrows(TSP_instance->env_SEP,TSP_instance->lp_SEP,0,TSP_instance->rcnt,TSP_instance->nzcnt,TSP_instance->rhs,TSP_instance->sense,TSP_instance->rmatbeg,TSP_instance->rmatind,TSP_instance->rmatval,NULL,NULL);
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


	//AT LEAST TWO VERTICES
	TSP_instance->rcnt=1;
	TSP_instance->nzcnt=TSP_instance->n_cities;
	TSP_instance->rhs=(double*) calloc(TSP_instance->rcnt,sizeof(double));
	TSP_instance->sense=(char*) calloc(TSP_instance->rcnt,sizeof(double));

	TSP_instance->rhs[0]=TSP_instance->n_cities-2;
	TSP_instance->sense[0]='L';


	TSP_instance->rmatbeg=(int*) calloc(TSP_instance->rcnt,sizeof(int));
	TSP_instance->rmatind=(int*) calloc(TSP_instance->nzcnt,sizeof(int));
	TSP_instance->rmatval=(double*) calloc(TSP_instance->nzcnt,sizeof(double));

	for(int k=0; k<TSP_instance->n_cities; k++)
	{
		TSP_instance->rmatval[k]=1.0;
		TSP_instance->rmatind[k]=position_w(TSP_instance,k);
	}

	TSP_instance->rmatbeg[0]=0;

	TSP_instance->status=CPXaddrows(TSP_instance->env_SEP,TSP_instance->lp_SEP,0,TSP_instance->rcnt,TSP_instance->nzcnt,TSP_instance->rhs,TSP_instance->sense,TSP_instance->rmatbeg,TSP_instance->rmatind,TSP_instance->rmatval,NULL,NULL);
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


	//LINKING CONSTRAINTS
	for(int j=0; j<TSP_instance->n_cities; j++)
	{
		for(int k=0; k<TSP_instance->n_cities; k++)
		{

			if(j==k){continue;}

			TSP_instance->rcnt=1;
			TSP_instance->nzcnt=2;
			TSP_instance->rhs=(double*) calloc(TSP_instance->rcnt,sizeof(double));
			TSP_instance->sense=(char*) calloc(TSP_instance->rcnt,sizeof(double));

			TSP_instance->rhs[0]=0;
			TSP_instance->sense[0]='L';


			TSP_instance->rmatbeg=(int*) calloc(TSP_instance->rcnt,sizeof(int));
			TSP_instance->rmatind=(int*) calloc(TSP_instance->nzcnt,sizeof(int));
			TSP_instance->rmatval=(double*) calloc(TSP_instance->nzcnt,sizeof(double));


			TSP_instance->rmatval[0]=1.0;
			TSP_instance->rmatind[0]=position_z(TSP_instance,j,k);

			TSP_instance->rmatval[1]=-1.0;
			TSP_instance->rmatind[1]=position_w(TSP_instance,j);


			TSP_instance->rmatbeg[0]=0;

			TSP_instance->status=CPXaddrows(TSP_instance->env_SEP,TSP_instance->lp_SEP,0,TSP_instance->rcnt,TSP_instance->nzcnt,TSP_instance->rhs,TSP_instance->sense,TSP_instance->rmatbeg,TSP_instance->rmatind,TSP_instance->rmatval,NULL,NULL);
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


			TSP_instance->rcnt=1;
			TSP_instance->nzcnt=2;
			TSP_instance->rhs=(double*) calloc(TSP_instance->rcnt,sizeof(double));
			TSP_instance->sense=(char*) calloc(TSP_instance->rcnt,sizeof(double));

			TSP_instance->rhs[0]=0;
			TSP_instance->sense[0]='L';


			TSP_instance->rmatbeg=(int*) calloc(TSP_instance->rcnt,sizeof(int));
			TSP_instance->rmatind=(int*) calloc(TSP_instance->nzcnt,sizeof(int));
			TSP_instance->rmatval=(double*) calloc(TSP_instance->nzcnt,sizeof(double));


			TSP_instance->rmatval[0]=1.0;
			TSP_instance->rmatind[0]=position_z(TSP_instance,j,k);

			TSP_instance->rmatval[1]=-1.0;
			TSP_instance->rmatind[1]=position_w(TSP_instance,k);


			TSP_instance->rmatbeg[0]=0;

			TSP_instance->status=CPXaddrows(TSP_instance->env_SEP,TSP_instance->lp_SEP,0,TSP_instance->rcnt,TSP_instance->nzcnt,TSP_instance->rhs,TSP_instance->sense,TSP_instance->rmatbeg,TSP_instance->rmatind,TSP_instance->rmatval,NULL,NULL);
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


	//	for(int k=0; k<TSP_instance->n_cities; k++)
	//	{
	//
	//		TSP_instance->rcnt=1;
	//		TSP_instance->nzcnt=2;
	//		TSP_instance->rhs=(double*) calloc(TSP_instance->rcnt,sizeof(double));
	//		TSP_instance->sense=(char*) calloc(TSP_instance->rcnt,sizeof(double));
	//
	//		TSP_instance->rhs[0]=0;
	//		TSP_instance->sense[0]='L';
	//
	//
	//		TSP_instance->rmatbeg=(int*) calloc(TSP_instance->rcnt,sizeof(int));
	//		TSP_instance->rmatind=(int*) calloc(TSP_instance->nzcnt,sizeof(int));
	//		TSP_instance->rmatval=(double*) calloc(TSP_instance->nzcnt,sizeof(double));
	//
	//
	//		TSP_instance->rmatval[0]=1.0;
	//		TSP_instance->rmatind[0]=position_z(TSP_instance,k,k);
	//
	//		TSP_instance->rmatval[1]=-1.0;
	//		TSP_instance->rmatind[1]=position_w(TSP_instance,k);
	//
	//
	//		TSP_instance->rmatbeg[0]=0;
	//
	//		TSP_instance->status=CPXaddrows(TSP_instance->env_SEP,TSP_instance->lp_SEP,0,TSP_instance->rcnt,TSP_instance->nzcnt,TSP_instance->rhs,TSP_instance->sense,TSP_instance->rmatbeg,TSP_instance->rmatind,TSP_instance->rmatval,NULL,NULL);
	//		if(TSP_instance->status!=0)
	//		{
	//			printf("error in CPXaddrows\n");
	//			exit(-1);
	//		}
	//
	//		free(TSP_instance->rmatbeg);
	//		free(TSP_instance->rmatval);
	//		free(TSP_instance->rmatind);
	//		free(TSP_instance->rhs);
	//		free(TSP_instance->sense);
	//	}


	TSP_instance->status=CPXwriteprob(TSP_instance->env_SEP,TSP_instance->lp_SEP,"SEP.lp",NULL);
	if(TSP_instance->status!=0)
	{
		printf("error in CPXwriteprob\n");
		exit(-1);
	}

	//	cout << "ESCO FURTIVO\n";
	//	exit(-1);

}

/***********************************************************************************/
void SEP_set_objective_function_cplex(data *TSP_instance)
/***********************************************************************************/
{

	int dummy=0;
	for(int j=0; j<TSP_instance->n_cities; j++)
	{
		for(int k=0; k<TSP_instance->n_cities; k++)
		{
			TSP_instance->valobj[dummy] = -(TSP_instance->x[dummy]);
			dummy++;
		}
	}

	TSP_instance->status = CPXchgobj(TSP_instance->env_SEP, TSP_instance->lp_SEP,TSP_instance->n_cities*TSP_instance->n_cities, TSP_instance->indobj, TSP_instance->valobj);
	if (TSP_instance->status != 0)
	{
		printf("error in CPXchgobj\n");
		exit(-1);
	}

//	TSP_instance->status=CPXwriteprob(TSP_instance->env_SEP,TSP_instance->lp_SEP,"SEP_obj.lp",NULL);
//	if(TSP_instance->status!=0)
//	{
//		printf("error in CPXwriteprob\n");
//		exit(-1);
//	}
//	cin.get();

}

/***********************************************************************************/
void SEP_free_cplex(data *TSP_instance)
/***********************************************************************************/
{
	TSP_instance->status=CPXfreeprob(TSP_instance->env_SEP,&(TSP_instance->lp_SEP));
	if(TSP_instance->status!=0) {printf("error in CPXfreeprob\n");exit(-1);}

	TSP_instance->status=CPXcloseCPLEX(&(TSP_instance->env_SEP));
	if(TSP_instance->status!=0) {printf("error in CPXcloseCPLEX\n");exit(-1);}

	free(TSP_instance->indobj);
	free(TSP_instance->valobj);
	free(TSP_instance->x_SEP);

	free(TSP_instance->cut_SEP_rmatind);
	free(TSP_instance->cut_SEP_rmatval);

}

/***********************************************************************************/
double  SEP_solve_cplex(data *TSP_instance)
/***********************************************************************************/
{


	TSP_instance->status=CPXmipopt(TSP_instance->env_SEP,TSP_instance->lp_SEP);
	if(TSP_instance->status!=0)
	{
		printf("error in CPXmipopt\n");
		exit(-1);
	}

	TSP_instance->status=CPXgetmipobjval(TSP_instance->env_SEP,TSP_instance->lp_SEP,&(TSP_instance->objval_SEP));
	if(TSP_instance->status!=0)
	{
		printf("error in CPXgetmipobjval\n");
		exit(-1);
	}

	//	printf("\n\nSEP solution value ->\t\%f",TSP_instance->objval_SEP);

	TSP_instance->status=CPXgetmipx(TSP_instance->env_SEP,TSP_instance->lp_SEP,TSP_instance->x_SEP,0,TSP_instance->n_cities*TSP_instance->n_cities+TSP_instance->n_cities-1);
	if(TSP_instance->status!=0)
	{
		printf("error in CPXgetmipx\n");
		exit(-1);
	}

//	cout << "\n\nVARIABLES Z\n";
//	for(int j=0; j<TSP_instance->n_cities; j++)
//	{
//		for(int k=0; k<TSP_instance->n_cities; k++)
//		{
//			printf("%.3f ",TSP_instance->x_SEP[position_z(TSP_instance,j,k)]);
//		}
//		cout << endl;
//	}
//	cout << endl;
//
//	cout << "\n\nVARIABLES W\n";
//	for(int j=0; j<TSP_instance->n_cities; j++)
//	{
//		cout << TSP_instance->x_SEP[position_w(TSP_instance,j)] << "\t";
//	}
//	cin.get();

	TSP_instance->lpstat=CPXgetstat(TSP_instance->env_SEP,TSP_instance->lp_SEP);
//	cout << "\nlpstat\t" << TSP_instance->lpstat << endl;

	if(TSP_instance->lpstat!=101 && TSP_instance->lpstat!=102 )
	{
		cout << "ERROR in SEPARATION PROBLEM\n";
		exit(-1);
	}

	return TSP_instance->objval_SEP;
}

