package util;

import java.io.*;
import java.net.Socket;

public class NetworkUtil {
    private Socket socket;
    private ObjectOutputStream oos;
    private ObjectInputStream ois;
    private static DataOutputStream dataOutputStream = null;
    private static DataInputStream dataInputStream = null;

    public NetworkUtil(String s, int port) throws IOException {
        this.socket = new Socket(s, port);
        oos = new ObjectOutputStream(socket.getOutputStream());
        ois = new ObjectInputStream(socket.getInputStream());
        dataInputStream = new DataInputStream(socket.getInputStream());
        dataOutputStream = new DataOutputStream(socket.getOutputStream());
    }

    public NetworkUtil(Socket s) throws IOException {
        this.socket = s;
        oos = new ObjectOutputStream(socket.getOutputStream());
        ois = new ObjectInputStream(socket.getInputStream());
        dataInputStream = new DataInputStream(socket.getInputStream());
        dataOutputStream = new DataOutputStream(socket.getOutputStream());
    }

    public Object read() throws IOException, ClassNotFoundException {
        return ois.readUnshared();
    }

    public void write(Object o) throws IOException {
        oos.writeUnshared(o);
    }

    public void closeConnection() throws IOException {
        ois.close();
        oos.close();
        dataInputStream.close();
        dataOutputStream.close();
    }

    public  void Sender(String path)
    {
        System.out.println("Sending the File to the Server");
        try {
            sendFile(path);
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    public void Receiver(String path) {
        try{
            receiveFile(path);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static void sendFile(String path)
            throws Exception
    {
        int bytes = 0;
        // Open the File where he located in your pc
        File file = new File(path);
        FileInputStream fileInputStream = new FileInputStream(file);
        // Here we send the File to Server
        dataOutputStream.writeLong(file.length());
        //System.out.println(file.length());

        long size=file.length();
        // Here we  break file into chunks
        byte[] buffer = new byte[4 * 1024];
        long passed=0;
        while ((bytes = fileInputStream.read(buffer)) != -1) {
            passed++;
            // Send the file to Server Socket
            dataOutputStream.write(buffer, 0, bytes);
            dataOutputStream.flush();
            //System.out.println("y");
        }
        System.out.println("Data sent");
        // close the file here
        fileInputStream.close();
    }

    private static void receiveFile(String fileName) throws Exception {
        int bytes = 0;
        FileOutputStream fileOutputStream = new FileOutputStream(fileName);
        //System.out.println("t");
        long size = dataInputStream.readLong(); // read file size
        System.out.println(size);
        byte[] buffer = new byte[4 * 1024];
        while (size > 0 && (bytes = dataInputStream.read(buffer, 0, (int)Math.min(buffer.length, size))) != -1) {
            // Here we write the file using write method
            fileOutputStream.write(buffer, 0, bytes);
            size -= bytes; // read upto file size
        }
        // Here we received file
        System.out.println("File is Received");
        fileOutputStream.close();
    }
}

