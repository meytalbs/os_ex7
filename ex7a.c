
// --------------------------------includes------------------------------------
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// ----------------------------defines & globals-------------------------------
#define ARR_SIZE 5000
#define MANU_NUM 3
#define HI_RAND_LIM 999
#define LOW_RAND_LIM 2
#define CLOSE_CELL 0
#define OPEN_CELL 1

int prime_arr[ARR_SIZE];
int index = 0;

// ----------------------------------------------------------------------------
void* create_primes(void*);
int get_prime();
int is_prime(int number);
void print_info();
void map_prime_arr(int* new_primes, int* min_prime, int* max_prime);
int is_in_prime_arr(int num);
int count_frequency(int num);
// ----------------------------------------------------------------------------

// ---------------------------------main---------------------------------------
int main() 
{
	pthread_t thread_data[MANU_NUM];
	int status;
	int i;

	srand(17);

	for (i = 0; i < ARR_SIZE; ++i)
		prime_arr[i] = 0;

	// create manu
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
	++index;
	prime_arr[0] = OPEN_CELL;

	// wait for all Threads
	for (i = 0; i < MANU_NUM; ++i) 
		pthread_join(thread_data[i], NULL);

	// print info
	print_info();
	
	return(EXIT_SUCCESS);
}
// ----------------------------------------------------------------------------

// manu func
void* create_primes(void* args)
{
	int num,
		new_primes = 0,
		max_prime_freq = 0,
		max_freq = 0,
		freq;
	
	// main loop
	while(index != ARR_SIZE) 
	{ 
		num = get_prime();

		while (prime_arr[0] == CLOSE_CELL) {}

		prime_arr[0] == CLOSE_CELL;
		prime_arr[index] = num;

		if (!is_in_prime_arr(num))
			++new_primes;

		freq = count_frequency(num);
		if (freq > max_freq)
		{
			max_freq = freq;
			max_prime_freq = num;
		}		

		++index;
		prime_arr[0] == OPEN_CELL;
	}

	printf("The amount of new primes I added to the array: %d\n", new_primes);
	printf("The number with the highest frequency is: %d\n", max_prime_freq);

	pthread_exit(NULL);
}
// ----------------------------------------------------------------------------

// this function get prime num
int get_prime()
{
	int num;		// for prime num

	while (!is_prime(num = rand() % HI_RAND_LIM + LOW_RAND_LIM)) {}

	return num;
}
// ----------------------------------------------------------------------------

// this function gets number and return 1 if its a prime number and 0 if not
int is_prime(int number)
{
	int i;		    // for loop

	if (number < 2)
		return 0;

	for (i = 2; i * i < number; ++i)
	{
		if (number % i == 0)
			return 0;
	}

	return 1;
}
// ----------------------------------------------------------------------------

void print_info()
{
	int new_primes = 0,
		min_prime = 1000,
		max_prime = 0;

	map_prime_arr(&new_primes, &min_prime, &max_prime); 

	// print info
	printf("There is %d new primes in array\n", new_primes);
	printf("Min prime in array: %d\n", min_prime);
	printf("Max prime in array: %d\n", max_prime);
}
// ----------------------------------------------------------------------------

void map_prime_arr(int *new_primes, int *min_prime, int *max_prime)
{
	int i;

	for (i = 0; i < ARR_SIZE; ++i)
	{
		if (!is_in_arr(prime_arr[i]))
		{
			++(*new_primes);
			if (prime_arr[i] > (*max_prime))
				(*max_prime) = prime_arr[i];
			else if (prime_arr[i] > (*min_prime))
				(*min_prime) = prime_arr[i];
		}
	}
}
// ----------------------------------------------------------------------------

int is_in_prime_arr(int num)
{
	int i;

	for (i = 0; i < index; ++i)
		if (prime_arr[i] == num)
			return 0;

	return 1;
}
// ----------------------------------------------------------------------------

int count_frequency(int num)
{
	int i,
		counter = 0;

	for (i = 0; i < index; ++i)
		if (prime_arr[i] == num)
			++counter;

	return counter;
}
// ----------------------------------------------------------------------------
