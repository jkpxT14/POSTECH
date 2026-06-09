package edu.postech.csed232.exercise6.control;

import org.jetbrains.annotations.NotNull;

import edu.postech.csed232.exercise6.base.Observer;
import edu.postech.csed232.exercise6.base.Subject;
import edu.postech.csed232.exercise6.event.Event;
import edu.postech.csed232.exercise6.event.MessageEvent;
import edu.postech.csed232.exercise6.view.MessageUI;

/**
 * Observes message updates from a MessageService and updates the MessageUI component accordingly.
 */
public class MessageUIController implements Observer{
    @NotNull
    private final MessageUI messageUI;

    /**
     * Constructs the controller and registers it to the given MessageService.
     *
     * @param messageUI      the message display component
     * @param messageService the message model to observe
     */
    public MessageUIController(@NotNull MessageUI messageUI, @NotNull MessageService messageService){
        this.messageUI=messageUI;
        messageService.addObserver(this);
    }

    /**
     * Updates the displayed message when a MessageEvent is received.
     *
     * @param caller the observable object
     * @param arg    the event containing the message text
     */
    @Override
    public void update(@NotNull Subject caller, @NotNull Event arg){
        if(arg instanceof MessageEvent(String message)){
            messageUI.setText(message);
        }
    }
}