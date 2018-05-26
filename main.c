#include <stdio.h>

struct GOL_Grid {
	char squares[8];
};

void print_byte(char unsigned b) {
	char i = 0;
	for(i = 7; i >= 0; i--) {
		printf("%c", b & (1 << i)? '1' : '0');
	}	
	printf("\n");
}

char unsigned count_bits (char unsigned b) {
	char unsigned i = 0;
	while(b) {
		i++;
		b &= b - 1;
	}
	return i;
}



struct GOL_Grid next_GOL_configuration(struct GOL_Grid const * const grid) {
	struct GOL_Grid next_grid;
	char unsigned i = 0;
	char unsigned row = 0;
	char unsigned j = 0;
	char unsigned neighborhood_count = 0;
	char unsigned n_mask = 0;

	for(i = 0; i < 8; i++) {
		next_grid.squares[i] = 0x00;
		row = grid->squares[i];
		while(row) {
			//obtain least significant one

			j = ((row ^ (row - 1)) + 1) >> 1;
			
			n_mask = (j << 1) | j | (j >> 1);

			neighborhood_count = count_bits(grid->squares[i] & ((j << 1) | (j >> 1)))
				           + count_bits(grid->squares[(i + 1) & 0x7] & n_mask)
					   + count_bits(grid->squares[(i - 1) & 0x7] & n_mask);
			
			//an alive cell has been found!!!
			if(neighborhood_count == 2 || neighborhood_count == 3) {
				//printf("Hello\n");
				next_grid.squares[i] |= j;
			}

			row &= row - 1;
		}	
	}
	
	return next_grid;
}







void print_grid(struct GOL_Grid grid) {
	for(int i = 0; i < 8; i++) {
		print_byte(grid.squares[i]);
	}
}

int main(void) {
	struct GOL_Grid grid;
	grid.squares[0] = 0x03;
	grid.squares[1] = 0x03;
	grid.squares[2] = 0x00;
	grid.squares[3] = 0x00;
	grid.squares[4] = 0x00;
	grid.squares[5] = 0x00;
	grid.squares[6] = 0x00;
	grid.squares[7] = 0x00;
	printf("Gen 1: \n");
	print_grid(grid);
	printf("Gen 2: \n");
	grid = next_GOL_configuration(&grid);
	print_grid(grid);
	


}
