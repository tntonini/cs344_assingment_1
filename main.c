/***************
 ** Thomas Tonini
 ** CS 344
 ** Assingment #1 Movies
 ** October 10, 2020
 ******************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct languange
{
	char *lang;
	struct language *next;
};

struct movie
{
	char *title;
	int year;
	// struct language *lang;
	char *lang;
	int rating;
	struct movie *next;
};

void bubbleSort(struct movie *start); 
void swap(struct movie *a, struct movie *b); 



/* Parse the current line which is space delimited and create a
 *  movie struct with the data in this line
 */
struct movie *createMovie(char *currLine)
{
	struct movie *currMovie = malloc(sizeof(struct movie));

	// For use with strtok_r
	char *saveptr;

	// The first token is the title
	char *token = strtok_r(currLine, ",", &saveptr);
	currMovie->title = calloc(strlen(token) + 1, sizeof(char));
	strcpy(currMovie->title, token);

	// The next token is the year
	token = strtok_r(NULL, ",", &saveptr);
	int yr = atoi(token);
	currMovie->year = yr; // calloc(strlen(token) + 1, sizeof(char));
	//strcpy(currMovie->year, token);

	// The next token is the lang
	token = strtok_r(NULL, ",", &saveptr);
	currMovie->lang = calloc(strlen(token) + 1, sizeof(char));
	strcpy(currMovie->lang, token);

	// The next token is the rating
	token = strtok_r(NULL, "\n", &saveptr);
	int rate = atoi(token);
	currMovie->rating = rate; //= calloc(strlen(token) + 1, sizeof(char));

	// Set the next node to NULL in the newly created Movie entry
	currMovie->next = NULL;

	return currMovie;
}

struct movie *processFile(char *filePath)
{
	//Open the file and read it
	FILE *movieFile = fopen(filePath, "r");

	if (movieFile == NULL)
	{
		printf("Failed to open %d\n", filePath);
		exit(0);
	}

	char *currLine = NULL;
	size_t len = 0;
	ssize_t nread;
	char *token;

	// The head of the linked list
	struct movie *head = NULL;
	// The tail of the linked list
	struct movie *tail = NULL;

	// Read the file line by line
	while ((nread = getline(&currLine, &len, movieFile)) != -1)
	{
		// Get a new movie node corresponding to the current line
		struct movie *newNode = createMovie(currLine);

		// Is this the first node in the linked list?
		if (head == NULL)
		{
			// This is the first node in the linked link
			// Set the head and the tail to this node
			head = newNode;
			tail = newNode;
		}
		else
		{
			// This is not the first node.
			// Add this node to the list and advance the tail
			tail->next = newNode;
			tail = newNode;
		}
	}
	free(currLine);
	fclose(movieFile);  

	//These next three lines remove the current head of the linked list
	//this is currently being held by the 1st row information that we dont want
	struct movie *temp = head; 
	head = head->next; 
	free(temp); 


	return head;
}



void printMovie(struct movie *aMovie)
{
	printf("%s, %d %s, %d\n", aMovie->title,
			aMovie->year,
			aMovie->lang,
			aMovie->rating);
}
/*
 * Print the linked list of Movies
 */
void printMovieList(struct movie *list)
{
	while (list != NULL)
	{
		printMovie(list);
		list = list->next;
	}
}

//Printing the sucessful execution of reading in the file and printing how many items there are
void printSucess(struct movie *list, char *file)
{
	//we start at -1 to take into account for the first line that is just  title, year, language etc..
	int i = -1;
	while (list != NULL)
	{
		i++;
		list = list->next;
	}
	printf("Processed file %s and parsed data for %d movies\n", file, i);
}

//runYear takes in the linked list of movies, prompts the user about year, then prints all the movies from that year
void runYear(struct movie *list)
{
	printf("\nEnter the year for which you want to see movies: ");
	int input;
	scanf("%d", &input);
	int yearExists = 0; //counter to see if we have information on any movies from that year
	while (list != NULL)
	{
		/****
		 * Check to see if the input year is actually in the list]
		 * ***/
		if (list->year == input)
		{
			printf("%s\n", list->title);
			yearExists++;
		}
		list = list->next;
	}

	/**
	 * If the year input is not int the list then print this
	 * */
	if (yearExists == 0)
	{
		printf("No data about movies released in the year %d\n", input);
	}
}

