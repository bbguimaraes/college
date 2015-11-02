package operators;

import java.util.Random;
import model.Solution;

public abstract class GeneticOperations {
    public static GeneticOperator operator_by_name(String name) {
        name = name.toLowerCase();
        if(name.equals("uniform"))
            return new UniformOperator();
        else if(name.equals("cyclic"))
            return new CyclicOperator();
        else
            return null;
    }

    public static int[] choose_parents(Solution[] generation, float[] chance) {
        int parents[] = new int[2];
        Random random = new Random(System.currentTimeMillis());
        float c;
        float n;
        n = random.nextFloat();
        c = 0;
        for(int i = 0; i < generation.length; i++)
            if(chance[i] + c > n) {
                parents[0] = i;
                break;
            } else
                c += chance[i];
        n = random.nextFloat();
        c = 0;
        for(int i = 0; i < generation.length; i++)
            if(chance[i] + c > n) {
                parents[1] = i;
                break;
            } else
                c += chance[i];
        return parents;
    }

    public static int copy_with_mutation(int value, float rate) {
        if(Math.random() < rate)
            return (int) (Math.random() * 10);
        else
            return value;
    }

    public static Solution create_random_solution(int id) {
        int matrix[][] = new int[9][9];
        for(int i = 0; i < 9; i++)
            for(int j = 0; j < 9; j++)
                matrix[i][j] = 0;
        for(int line = 0; line < 9; line++) {
            for(int col = 1; col < 10; col++) {
                int i;
                do {
                    i = (int) (Math.random() * 9);
                } while(matrix[line][i] != 0);
                matrix[line][i] = col;
            }
        }
        return new Solution(id, matrix);
    }
}
