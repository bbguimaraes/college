package gui;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.DefaultListModel;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSpinner;
import javax.swing.ScrollPaneConstants;
import javax.swing.SpinnerNumberModel;
import model.Family;
import model.Generation;
import model.Population;
import model.Solution;

public class MainWindow extends JFrame {
    private static final long serialVersionUID = 1L;
    private Population population;
    private boolean solved;
    private SolutionsPanel west_panel;
    private SolutionDetailsDisplay detail_display;
    private JComboBox<String> generations_combo;
    private JList<Family> list;
    private JButton button_next_gen;
    private JButton button_next_10_gen;
    private JButton button_next_n_gen;
    private JSpinner spinner_n_gen;
    private JMenuItem menu_fitness;
    private JMenuItem menu_details;

    public MainWindow(Population population) {
        this.population = population;
        this.solved = false;
        init();
    }

    private void init() {
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setLayout(new BorderLayout());
        MainWindowListener listener = new MainWindowListener();
        JMenuBar menu_bar = new JMenuBar();
        setJMenuBar(menu_bar);
        JMenu menu_solutions = new JMenu("Solutions");
        JMenu menu_generations = new JMenu("Generation");
        this.menu_fitness = new JMenuItem("Fitness");
        this.menu_details = new JMenuItem("Details");
        menu_bar.add(menu_solutions);
        menu_bar.add(menu_generations);
        menu_solutions.add(this.menu_fitness);
        menu_generations.add(this.menu_details);
        ActionListener menu_listener = new MenuList();
        this.menu_fitness.addActionListener(menu_listener);
        this.menu_details.addActionListener(menu_listener);
        JPanel upper_panel = new JPanel();
        this.button_next_gen = new JButton("Next");
        this.button_next_10_gen = new JButton("Next 10");
        this.button_next_n_gen = new JButton("Next n");
        this.spinner_n_gen = new JSpinner(
            new SpinnerNumberModel(0, 0, Integer.MAX_VALUE, 1));
        this.generations_combo = new JComboBox<String>();
        upper_panel.add(this.button_next_gen);
        upper_panel.add(this.button_next_10_gen);
        upper_panel.add(Box.createRigidArea(new Dimension(32, 0)));
        upper_panel.add(this.button_next_n_gen);
        upper_panel.add(this.spinner_n_gen);
        upper_panel.add(Box.createRigidArea(new Dimension(32, 0)));
        upper_panel.add(new JLabel("Generations:"));
        upper_panel.add(this.generations_combo);
        this.button_next_gen.addActionListener(listener);
        this.button_next_10_gen.addActionListener(listener);
        this.button_next_n_gen.addActionListener(listener);
        this.generations_combo.addActionListener(listener);
        this.west_panel = new SolutionsPanel();
        this.west_panel.setLayout(new GridLayout(0, 1));
        this.west_panel.setBorder(BorderFactory.createBevelBorder(1));
        this.west_panel.setBackground(Color.WHITE);
        this.west_panel.addMouseListener(listener);
        JScrollPane west_scroll = new JScrollPane(this.west_panel);
        west_scroll.setVerticalScrollBarPolicy(
            ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
        JPanel center_panel = new JPanel();
        center_panel.setBorder(BorderFactory.createBevelBorder(1));
        center_panel.setLayout(new BorderLayout());
        DefaultListModel<Family> model = new DefaultListModel<Family>();
        list = new JList<Family>(model);
        list.setPreferredSize(new Dimension(300, 0));
        list.setFont(new Font("Lucida Console", Font.PLAIN, 18));
        list.addMouseListener(listener);
        center_panel.add(list, BorderLayout.CENTER);
        JPanel bottom_panel = new JPanel();
        this.detail_display = new SolutionDetailsDisplay(null);
        bottom_panel.add(this.detail_display);
        add(west_scroll, BorderLayout.LINE_START);
        add(upper_panel, BorderLayout.PAGE_START);
        add(center_panel, BorderLayout.CENTER);
        add(bottom_panel, BorderLayout.PAGE_END);
        next_generation();
        changeGeneration(0);
        pack();
        setLocationRelativeTo(null);
    }

    public void next_generation() {
        population.create_next_generation();
        if(!solved && population.solved) {
            solved = true;
            String s = "";
            s += "Solution found!\n";
            s += "Generation number:" + (population.current_index());
            JOptionPane.showMessageDialog(this, s);
        }
        this.generations_combo.addItem(
            Integer.toString(population.current_index()));
    }

    public void nextGenerations(int n) {
        for(int i = 0; i < n - 1; i++)
            next_generation();
    }

    public void createAndShowProgress(int n) {
        ProgressPanel progress_window = new ProgressPanel(n);
        JFrame frame = new JFrame();
        frame.add(progress_window);
        frame.pack();
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
        int inc;
        if(n > 10)
            inc = n / 10;
        else
            inc = 10;
        for(int i = 0; i < n; i += inc) {
            nextGenerations(inc);
            progress_window.update(i);
        }
        this.west_panel.paint(this.west_panel.getGraphics());
        frame.dispose();
    }

    public void changeGeneration(int generationIndex) {
        this.west_panel.setSolutions(
            population.generation(generationIndex).solutions,
            true);
        if(generationIndex > 0)
            populateFamilyList(generationIndex);
        this.generations_combo.setSelectedIndex(generationIndex);
    }

    private void populateFamilyList(int generationIndex) {
        DefaultListModel<Family> model =
            (DefaultListModel<Family>) list.getModel();
        model.clear();
        Generation generation = population.generation(generationIndex);
        float best_fitness = -1;
        Solution best_solution = null;
        Solution[] solutions = generation.solutions;
        for(Solution s : solutions)
            if(s.fitness > best_fitness) {
                best_fitness = s.fitness;
                best_solution = s;
            }
        for(Family f : generation.families)
            model.addElement(f);
    }

    private class MainWindowListener
            extends MouseAdapter implements ActionListener {
        @Override
        public void actionPerformed(ActionEvent e) {
            if(e.getSource() == generations_combo) {
                changeGeneration(generations_combo.getSelectedIndex());
                return;
            }
            if(e.getSource() == button_next_gen)
                next_generation();
            else if(e.getSource() == button_next_10_gen)
                nextGenerations(10);
            else if(e.getSource() == button_next_n_gen)
                createAndShowProgress(
                    ((SpinnerNumberModel) spinner_n_gen.getModel())
                        .getNumber().intValue());
            changeGeneration(population.current_index());
        }

        @Override
        public void mouseEntered(MouseEvent e) {
            Solution s = null;
            if(e.getSource() instanceof SolutionDisplay) {
                SolutionDisplay display = (SolutionDisplay) e.getSource();
                s = display.getSolution();
            }
            detail_display.setSolution(s);
        }

        @Override
        public void mouseClicked(MouseEvent e) {
            if(e.getClickCount() != 2 || list.getSelectedIndex() < 0)
                return;
            new FamilyWindow(list.getSelectedValue()).setVisible(true);
        }
    }

    private class MenuList implements ActionListener {
        public void actionPerformed(ActionEvent e) {
            if(e.getSource() == menu_fitness) {
                if(population.generation_count() == 0)
                    return;
                String s = new String();
                Solution[] gen = population.current_generation().solutions;
                for(Solution sol : gen)
                    s += "ID: " + sol.id + " Fitness: " + sol.fitness + "\n";
                JOptionPane.showMessageDialog(MainWindow.this, s);
            } else if(e.getSource() == menu_details) {
                Generation generation = population.current_generation();
                Family[] families = generation.families;
                if(families.length == 0)
                    return;
                String s = new String();
                for(Family f : families) {
                    s += "Family: " + f.id + "\n\n";
                    s += "Parents:\n";
                    s += f.parent0.id + " (chance: ";
                    s += f.parent0_chance + ")\n";
                    s += f.parent1.id + " (chance: ";
                    s += f.parent1_chance + ")\n\n";
                    s += "Children:\n";
                    s += f.child0.id + "\n";
                    s += f.child1.id;
                    s += "\n\n";
                    s += "---------------------------------------------------";
                    s += "----------------------";
                    s += "\n\n";
                }
                TextDisplay textDisplay = new TextDisplay(s);
                textDisplay.setVisible(true);
            }
        }
    }
}
