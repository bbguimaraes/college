package operators;

import model.Solution;

/**
 * Performs uniform crossover operation over two solutions. The new solutions
 * receives 50% of the elements of each solution.
 */
public class UniformOperator implements GeneticOperator {
    @Override
    public Solution[] generate(
            Solution s0, Solution s1, float mutation_rate, int new_id) {
        int new_s0[][] = new int[9][9];
        int new_s1[][] = new int[9][9];
        for(int i = 0; i < 9; i++)
            for(int j = 0; j < 9; j++)
                if(Math.random() < 0.5) {
                    new_s0[i][j] =
                        GeneticOperations.copy_with_mutation(
                            s0.matrix[i][j], mutation_rate);
                    new_s1[i][j] =
                        GeneticOperations.copy_with_mutation(
                            s1.matrix[i][j], mutation_rate);
                } else {
                    new_s0[i][j] =
                        GeneticOperations.copy_with_mutation(
                            s1.matrix[i][j], mutation_rate);
                    new_s1[i][j] =
                        GeneticOperations.copy_with_mutation(
                            s0.matrix[i][j], mutation_rate);
                }
        Solution children[] = new Solution[2];
        children[0] = new Solution(new_id, new_s0);
        children[1] = new Solution(new_id + 1, new_s1);
        return children;
    }
}
