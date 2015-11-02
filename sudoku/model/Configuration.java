package model;

import operators.GeneticOperator;

public class Configuration {
    /** Number of times this configuration will be tested. */
    public int n_tries;
    /** Number of elements on each generation. */
    public int n_elems;
    /** Chance of mutation on genetic operations. */
    public float mutation_rate;
    /** Operator used to create new generations. */
    public GeneticOperator operator;

    /** Constructor. */
    public Configuration(
            int n_tries, int n_elems, float mutation_rate,
            GeneticOperator operator) {
        this.n_tries = n_tries;
        this.n_elems = n_elems;
        this.mutation_rate = mutation_rate;
        this.operator = operator;
    }
}
