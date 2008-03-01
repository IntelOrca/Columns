#include <math.h>

#include "Grid.h"

/* Calculates the total move score that will be achived
 * on a grid using a peice structure and an x position.
 */
int Grid::CalculateMoveScore(Piece piece, int x)
{
	//Create a copy of the grid so we can simulate the move
	Grid simGrid;
	memcpy(&simGrid, this, sizeof(Grid));

	/* Add the number of crushed jewels
	 * multiplied by chain reactions.
	 */
	//Place the piece in the mGrid
	for (int y = 0; y < BOARD_HEIGHT - 2; y++) {
		if (!IsJewel(simGrid.jewel[x][y]) && simGrid.jewel[x][y] != JEWEL_CRUSHBAR) {
			simGrid.jewel[x][y] = piece.jewel[0];
			simGrid.jewel[x][y + 1] = piece.jewel[1];
			simGrid.jewel[x][y + 2] = piece.jewel[2];
			break;
		}
	}

	int mScore = 0;
	int jewels, loops = 1;
	jewels = simGrid.SetCrushedJewels();
	while (jewels > 0) {
		mScore += pow((double)jewels, loops);
		simGrid.DestroyCrushedJewels();
		while (simGrid.MarkFloatingJewels()) {
			simGrid.DropFloatingJewels();
		}
		loops++;
		jewels = simGrid.SetCrushedJewels();
	}
	return mScore;
}

//TODO: Work out what this is for and write a comment
int Grid::CalculatePartialScore(Piece piece, int x, short direction)
{
	//Create a copy of the grid so we can simulate the move
	Grid simGrid;
	memcpy(&simGrid, this, sizeof(Grid));

	/* Add the number of matched jewels
	 */
	//Place the piece in the mGrid (but not within 3 places from the top)
	for (int y = 0; y < VBOARD_HEIGHT - 6; y++) {
		if (!IsJewel(simGrid.jewel[x][y]) && simGrid.jewel[x][y] != JEWEL_CRUSHBAR) {
			simGrid.jewel[x][y] = piece.jewel[0];
			simGrid.jewel[x][y + 1] = piece.jewel[1];
			simGrid.jewel[x][y + 2] = piece.jewel[2];
			break;
		}
	}

	return simGrid.SetMatchedJewels(direction);
}

/* Actual function to mark all the jewels that
 * match a row of three or more.s
 */
int Grid::SetCrushedJewels()
{
	JEWEL specialj = JEWEL_NULL;
	int x, y;
	//Reset crushed jewels
	for (x = 0; x < BOARD_WIDTH; x++) {
		for (y = 0; y < BOARD_HEIGHT; y++) {
			if (jewel[x][y] == JEWEL_CRUSHBAR)
				x = x;
			crushed[x][y] = false;
			//Check for special jewels
			if (jewel[x][y] == JEWEL_SPECIAL)
				if (y > 0)
					if (jewel[x][y - 1] < JEWEL_SPECIAL)
						specialj = jewel[x][y - 1];
		}
	}
	
	//Search for horizontal matches (-)
	for (y = 0; y < BOARD_HEIGHT; y++) {
		for (x = 0; x < BOARD_WIDTH - 2; x++) {
			if (AllMatch(jewel[x][y], jewel[x + 1][y], jewel[x + 2][y])) {
				crushed[x][y] = true;
				crushed[x + 1][y] = true;
				crushed[x + 2][y] = true;
			}
		}
	}
	//Search for vertical matches (|)
	for (x = 0; x < BOARD_WIDTH; x++) {
		for (y = 0; y < BOARD_HEIGHT - 2; y++) {
			if (AllMatch(jewel[x][y], jewel[x][y + 1], jewel[x][y + 2])) {
				crushed[x][y] = true;
				crushed[x][y + 1] = true;
				crushed[x][y + 2] = true;
			}
		}
	}
	//Search for diagonal matches (/)
	for (y = 0; y < BOARD_HEIGHT - 2; y++) {
		for (x = 0; x < BOARD_WIDTH - 2; x++) {
			if (AllMatch(jewel[x][y], jewel[x + 1][y + 1], jewel[x + 2][y + 2])) {
				crushed[x][y] = true;
				crushed[x + 1][y + 1] = true;
				crushed[x + 2][y + 2] = true;
			}
		}
	}
	//Search for diagonal matches (\)
	for (y = 2; y < BOARD_HEIGHT; y++) {
		for (x = 0; x < BOARD_WIDTH - 2; x++) {
			if (AllMatch(jewel[x][y], jewel[x + 1][y - 1], jewel[x + 2][y - 2])) {
				crushed[x][y] = true;
				crushed[x + 1][y - 1] = true;
				crushed[x + 2][y - 2] = true;
			}
		}
	}

	//Special loop
	if (IsJewel(specialj)) {
		for (x = 0; x < BOARD_WIDTH; x++) {
			for (y = 0; y < BOARD_HEIGHT; y++) {
				if (jewel[x][y] == specialj)
					crushed[x][y] = true;
			}
		}
	}

	int mCrushes = 0;
	for (x = 0; x < BOARD_WIDTH; x++) {
		for (y = 0; y < BOARD_HEIGHT; y++) {
			if (crushed[x][y])
				mCrushes++;
		}
	}
	return mCrushes;
}

