package gui;

import java.awt.GridLayout;
import javax.swing.DefaultBoundedRangeModel;
import javax.swing.JPanel;
import javax.swing.JProgressBar;

public class ProgressPanel extends JPanel {
    private static final long serialVersionUID = 1L;
    private JProgressBar bar;

    public ProgressPanel(int max) {
        init();
        this.bar.setMaximum(max);
    }

    private void init() {
        setLayout(new GridLayout());
        this.bar = new JProgressBar(new DefaultBoundedRangeModel(0, 0, 0, 0));
        add(this.bar);
    }

    public void update(int value) {
        this.bar.setValue(value);
        paint(getGraphics());
    }

    public void setIndeterminate(boolean indeterminate) {
        this.bar.setIndeterminate(indeterminate);
    }
}
