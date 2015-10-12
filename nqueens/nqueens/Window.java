package nqueens;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.Graphics;
import java.util.List;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.ScrollPaneConstants;

public class Window extends JFrame {
    private static final long serialVersionUID = 1L;
    private final int TAM = 50;

    public Window() {
        super();
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setSize(new Dimension(800, 600));
        setVisible(true);
    }

    public void set_solutions(List<char[][]> r) {
        final int n = (int)Math.floor(Math.sqrt(r.size()));
        JPanel p = new JPanel(new GridLayout(n, n));
        for(char[][] m : r) {
            JPanel new_panel = new JPanel();
            new_panel.add(new Display(m));
            p.add(new_panel);
        }
        setContentPane(new JScrollPane(p));
    }

    private class Display extends JPanel {
        private static final long serialVersionUID = 1L;
        char[][] mat;

        public Display(char[][] mat) {
            super();
            this.mat = mat;
            this.setBackground(Color.WHITE);
            Dimension d = new Dimension(
                mat.length * TAM + 1, mat.length * TAM + 1);
            this.setSize(d);
            this.setPreferredSize(d);
            this.setMinimumSize(d);
            this.setMaximumSize(d);
        }

        public void paintComponent(Graphics g) {
            super.paintComponent(g);
            int n = this.mat.length;
            for(int i = 0; i <= n; i++) {
                g.drawLine(0, i * TAM, n * TAM, i * TAM);
                g.drawLine(i * TAM, 0, i * TAM, n * TAM);
            }
            for(int i = 0; i < n; i++)
                for(int j = 0; j < n; j++)
                    if(this.mat[i][j] == 'q')
                        g.drawString(
                            "q",
                            (j + 1) * TAM - TAM / 2,
                            (i + 1) * TAM - TAM / 2);
        }
    }
}
