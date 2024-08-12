package main;

import util.NetworkUtil;
import java.io.File;
import java.util.Scanner;

public class Client {

    static String SERVER_ADDRESS = "127.0.0.1";
    static int SERVER_PORT = 33333;

    public Client(String serverAddress, int serverPort) {
        try {
            //Client Name:
            System.out.print("Enter name of the client: ");
            Scanner scanner = new Scanner(System.in);
            String clientName = scanner.nextLine();

            //Network Utility:
            NetworkUtil networkUtil = new NetworkUtil(serverAddress, serverPort);
            networkUtil.write(clientName);

            //Write Thread:
            WriteThreadClient writeThreadClient= new WriteThreadClient(networkUtil, clientName);

            //Read Thread:
            new ReadThreadClient(networkUtil,writeThreadClient);

            //Local Directory Creation:
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
        Client client = new Client(SERVER_ADDRESS, SERVER_PORT);
    }
}


