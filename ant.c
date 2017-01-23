#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FALSE 0
#define TRUE  1

#define LEFT(X)  ( ((X)-1) & 3 )
#define RIGHT(X) ( ((X)+1) & 3 )

size_t *board = NULL;
size_t posX=0, posY=0;
size_t size = 0;
size_t dir = 0;

size_t stateLen = 0;
size_t *state;

const size_t MAX_STEPS = (size_t) 1000*1000*1000;

void freeBoard()
{
	if (board) free(board);
	if (state) free(state);
	board = NULL;
	state = NULL;
	posX = posY = size = stateLen = dir = 0;
}

typedef enum { OK, NO_MEMORY, ANTSTRING_TOO_LONG, BORING_ANT } InitState;

InitState initBoard(size_t s, const char* stateString)
{
	InitState ret = OK;

	// clear possible existing old board
	freeBoard();

	// we support only 256 bits long ants!
	// (only reason for this limitation is, that the PGM function
 	// is not working for larger state lengths)
	if (strlen(stateString) >= 256)
	{
		ret = ANTSTRING_TOO_LONG;
		goto cleanup;
	}

	// allocate board
	board = calloc(s*s, sizeof(size_t));
	if (board == NULL)
	{
		ret = NO_MEMORY;
		goto cleanup;
	}
	posX = posY = s/2;
	size = s;
	
	// parse the state string, e.g. "1001"
	stateLen = strlen(stateString);
	state = calloc(stateLen, sizeof(size_t));
	if (!state)
	{
		ret = NO_MEMORY;
		goto cleanup;
	}
	size_t i;
	for (i=0; i<stateLen; ++i)
		state[i] = (stateString[i] == '0' ? 0 : 1);

	// check for boring ant (i.e. only 0 and 1)
	// i.e. check if there is at least one different bit
	// as the first one (state[0])	
	int boring = TRUE;
	for (i=1; i<stateLen; ++i)
		if (state[0] != state[i])
		{
			boring = FALSE; 
			break;
		}
	if (boring == TRUE)
	{
		ret = BORING_ANT;
		goto cleanup;
	}

	// everything fine, we have successfully initialized a 
	// non-boring ant!
	return OK;

cleanup:
	freeBoard();
	return ret;
}


int step()
{
	size_t tmp = board[posY*size + posX];

	// increment current field on board by 1 (modulo stateLen)
	board[posY*size + posX] = (board[posY*size + posX] + 1) % stateLen;

	// turn in the corresponding direction
	if (state[tmp])
		dir = RIGHT(dir);
	else
		dir = LEFT(dir);

	// do a step in the correct direction
	switch (dir)
	{
	case 0: // north
		if (posY > 0) posY--; else return FALSE;
		break;
	case 1: // east
		if (posX < size-1) posX++; else return FALSE;
		break;
	case 2: // south
		if (posY < size-1) posY++; else return FALSE;
		break;
	case 3: // west
		if (posX > 0) posX--; else return FALSE;
		break;
	}
	return TRUE;
}


int writePGM(char* filename)
{
	FILE *f = fopen(filename, "w");
	if (!f)	return FALSE;
	size_t k,l;

	fprintf(f, "P5 %d %d %d ", size, size, stateLen);
	for (k=0; k<size; ++k)
		for (l=0; l<size; ++l)
			fputc(board[l*size+k], f);

	fclose(f);
	return TRUE;
}

void usage()
{
	printf("usage:\n");
	printf("  ./ant 10 25\n");
	printf("     Simulate Langton's Ant 10 on a quadratic board with size 25x25\n");
	printf("\n");
	printf("  ./ant 10\n");
	printf("     Same as above, but with default board size of 500\n");
	printf("\n");
}

int main(int argc, char* argv[])
{
	
	if (argc != 2 && argc != 3)
	{
		usage();
		return 1;
	}
	size_t s = 500;
	if (argc == 3)
		s = atoi(argv[2]);


	switch (initBoard(s, argv[1]))
	{
		case NO_MEMORY:
			fputs("Out of memory!\n", stderr);
			return 2;
		case ANTSTRING_TOO_LONG: 
			fputs("Antstring too long!\n", stderr);
			return 3;
		case BORING_ANT: 
			fputs("Boring ant!\n", stderr);
			return 4;
	}

	size_t steps = 0;
	//while (step() == TRUE);
	while (step() == TRUE && steps < MAX_STEPS)
		steps++;

	if (writePGM("test.pgm") == FALSE)
	{
		printf("Error saving output\n");
		return 5;
	}

	printf("state ");
	size_t i;
	for (i=0; i<stateLen; ++i)
			putchar('0' + state[i]);
	printf(", %d steps\n", steps);

	freeBoard();
	return 0;
}
