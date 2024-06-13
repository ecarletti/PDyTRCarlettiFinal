package com.example.esp32client;

import java.io.*;
import java.net.Socket;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;

public class ESP32Client {
    private static final String SERVER_IP = "192.168.0.143";
    private static final int SERVER_PORT = 80;

    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("Usage: ESP32Client <command> [options]");
            return;
        }

        String command = args[0];

        switch (command) {
            case "read":
                if (args.length < 3) {
                    System.out.println("Usage: ESP32Client read <serverFile> <localFile>");
                    return;
                }
                readFileFromServer(args[1], args[2]);
                break;

            case "write":
                if (args.length < 3) {
                    System.out.println("Usage: ESP32Client write <localFile> <serverFile>");
                    return;
                }
                sendFileToServer(args[1], args[2]);
                break;

            case "list":
                if (args.length < 2) {
                    System.out.println("Usage: ESP32Client list <directory>");
                    return;
                }
                listFilesOnServer(args[1]);
                break;

            default:
                System.out.println("Unknown command: " + command);
                System.out.println("Usage: ESP32Client <read|write|list> [options]");
                break;
        }
    }

    public static void readFileFromServer(String serverFileName, String localFileName) {
        try (Socket socket = new Socket(SERVER_IP, SERVER_PORT);
             PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
             BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
             FileOutputStream fos = new FileOutputStream(localFileName)) {

            out.println("READ " + serverFileName + " 0 256");  // Leer los primeros 256 bytes

            char[] buffer = new char[256];
            int bytesRead = in.read(buffer);
            fos.write(new String(buffer, 0, bytesRead).getBytes());

            // Leer y mostrar la cantidad de bytes leídos desde el servidor
            System.out.println("Bytes read: " + in.readLine());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void sendFileToServer(String localFileName, String serverFileName) {
        try (Socket socket = new Socket(SERVER_IP, SERVER_PORT);
             PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
             FileInputStream fis = new FileInputStream(localFileName);
             BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()))) {

            byte[] buffer = new byte[256];
            int bytesRead = fis.read(buffer);

            out.println("WRITE " + serverFileName + " " + bytesRead);
            socket.getOutputStream().write(buffer, 0, bytesRead);
            socket.getOutputStream().flush();

            // Leer y mostrar la cantidad de bytes escritos en el servidor
            System.out.println("Bytes written: " + in.readLine());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void listFilesOnServer(String directory) {
        try (Socket socket = new Socket(SERVER_IP, SERVER_PORT);
             PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
             BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()))) {

            out.println("LIST " + directory);

            String fileList;
            while ((fileList = in.readLine()) != null) {
                // Leer y mostrar los archivos del servidor
                System.out.println(fileList);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
