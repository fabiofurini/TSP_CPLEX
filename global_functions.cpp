

#include "global_functions.h"

//#define print_DISTANCES


// return a integer random value in range min-max
/***********************************************************************************/
int randomBETWEEN(int min,int max)
/***********************************************************************************/
{
	return rand() % (max - min +1) +min;
}


/***********************************************************************************/
void TSP_instance_free(data *TSP_instance)
/***********************************************************************************/
{

	for (int i = 0; i < TSP_instance->n_cities; i++)
	{
		delete []TSP_instance->DISTANCE_MATRIX[i];
	}

	delete []TSP_instance->DISTANCE_MATRIX;

	delete [] TSP_instance->istname;
}


/***********************************************************************************/
void TSP_instance_read(data *TSP_instance)
/***********************************************************************************/
{

	ifstream in(TSP_instance->istname);

	if(in.fail())
	{
		cout << "Instance reading ERROR" << endl ;
		exit(1);
	}

	in >> TSP_instance->n_cities;

	TSP_instance->DISTANCE_MATRIX=new int*[TSP_instance->n_cities];

	for (int i = 0; i < TSP_instance->n_cities; i++)
	{
		TSP_instance->DISTANCE_MATRIX[i]=new int[TSP_instance->n_cities];
	}

	for (int i = 0; i < TSP_instance->n_cities; i++)
	{

		for (int j = 0; j < TSP_instance->n_cities; j++)
		{
			in >> TSP_instance->DISTANCE_MATRIX[i][j];
		}
	}

	in.close();

#ifdef print_DISTANCES

	for (int i = 0; i < TSP_instance->n_cities; i++)
	{

		for (int j = 0; j < TSP_instance->n_cities; j++)
		{
			cout << TSP_instance->DISTANCE_MATRIX[i][j] << "\t";
		}
		cout << endl;
	}

#endif

}

/***********************************************************************************/
void TSP_instance_random(data *TSP_instance)
/***********************************************************************************/
{


	TSP_instance->n_cities=TSP_instance->n_input;

	TSP_instance->DISTANCE_MATRIX=new int*[TSP_instance->n_cities];

	for (int i = 0; i < TSP_instance->n_cities; i++)
	{
		TSP_instance->DISTANCE_MATRIX[i]=new int[TSP_instance->n_cities];
	}

	for (int i = 0; i < TSP_instance->n_cities; i++)
	{

		for (int j = 0; j < TSP_instance->n_cities; j++)
		{
			TSP_instance->DISTANCE_MATRIX[i][j]=randomBETWEEN(10,100);

			if(i==j)
			{
				TSP_instance->DISTANCE_MATRIX[i][j]=10e6;
			}
		}
	}

#ifdef print_DISTANCES

	for (int i = 0; i < TSP_instance->n_cities; i++)
	{

		for (int j = 0; j < TSP_instance->n_cities; j++)
		{
			cout << TSP_instance->DISTANCE_MATRIX[i][j] << "\t";
		}
		cout << endl;
	}

#endif

}


