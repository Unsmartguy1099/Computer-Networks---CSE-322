package main;

import util.NetworkUtil;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class SharedFileRequests {
    public HashMap<String, FileRequstMessage> requestMap;
    private Lock lock = new ReentrantLock();

    public SharedFileRequests(){
        requestMap = new HashMap<>();
    }

    public void setRequestMap(HashMap<String, FileRequstMessage> requestMap) {
        lock.lock();
        try {
            this.requestMap = requestMap;
        } finally {
            lock.unlock();
        }
    }

    public HashMap<String, FileRequstMessage> getRequestMap() {
        lock.lock();
        try {
            return requestMap;
        } finally {
            lock.unlock();
        }
    }
}
