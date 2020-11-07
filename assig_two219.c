// KIT107 Assignment 2

/*
* Implementation for driver program to manage club's collection of marbles
* Author  Julian Dermoudy and Zuhayer Alam
* Version 31/08/2019
*/

#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "marble.h"
#include "node.h"

// Symbols
#define NUM_MEMBERS 7 // number of members of the club 

// Types
typedef node collection; // collection of marbles for a member

typedef struct {
	char *name; // particular member's name
	collection marbles; // the member's marble collection
} member; 

typedef member *members[NUM_MEMBERS]; // array type for holding collection of members

// Constants
const char *MEMBERS[] = { "Scott", "Malcolm", "Tony", "Kevin", "Julia", "John", "Paul" }; // contains the names of the members
const int INVALID = -1; // value for indicating something is invalid such as array index
const int FORMAT_NUM = 7; // the highest number of letters among members' name in MEMBERS array, used for width in text formatting in show_graph() function 
const int STAR_REPRESENTATION = 25; // value of one star in the histogram output

// Global Variables
members club; // a collection of members 
int invalid = 0; // variable to store number of invalid marbles

// Functions

/**
* takes a member name, locates it in the MEMBERS array
* and returns its index (or -1 if it is not present)
* param  member name of a member which will be checked for presence in MEMBERS array
* return        integer value of index of member in MEMBERS array 
*  		        or INVALID = -1 if index is not found in the array
*/
int get_member(char *member)
{
	for (int i = 0; i < NUM_MEMBERS; i++)
	{
		if (strcmp(member, MEMBERS[i]) == 0)
		{
			return i;
		}
	}
	return INVALID;
}

/*
* takes the address of a non-empty collection of marbles for a particular member and an marble
* to add to that member's collection and which adds the marble to the collection
* in increasing order of diameter
*
* param l address of the collection of a particular member
* param e marble to be inserted to the collection which is also a parameter
*/
void add_existing(collection *l, marble e)
{
	node n; // used to store the marble inside, this node will be then added to the collection
	node previous; // used to keep track of the previous node while traversing
	node current; // keeps track of the current node that is being traversed
	init_node(&n, e);
	previous = NULL; // previous is initialised as null because at first current is set to the head of the collection list, so the previous one is null
	current = *l; // current is initialised to be pointing to the exact node where the head of the list is pointing at 

	/*traversing the collection and inserting the new node considering it has to be in increasing order of diameter*/
	while ((current != NULL)  && (get_diameter(get_data(current)) < get_diameter(e)))
	{
		previous = current;
		current = get_next(current);
	}
	
	/*if the first node is the one with marble of smaller diameter than e*/
	if (previous == NULL && current == *l)
	{
		set_next(n, current);
		*l = n;
	}
	else
	{
		set_next(previous, n);
		set_next(n, current);
	}
}

/*
* takes the name of a member and an marble and if there is none already stored for that member yet,
* stores the name and marble in club array.But if the member's name and their marble are already stored, 
* then calls the add_existing() function just to store more marbles in the collection.
* param  member		the name of a member
* param  a_marble	marble of the particular member
*/
void add_marble(char *member, marble a_marble)
{
	int i; // used to store index where the member's name is stored in the MEMBERS array
	i = get_member(member);
	
	if (i != INVALID)
	{
		if (club[i]->name == NULL && club[i]->marbles == NULL)
		{
			node n;
			init_node(&n, a_marble);

			club[i]->name = member;
			club[i]->marbles = n;
		}
		
		else
		{
			add_existing(&(club[i]->marbles), a_marble);
		}
	}
	else
	{
		invalid++;
	}
}

/*
* this function read in the data from a text file 
* and stores the data in appropriate variables
* which are then used to initialise a marble type and
* all the information are added to the club array 
* using the add_marble function
*
* Author Julian Dermoudy
*/
void read_in_data()
{
	const char *FILENAME="marbles.txt";

	char *member; // the name of a member from the text file
	double diameter; // the diameter of a marble from the text file
	material made_of; // the material that a marble is made of
	content look; // the content of the marble in text file
	int new; // stores 0 and 1 which indicates if the marble is new(1) or old(0)

	FILE *fp; // this is a pointer to file type

	marble a_marble; // a marble type is stored here from the information in the text file
	fp = fopen(FILENAME, "r");

	if (fp == NULL)
	{
		printf("Cannot open file.\n");
		exit(1);
	}
	//comments in the next block of code are meant to be uncommented for debugging purposes
	else
	{
		//printf("no problem\n");
		member=(char *)malloc(30*sizeof(char));
		while (fscanf(fp,"%[^,],%lf,%d,%d,%d\n",member,&diameter,&made_of,&look,&new) != EOF)
		{
			//printf("count is %d\n",count++);
			//count++;
			init_marble(&a_marble, diameter, made_of, look, (bool)new);
			//if (count<=limit)
			//{
				//printf("%s: %s\n", member, to_string(a_marble));
				//printf("calling add_marble\n");
			    add_marble(member, a_marble);
				//printf("add_marble called\n");
			//}
			//printf("endif\n");
			member=(char *)malloc(30*sizeof(char));
			//printf("new loop\n");
		}
	}
	fclose(fp);
}

