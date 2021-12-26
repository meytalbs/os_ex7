/*
==============================================================================
Written by : Meytal Abrahamian  login : meytalben  id : 211369939
Tomer Akrish               tomerak         315224295
==============================================================================
This program use threads to create array of prime with ARR_SIZE cell.
Its have a main thread that define the arrayand create MANU_NUM threads
that each one of them create a prime numberand add it to array.

The first cell of array used for lock cell which means that if it holds 0
its lock and if it holds 1 its open.that way we make sure that the threads
will not insert data to array together.
*/

// --------------------------------includes------------------------------------
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// ----------------------------defines & globals-------------------------------
#define ARR_SIZE 5000			// for prime array size
#define MANU_NUM 3				// for num of manufacturers
#define HI_RAND_LIM 999			// for hi limit for rand
#define LOW_RAND_LIM 2			// for low limit for rand
#define CLOSE_CELL 0			// to lock first cell of prime array
#define OPEN_CELL 1				// to unlock first cell of prime array
#define SRAND_NUM 17			// for number in srand
#define MIN_PRIME 2				// for min prime 
#define MAX_PRIME 997			// for max prime

int prime_arr[ARR_SIZE];		// the prime array
int next_index = 0;					// for next free index in prime array

// ----------------------------------------------------------------------------
void array_owner();
void* create_primes(void*);
int get_prime();
int is_prime(int number);
void print_info();
void map_prime_arr(int* new_primes, int* min_prime, int* max_prime);
int count_frequency(int num);
int is_in_arr(int arr[], int num);
// ----------------------------------------------------------------------------

// ---------------------------------main---------------------------------------
int main()
{
	array_owner();

	return(EXIT_SUCCESS);
}
// ----------------------------------------------------------------------------

// array owner
void array_owner()
{
	pthread_t thread_data[MANU_NUM];		// for manufacturers
	int status;								// for status
	int i;									// for loop

	srand(SRAND_NUM);

	// initialize prime array to 0
	for (i = 0; i < ARR_SIZE; ++i)
		prime_arr[i] = 0;

	// create threads to manufacturers
	for (i = 0; i < MANU_NUM; ++i)
	{
		status = pthread_create(&thread_data[i], NULL,
			create_primes, NULL);

		if (status != 0)
		{
			fputs("pthread_create failed in main", stderr);
			exit(EXIT_FAILURE);
		}
	}

	// open lock cell
	++next_index;
	prime_arr[0] = OPEN_CELL;

	// wait for all manufacturers threads
	for (i = 0; i < MANU_NUM; ++i)
		pthread_join(thread_data[i], NULL);

	// print info of prime array
	print_info();

	exit(EXIT_SUCCESS);
}
// ----------------------------------------------------------------------------

// manufacturers 
void* create_primes(void* args)
{
	int num,						// for prime num to unsert
		new_primes = 0,				// for new primes from curr manufacturer
		max_prime_freq = 0,			// for max prime frequency 
		max_freq = 0,				// for max frequency 
		freq;						// for curr frequency 

	// main loop
	while (next_index < ARR_SIZE)
	{
		// get prime
		num = get_prime();
		if (num == 0)
		{
			printf("num; %d\n", num);
		}
		// while first cell is lock
		while (prime_arr[0] == CLOSE_CELL);

		// close locke cell insert num to array
		prime_arr[0] = CLOSE_CELL;
		prime_arr[next_index] = num;

		// chack info about num
		freq = count_frequency(num);
		
		if (freq == 0)
			++new_primes;

		if (freq > max_freq)
		{
			max_freq = freq;
			max_prime_freq = num;
		}

		// increase index and open lock cell
		++next_index;
		prime_arr[0] = OPEN_CELL;
		
	}

	// print info about curr manufacturer
	printf("The amount of new primes I added to the array: %d\n", new_primes);
	printf("The number with the highest frequency is: %d\n\n", max_prime_freq);
	prime_arr[0] = OPEN_CELL;

	pthread_exit(NULL);
}
// ----------------------------------------------------------------------------

// find prime and returns it
int get_prime()
{
	int num;		// for prime num

	while (!is_prime(num = rand() % HI_RAND_LIM + LOW_RAND_LIM)) {}

	return num;
}
// ----------------------------------------------------------------------------

// gets number and return 1 if its a prime number and 0 if not
int is_prime(int number)
{
	int i;		    // for loop

	if (number < MIN_PRIME)
		return 0;

	for (i = MIN_PRIME; i * i < number; ++i)
	{
		if (number % i == 0)
			return 0;
	}

	return 1;
}
// ----------------------------------------------------------------------------

// print array prime info
void print_info()
{
	int new_primes = 0,				// for new primes
		min_prime = MAX_PRIME,		// for min prime
		max_prime = MIN_PRIME;		// for max prime

	map_prime_arr(&new_primes, &min_prime, &max_prime);

	// print info
	printf("There is %d new primes in array\n", new_primes);
	printf("Min prime in array: %d\n", min_prime);
	printf("Max prime in array: %d\n", max_prime);
}
// ----------------------------------------------------------------------------

// map array prime and update value of: new_prines, min_prime and max_prime
void map_prime_arr(int* new_primes, int* min_prime, int* max_prime)							
{
	int i,				// for loop
		arr[ARR_SIZE];	// helping arr

	for (i = 0; i < ARR_SIZE; ++i)
		arr[i] = 0;

	for (i = 1; i < ARR_SIZE; ++i)
	{
		if (!is_in_arr(arr, prime_arr[i]))
			++(*new_primes);
		if (prime_arr[i] > (*max_prime))
			(*max_prime) = prime_arr[i];
		else if (prime_arr[i] < (*min_prime) && prime_arr[i] != 0)
			(*min_prime) = prime_arr[i];
	}
}
// ----------------------------------------------------------------------------

// returns if num is in arr
int is_in_arr(int arr[], int num)
{
	int i;		// for loop

	for (i = 0; i < ARR_SIZE && arr[i] != 0; ++i)
	{
		if (arr[i] == num)
			return 1;
	}

	arr[i] = num;
	return 0;
}
// ----------------------------------------------------------------------------

// counts frequency of num in prime array
int count_frequency(int num)
{
	int i,					// for loop
		counter = 0;		// to count frequency

	for (i = 1; i < next_index && i < ARR_SIZE; ++i)
		if (prime_arr[i] == num)
			++counter;

	return counter;
}
// ----------------------------------------------------------------------------
