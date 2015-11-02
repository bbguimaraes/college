package model;

import java.util.Collections;
import java.util.Vector;
import operators.GeneticOperations;
import operators.GeneticOperator;

public class Population {
    /** Array of generations. */
    private Vector<Generation> generations;
    /** Number of elements on each generation. */
    public int generation_size;
    /** Operator used to create new generations. */
    private GeneticOperator operator;
    /** Percentage of mutations over total of operations. */
    private float mutation_rate;
    /** Set to true when a solution with no errors is created. */
    public boolean solved;

    /** Create a population. */
    public Population(int n_elems, float mutation_rate, GeneticOperator op) {
        this.generations = new Vector<Generation>();
        this.generation_size = n_elems;
        this.mutation_rate = mutation_rate;
        this.operator = op;
        this.solved = false;
    }

    /** Create a population from a configuration. */
    public Population(Configuration c) {
        this(c.n_elems, c.mutation_rate, c.operator);
    }

    public int generation_count() {
        return this.generations.size();
    }

    public int current_index() {
        return this.generations.size() - 1;
    }

    public Generation current_generation() {
        return generations.lastElement();
    }

    public Generation generation(int n) {
        if(0 > n || n >= generations.size())
            throw new IndexOutOfBoundsException(Integer.toString(n));
        return generations.get(n);
    }

    /** Create first generation with random values. */
    public void create_first_generation() {
        Solution next[] = new Solution[this.generation_size];
        for(int i = 0; i < this.generation_size; i++) {
            next[i] = GeneticOperations.create_random_solution(i);
            if(next[i].fitness == 972)
                solved = true;
        }
        generations.add(new Generation(0, next, new Family[0]));
    }

    /** Create next generation from current one and return its index. */
    public int create_next_generation() {
        if(generations.size() == 0) {
            this.create_first_generation();
            return 0;
        }
        Generation generation = generations.lastElement();
        Solution[] solutions = generation.solutions;
        float[] chance = this.create_chance_array(generation);
        Solution[] new_solutions = new Solution[this.generation_size];
        Family[] families = this.create_families(
            solutions.length / 2, solutions, chance, new_solutions);
        Solution[] new_generation =
            this.merge_solutions(solutions, new_solutions);
        if(new_generation[this.generation_size - 1].fitness == 972)
            solved = true;
        generations.add(
            new Generation(generations.size(), new_generation, families));
        return generations.size() - 1;
    }

    private float[] create_chance_array(Generation generation) {
        float[] ret = new float[this.generation_size];
        for(int i = 0; i < this.generation_size; i++)
            ret[i] = (float) generation.solutions[i].fitness
                / generation.total_fitness;
        return ret;
    }

    private Family[] create_families(
            int n, Solution[] solutions, float[] chance,
            Solution[] tmp_solutions) {
        Family[] families = new Family[n];
        for(int i = 0; i < n; i++) {
            int[] parents =
                GeneticOperations.choose_parents(solutions, chance);
            Solution children[] = operator.generate(
                solutions[parents[0]],
                solutions[parents[1]],
                this.mutation_rate,
                ((generations.size() - 1) * this.generation_size)
                    + this.generation_size + (i * 2));
            int newFamilyID = i
                + (this.generation_size * (generations.size() - 1) / 2);
            Family family = new Family(
                newFamilyID,
                solutions[parents[0]],
                solutions[parents[1]],
                children[0],
                children[1],
                chance[parents[0]],
                chance[parents[1]],
                generations.size() - 1,
                "uniform");
            families[i] = family;
            tmp_solutions[i * 2] = children[0];
            tmp_solutions[(i * 2) + 1] = children[1];
        }
        return families;
    }

    private Solution[] merge_solutions(
            Solution[] current, Solution[] next) {
        Vector<Solution> ret = new Vector<Solution>(this.generation_size * 2);
        for(int i = 0; i < this.generation_size; i++)
            ret.add(current[i]);
        for(int i = 0; i < this.generation_size; i++)
            if(!ret.contains(next[i]))
                ret.add(next[i]);
        ret.trimToSize();
        Collections.sort(ret);
        ret.subList(0, ret.size() - this.generation_size).clear();
        return ret.toArray(new Solution[0]);
    }
}
