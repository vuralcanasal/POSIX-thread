// Vuralcan Asal 1862531
/*	main function: line 293
	Part1: line 34 - 210
	You need one input.txt file which begins with m and then one row per line.
	The input.txt file must be created before running the code
	The code take the value of 'm' from the input.txt file and create the original
	matrix according to this file.
	Also, the code read the 'd' and 's' file from keyboard.
	The value of 'd' must be equal or smaller than the value  of 'm', and
	the value of 's' must be greater than zero '0'.
	The code starts with shifting rows.
	When the code completes the shifting row operation, shifting columns operation starts.
	The mutex lock is not necessary for part 1 because all threads is related with one (value row or column)
	
	Part2: line 210 - 292
	Since there is no monitor future in C, mutex lock and condition variables of pthread is used for Dining Philosophers.
	There are some important values for this part2.
	Philosophers: the value of 'd'/// it must be equal or greater than three '3' - the code read the value from keybord
	The forks in the box: the value of 'f' which calculated in the code like 'd/3'
	MAX_EAT: in line 224 //maximum number of that philosophers can eat !!To complete the program, all philosophers must eat MAX_EAT times
	SLEEP_TIME: in line 225 //maximum waiting time in second for philosopher's eating and sleeping
	In the solution, mutex lock is used at the critical position. 
	That means pthreads do operations in global common values with mutex lock.
	pthreads can communicate between eachthem by using their condition variables.	
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

int gl_d; // number of threads (common for part1 and part2)
/////////////////////////////////part 1 shifting START///////////////////////////////////////////////
// global variables
int gl_m; // matrix size m x m
int **gl_arr; //m x m matrix

void readInputFile(int *m){
	FILE *file;
	if((file = fopen("input.txt","r"))==NULL){
		printf("You do NOT have an INPUT file name is input.txt\n");
		printf("Please create 1 input file with the format:\n");
		printf("Name: input.txt \nThe file begins with m and then one row per line.\n");
		exit(1);
	}
	
	char word[5];
	bool first_line = true;
	int number;
	int i = 0, j = 0;
	while(fscanf(file, "%s", word)!=EOF){
		number = atoi(word);
		//create the m x m matrix
		if(first_line){
			*m = number;
			gl_arr = (int **)malloc(number * sizeof(int *));
			for(int i = 0; i < number; i++)
			{
				gl_arr[i] = (int *) malloc (number * sizeof(int));
			}
			first_line = false;
		}
		// initialize the matrix
		else{
			if(i<*m)
			{
				if(j<*m)
				{
					gl_arr[i][j] = number;
					j++;
					
				}
				else
				{
					j=0;
					i++;
					gl_arr[i][j] = number;
					j++;
				}
			}
		}
	}
	fclose(file);
}
// shifting for rows from left to right
void* rowShift(void* p)
{
	int offset = (intptr_t) p;
	int numberOFrow = gl_m / gl_d;
	
	int startP = offset * numberOFrow;
	int endP;
	if(offset == gl_d-1)
		endP = gl_m;
	else
		endP = startP + numberOFrow;
	
	while(startP < endP)
	{
		int temp = gl_arr[startP][gl_m-1];	
		for(int i = gl_m-1; i > 0; i--)
			gl_arr[startP][i] = gl_arr[startP][i-1];
		gl_arr[startP][0] = temp;
		startP++;
	}
	pthread_exit(0);
}
// shifting for cols from bottom to top
void* colShift(void* p)
{
	int offset = (intptr_t) p;
	int numberOFrow = gl_m / gl_d;
	
	int startP = offset * numberOFrow;
	int endP;
	if(offset == gl_d-1)
		endP = gl_m;
	else
		endP = startP + numberOFrow;
	
	while(startP < endP)
	{
		int temp = gl_arr[0][startP];	
		for(int i = 0; i < gl_m-1; i++)
			gl_arr[i][startP] = gl_arr[i+1][startP];
		gl_arr[gl_m-1][startP] = temp;
		startP++;
	}
	pthread_exit(0);
}
// part1 shifting main fuction
void shifting(){
	printf("\nWelcome to Part1 Shifting\n\n");
	pthread_t *threads = NULL;
	int s; // many times shift
	// read the input file to create the matrix and initialize the value of m
	readInputFile(&gl_m);
	
	// read d and s from keyboard
	do
	{
		printf("Enter value of d:");
		scanf("%d",&gl_d);
		printf("Enter value of s:");
		scanf("%d",&s);

		if(gl_d > gl_m){
			printf("The value of d must be equal or smaller than m!! d<=m\n");
			printf("Tips:!: m is %d\n",gl_m);
			continue;
		}
		if(s<1){
			printf("The value of d must be bigger than zero '0'!! s>0\n");
			continue;
		}
		
		break;
	}while(true);
	
	// create threads array
	if(threads == NULL)
		threads = (pthread_t *)malloc(gl_d * sizeof(pthread_t));
	
	printf("\nOriginal Matrix\n");
	for(int i = 0; i<gl_m;i++)
	{
		for(int j = 0; j<gl_m;j++)
			printf("%d ",gl_arr[i][j]);
		printf("\n");
	}
	printf("\n");
	
	while(s > 0)
	{
		// row shifting Create threads
		for(int i = 0; i < gl_d; i++)
		{
			pthread_create(&threads[i], NULL, rowShift, (void *)(intptr_t)i);
		}
		// row shifting Join threads
		for(int i = 0; i < gl_d; i++)
		{
			pthread_join(threads[i], NULL);
		}
		// col shifting Create threads
		for(int i = 0; i < gl_d; i++)
		{
			pthread_create(&threads[i], NULL, colShift, (void *)(intptr_t)i);
		}
		// row shifting Join threads
		for(int i = 0; i < gl_d; i++)
		{
			pthread_join(threads[i], NULL);
		}
		
		s--;
	}
	
	
	printf("\nShifted Matrix\n");
	for(int i = 0; i<gl_m;i++)
	{
		for(int j = 0; j<gl_m;j++)
			printf("%d ",gl_arr[i][j]);
		printf("\n");
	}
	printf("\n");
}
/////////////////////////////////part 1 shifting END///////////////////////////////////////////////
/////////////////////////////////part 2 shifting START///////////////////////////////////////////////
int gl_f; //number of forks in the box
enum states{ THINKING,
	HUNGRY,
    	EATING };
      	
struct philosopher{
	enum states state;
	int hungryTime;
	int numberOFeat;
	pthread_cond_t condVar;
};
struct philosopher *gl_ph = NULL;
#define MAX_EAT 50 //philosopher can eat max_eat times
#define SLEEP_TIME 5 //philosopher waiting time for eating and sleeping
pthread_mutex_t myMutex; //Mutex lock.

//display philosopher's ids and states
void displayPhSt(int i)
{
	if(gl_ph[i].state == 0)
		printf("Philosopher_%d state: THINKING\n", i);
	else if(gl_ph[i].state == 1)
		printf("Philosopher_%d state: HUNGRY\n", i);
	else if(gl_ph[i].state == 2)
		printf("Philosopher_%d state: EATING\n", i);
	else
		printf("Philosopher_%d state: Unknown State\n", i);
}
//time to eat and think
void sleepEatThink()
{
	int now = (int)time(NULL);
	int sleepTime = rand() % SLEEP_TIME + 1;
	while(time(NULL)!=now+sleepTime);
}
//Warning if there is 1 fork left at the box
void forkWarning(int f)
{
	if(f==1)
		printf("!!! There is only %d fork left at the box !!!\n",gl_f);
}
// find left Neighbor
int leftNeighbor(int i)
{
    return ((i + gl_d - 1) % gl_d);
}
// find right Neighbor
int rightNeighbor(int i)
{
    return ((i + 1) % gl_d);
}
// initialize philosophers and number of forks in the box
void initialization(int phsize)
{
	gl_d = phsize;
	// calculate f (number of fork in the box)
	gl_f = gl_d / 3;
	// create philosophers
	if(gl_ph == NULL)
		gl_ph = (struct philosopher *)malloc(gl_d * sizeof(struct philosopher));
	// set the initial values for philosopher
	for(int i = 0; i < gl_d; i++)
	{
		gl_ph[i].state = THINKING;
		gl_ph[i].hungryTime = 0;
		gl_ph[i].numberOFeat = 0;
		pthread_cond_init(&gl_ph[i].condVar, NULL);
	}
	
}
// dining philosopher algorithm's 3 main functions test / putdown / pickup
void test(int i)
{
	if((gl_ph[leftNeighbor(i)].state != EATING) && (gl_ph[rightNeighbor(i)].state != EATING) && (gl_ph[i].state == HUNGRY) && (gl_f > 0)){
		gl_f--; // dec fork in the box
		gl_ph[i].state = EATING;
		displayPhSt(i); // display philosopher and state
		forkWarning(gl_f);
		sleepEatThink(); // eating time
		gl_ph[i].numberOFeat++; // inc number of feat(meal/eating)
		pthread_cond_signal(&gl_ph[i].condVar);
	}
}
void* putdown(void* p)
{
	pthread_mutex_lock(&myMutex);
	int i = (intptr_t) p;
    	gl_ph[i].state = THINKING;
    	displayPhSt(i);// display philosopher and state
    	gl_f++;
    	forkWarning(gl_f);
    	pthread_mutex_unlock(&myMutex);
	
    	sleepEatThink(); //thinking time
    	test(leftNeighbor(i));
    	test(rightNeighbor(i));
//    	pthread_mutex_unlock(&myMutex);
}
void* pickup(void* p)
{
	int i = (intptr_t) p;
	
	while(gl_ph[i].numberOFeat < MAX_EAT){
		gl_ph[i].state = HUNGRY;
		displayPhSt(i); // display philosopher and state
		time_t startHungry = time(NULL); // take the start time of hungrary state
		
		pthread_mutex_lock(&myMutex);
		test(i);	
		while(gl_ph[i].state != EATING){
			pthread_cond_wait(&gl_ph[i].condVar, &myMutex);
		}
		pthread_mutex_unlock(&myMutex); 
		
		time_t endHungry = time(NULL);// take the end time of hungrary state
		// calculate hugary time
		gl_ph[i].hungryTime = gl_ph[i].hungryTime + (int)(endHungry - startHungry);
		
		putdown(p);
	}
}
// part2 dining main fuction
void dining()
{
	printf("\nWelcome to Part2 Dining\n\n");
	pthread_t *threads = NULL;
	srand(time(0));
	// read d keyboard
	do
	{
		printf("Enter value of d:");
		scanf("%d",&gl_d);

		if(gl_d < 3){
			printf("The value of d must be equal or smaller than 3!! \n");
			continue;
		}		
		break;
	}while(true);
	
	//create philosophers with their initial values
	initialization(gl_d);
	// create thread array
	if(threads == NULL)
		threads = (pthread_t *)malloc(gl_d * sizeof(pthread_t));	
	//Initialize the mutex lock.
        pthread_mutex_init(&myMutex, NULL);
	//printf for start stage
	printf("All philosophers was initialized with:\n");
	for(int i = 0; i < gl_d; i++)
		displayPhSt(i);
	//if there is only one fork at the begining state, inform it
	forkWarning(gl_f);
	printf("----------------------------------------------\n");
	//Create threads for the philosophers
	for(int i = 0; i < gl_d; i++)
	{
		pthread_create(&threads[i], NULL, pickup,  (void *)(intptr_t)i);
	}
	//Join threads
	for(int i = 0; i < gl_d; i++)
	{
		pthread_join(threads[i], NULL);
	}
	
	printf("----------------------------------------------\nThe philosophers dining is completed with:\n");
	// calculate avarage hungry time over all philosophers and find max hungry time and philosopher
	int sumHungry = 0;
	int id = 0;
	int max = gl_ph[id].hungryTime;
	for(int i = 0; i < gl_d; i++){
		sumHungry = sumHungry + gl_ph[i].hungryTime;
		if(max < gl_ph[i].hungryTime){
			max = gl_ph[i].hungryTime;
			id = i;
		}
	}
	printf("Avarage Hungry Time: %.2f seconds\n",(float)sumHungry/(float)gl_d);
	printf("Philosopher-%d with the largest Hungry time %d seconds\n", id, max);
}
// the main function
void main(){
	int part;
	printf("To test part1, enter 1\nTo test part2, enter 2\nTo exit, enter any key\nEnter: ");
	scanf("%d",&part);
	
	if(part == 1)
		shifting();
	else if(part == 2)
		dining();	
	else
		printf("BYE BYE!!\n");	
	
}


