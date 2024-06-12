package com.example.esp32client;

import java.io.*;
import java.net.Socket;

public class ESP32Client {
    private static final String SERVER_IP = "192.168.0.143"; 
    private static final int SERVER_PORT = 80;

    public static void main(String[] args) {
        try {
            // Leer archivo del servidor y guardarlo localmente
            readFileFromServer("serverFile.txt", "localCopy.txt");

            // Enviar archivo local al servidor
            sendFileToServer("localCopy.txt", "serverCopy.txt");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void readFileFromServer(String serverFileName, String localFileName) throws IOException {
        Socket socket = new Socket(SERVER_IP, SERVER_PORT);
        PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
        BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));

        out.println("READ " + serverFileName + " 0 256");  // Leer los primeros 256 bytes

        FileOutputStream fos = new FileOutputStream(localFileName);
        char[] buffer = new char[256];
        int bytesRead = in.read(buffer);
        fos.write(new String(buffer, 0, bytesRead).getBytes());
        fos.close();

        String bytesReadStr = in.readLine();
        if (bytesReadStr != null) {
            System.out.println("Bytes read: " + in.readLine());
        } else {
            System.out.println("No bytes read from server.");
        }

        socket.close();
    }

    public static void sendFileToServer(String localFileName, String serverFileName) throws IOException {
        Socket socket = new Socket(SERVER_IP, SERVER_PORT);
        PrintWriter out = new PrintWriter(socket.getOutputStream(), true);

        FileInputStream fis = new FileInputStream(localFileName);
        byte[] buffer = new byte[256];
        int bytesRead = fis.read(buffer);

        out.println("WRITE " + serverFileName + " " + bytesRead);
        socket.getOutputStream().write(buffer, 0, bytesRead);
        socket.getOutputStream().flush();

        BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        String bytesWrittenStr = in.readLine();
        if (bytesWrittenStr != null) {
            System.out.println("Bytes written: " + in.readLine());
        } else {
            System.out.println("No bytes written to server.");
        }

        fis.close();
        socket.close();
    }
}
