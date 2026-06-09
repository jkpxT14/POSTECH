package edu.postech.csed232.exercise6.view;

import javax.swing.BorderFactory;
import javax.swing.JLabel;

import java.awt.Dimension;

import static edu.postech.csed232.exercise6.base.GameConstants.MESSAGE_FONT;

/**
 * A visual component that displays game messages.
 */
public class MessageUI extends JLabel {

    /**
     * Constructs a MessageUI with appropriate visual settings.
     */
    public MessageUI() {
        super("");
        setBorder(BorderFactory.createTitledBorder("Message"));
        setFont(MESSAGE_FONT);
        setPreferredSize(new Dimension(400, 40));
    }

}