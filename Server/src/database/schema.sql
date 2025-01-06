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
    status TEXT CHECK(status IN ('pending', 'cancelled', 'accepted', 'rejected')) NOT NULL DEFAULT 'pending',
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

CREATE TABLE IF NOT EXISTS messages (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    sender TEXT NOT NULL,
    receiver TEXT NOT NULL,
    content TEXT NOT NULL,
    timestamp BIGINT NOT NULL,
    FOREIGN KEY(sender) REFERENCES users(username),
    FOREIGN KEY(receiver) REFERENCES users(username)
);

CREATE TABLE IF NOT EXISTS groups (
    group_name TEXT PRIMARY KEY,
    creator TEXT NOT NULL,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(creator) REFERENCES users(username)
);

-- Remove the 'status' column from group_members
DROP TABLE IF EXISTS group_members;

CREATE TABLE IF NOT EXISTS group_members (
    group_name TEXT NOT NULL,
    member TEXT NOT NULL,
    created_at TEXT NOT NULL,
    FOREIGN KEY(group_name) REFERENCES groups(group_name) ON DELETE CASCADE,
    FOREIGN KEY(member) REFERENCES users(username),
    PRIMARY KEY(group_name, member)
);

CREATE TABLE IF NOT EXISTS group_invitations (
    group_name TEXT NOT NULL,
    inviter TEXT NOT NULL,
    invitee TEXT NOT NULL,
    timestamp BIGINT NOT NULL,
    FOREIGN KEY(group_name) REFERENCES groups(group_name) ON DELETE CASCADE,
    FOREIGN KEY(inviter) REFERENCES users(username),
    FOREIGN KEY(invitee) REFERENCES users(username),
    PRIMARY KEY(group_name, invitee)
);