/*
* takes a collection of marbles for a particular member,
* a letter of the alphabet indicating which category should be processed,
* an 'int' indicating which value in the category the processing should occur for.
* param  head	the collection which is processed
* param  cat	the English letter which indicates which process it is going to be
* param  val	this variable indicates which value in the category the processing should occur for
* return		the number of marbles of the particular category
*/
int process_category(collection head, char cat, int val)
{
	/*considering enum types do not have indexes to access in c,
	two arrays for enum type material and content are defined which are present in the "marble.h" file
	the order of them in the arrays below are identical to the order in the enum definition*/
	enum material MATERIAL_ARRAY[] = { glass, wooden, plastic };
	enum content CONTENT_ARRAY[] = { plain, swirled, cats_eye };

	node current = head;//keeps track of the current node while traversing collection, initialised to point where the head or front of collection points at
	int total = 0;// total number of particular type of marbles. initialised to 0, later to be updated according to increase in marble number,
	switch (cat)
	{
		/*letter t indicates the process should be calculating total number of marbles in the collection*/
	case 't':
		while (current != NULL)
		{
			current = get_next(current);
			total++;
		}
		break;
		/*letter m indicates calculating the number of marbles of that particular material(plastic, wooden or glass)*/
	case 'm':
		while (current != NULL)
		{
			if (get_material(get_data(current)) == MATERIAL_ARRAY[val])
			{
				total++;
			}
			current = get_next(current);
		}
		break;
		/*letter c indicates calculating the number of marbles of that particular content value*/
	case 'c':
		while (current != NULL) {
			if (get_look(get_data(current)) == CONTENT_ARRAY[val])
			{
				total++;
			}
			current = get_next(current);
		}
		break;
		/*letter a indicates calculating the number of marbles of that particular age(old or new)*/
	case 'a':
		while (current != NULL)
		{
			if (get_age(get_data(current)) == val)
			{
				total++;
			}
			current = get_next(current);
		}
		break;
	}
	return total;
}

/*
* takes a letter, a string description of the output to be diplayed,
* and a value for the category or -1 if the category is total.
* this function displays the histogram and totals of each category,
* uses process_category function in the process.
* param  cat	a character value letter which indicates which process it will go through in process_category
* param  desc	the string description of the output to be displayed
* param  val	the int value basically the purpose of using process_category
* see			the process_category function for further details
*/
void show_graph(char cat, char *desc, int val)
{
	int biggest_num = 0;// this variable keeps track of which member has the most marbles of certain category by looking at the total for updating
	char *winner = NULL;// the member's name who has the most marbles of the processed  category

	printf("\t\t Marbles -- %s\n\n", desc);
	
	for (int i = 0; i < NUM_MEMBERS; i++)
	{
		int total; // the total number of marbles that a member has of that particular category
		total = process_category(club[i]->marbles, cat, val);
		printf("%*s | ", FORMAT_NUM, club[i]->name); 

		/*this for loop is for printing asterisks as part of showing histogram.*/
		for (int i = 0; i < (total / STAR_REPRESENTATION); i++)
		{
			printf("*");
		}
		printf(" %d \n", total);

		if (total > biggest_num)
		{
			biggest_num = total;
			winner = club[i]->name;
		}
	}
	printf("invalid marbles: %d\nAnd the most common member for %s marbles is ...... %s!\n\n", invalid, desc, winner);
}


// Entry Point
int main(int argc, char *argv[])
{
	// Initialise club variable

	/*in this for loop, memory is allocated to each index of the club variable and also the properties in the structure each index points to- are initialised to null*/
	for (int i = 0; i < NUM_MEMBERS; i++)
	{
		club[i] = (members *)malloc(sizeof(member));
		club[i]->marbles = NULL;
		club[i]->name = NULL; 
	}

	// Read in collection
	read_in_data();

	// Display histograms
	show_graph('t', "total number of", INVALID);
	show_graph('m', "glass", glass);
	show_graph('m', "wooden", wooden);
	show_graph('m', "plastic", plastic);
	show_graph('c', "solid colour", plain);
	show_graph('c', "swirled colour", swirled);
	show_graph('c', "cats_eye pattern", cats_eye);
	show_graph('a', "old", false);
	show_graph('a', "new", true);
}