void runHighRate(struct movie *list)
{
	//returns a sorted linked list of movies by year
	bubbleSort(list);

	//struct to print the top movies for a given year
	struct printList{
		int year;
		char *name;
		int rating;

	};
	struct printList print[50];

	int currentYear = 0, previousYear = 0, i=0, year = 0, numMovies;
	
	//loop through the entire linked list
	while(list != NULL){
		currentYear = list->year;
		/*********
		 * If were still at the same year then we know we have to compare
		 * the ratings against each other
		 * *******/
		if(currentYear == previousYear){
			if(list->rating > print[i].rating){
				year++;

				print[i].year = list->year;
				print[i].name = list->title;
				print[i].rating = list->rating;

			}
			else{
				year--;
			}
		}
		else{
			/**********
			 * If were at a new year, print the previous year as long as its not the first
			 * *******/

			if(i != 0){
				if(print[i-1].year < 2020 && print[i-1].year > 1900){
					printf("%d %d %s\n", print[i-1].year, print[i-1].rating, print[i-1].name);
				}
			}
			/*****
			 * Once we print the previous year
			 * now move onto the next item in the list
			 * ***/

			year++;
			print[i].year = list->year;
			print[i].name = list->title;
			print[i].rating = list->rating; 
		}

		previousYear = currentYear;
		i++;
		numMovies = year;
		list = list->next;
	}
}


/***********************
 * this bubbleSort Algorithm is adapted from a sorting algortihm found here
 * https://www.geeksforgeeks.org/c-program-bubble-sort-linked-list/
 * *********************/
void bubbleSort(struct movie *start) 
{ 
	int swapped, i; 
	struct movie *ptr1; 
	struct movie *lptr = NULL; 

	/* Checking for empty list */
	if (start == NULL) 
		return; 

	do
	{ 
		swapped = 0; 
		ptr1 = start; 

		while (ptr1->next != lptr) 
		{ 
			if (ptr1->year > ptr1->next->year) 
			{  
				swap(ptr1, ptr1->next); 
				swapped = 1; 
			} 
			ptr1 = ptr1->next; 
		} 
		lptr = ptr1; 
	} 
	while (swapped); 
} 

/***********************
 * this bubbleSort Algorithm is adapted from a sorting algortihm found here
 * https://www.geeksforgeeks.org/c-program-bubble-sort-linked-list/
 * *********************/
void swap(struct movie *a, struct movie *b) 
{ 
	int temp = a->year; 
	a->year = b->year; 
	b->year = temp; 
}

void runLanguage(struct movie *list){
	char input[20];
	int langExists;
	printf("Enter the language for which you want to see movies: ");
	scanf (" %s", input);
	//To check if a certain language is offered in any movies were going to loop over every one and check if its a substring if it
	//This prevents from having to split it apart

	while(list != NULL){
		char *language;
		language = list->lang;

		//strstr compares to see if the input is a substring of the language string
		//if it exists then print it and change the flag to true
		if (strstr(language, input) != NULL){
			printf("%s %d\n", list->title, list->year);
			langExists = 1;
		}
		list = list->next;
	}
	//if we never encoutered a movie made in the input language
	if(langExists != 1){
		printf("No data about movies released in %s", input);
	}
}



//runMenu is the main logic of the program that will print the menu every time and allow the user to enter the options
void runMenu(struct movie *list)
{
	int run = 1;
	while (run == 1)
	{
		printf("\n1. Show moves released in the specified year\n2. Show higest rated movie for each year\n3. Show the title and year of a release of all movies in a specific language\n4. Exit from the program\n\nEnter a choice from 1 to 4: ");
		int input;
		scanf("%d", &input);
		if (input == 1)
		{
			runYear(list);
		}
		else if (input == 2)
		{
			runHighRate(list);
		}
		else if (input == 3)
		{
			runLanguage(list);
		}
		else if (input == 4)
		{
			run = 0;
		}
		else
		{
			printf("You entered an incorrect choice. Try again.\n");
		}

	}
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("You must provide the name of the file to process\n");
		printf("Example usage: ./movie movie_sample_1.csv\n");
		return EXIT_FAILURE;
	}
	struct movie *list = processFile(argv[1]);
	printSucess(list, argv[1]);
	runMenu(list);
	return EXIT_SUCCESS;
}
