package main;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class SharedUserList {
    private List<String> onlineUsers;
    private List<String> offlineUsers;

    private Lock lock = new ReentrantLock();

    public SharedUserList(){
        onlineUsers = new ArrayList<String>();
        offlineUsers = new ArrayList<String>();
    }

    public void setOfflineUsers(List<String> offlineUsers) {
        lock.lock();
        try {
            this.offlineUsers = offlineUsers;
        } finally {
            lock.unlock();
        }
    }

    public void setOnlineUsers(List<String> onlineUsers) {
        lock.lock();
        try {
            this.onlineUsers = onlineUsers;
        } finally {
            lock.unlock();
        }
    }

    public List<String> getOfflineUsers() {
        lock.lock();
        try {
            return offlineUsers;
        } finally {
            lock.unlock();
        }
    }

    public List<String> getOnlineUsers() {
        lock.lock();
        try {
            return onlineUsers;
        } finally {
            lock.unlock();
        }
    }
}
