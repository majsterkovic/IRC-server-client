package irc.client;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.nio.charset.StandardCharsets;

/**
 * A class that represents a connection to a server.
 */
public class Connection {
    public Socket socket;
    public PrintWriter writer;
    public BufferedReader reader;

    /**
     * Creates a new connection to the specified address and port.
     * It also creates the writer and reader for the connection.
     * @param address The address to connect to.
     * @param port The port to connect to.
     */
    public Connection(String address, int port) {
        try {
            socket = new Socket(address, port);

        } catch (IOException e) {
            throw new RuntimeException(e);
        }

        try {
            writer = new PrintWriter(socket.getOutputStream(), true, StandardCharsets.UTF_8);
            reader = new BufferedReader(new InputStreamReader(socket.getInputStream(), StandardCharsets.UTF_8));
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
