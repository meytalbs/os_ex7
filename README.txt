==============================================================================
Written by : Meytal Abrahamian & Tomer Akrish 
==============================================================================

Program file: ex6a1.c
===============================================================================
This program use threads to create array of prime with ARR_SIZE cell.
Its have a main thread that define the array and create MANU_NUM threads
that each one of them create a prime number and add it to array.

The first cell of array used for lock cell which means that if it holds 0 
its lock and if it holds 1 its open. that way we make sure that the threads
will not insert data to array together.

input:	no input
output:		
		(for each MANU_NUM threads - in our case 3 times):
		The amount of new primes I added to the array: NEW_PRIMES
		The number with the highest frequency is: MAX_APPEAR
	
		(for main thread):
		There is NEW-PRIMES new primes in array
		Min prime in array: MIN_PRIME
		Max prime in array: MAX_PRIME

compile: gcc -Wall ex7a.c -o ex7a -l pthread
run: ./ex7a

Run mode:
To purchase the lock the process performs 2 operations:
1. Check the value of the lock
2. Update its value
Therefore, there is a race mode where 2 processes have accepted that the lock
is open at the same time so both will lock it and change the array of the prime
numbers however the change of only one of them will absorb 
(and it will override the change made by the other)
