package operators;

import java.util.LinkedList;
import java.util.List;
import model.Solution;

public class CyclicOperator implements GeneticOperator {
    @Override
    public Solution[] generate(
            Solution s1, Solution s2, float mutationRate, int new_id) {
        int m1[][] = s1.matrix;
        int m2[][] = s2.matrix;
        int[][] new_matrix0 = new int[9][9];
        int[][] new_matrix1 = new int[9][9];
        for(int line = 0; line < 9; line++) {
            List<Integer> positions = new LinkedList<Integer>();
            int first_number = m1[line][0];
            int last_number = first_number;
            int last_position = 0;
            positions.add(last_position);
            do {
                int opposite = m2[line][last_position];
                int column = 0;
                while(m1[line][column] != opposite)
                    column++;
                last_position = column;
                last_number = m1[line][last_position];
                if(last_number != first_number)
                    positions.add(last_position);
            } while(last_number != first_number);
            for(int column = 0; column < 9; column++)
                if(positions.indexOf(column) != -1) {
                    new_matrix0[line][column] = m2[line][column];
                    new_matrix1[line][column] = m1[line][column];
                } else {
                    new_matrix0[line][column] = m1[line][column];
                    new_matrix1[line][column] = m2[line][column];
                }
        }
        Solution[] children = new Solution[2];
        children[0] = new Solution(new_id, new_matrix0);
        children[1] = new Solution(new_id + 1, new_matrix1);
        return children;
    }
}
