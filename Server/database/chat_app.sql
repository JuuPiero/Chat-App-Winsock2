create database chat_app;
use chat_app;

create table users (
	id INT AUTO_INCREMENT PRIMARY KEY,
	username varchar(255) NOT NULL UNIQUE,
	password varchar(255) NOT NULL,
	status TINYINT DEFAULT 0, -- 0 - offline 1 - online
	last_login TIMESTAMP,
	created_at TIMESTAMP DEFAULT NOW()
);

CREATE TABLE conversations (
    id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(100), -- Chỉ sử dụng cho nhóm chat, hội thoại giữa hai người có thể để NULL
    is_group BOOLEAN NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE participants (
    id INT PRIMARY KEY AUTO_INCREMENT,
    conversation_id INT NOT NULL,
    user_id INT NOT NULL,
    joined_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (conversation_id) REFERENCES Conversations(id),
    FOREIGN KEY (user_id) REFERENCES Users(id),
    UNIQUE (conversation_id, user_id) -- Một người dùng chỉ có thể tham gia vào một cuộc hội thoại một lần
);

CREATE TABLE Messages (
    id INT PRIMARY KEY AUTO_INCREMENT,
    conversation_id INT NOT NULL,
    user_id INT NOT NULL,
    content TEXT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (conversation_id) REFERENCES Conversations(id),
    FOREIGN KEY (user_id) REFERENCES Users(id)
);

--  Get all conversations of an user

-- SELECT DISTINCT c.id, c.name, c.is_group
-- FROM conversations c
-- JOIN participants p ON c.id = p.conversation_id
-- WHERE p.user_id = ? AND EXISTS (
--     SELECT 1
--     FROM messages m
--     WHERE m.conversation_id = c.id
-- )

-- Get a Conversation of 2 user(not group have 2 user)

-- SELECT c.id, c.name, c.is_group
-- FROM conversations c
-- JOIN participants p1 ON c.id = p1.conversation_id
-- JOIN participants p2 ON c.id = p2.conversation_id
-- WHERE p1.user_id = ? AND p2.user_id = ?
--   AND c.is_group = 0
--   AND c.id IN (
--     SELECT conversation_id
--     FROM participants
--     GROUP BY conversation_id
--     HAVING COUNT(user_id) = 2
--   )
  
-- INSERT INTO conversations (is_group) VALUES (0)