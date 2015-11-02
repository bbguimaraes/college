package test;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import model.Solution;

public class SolutionReader {
    public static Solution[] readSolutions(String fileName)
            throws IOException {
        FileInputStream freader = new FileInputStream(new File(fileName));
        DataInputStream distream = new DataInputStream(freader);
        BufferedReader reader =
            new BufferedReader(new InputStreamReader(distream));
        ArrayList<Solution> solutions = new ArrayList<Solution>();
        String buffer;
        while((buffer = reader.readLine()) != null) {
            int matrix[][] = new int[9][9];
            for(int i = 0; i < 9; i++) {
                for(int j = 0; j < 9; j++)
                    matrix[i][j] = buffer.charAt(j * 2) - '0';
                buffer = reader.readLine();
            }
            solutions.add(new Solution(solutions.size() + 1, matrix));
        }
        Solution[] ret = new Solution[solutions.size()];
        return solutions.toArray(ret);
    }
}
