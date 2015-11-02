package gui;

import java.awt.FlowLayout;
import javax.swing.JLabel;
import javax.swing.JPanel;
import model.Solution;

public class SolutionDetailsDisplay extends JPanel {
    private static final long serialVersionUID = 1L;
    private JLabel[] labels;

    public SolutionDetailsDisplay(Solution solution) {
        init();
        setSolution(solution);
    }

    private void init() {
        setLayout(new FlowLayout(FlowLayout.LEADING));
        this.labels = new JLabel[2];
        for(int i = 0; i < this.labels.length; i++) {
            this.labels[i] = new JLabel();
            add(this.labels[i]);
        }
    }

    public void setSolution(Solution s) {
        if(s == null) {
            this.labels[0].setText("ID:");
            this.labels[1].setText("Fitness:");
        } else {
            this.labels[0].setText("ID: " + s.id);
            this.labels[1].setText("Fitness: " + s.fitness);
        }
    }
}
