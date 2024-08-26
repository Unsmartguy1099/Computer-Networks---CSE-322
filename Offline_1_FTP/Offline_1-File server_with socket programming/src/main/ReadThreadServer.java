package main;


import util.NetworkUtil;

import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;
import java.io.File;

public class ReadThreadServer implements Runnable {
    private Thread thr;
    private NetworkUtil networkUtil;
    public HashMap<String, NetworkUtil> clientMap;
    public SharedUserList sharedUserList;
    public SharedFileRequests sharedFileRequests;

    public ReadThreadServer(HashMap<String, NetworkUtil> map, NetworkUtil networkUtil, SharedFileRequests sharedFileRequests,SharedUserList sharedUserList) {
        this.clientMap = map;
        this.networkUtil = networkUtil;
        this.thr = new Thread(this);
        thr.start();
        this.sharedFileRequests = sharedFileRequests;
        this.sharedUserList = sharedUserList;
    }

    public void run() {
        try {
            while (true) {
                Object o = networkUtil.read();
                if (o instanceof Message) {
                    Message obj = (Message) o;

                    //Server options-----------------------------------
                    if(obj.getText().equals("userList")){
                        System.out.println("check_userList");
                        UserList userListObject=new UserList(sharedUserList.getOnlineUsers(),sharedUserList.getOfflineUsers());
                        networkUtil.write(userListObject);
                    }else if(obj.getText().equals("public")){
                        System.out.println("check_public");
                        List<String> results = new ArrayList<>();
                        File[] files = new File("files/"+obj.getFrom()+"/public").listFiles();
                        for (File file : files) {
                            if (file.isFile()) {
                                results.add(file.getName());
                            }
                        }
                        FileList fileListObject=new FileList(results,"Public");
                        networkUtil.write(fileListObject);
                    }else if (obj.getText().equals("private")){
                        System.out.println("check_private");
                        List<String> results = new ArrayList<>();
                        File[] files = new File("files/"+obj.getFrom()+"/private").listFiles();
                        for (File file : files) {
                            if (file.isFile()) {
                                results.add(file.getName());
                            }
                        }
                        FileList fileListObject=new FileList(results,"Private");
                        networkUtil.write(fileListObject);
                    }else if (obj.getText().equals("local")){
                        System.out.println("check_local");
                        List<String> results = new ArrayList<>();
                        File[] files = new File("local/"+obj.getFrom()).listFiles();
                        for (File file : files) {
                            if (file.isFile()) {
                                results.add(file.getName());
                            }
                        }
                        FileList fileListObject=new FileList(results,"Local");
                        networkUtil.write(fileListObject);
                    }
//                    else if (obj.getText().equals("chat")){
//                        System.out.println("check_chat");
//                        Chat chatObject= new Chat(chat);
//                        networkUtil.write(chatObject);
//                    }
                    else if (obj.getText().equals("logout")){
                        System.out.println(obj.getFrom()+" Logout Server thread");
                        clientMap.remove(obj.getFrom());

                        List<String> onlineUsers = sharedUserList.getOnlineUsers();
                        List<String> offlineUsers = sharedUserList.getOfflineUsers();
                        offlineUsers.add(obj.getFrom());
                        onlineUsers.remove(obj.getFrom());
                        sharedUserList.setOfflineUsers(offlineUsers);
                        sharedUserList.setOnlineUsers(onlineUsers);

                        Message message=new Message();
                        message.setText("logout");
                        message.setFrom("server");
                        message.setTo(obj.getFrom());
                        networkUtil.write(message);
                        Thread.currentThread().interrupt();
                    }else if (obj.getText().equals("other")){
                        System.out.println("check_other: "+obj.getTo());
                        List<String> results = new ArrayList<>();
                        File[] files = new File("files/"+obj.getTo()+"/public").listFiles();
                        for (File file : files) {
                            if (file.isFile()) {
                                results.add(file.getName());
                            }
                        }
                        FileList fileListObject=new FileList(results,obj.getTo()+" public ");
                        networkUtil.write(fileListObject);
                    }else if( obj.getText().equals("req")){
                        HashMap<String, FileRequstMessage> requestMap = sharedFileRequests.getRequestMap();
                        String filename = obj.getFileName();
                        if(requestMap.containsKey(filename)){
                            requestMap.get(obj.getFileName()).setSenderList(obj.getFrom());
                        }else{
                            requestMap.put(obj.getFileName(),new FileRequstMessage(obj.getFileName(),obj.getDescription()));
                            requestMap.get(obj.getFileName()).setSenderList(obj.getFrom());
                        }
                    }
                }else if(o instanceof FileRequest){
                    FileRequest obj= (FileRequest) o;
                    if(obj.getRequest().equals("upload")){
                        HashMap<String, FileRequstMessage> requestMap = sharedFileRequests.getRequestMap();
                        if(requestMap.containsKey(obj.getFileName())&&obj.scope.equals("public")) {
                                requestMap.get(obj.getFileName()).setResolved(true);
                                requestMap.get(obj.getFileName()).setUploader(obj.getFrom());
                                for(String sender : requestMap.get(obj.getFileName()).getSenderList()){
                                    clientMap.get(sender).write(requestMap.get(obj.getFileName()));
                                }
                            }
                        networkUtil.Receiver(obj.filepath);
                    }else if(obj.getRequest().equals("download")){
                        System.out.println(obj.getFilepath());
                        File f=new File(obj.getFilepath());
                        if(f.exists())
                            System.out.println("requested file exist");
                        else
                            System.out.println("requested file doesn't exist");
                        networkUtil.write(obj);
                        networkUtil.Sender(obj.filepath);
                    }
                }
            }
        } catch (Exception e) {
            System.out.println(e);
        } finally {
            try {
                networkUtil.closeConnection();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}



