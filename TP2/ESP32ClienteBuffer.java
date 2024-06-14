package com.example.esp32client;

import java.io.*;
import java.net.Socket;

public class ESP32Client {
    private static final String SERVER_IP = "192.168.0.143";
    private static final int SERVER_PORT = 80;

    public static void main(String[] args) {
        if (args.length < 2) {
            System.out.println("Usage: ESP32Client <test> <messageSize>");
            return;
        }

        String command = args[0];
        int messageSize = Integer.parseInt(args[1]);

        switch (command) {
            case "test":
                testMessageSize(messageSize);
                break;

            default:
                System.out.println("Unknown command: " + command);
                System.out.println("Usage: ESP32Client <test> <messageSize>");
                break;
        }
    }

    public static void testMessageSize(int messageSize) {
        try (Socket socket = new Socket(SERVER_IP, SERVER_PORT);
             PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
             BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()))) {

            char[] message = new char[messageSize];
            for (int i = 0; i < messageSize; i++) {
                message[i] = 'A';  // Rellenar arreglo
            }

            long startTime = System.nanoTime();
            out.println(messageSize);
            out.flush();

            int bytesSent = 0;
            while (bytesSent < messageSize) {
                int chunkSize = Math.min(1024, messageSize - bytesSent);
                out.write(message, bytesSent, chunkSize);
                out.flush();
                bytesSent += chunkSize;
            }

            // respuesta del servidor
            int totalBytesReceived = 0;
            int bytesRead;
            char[] buffer = new char[1024];
            while (totalBytesReceived < messageSize && (bytesRead = in.read(buffer)) != -1) {
                totalBytesReceived += bytesRead;
            }
            long endTime = System.nanoTime();

            System.out.println("Bytes recibidos: " + totalBytesReceived);
            System.out.println("Tiempo total: " + (endTime - startTime) / 1000000.0 + " ms");

        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
