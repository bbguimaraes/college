package gui;

import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.border.BevelBorder;
import model.Configuration;
import model.Population;

public class ConfigurationWindow extends JFrame implements ActionListener {
    private static final long serialVersionUID = 1L;
    private ConfigurationPanel panel;
    private JCheckBox checkbox_detailed;
    private JButton button_ok;
    private JButton button_cancel;

    public ConfigurationWindow() {
        init();
    }

    private void init() {
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setLayout(new BorderLayout());
        JPanel center_panel = new JPanel();
        center_panel.setLayout(new GridLayout(0, 1));
        center_panel.setBorder(
            BorderFactory.createBevelBorder(BevelBorder.LOWERED));
        this.panel = new ConfigurationPanel();
        center_panel.add(this.panel);
        JPanel south_panel = new JPanel();
        this.button_ok = new JButton("OK");
        this.button_cancel = new JButton("Cancel");
        this.checkbox_detailed = new JCheckBox("Detailed window");
        south_panel.add(this.button_ok);
        south_panel.add(this.button_cancel);
        south_panel.add(this.checkbox_detailed);
        getRootPane().setDefaultButton(this.button_ok);
        this.button_ok.addActionListener(this);
        this.button_cancel.addActionListener(this);
        add(center_panel, BorderLayout.CENTER);
        add(south_panel, BorderLayout.PAGE_END);
        pack();
        setResizable(false);
        setLocationRelativeTo(null);
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if(e.getSource() == this.button_cancel)
            System.exit(0);
        else if(e.getSource() == this.button_ok)
            if(this.create_main_window())
                dispose();
    }

    private boolean create_main_window() {
        Configuration c;
        try {
            c = this.panel.getValue();
        } catch(Exception ex) {
            JOptionPane.showMessageDialog(
                this, ex.getMessage(), "", JOptionPane.ERROR_MESSAGE);
            return false;
        }
        JFrame window;
        if(this.checkbox_detailed.isSelected())
            window = new MainWindow(new Population(c));
        else
            window = new SimpleWindow(c);
        window.setVisible(true);
        return true;
    }
}
