package model;

public class Solution implements Comparable<Solution> {
    public int id;
    public int n_errors;
    public int fitness;
    public int matrix[][];
    public boolean errors[][];

    public Solution(int id, int matrix[][]) {
        this.id = id;
        this.matrix = matrix;
        this.fitness = calculate_fitness();
    }

    public int compareTo(Solution other) {
        return this.fitness - other.fitness;
    }

    public boolean equals(Object o) {
        if(!(o instanceof Solution))
            return false;
        Solution other = (Solution) o;
        if(this.fitness != other.fitness)
            return false;
        for(int i = 0; i < 9; i++)
            for(int j = 0; j < 9; j++)
                if(this.matrix[i][j] != other.matrix[i][j])
                    return false;
        return true;
    }

    public String toString() {
        String str = "";
        for(int i = 0; i < 9; i++) {
            for(int j = 0; j < 8; j++) {
                str += this.matrix[i][j] + " ";
            }
            str += this.matrix[i][8] + "\n";
        }
        return str;
    }

    private int calculate_fitness() {
        this.errors = new boolean[9][9];
        this.n_errors =
            this.check_line_errors()
            + this.check_column_errors()
            + this.check_block_errors();
        this.fitness = 972 - this.n_errors;
        return this.fitness;
    }

    private int check_line_errors() {
        int error_count = 0;
        for(int line = 0; line < 9; line++) {
            for(int col = 0; col < 9; col++) {
                int number = this.matrix[line][col];
                for(int next = col + 1; next < 9; next++) {
                    if(this.matrix[line][next] == number) {
                        this.errors[line][col] = true;
                        this.errors[line][next] = true;
                        error_count++;
                    }
                }
            }
        }
        return error_count;
    }

    private int check_column_errors() {
        int error_count = 0;
        for(int col = 0; col < 9; col++) {
            for(int line = 0; line < 9; line++) {
                int number = this.matrix[line][col];
                for(int next = line + 1; next < 9; next++) {
                    if(this.matrix[next][col] == number) {
                        this.errors[line][col] = true;
                        this.errors[next][col] = true;
                        error_count++;
                    }
                }
            }
        }
        return error_count;
    }

    private int check_block_errors() {
        int error_count = 0;
        for(int block = 0; block < 9; block++) {
            for(int elem = 0; elem < 9; elem++) {
                int elem_line = (3 * (block / 3)) + (elem / 3);
                int elem_col = (3 * (block % 3)) + (elem % 3);
                int number = this.matrix[elem_line][elem_col];
                for(int next = elem + 1; next < 9; next++) {
                    int next_line = (3 * (block / 3)) + (next / 3);
                    int next_col = (3 * (block % 3)) + (next % 3);
                    if(this.matrix[next_line][next_col] == number) {
                        this.errors[elem_line][elem_col] = true;
                        this.errors[next_line][next_col] = true;
                        error_count++;
                    }
                }
            }
        }
        return error_count;
    }
}
