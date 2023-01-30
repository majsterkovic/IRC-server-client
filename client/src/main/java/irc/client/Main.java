package irc.client;

/**
 * A class that represents the main class of the client.
 * It is responsible for creating the connection and the IRC object.
 * It also starts a thread that reads messages from the server.
 */
public class Main {
    public static void main(String[] args) {
        int portNo = 1234;
        int argc = args.length;
        if (argc < 1) {
            throw new RuntimeException("No server address provided");
        }
        if (argc > 1) {
            try {
                portNo = Integer.parseInt(args[1]);
            } catch (NumberFormatException e) {
                throw new RuntimeException("Illegal argument");
            }
        }

        Connection connection = new Connection(args[0], portNo);
        IRC irc = new IRC(connection.writer, connection.reader, connection.socket);

        Thread thread = new Thread(() -> {
            while (true) {
                irc.readMessage();
            }
        });
        thread.start();
    }
}