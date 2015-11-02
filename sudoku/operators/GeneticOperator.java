package operators;

import model.Solution;

public interface GeneticOperator {
    public Solution[] generate(
        Solution s1, Solution s2, float mutation_rate, int new_id);
}
