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
	//K&R bit manipulation counting algorithm
	char unsigned i = 0;
	while(b) {
		i++;
		b &= b - 1;
	}
	return i;
}

char unsigned handle_dead_cell(char unsigned dead_mask, struct GOL_Grid const * const grid, char unsigned i) {
	char unsigned bit = 0;
	char unsigned n_mask = 0x00;
	char unsigned central_n_mask = 0x00;
	char unsigned neighborhood_count = 0;
	char unsigned total_alive_cells = 0x00;
	while(dead_mask) {
		
		bit = ((dead_mask ^ (dead_mask - 1)) + 1) >> 1;
		central_n_mask = (bit == 0x80? 0x01 : (bit << 1)) | (bit == 0x01? 0x80 : (bit >> 1));
		n_mask = central_n_mask | bit;

		neighborhood_count = count_bits(grid->squares[i] & central_n_mask)
			           + count_bits(grid->squares[(i + 1) & 0x07] & n_mask)
				   + count_bits(grid->squares[(i - 1) & 0x07] & n_mask);
		//dead cell turns alive!!!
		if(neighborhood_count == 3)
		{
			total_alive_cells |= bit;
		}

		dead_mask &= dead_mask - 1;
	}

	return total_alive_cells;
}

struct GOL_Grid next_GOL_configuration(struct GOL_Grid const * const grid) {
	struct GOL_Grid next_grid;
	char unsigned i = 0;
	char unsigned row = 0;
	char unsigned j = 0;
	char unsigned neighborhood_count = 0;
	char unsigned n_mask = 0;
	char unsigned central_n_mask = 0;

	for(i = 0; i < 8; i++) {
		next_grid.squares[i] = 0x00;
		row = grid->squares[i];
		while(row) {
			//obtain least significant one

			j = ((row ^ (row - 1)) + 1) >> 1;
			central_n_mask = (j == 0x80? 0x01 : (j << 1)) | (j == 0x01? 0x80 : (j >> 1));
			n_mask = j | central_n_mask;
			neighborhood_count = count_bits(grid->squares[i] & central_n_mask)
				           + count_bits(grid->squares[(i + 1) & 0x07] & n_mask)
					   + count_bits(grid->squares[(i - 1) & 0x07] & n_mask);
			
			//an alive cell has been found!!!
			if(neighborhood_count == 2 || neighborhood_count == 3) {
				next_grid.squares[i] |= j;
			}
			
			next_grid.squares[i] |= handle_dead_cell((grid->squares[i] & central_n_mask) ^ central_n_mask, grid, i);
			next_grid.squares[(i + 1) & 0x07] |= handle_dead_cell((grid->squares[(i + 1) & 0x07] & n_mask) ^ n_mask,
				       	grid, (i + 1) & 0x07);
			
			next_grid.squares[(i - 1) & 0x07] |= handle_dead_cell((grid->squares[(i - 1) & 0x07] & n_mask) ^ n_mask, 
					grid, (i - 1) & 0x07);
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
	grid.squares[0] = 0x07;
	grid.squares[1] = 0x00;
	grid.squares[2] = 0x00;
	grid.squares[3] = 0x00;
	grid.squares[4] = 0x00;
	grid.squares[5] = 0x00;
	grid.squares[6] = 0x00;
	grid.squares[7] = 0x00;
	
	for(int i = 0; i < 5; i++) {
		printf("Generation: %d\n", i);
		print_grid(grid);
		grid = next_GOL_configuration(&grid);
	}

	


}
