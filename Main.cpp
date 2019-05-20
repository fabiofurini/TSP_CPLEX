/*
 *		Main.cpp
 *		Created on: 01/05/2019
 *		Author: Fabio Furini
 */



#include "global_functions.h"
#include "global_variables.h"
#include "MTZ_FORM.h"


/*******************************************************************************************************/
int main(int argc, char** argv)
/*******************************************************************************************************/
{


	data TSP_instance;


	TSP_instance.istname=new char[2000];
	if (argc == 8)
	{

		TSP_instance.algo=atoi(argv[1]);
		TSP_instance.option=atoi(argv[2]);

		strcpy(TSP_instance.istname, argv[3]);
		TSP_instance.timeLimit=atof(argv[4]);
		TSP_instance.n_input=atoi(argv[5]);

		TSP_instance.cut_integer=atoi(argv[6]);
		TSP_instance.cut_fractional=atoi(argv[7]);

	}
	else
	{
		printf("ERROR INPUT:\n");
		exit(2);
	}

	cout <<         "---------------------------------------------" << endl ;
	cout << "Instance\t->" << TSP_instance.istname << endl;
	cout <<         "---------------------------------------------" << endl ;
	cout << "Time limit\t->" << TSP_instance.timeLimit << endl;


	///////////////////////////////////////////////////////////////////////////////////
	if(!strcmp(argv[3],"NULL"))
	{
		cout << "\n\nRANDOM INSTANCE\n\n";

		TSP_instance_random(&TSP_instance);
	}
	else
	{

		TSP_instance_read(&TSP_instance);
	}
	///////////////////////////////////////////////////////////////////////////////////



	if(TSP_instance.algo==1)
	{

		cout << "\n\n*****MTZ FOTMULATION\n\n";

		///////////////////////////////////////////////////////////////////////////////////
		MTZ_load_cplex(&TSP_instance);
		printf("\n-> model loaded!\n");
		///////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////////
		MTZ_solve_cplex(&TSP_instance);
		printf("\n-> model solved!\n");
		///////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////////
		MTZ_free_cplex(&TSP_instance);
		printf("\n-> model freed!\n");
		///////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////////
		TSP_instance_free(&TSP_instance);
		///////////////////////////////////////////////////////////////////////////////////
	}


	printf("\nDONE!\n");

	return 1;
}

