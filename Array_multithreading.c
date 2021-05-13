#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SIZE 300      // remain
//#define SIZE_ARR 10
#define CMP_LT(a, b) ((a) < (b))
//#define ROW 10
//#define LINE 30


float arr[SIZE] ={ 0 };
static int counter = 0;

int worker_cmp(const void *a, const void *b);
void initial_array(float arr[], int size);
void worker_main(int part, int total);
float* array_sort(float **local_arr, int total);
int run_parent(float **res, int numb_process, int total);
////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
	int i = 0;
	float **res = NULL;
	float *calculation_result = NULL;

	if (2 != argc)
	{
		printf("Error: invalid input param (expect 1 value)");
		return 1;
	}

	int total = atoi(argv[1]);

	if (0 >= total)
	{
		printf("Error: invalid values in params: total = %d\n", total);
		return 1;
	}

	res = (float**) malloc(total * sizeof(float*));
	if(NULL == res)
	{
		printf("Error: fail to allocate memory");
		return 1;
	}
	memset(res, NULL, total * sizeof(float*));

	initial_array(arr, SIZE);
	for (i = 0; i < total; i++)
	{
		if (0 == fork())
		{
			/* run child worker */
			worker_main(i, total);
			return 0;
		}
		else
		{
			/* run parent */
			printf("Im in parent\n");
			sleep(1);
		}
		run_parent(res, i, total);
	}
	counter++;
	printf("counter = %d\n", counter);
	//qsort(res,SIZE,sizeof(float),(int(*)(const void*, const void*)) cmp);
	calculation_result = array_sort(res,total);
	printf("*******************************************\n");
	for(int k = 0; k < SIZE; k += 10)
	{
		printf("[%d] " "%0.3f %0.3f %0.3f %0.3f %0.3f %0.3f %0.3f %0.3f %0.3f %0.3f\n", k, calculation_result[k],calculation_result[k+1],calculation_result[k+2],calculation_result[k+3],calculation_result[k+4],
				calculation_result[k+5],calculation_result[k+6],calculation_result[k+7],calculation_result[k+8],calculation_result[k+9]);
	}
	printf("\n");
	return EXIT_SUCCESS;
}
///////////////////////////////////////
int worker_cmp(const void *a, const void *b)
{
	return (*(float*) a > *(float*) b) ? 1 : -1;
}
////////////////////////////////////////////////
void initial_array(float arr[], int size)
{
#if 0
	int j = 0;
	for( j = 0; j < size; j++)
	{
		arr[j] = (float)(rand() % 1000) / 1000;
		printf("%6.3f ",arr[j]);
	}
#else
	float local_arr[SIZE] =
	{ 0.383, 0.886, 0.777, 0.915, 0.793, 0.335, 0.386, 0.492, 0.649, 0.421,
			0.362, 0.027, 0.690, 0.059, 0.763, 0.926, 0.540, 0.426, 0.172,
			0.736, 0.211, 0.368, 0.567, 0.429, 0.782, 0.530, 0.862, 0.123,
			0.067, 0.135, 0.929, 0.802, 0.022, 0.058, 0.069, 0.167, 0.393,
			0.456, 0.011, 0.042, 0.229, 0.373, 0.421, 0.919, 0.784, 0.537,
			0.198, 0.324, 0.315, 0.370, 0.413, 0.526, 0.091, 0.980, 0.956,
			0.873, 0.862, 0.170, 0.996, 0.281, 0.305, 0.925, 0.084, 0.327,
			0.336, 0.505, 0.846, 0.729, 0.313, 0.857, 0.124, 0.895, 0.582,
			0.545, 0.814, 0.367, 0.434, 0.364, 0.043, 0.750, 0.087, 0.808,
			0.276, 0.178, 0.788, 0.584, 0.403, 0.651, 0.754, 0.399, 0.932,
			0.060, 0.676, 0.368, 0.739, 0.012, 0.226, 0.586, 0.094, 0.539,
			0.795, 0.570, 0.434, 0.378, 0.467, 0.601, 0.097, 0.902, 0.317,
			0.492, 0.652, 0.756, 0.301, 0.280, 0.286, 0.441, 0.865, 0.689,
			0.444, 0.619, 0.440, 0.729, 0.031, 0.117, 0.097, 0.771, 0.481,
			0.675, 0.709, 0.927, 0.567, 0.856, 0.497, 0.353, 0.586, 0.965,
			0.306, 0.683, 0.219, 0.624, 0.528, 0.871, 0.732, 0.829, 0.503,
			0.019, 0.270, 0.368, 0.708, 0.715, 0.340, 0.149, 0.796, 0.723,
			0.618, 0.245, 0.846, 0.451, 0.921, 0.555, 0.379, 0.488, 0.764,
			0.228, 0.841, 0.350, 0.193, 0.500, 0.034, 0.764, 0.124, 0.914,
			0.987, 0.856, 0.743, 0.491, 0.227, 0.365, 0.859, 0.936, 0.432,
			0.551, 0.437, 0.228, 0.275, 0.407, 0.474, 0.121, 0.858, 0.395,
			0.029, 0.237, 0.235, 0.793, 0.818, 0.428, 0.143, 0.011, 0.928,
			0.529, 0.776, 0.404, 0.443, 0.763, 0.613, 0.538, 0.606, 0.840,
			0.904, 0.818, 0.128, 0.688, 0.369, 0.917, 0.917, 0.996, 0.324,
			0.743, 0.470, 0.183, 0.490, 0.499, 0.772, 0.725, 0.644, 0.590,
			0.505, 0.139, 0.954, 0.786, 0.669, 0.082, 0.542, 0.464, 0.197,
			0.507, 0.355, 0.804, 0.348, 0.611, 0.622, 0.828, 0.299, 0.343,
			0.746, 0.568, 0.340, 0.422, 0.311, 0.810, 0.605, 0.801, 0.661,
			0.730, 0.878, 0.305, 0.320, 0.736, 0.444, 0.626, 0.522, 0.465,
			0.708, 0.416, 0.282, 0.258, 0.924, 0.637, 0.062, 0.624, 0.600,
			0.036, 0.452, 0.899, 0.379, 0.550, 0.468, 0.071, 0.973, 0.131,
			0.881, 0.930, 0.933, 0.894, 0.660, 0.163, 0.199, 0.981, 0.899,
			0.996, 0.959, 0.773, 0.813, 0.668, 0.190, 0.095, 0.926, 0.466,
			0.084, 0.340 };
	memcpy(arr, local_arr, sizeof(float) * size);
#endif
}
/////////////////////////////////////////////////////////////////////////////
void worker_main(int part, int total)
{
	char name[20] = {'\0'};
	int shmid = 0;
	float *shm_array = NULL;

	printf("part = %d total thread = %d\n", part, total);
	qsort(&arr[SIZE / total * part], SIZE / total, sizeof(float), worker_cmp);

	// writer
	sprintf(name, "%s%d", "shmfile", part);
	key_t key = ftok(name, 65);
	// shmget returns an identifier in shmid
	shmid = shmget(key, (SIZE/total) * sizeof(float), 0666 | IPC_CREAT);

	// shmat to attach to shared memory
	shm_array = (float*) shmat(shmid, (void*) 0, 0);
	if(NULL == shm_array)
	{
		printf("function shmat() return error\n");
	}
	memcpy( shm_array, &arr[SIZE / total * part], (SIZE/total) * sizeof(float));
	//detach from shared memory
	shmdt(shm_array);
}
///////////////////////////////////////////////

