package gui;

import java.awt.Color;
import java.awt.GridLayout;
import javax.swing.BorderFactory;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.border.BevelBorder;
import model.Configuration;
import operators.GeneticOperations;
import operators.GeneticOperator;

public class ConfigurationPanel extends JPanel {
    private static final long serialVersionUID = 1L;
    private final int N_FIELDS = 3;
    private final String[] OPERATORS = {"Uniform"};
    private JLabel labels[];
    private JTextField tfield_n_elems;
    private JTextField tfield_mutation;
    private JComboBox<String> combo_operator;

    public ConfigurationPanel() {
        init();
    }

    private void init() {
        setBackground(Color.WHITE);
        tfield_n_elems = new JTextField("8");
        tfield_mutation = new JTextField("0.05");
        combo_operator = new JComboBox<String>(OPERATORS);
        JPanel panel = new JPanel(new GridLayout(0, 4, 15, 0));
        panel.setBorder(BorderFactory.createBevelBorder(BevelBorder.LOWERED));
        panel.add(new JLabel("Number of elements"));
        panel.add(tfield_n_elems);
        panel.add(new JLabel("Mutation rate"));
        panel.add(tfield_mutation);
        panel.add(new JLabel("Operator"));
        panel.add(combo_operator);
        add(panel);
    }

    public Configuration getValue() throws Exception {
        int n_tries = 1;
        int n_elems = this.getNElems(this.tfield_n_elems.getText());
        float mutation_rate = this.getMutationRate(tfield_mutation.getText());
        GeneticOperator operator =
            GeneticOperations.operator_by_name(
                (String) combo_operator.getSelectedItem());
        return new Configuration(n_tries, n_elems, mutation_rate, operator);
    }

    private static int getNElems(String v) throws Exception {
        int n_elems = 0;
        try {
            n_elems = Integer.parseInt(v);
        } catch(NumberFormatException e) {
            throw new Exception("Invalid argument: Number of elements.");
        }
        if(n_elems % 2 != 0)
            throw new Exception(
                "Invalid argument: " +
                "Number of elements (only even numbers alowed).");
        return n_elems;
    }

    private static float getMutationRate(String v) throws Exception {
        try {
            return Float.parseFloat(v);
        } catch(NumberFormatException e) {
            throw new Exception("Invalid argument: Mutation rate.");
        }
    }
}
