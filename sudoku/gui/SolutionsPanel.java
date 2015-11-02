package gui;

import java.awt.Color;
import java.awt.GridLayout;
import java.awt.event.MouseListener;
import javax.swing.JPanel;
import model.Solution;

public class SolutionsPanel extends JPanel {
    private static final long serialVersionUID = 1L;
    private SolutionDisplay[] solution_displays;

    public void setSolutions(Solution[] solutions, boolean drawErrors) {
        removeAll();
        int n = solutions.length;
        int n_panels = ((n - 1) / 4) + 1;
        setLayout(new GridLayout(0, 1));
        this.solution_displays = new SolutionDisplay[n];
        for(int i = 0; i < n; i++) {
            this.solution_displays[i] = new SolutionDisplay(solutions[i]);
            this.solution_displays[i].setDrawErrors(drawErrors);
            for(MouseListener l : getMouseListeners())
                this.solution_displays[i].addMouseListener(l);
        }
        JPanel[] panels = new JPanel[n_panels];
        int i = 0;
        for(; i < n_panels; i++) {
            panels[i] = new JPanel();
            panels[i].setBackground(Color.WHITE);
            for(int j = 0; j < 4 && (i * 4) + j < n; j++)
                panels[i].add(this.solution_displays[(i * 4) + j]);
        }
        if(i * 4 < n) {
            i *= 4;
            panels[n_panels - 1] = new JPanel();
            panels[n_panels - 1].setBackground(Color.WHITE);
            for(; i < n; i++)
                panels[n_panels - 1].add(this.solution_displays[i]);
            add(panels[n_panels - 1]);
        }
        for(JPanel p : panels) {
            p.setBackground(Color.WHITE);
            add(p);
        }
        validate();
    }
}
