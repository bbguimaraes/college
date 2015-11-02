package model;

public class Generation {
    public int id;
    public Solution[] solutions;
    public Family[] families;
    public int total_fitness;

    public Generation(int id, Solution[] solutions, Family[] families) {
        this.id = id;
        this.solutions = solutions;
        this.families = families;
        this.total_fitness = 0;
        for(Solution s : solutions) {
            this.total_fitness += s.fitness;
        }
    }
}