//TODO: Work out what this is for and write a comment
int Grid::SetMatchedJewels(short direction)
{
	JEWEL specialj = JEWEL_NULL;
	int x, y, pscore = 0;

	if (direction & DIRECTION_HORIZONTAL) {
		//Search for horizontal matches (-)
		for (y = 0; y < BOARD_HEIGHT; y++) {
			for (x = 0; x < BOARD_WIDTH - 2; x++) {
				if (AllMatch(jewel[x][y], jewel[x + 1][y], jewel[x + 2][y])) {
					return -1; //signal crush match
				} else {
					if (PairMatch(jewel[x][y], jewel[x + 1][y], jewel[x + 2][y])) {
						pscore++;
					}
				}
			}
		}
	}

	if (direction & DIRECTION_VERTICAL) {	
		//Search for vertical matches (|)
		for (x = 0; x < BOARD_WIDTH; x++) {
			for (y = 0; y < BOARD_HEIGHT - 2; y++) {
				if (AllMatch(jewel[x][y], jewel[x][y + 1], jewel[x][y + 2])) {
					return -1; //signal crush match
				} else {
					if (PairMatch(jewel[x][y], jewel[x][y + 1], jewel[x][y + 2])) {
						pscore++;
					}
				}
			}
		}
	}
	if (direction & DIRECTION_DIAGONAL) {
		//Search for diagonal matches (/)
		for (y = 0; y < BOARD_HEIGHT - 2; y++) {
			for (x = 0; x < BOARD_WIDTH - 2; x++) {
				if (AllMatch(jewel[x][y], jewel[x + 1][y + 1], jewel[x + 2][y + 2])) {
					return -1; //signal crush match
				} else {
					if (PairMatch(jewel[x][y], jewel[x + 1][y + 1], jewel[x + 2][y + 2])) {
						pscore++;
					}
				}
			}
		}
	}
	if (direction & DIRECTION_DIAGONAL) {
		//Search for diagonal matches (\)
		for (y = 2; y < BOARD_HEIGHT; y++) {
			for (x = 0; x < BOARD_WIDTH - 2; x++) {
				if (AllMatch(jewel[x][y], jewel[x + 1][y - 1], jewel[x + 2][y - 2])) {
					return -1; //signal crush match
				}
				else {
					if (PairMatch(jewel[x][y], jewel[x + 1][y - 1], jewel[x + 2][y - 2])) {
						pscore++;
					}
				}
			}
		}
	}

	//Special loop
	if (IsJewel(specialj))
		return -1; //signal crush match
	else
		return pscore;
}

/* Removes all the jewels that are marked
 * to be crushed.
 */
void Grid::DestroyCrushedJewels()
{
	for (int x = 0; x < BOARD_WIDTH; x++) {
		for (int y = 0; y < BOARD_HEIGHT; y++) {
			//Set the jewel to nothing
			if (crushed[x][y])
				jewel[x][y] = JEWEL_NULL;
		}
	}
}

/* Drops all the marked jewels by one
 * block down.
 */
void Grid::DropFloatingJewels()
{
	//Drop marked jewels by setting the lower one to the block above it
	for (int x = 0; x < BOARD_WIDTH; x++) {
		for (int y = 0; y < BOARD_HEIGHT; y++) {
			if (crushed[x][y]) {
				jewel[x][y - 1] = jewel[x][y];
				jewel[x][y] = JEWEL_NULL;
			}
		}
	}
}

/* Return and mark all the floating
 * jewels.
 */
bool Grid::MarkFloatingJewels()
{
	bool somefalls = false;
	//Reset floating jewels
	for (int x = 0; x < BOARD_WIDTH; x++) {
		for (int y = 0; y < BOARD_HEIGHT; y++) {
			crushed[x][y] = false;
		}
	}
	//Mark floating jewels
	for (int x = 0; x < BOARD_WIDTH; x++) {
		for (int y = 1; y < BOARD_HEIGHT; y++) {
			if (IsJewel(jewel[x][y])) {
				if (!IsJewel(jewel[x][y - 1]) && jewel[x][y - 1] != JEWEL_CRUSHBAR) {
					for (int k = y; k < BOARD_HEIGHT; k++) {
						crushed[x][k] = true;
						somefalls = true;
					}
				}
			}
		}
	}
	return somefalls;
}

/* Checks to see if each jewel
 * is the same.
 */
bool AllMatch(JEWEL a, JEWEL b, JEWEL c)
{
	if (a == JEWEL_NULL || a == JEWEL_CRUSHBAR)
		return false;
	if (a == b && a == c)
		return true;
	else
		return false;
}

/* Checks if there is a match for a and b or
 * b and c and returns true if there is.
 */
bool PairMatch(JEWEL a, JEWEL b, JEWEL c)
{
	if (a != JEWEL_NULL && a != JEWEL_CRUSHBAR) {
		if (a == b)
			return true;
	}
	if (b != JEWEL_NULL && b != JEWEL_CRUSHBAR) {
   		if (b == c)
			return true;
	}
	return false;
}

/* Return wether the jewel is null or not
 */
bool IsJewel(JEWEL j)
{
	if (j != JEWEL_NULL)
		return true;
	else
		return false;
}