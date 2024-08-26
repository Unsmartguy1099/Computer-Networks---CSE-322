package main;

import java.util.HashMap;

public class RequestList {
    private HashMap<String, FileRequstMessage> requestMap;
    public RequestList(HashMap<String, FileRequstMessage> requestMap){
        this.requestMap = requestMap;
    }

    public void setRequestMap(HashMap<String, FileRequstMessage> requestMap) {
        this.requestMap = requestMap;
    }

    public HashMap<String, FileRequstMessage> getRequestMap() {
        return requestMap;
    }
}
