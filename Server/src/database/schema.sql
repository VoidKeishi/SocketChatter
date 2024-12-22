-- Users table
CREATE TABLE IF NOT EXISTS users (
    username TEXT PRIMARY KEY,
    password TEXT NOT NULL,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

-- Friend requests table
CREATE TABLE IF NOT EXISTS friend_requests (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    from_user TEXT NOT NULL,
    to_user TEXT NOT NULL,
    status TEXT CHECK(status IN ('pending', 'accepted', 'rejected')) NOT NULL DEFAULT 'pending',
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (from_user) REFERENCES users(username),
    FOREIGN KEY (to_user) REFERENCES users(username),
    UNIQUE(from_user, to_user)
);

-- Friends table (for accepted friendships)
CREATE TABLE IF NOT EXISTS friends (
    user1 TEXT NOT NULL,
    user2 TEXT NOT NULL,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (user1, user2),
    FOREIGN KEY (user1) REFERENCES users(username),
    FOREIGN KEY (user2) REFERENCES users(username)
);