package main;


import java.io.Serializable;
import java.util.List;

public class UserList implements Serializable {
    private List<String> onlineList;
    private List<String> offlineList;
    public UserList(List<String> onlineList,List<String> offlineList){
        this.onlineList=onlineList;
        this.offlineList=offlineList;
    }
    public List<String> getList() {
        return onlineList;
    }
    public void setList(List<String> list) {
        this.onlineList = onlineList;
    }

    public void setOfflineList(List<String> offlineList) {
        this.offlineList = offlineList;
    }

    public List<String> getOfflineList() {
        return offlineList;
    }
}
