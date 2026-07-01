package repository

import (
	"database/sql"
	"log"
)

func CreateSchema(db *sql.DB) error {
	schema := `
	-- 1. Таблица ролей
	CREATE TABLE IF NOT EXISTS roles (
		id SERIAL PRIMARY KEY,
		name VARCHAR(50) UNIQUE NOT NULL
	);

	-- 2. Таблица пользователей
	CREATE TABLE IF NOT EXISTS users (
		id SERIAL PRIMARY KEY,
		username VARCHAR(50) UNIQUE NOT NULL,
		password_hash VARCHAR(255) NOT NULL,
		role_id INTEGER REFERENCES roles(id) ON DELETE RESTRICT,
		created_at TIMESTAMP WITH TIME ZONE DEFAULT NOW()
	);

	-- 3. Таблица комнат (чатов)
	CREATE TABLE IF NOT EXISTS rooms (
		id SERIAL PRIMARY KEY,
		name VARCHAR(100) UNIQUE NOT NULL
	);

	-- 4. Таблица связей ролей и комнат (кто куда имеет доступ)
	CREATE TABLE IF NOT EXISTS role_rooms (
		role_id INTEGER REFERENCES roles(id) ON DELETE CASCADE,
		room_id INTEGER REFERENCES rooms(id) ON DELETE CASCADE,
		PRIMARY KEY (role_id, room_id)
	);

	-- 5. Таблица сообщений
	CREATE TABLE IF NOT EXISTS messages (
		id SERIAL PRIMARY KEY,
		sender_id INTEGER REFERENCES users(id) ON DELETE CASCADE,
		room_id INTEGER REFERENCES rooms(id) ON DELETE CASCADE,
		text TEXT NOT NULL,
		created_at TIMESTAMP WITH TIME ZONE DEFAULT NOW()
	);

	-- Заполняем дефолтные роли, если таблица пуста
	INSERT INTO roles (id, name) VALUES 
		(1, 'admin'),
		(2, 'moderator'),
		(3, 'user')
	ON CONFLICT (id) DO NOTHING;
	`

	_, err := db.Exec(schema)
	if err != nil {
		return err
	}

	log.Println("Схема базы данных успешно проверена и инициализирована.")
	return nil
}
