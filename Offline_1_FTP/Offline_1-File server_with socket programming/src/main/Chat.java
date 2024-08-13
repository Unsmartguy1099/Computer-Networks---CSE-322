package main;

import java.io.Serializable;
import java.util.List;

public class Chat implements Serializable {
    List<String> chat;
    public Chat(List<String> chat){
        this.chat=chat;
    }

    public void setChat(List<String> chat) {
        this.chat = chat;
    }

    public List<String> getChat() {
        return chat;
    }
}
