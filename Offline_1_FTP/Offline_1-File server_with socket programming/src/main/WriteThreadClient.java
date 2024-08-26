package main;


import util.NetworkUtil;

import java.io.IOException;
import java.util.Scanner;

public class WriteThreadClient implements Runnable {

    private Thread thr;
    private NetworkUtil networkUtil;// network util with server
    String name;//own name
    public boolean close;

    public WriteThreadClient(NetworkUtil networkUtil, String name) {
        this.networkUtil = networkUtil;
        this.name = name;
        this.thr = new Thread(this);
        thr.start();
        close=false;
    }

    public void run() {
        try {
            Scanner input = new Scanner(System.in);
            while (true) {
                if(close) {
                    System.out.println("Connection terminated");
                    Thread.currentThread().interrupt();
                    break;
                }
                    //login         ------------------------------------------------
                System.out.println("Press 1 to see user list");
                System.out.println("Press 2 to lookup files");
                //System.out.println("Press 3 to view messages");
                    System.out.println("Press 4 to logout");//----------------------
                    System.out.println("Press 5 to upload file");//-----------------
                System.out.println("Press 6 to see other users' public files");
                    System.out.println("Press 7 to Request a file");//--------------
                System.out.println("Press 8 to download a file");
                int n=input.nextInt();
                input.nextLine();
                if(n==1){
                    String from = name;
                    String to = "server";
                    String text = "userList";
                    Message message = new Message();
                    message.setFrom(from);
                    message.setTo(to);
                    message.setText(text);
                    networkUtil.write(message);
                }else if(n==2){
                    System.out.println("    Press 1 to see public files");
                    System.out.println("    Press 2 to see private files");
                    System.out.println("    Press 3 to see local files");
                    int n2=input.nextInt();
                    input.nextLine();
                    if(n2==1){
                        String from = name;
                        String to = "server";
                        String text = "public";
                        Message message = new Message();
                        message.setFrom(from);
                        message.setTo(to);
                        message.setText(text);
                        networkUtil.write(message);
                    }else if(n2==2){
                        String from = name;
                        String to = "server";
                        String text = "private";
                        Message message = new Message();
                        message.setFrom(from);
                        message.setTo(to);
                        message.setText(text);
                        networkUtil.write(message);
                    }else if(n2==3){
                        String from = name;
                        String to = "server";
                        String text = "local";
                        Message message = new Message();
                        message.setFrom(from);
                        message.setTo(to);
                        message.setText(text);
                        networkUtil.write(message);
                    }
                }
//                else if(n==3){
//                    String from = name;
//                    String to = "server";
//                    String text = "chat";
//                    Message message = new Message();
//                    message.setFrom(from);
//                    message.setTo(to);
//                    message.setText(text);
//                    networkUtil.write(message);
 //               }
            else if(n==4){
                    String from = name;
                    String to = "server";
                    String text = "logout";
                    Message message = new Message();
                    message.setFrom(from);
                    message.setTo(to);
                    message.setText(text);
                    networkUtil.write(message);
                    System.out.println("Connection terminated");
                }else if(n==5){
                    System.out.println("    press 1 to upload in your private folder");
                    System.out.println("    press 2 to upload in your public folder");
                    int scope=input.nextInt();
                    input.nextLine();
                    System.out.println("Type file name you want to upload:");
                    String filename=input.nextLine();
                    FileRequest fileRequest;
                    //step 1: Call server to be ready to receive file (Send object)
                    if(scope==1){
                        fileRequest=new FileRequest("files/"+name+"/private/"+filename,"upload",name,filename,"private");
                        networkUtil.write(fileRequest);
                    } else if(scope==2){
                        fileRequest=new FileRequest("files/"+name+"/public/"+filename,"upload",name,filename,"public");
                        networkUtil.write(fileRequest);
                    }
                    //step 2: Send file (Send File)
                    networkUtil.Sender("local/"+name+"/"+filename);
                }else if(n==6){
                    System.out.println("Lookup public files of user:");
                    String username =input.nextLine();
                    String from = name;
                    String to = username;
                    String text = "other";
                    Message message = new Message();
                    message.setFrom(from);
                    message.setTo(to);
                    message.setText(text);
                    networkUtil.write(message);
                }else if(n==7){
                    System.out.println("Type the name of the file you want with extension:");
                    String filename =input.nextLine();
                    System.out.println("Type a short description of the file:");
                    String description =input.nextLine();
                    String from = name;
                    String to = "server";
                    String text = "req";
                    Message message = new Message();
                    message.setFrom(from);
                    message.setTo(to);
                    message.setText(text);
                    message.setFileName(filename);
                    message.setDescription(description);
                    networkUtil.write(message);
                }else if(n==8){
                    System.out.println("Type user name:");
                    String username=input.nextLine();
                    FileRequest fileRequest;
                    if(username.equals(name)){
                        System.out.println("    press 1 for public");
                        System.out.println("    press 2 for private");
                        int scope=input.nextInt();
                        input.nextLine();
                        if(scope==1){
                            System.out.println("        Input file name with extension:");
                            String filename=input.nextLine();
                            fileRequest=new FileRequest("files/"+name+"/public/"+filename,"download",name,filename,"public");
                            networkUtil.write(fileRequest);
                            //Thread.sleep(2000);

                            //networkUtil.Receiver("local/"+name+"/"+filename);
                        }else if(scope==2){
                            System.out.println("        Input file name with extension:");
                            String filename=input.nextLine();
                            fileRequest=new FileRequest("files/"+name+"/private/"+filename,"download",name,filename,"private");
                            networkUtil.write(fileRequest);
                        }
                    }else{
                        System.out.println("    Input file name with extension:");
                        String filename=input.nextLine();
                        fileRequest=new FileRequest("files/"+username+"/public/"+filename,"download",name,filename,"public");
                        networkUtil.write(fileRequest);
                    }
                }
            }
        } catch (Exception e) {
            System.out.println(e);
        } finally {
            try {
                networkUtil.closeConnection();
            } catch (IOException e){
                System.out.println("connection terminated");
                //e.printStackTrace();
            }
        }
    }
}



