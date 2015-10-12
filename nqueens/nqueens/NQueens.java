package nqueens;

import java.util.ArrayList;
import java.util.List;
import javax.swing.JOptionPane;

public class NQueens {
    public static void main(String[] args) {
        String n_str = JOptionPane.showInputDialog("n:");
        if(n_str == null)
            return;
        int n = Integer.parseInt(n_str);
        Solver s = new Solver(n);
        List<char[][]> l = new ArrayList<char[][]>();
        for(int i = 0; i < n; i++)
            s.nqueen(0, i, l);
        Window w = new Window();
        w.set_solutions(l);
        w.revalidate();
        w.repaint();
        w.pack();
    }
}
