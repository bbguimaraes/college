package gui;

import java.awt.BorderLayout;
import javax.swing.JFrame;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;

public class TextDisplay extends JFrame {
    private static final long serialVersionUID = 1L;

    public TextDisplay(String s) {
        setLayout(new BorderLayout(4, 4));
        JTextArea t = new JTextArea(s);
        t.setEditable(false);
        add(new JScrollPane(t));
        pack();
        setLocationRelativeTo(null);
    }
}
