package model;

public class Family {
    public int id;
    public Solution parent0;
    public Solution parent1;
    public Solution child0;
    public Solution child1;
    public float parent0_chance;
    public float parent1_chance;
    /** Generation index of parents. */
    public int generation;
    /** Operator that generated this family. */
    public String operator;

    public Family(
            int id, Solution parent0, Solution parent1,
            Solution child0, Solution child1,
            float parent0_chance, float parent1_chance,
            int generation, String operator) {
        this.id = id;
        this.parent0 = parent0;
        this.parent1 = parent1;
        this.child0 = child0;
        this.child1 = child1;
        this.parent0_chance = parent0_chance;
        this.parent1_chance = parent1_chance;
        this.generation = generation;
        this.operator = operator;
    }

    public String toString() {
        String str = "";
        str += id + ": ";
        str += parent0.id + " " + parent1.id;
        str += " -> " + child0.id + " " + child1.id;
        return str;
    }
}
