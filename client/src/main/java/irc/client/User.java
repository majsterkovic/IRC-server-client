package irc.client;

/**
 * A class that represents a user.
 * It contains the username and the room of the user.
 */
public class User {
    private final String username;
    public Integer roomNumber;

    /**
     * Creates a new user.
     * @param username The username of the user.
     * @param roomNumber The number of the room the user is currently in.
     */
    public User(String username, Integer roomNumber) {
        this.username = username;
        this.roomNumber = roomNumber;
    }
}
