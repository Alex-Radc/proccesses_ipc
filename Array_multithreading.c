#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SIZE 300
#define SIZE_ARR 10
#define CMP_LT(a, b) ((a) < (b))

float arr[SIZE] ={ 0 };
static int counter = 0;

float merg_arr1[SIZE_ARR] = { 0.111, 0.222, 0.333, 0.444, 0.555, 0.557, 0.657, 0.735, 0.834, 0.868 };
float merg_arr2[SIZE_ARR] = { 0.145, 0.168, 0.255, 0.279, 0.345, 0.407, 0.478, 0.563, 0.648, 0.789 };
float resmerg_arr[SIZE_ARR * 2] = { 0 };

int cmp(const void *a, const void *b);
void initial_array(float arr[], int size);
void func_merg(float merg_arr1[], float merg_arr2[]);
void worker(int part, int total);

int main(int argc, char **argv)
{
	int i = 0;
	int k = 0;
	float *res = NULL;

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
	initial_array(arr, SIZE);
	for (i = 0; i < total; i++)
	{
		if (0 == fork())
		{
			/* run child worker */
			worker(i, total);
			return 0;
		}
		else
		{
			/* run parent */
			printf("Im in parent\n");
			sleep(1);
			char name[20] = {'\0'};
			sprintf(name, "%s%d", "shmfile", i);
			key_t key = ftok(name, 65);
			// shmget returns an identifier in shmid
			int shmid = shmget(key, (SIZE/total) * sizeof(float), 0666 | IPC_CREAT);
			if(-1 == shmid)
			{
				printf("Error shmget()\n");
				return 1;
			}
			// shmat to attach to shared memory
			float *arr_res = (float*) shmat(shmid, (void*) 0, 0);
			// TODO memory alokated for result and add data to result

			//printf("%p\n", arr_res);
			//printf("%6.3f\n", *arr_res);

			if(NULL == res)
			{
				res = (float*)malloc(sizeof(float)*SIZE);
			}
			memcpy(&res[(SIZE/total)*i], arr_res, (SIZE/total) * sizeof(float));


			/*for (k = 0; k < SIZE/total; k += 10)
			{
				printf("{%2d} [%d] "
						"%6.3f %6.3f %6.3f %6.3f %6.3f "
						"%6.3f %6.3f %6.3f %6.3f %6.3f\n", i, k, arr_res[k],
						arr_res[k + 1], arr_res[k + 2], arr_res[k + 3], arr_res[k + 4],
						arr_res[k + 5], arr_res[k + 6], arr_res[k + 7], arr_res[k + 8],
						arr_res[k + 9]);
			}*/

/*
			for (k = (SIZE/total)*i; k < (SIZE/total)*(i+1); k += 10)
			{
				printf("{%2d} [%d] "
						"%6.3f %6.3f %6.3f %6.3f %6.3f "
						"%6.3f %6.3f %6.3f %6.3f %6.3f\n", i, k, res[k],
						res[k + 1], res[k + 2], res[k + 3], res[k + 4],
						res[k + 5], res[k + 6], res[k + 7], res[k + 8],
						res[k + 9]);
			}
*/
			//detach from shared memory
			shmdt(arr_res);

			// destroy the shared memory
			shmctl(shmid, IPC_RMID, NULL);
		}
	}

	counter++;
	printf("counter = %d\n", counter);
	printf("LLLLLLLLLLL\n");
	/*for (k = 0; k < SIZE; k += 10)
	{
		printf("[%d] "
				"%6.3f %6.3f %6.3f %6.3f %6.3f "
				"%6.3f %6.3f %6.3f %6.3f %6.3f\n", k, res[k],
				res[k + 1], res[k + 2], res[k + 3], res[k + 4],
				res[k + 5], res[k + 6], res[k + 7], res[k + 8],
				res[k + 9]);
	}
	printf("\n");
	*/
	qsort(res,SIZE,sizeof(float),(int(*)(const void*, const void*)) cmp);
	for (k = 0; k < SIZE; k += 10)
		{
			printf("[%d] "
					"%6.3f %6.3f %6.3f %6.3f %6.3f "
					"%6.3f %6.3f %6.3f %6.3f %6.3f\n", k, res[k],
					res[k + 1], res[k + 2], res[k + 3], res[k + 4],
					res[k + 5], res[k + 6], res[k + 7], res[k + 8],
					res[k + 9]);
		}
		printf("\n");
	return EXIT_SUCCESS;
}
///////////////////////////////////////
int cmp(const void *a, const void *b)
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
	float local_arr[300] =
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
void func_merg(float merg_arr1[], float merg_arr2[])
{
	int i, j, k;
	i = j = k = 0;
	while (i < SIZE_ARR && j < SIZE_ARR)
	{
		if (CMP_LT(merg_arr1[i], merg_arr2[j]))
		{
			resmerg_arr[k++] = merg_arr1[i++];
		}
		else
		{
			resmerg_arr[k++] = merg_arr2[j++];
		}
	}
	while (i < SIZE_ARR)
	{
		resmerg_arr[k++] = merg_arr1[i++];
	}
	while (j < SIZE_ARR)
	{
		resmerg_arr[k++] = merg_arr2[j++];
	}
}
///////////////////////////////////////////////////////////////
void worker(int part, int total)
{
	//char *word = "Hello word";
	//int i = 0;
	printf("part = %d total thread = %d\n", part, total);
	qsort(&arr[SIZE / total * part], SIZE / total, sizeof(float), cmp);
	/*
	for (i = SIZE / total * part; i < SIZE / total * part + SIZE / total; i +=10)
	{

		  printf("{%2d} [%d] "
				"%6.3f %6.3f %6.3f %6.3f %6.3f "
				"%6.3f %6.3f %6.3f %6.3f %6.3f\n", part, i, arr[i], arr[i + 1],
				arr[i + 2], arr[i + 3], arr[i + 4], arr[i + 5], arr[i + 6],
				arr[i + 7], arr[i + 8], arr[i + 9]);

	}
	*/
	// writer
	char name[20] = {'\0'};
	sprintf(name, "%s%d", "shmfile", part);
	key_t key = ftok(name, 65);
	// shmget returns an identifier in shmid
	int shmid = shmget(key, (SIZE/total) * sizeof(float), 0666 | IPC_CREAT);

	// shmat to attach to shared memory
	float *shm_array = (float*) shmat(shmid, (void*) 0, 0);
	memcpy( shm_array, &arr[SIZE / total * part], (SIZE/total) * sizeof(float));
/*
	for (int k = 0; k < SIZE/total; k += 10)
	{
		printf("{%2d} [%d] "
				"%6.3f %6.3f %6.3f %6.3f %6.3f "
				"%6.3f %6.3f %6.3f %6.3f %6.3f\n", i, k, shm_array[k],
				shm_array[k + 1], shm_array[k + 2], shm_array[k + 3], shm_array[k + 4],
				shm_array[k + 5], shm_array[k + 6], shm_array[k + 7], shm_array[k + 8],
				shm_array[k + 9]);
	}*/

	//detach from shared memory
	shmdt(shm_array);
}
