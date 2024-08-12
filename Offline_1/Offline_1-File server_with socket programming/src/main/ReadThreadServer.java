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
    public List<String> userList;
    public List<String> chat;
    public List<String> offlineUsers;
    List<String> request ;

    public ReadThreadServer(HashMap<String, NetworkUtil> map, NetworkUtil networkUtil, List<String> userList,List<String> chat,List<String> offlineUsers,List<String> request) {
        this.clientMap = map;
        this.networkUtil = networkUtil;
        this.thr = new Thread(this);
        thr.start();
        this.userList=userList;
        this.chat=chat;
        this.offlineUsers=offlineUsers;
        this.request=request;
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
                        UserList userListObject=new UserList(userList,offlineUsers);
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
                    }else if (obj.getText().equals("chat")){
                        System.out.println("check_chat");
                        Chat chatObject= new Chat(chat);
                        networkUtil.write(chatObject);
                    }else if (obj.getText().equals("logout")){
                        System.out.println(obj.getFrom()+" Logout Server thread");
                        clientMap.remove(obj.getFrom());
                        offlineUsers.add(obj.getFrom());
                        userList.remove(obj.getFrom());
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
                        chat.add(obj.getFrom()+" requested: "+obj.getTo());
                        request.add(obj.getTo());
                    }
                    //not needed below
//                    String to = obj.getTo();
//                    NetworkUtil nu = clientMap.get(to);
//                    if (nu != null) {
//                        nu.write(obj);
//                    }
                }else if(o instanceof FileRequest){
                    FileRequest obj= (FileRequest) o;
                    if(obj.getRequest().equals("upload")){
                        if(request.contains(obj.getFileName())&&obj.scope.equals("public")) {
                                chat.add("The file <"+obj.getFileName()+"> has arrived");
                                request.remove(obj.getFileName());
                            }
                        networkUtil.Receiver(obj.filepath);
                    }else if(obj.getRequest().equals("download")){
                        System.out.println(obj.getFilepath());
                        File f=new File(obj.getFilepath());
                        if(f.exists())
                            System.out.println("requested file exist");
                        else
                            System.out.println("requested file doesn't exist");
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



