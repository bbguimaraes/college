package gui;

import java.awt.Color;
import java.awt.FlowLayout;
import javax.swing.JFrame;
import model.Family;

public class FamilyWindow extends JFrame {
    private static final long serialVersionUID = 1L;

    public FamilyWindow(Family family) {
        init(family);
    }

    private void init(Family family) {
        setLayout(new FlowLayout());
        SolutionDisplay panel = new SolutionDisplay(family.parent0);
        panel.setBackground(new Color(192, 255, 192));
        add(panel);
        panel = new SolutionDisplay(family.parent1);
        panel.setBackground(new Color(192, 192, 255));
        add(panel);
        panel = new SolutionDisplay(family.child0);
        panel.setDrawnFamily(family);
        add(panel);
        panel = new SolutionDisplay(family.child1);
        panel.setDrawnFamily(family);
        add(panel);
        setSize(390, 412);
    }
}
