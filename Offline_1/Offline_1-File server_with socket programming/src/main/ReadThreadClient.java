package main;


import util.NetworkUtil;

import java.io.FileInputStream;
import java.io.IOException;

public class ReadThreadClient implements Runnable {
    private Thread thr;
    private NetworkUtil networkUtil;
    private WriteThreadClient writeThreadClient;

    public ReadThreadClient(NetworkUtil networkUtil, WriteThreadClient writeThreadClient) {
        this.networkUtil = networkUtil;
        this.thr = new Thread(this);
        thr.start();
        this.writeThreadClient=writeThreadClient;
    }

    public void run() {
        try {
            while (true) {
                Object o = networkUtil.read();
                if (o instanceof Message) {
                    Message obj = (Message) o;
                    System.out.println("---Incoming message from "+obj.getFrom() + ": " + obj.getText());
                    if(obj.getFrom().equals("server")&& obj.getText().equals("Same username exists")){
                        writeThreadClient.close=true;
                       Thread.currentThread().interrupt();
                       break;
                    }else if(obj.getFrom().equals("server")&& obj.getText().equals("logout")){
                        writeThreadClient.close=true;
                        Thread.currentThread().interrupt();
                    }
                } else if(o instanceof UserList){
                    UserList obj = (UserList) o;
                    System.out.println("UserList:-----------");
                    for(int i=0;i<obj.getList().size();i++){
                        System.out.println(obj.getList().get(i)+"(Online)");
                    }
                    for(int i=0;i<obj.getOfflineList().size();i++){
                        System.out.println(obj.getOfflineList().get(i)+"(Offline)");
                    }
                } else if(o instanceof FileList){
                    FileList obj = (FileList) o;
                    System.out.println(obj.getType()+" Files:-----");
                    for(int i=0;i<obj.getFiles().size();i++){
                        System.out.println(obj.getFiles().get(i));
                    }
                } else if(o instanceof Chat){
                    Chat obj =(Chat) o;
                    System.out.println("Chat:--------------");
                    for(int i=0;i<obj.getChat().size();i++){
                        System.out.println(obj.getChat().get(i));
                    }
                } else if(o instanceof FileRequest){
                    FileRequest obj =(FileRequest) o;
                    networkUtil.Receiver("local/"+obj.getFrom()+"/"+obj.fileName);
                }
            }
        } catch (Exception e) {
           // System.out.println(e);
        } finally {
            try {
                networkUtil.closeConnection();
            } catch (IOException e) {
                //e.printStackTrace();
            }
        }
    }
}



