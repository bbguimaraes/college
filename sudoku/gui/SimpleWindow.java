package gui;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import model.Configuration;
import model.Population;
import model.Solution;

public class SimpleWindow extends JFrame implements ActionListener {
    private static final long serialVersionUID = 1L;
    private Configuration configuration;
    private String log;
    private JLabel progress_label;
    private JButton start_button;
    private JButton log_button;
    private ProgressPanel progress_panel;

    public SimpleWindow(Configuration configuration) {
        this.configuration = configuration;
        this.log = "";
        init();
    }

    private void init() {
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setLayout(new BorderLayout());
        this.progress_label =
            new JLabel("Generation - Best solution: , fitness");
        this.start_button = new JButton("Start");
        this.log_button = new JButton("Log");
        this.log_button.setEnabled(false);
        this.start_button.addActionListener(this);
        this.log_button.addActionListener(this);
        JPanel ne_panel = new JPanel();
        ne_panel.add(this.start_button);
        ne_panel.add(this.log_button);
        JPanel nc_panel = new JPanel();
        nc_panel.add(this.progress_label);
        JPanel n_panel = new JPanel();
        n_panel.setLayout(new BorderLayout());
        n_panel.add(nc_panel, BorderLayout.CENTER);
        n_panel.add(ne_panel, BorderLayout.LINE_END);
        this.progress_panel = new ProgressPanel(1);
        add(n_panel, BorderLayout.PAGE_START);
        add(this.progress_panel, BorderLayout.PAGE_END);
        setSize(600, 100);
        setLocationRelativeTo(null);
    }

    private boolean next_generation(Population population) {
        population.create_next_generation();
        Solution[] solutions = population.current_generation().solutions;
        Solution best = solutions[population.generation_size - 1];
        String s = ""
            + "Generation: " + population.current_generation().id + " - "
            + "Best solution: " + best.id
            + ", fitness " + best.fitness;
        this.progress_label.setText(s);
        return best.n_errors == 0;
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if(e.getSource() == this.start_button) {
            Thread thread = new SimpleWindowThread();
            thread.start();
        } else if(e.getSource() == this.log_button) {
            JOptionPane.showMessageDialog(
                SimpleWindow.this, this.log, "Log", JOptionPane.PLAIN_MESSAGE);
        }
    }

    private class SimpleWindowThread extends Thread {
        @Override
        public void run() {
            start_button.setEnabled(false);
            progress_panel.setIndeterminate(true);
            long time = System.currentTimeMillis();
            Population p = new Population(configuration);
            while(next_generation(p) == false);
            time = System.currentTimeMillis() - time;
            long s = time / 1000;
            long m = s / 60;
            s %= 60;
            long h = m / 60;
            m %= 60;
            long mili = time % 1000;
            log +=
                "Total time: " + h + ":" + m + ":" + s + ":" + mili + "\n";
            log += "Generation number: " + p.current_generation().id;
            progress_panel.update(1);
            progress_panel.setIndeterminate(false);
            log_button.setEnabled(true);
        }
    }
}
