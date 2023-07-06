package main;


import util.NetworkUtil;

import java.io.File;
import java.util.Scanner;

public class Client {

    public Client(String serverAddress, int serverPort) {
        try {
            System.out.print("Enter name of the client: ");
            Scanner scanner = new Scanner(System.in);
            String clientName = scanner.nextLine();
            NetworkUtil networkUtil = new NetworkUtil(serverAddress, serverPort);
            //FileSendUtil fileSendUtil = new FileSendUtil(serverAddress, serverPort);
            //FileRecieveUtil fileRecieveUtil = new FileRecieveUtil(serverAddress, serverPort);
            System.out.println(clientName);
            networkUtil.write(clientName);
            WriteThreadClient writeThreadClient= new WriteThreadClient(networkUtil, clientName);
            new ReadThreadClient(networkUtil,writeThreadClient);

            //creating client directory in server:
            File filePublic = new File("local/" + clientName);
            if (!filePublic.exists()) {
                if (filePublic.mkdirs()) {
                    System.out.println("Directory is created!");
                } else {
                    System.out.println("Failed to create directory!");
                }
            } else
                System.out.println("Directory already exists");
        } catch (Exception e) {
            System.out.println(e);
        }
    }

    public static void main(String args[]) {
        String serverAddress = "127.0.0.1";
        int serverPort = 33333;
        Client client = new Client(serverAddress, serverPort);
    }
}


