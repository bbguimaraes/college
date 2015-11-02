package gui;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import javax.swing.BorderFactory;
import javax.swing.JPanel;
import model.Family;
import model.Solution;

public class SolutionDisplay extends JPanel {
    private static final long serialVersionUID = 1L;
    private static final int SIZE = 20;
    private Solution solution;
    private boolean draw_errors = false;
    private Family family;

    public SolutionDisplay(Solution solution) {
        if(solution == null)
            throw new NullPointerException();
        this.solution = solution;
        init();
    }

    private void init() {
        setBackground(Color.WHITE);
        setBorder(BorderFactory.createLineBorder(Color.BLACK, 2));
        Dimension size = new Dimension(180, 180);
        setSize(size);
        setPreferredSize(size);
    }

    public void paintComponent(Graphics g) {
        super.paintComponent(g);
        colorRelations(g);
        colorErrors(g);
        for(int i = 1; i < 9; i++) {
            int pos = i * SIZE;
            if(i == 3 || i == 6) {
                g.drawLine(pos - 1, 0, pos - 1, 180);
                g.drawLine(0, pos - 1, 180, pos - 1);
            }
            g.drawLine(pos, 0, pos, 180);
            g.drawLine(0, pos, 180, pos);
        }
        for(int i = 0; i < 9; i++)
            for(int j = 0; j < 9; j++) {
                g.drawString(
                    Integer.toString(solution.matrix[i][j]),
                    j * SIZE + 7, i * SIZE + 15);
            }
    }

    public void colorErrors(Graphics g) {
        if(!this.draw_errors)
            return;
        Color old = g.getColor();
        g.setColor(new Color(255, 192, 192));
        for(int i = 0; i < 9; i++)
            for(int j = 0; j < 9; j++)
                if(solution.errors[i][j])
                    g.fillRect(j * SIZE, i * SIZE, SIZE, SIZE);
        g.setColor(old);
    }

    public void colorRelations(Graphics g) {
        if(this.family == null)
            return;
        int p0_matrix[][] = this.family.parent0.matrix;
        int p1_matrix[][] = this.family.parent1.matrix;
        Color old = g.getColor();
        for(int i = 0; i < 9; i++) {
            for(int j = 0; j < 9; j++) {
                Color c = null;
                if(p0_matrix[i][j] == solution.matrix[i][j])
                    c = new Color(192, 255, 192);
                else if(p1_matrix[i][j] == solution.matrix[i][j])
                    c = new Color(192, 192, 255);
                else
                    c = new Color(255, 255, 192);
                g.setColor(c);
                g.fillRect(j * SIZE, i * SIZE, SIZE, SIZE);
            }
        }
        g.setColor(old);
    }

    public void setDrawnFamily(Family f) {
        this.family = f;
    }

    public void setDrawErrors(boolean draw_errors) {
        this.draw_errors = draw_errors;
    }

    public Solution getSolution() {
        return solution;
    }
}
