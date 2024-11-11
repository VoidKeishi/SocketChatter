# Chatting App Database Design

## Tables

### Users
| Column Name | Data Type | Constraints          |
|-------------|-----------|----------------------|
| user_id     | INT       | PRIMARY KEY, AUTO_INCREMENT |
| username    | VARCHAR   | NOT NULL, UNIQUE     |
| email       | VARCHAR   | NOT NULL, UNIQUE     |
| password    | VARCHAR   | NOT NULL             |
| created_at  | TIMESTAMP | DEFAULT CURRENT_TIMESTAMP |

### Messages
| Column Name | Data Type | Constraints          |
|-------------|-----------|----------------------|
| message_id  | INT       | PRIMARY KEY, AUTO_INCREMENT |
| sender_id   | INT       | FOREIGN KEY REFERENCES Users(user_id) |
| receiver_id | INT       | FOREIGN KEY REFERENCES Users(user_id) |
| content     | TEXT      | NOT NULL             |
| sent_at     | TIMESTAMP | DEFAULT CURRENT_TIMESTAMP |

### Chats
| Column Name | Data Type | Constraints          |
|-------------|-----------|----------------------|
| chat_id     | INT       | PRIMARY KEY, AUTO_INCREMENT |
| user1_id    | INT       | FOREIGN KEY REFERENCES Users(user_id) |
| user2_id    | INT       | FOREIGN KEY REFERENCES Users(user_id) |
| created_at  | TIMESTAMP | DEFAULT CURRENT_TIMESTAMP |

### Friends
| Column Name | Data Type | Constraints          |
|-------------|-----------|----------------------|
| user_id     | INT       | FOREIGN KEY REFERENCES Users(user_id) |
| friend_id   | INT       | FOREIGN KEY REFERENCES Users(user_id) |
| status      | VARCHAR   | NOT NULL             |
| created_at  | TIMESTAMP | DEFAULT CURRENT_TIMESTAMP |

### GroupChats
| Column Name | Data Type | Constraints          |
|-------------|-----------|----------------------|
| group_id    | INT       | PRIMARY KEY, AUTO_INCREMENT |
| group_name  | VARCHAR   | NOT NULL, UNIQUE     |
| created_at  | TIMESTAMP | DEFAULT CURRENT_TIMESTAMP |

### GroupMembers
| Column Name | Data Type | Constraints          |
|-------------|-----------|----------------------|
| group_id    | INT       | FOREIGN KEY REFERENCES GroupChats(group_id) |
| user_id     | INT       | FOREIGN KEY REFERENCES Users(user_id) |
| joined_at   | TIMESTAMP | DEFAULT CURRENT_TIMESTAMP |

### MediaMessages
| Column Name | Data Type | Constraints          |
|-------------|-----------|----------------------|
| media_id    | INT       | PRIMARY KEY, AUTO_INCREMENT |
| message_id  | INT       | FOREIGN KEY REFERENCES Messages(message_id) |
| media_type  | VARCHAR   | NOT NULL             |
| media_url   | VARCHAR   | NOT NULL             |
| uploaded_at | TIMESTAMP | DEFAULT CURRENT_TIMESTAMP |

## Relationships

- Each user can participate in multiple chats.
- Each chat involves two users.
- Each message is sent by one user to another user within a chat.
- Each user can have multiple friends.
- Each user can participate in multiple group chats.
- Each group chat can have multiple users.
- Each media message is associated with a specific message.

## Indexes

- Create indexes on `username` and `email` in the `Users` table for faster lookups.
- Create an index on `sent_at` in the `Messages` table for efficient querying of message history.