float* array_sort(float **local_arr, int total)
{
		int *index =(int*)malloc(total*sizeof(int));
		memset(index,0,total*sizeof(int));
		int i = 0;
		int j = 0;
		float min = 0;
		int min_index = 0;
		float *arr_result = (float*)malloc(SIZE* sizeof(float));

		for (j = 0; j < SIZE ; j++)
		{


			for(i = 0; i < total; i++)
			{
				if (index[i] < SIZE/10)
				{
					min_index = i;
					min = *(local_arr[i] + index[i]);
					i++;
					break;
				}
			}

			for(; i < total; i++)
			{
				if (index[i] < SIZE/10)
				{
					if(min > *(local_arr[i] + index[i]))
					{
						min = *(local_arr[i] + index[i]);
						min_index = i;
					}
				}
			}
			arr_result[j] = min;
			//printf("min = %f\n", min);
			if (index[min_index] < SIZE/10)
			{
				index[min_index]++;
			}
		}
		return arr_result;
}
///////////////////////////////////////////////
int run_parent(float **res, int numb_process, int total)
{
		char name[20] ={ '\0' };
		int shmid = 0;
		float *arr_res = NULL;

		res[numb_process] = (float*)malloc(SIZE / total * sizeof(float));
		sprintf(name, "%s%d", "shmfile", numb_process);
		key_t key = ftok(name, 65);
		// shmget returns an identifier in shmid
		shmid = shmget(key, (SIZE / total) * sizeof(float), 0666 | IPC_CREAT);
		if (-1 == shmid)
		{
			printf("Error shmget()\n");
			return 1;
		}
		// shmat to attach to shared memory
		arr_res = (float*) shmat(shmid, (void*) 0, 0);

		memcpy(res[numb_process], arr_res, (SIZE / total) * sizeof(float));
		//detach from shared memory
		shmdt(arr_res);
		// destroy the shared memory
		shmctl(shmid, IPC_RMID, NULL);
		return 0;
}
