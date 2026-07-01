package repository

import (
	"database/sql"
	"errors"
	"strings"
)

func CreateRoom(db *sql.DB, name string) error {
	name = strings.TrimSpace(name)
	if name == "" {
		return errors.New("имя комнаты не может быть пустым")
	}
	if len(name) > 100 {
		return errors.New("имя комнаты слишком длинное (максимум 100 символов)")
	}

	query := `INSERT INTO rooms (name) VALUES ($1) ON CONFLICT (name) DO NOTHING;`

	_, err := db.Exec(query, name)
	if err != nil {
		return err
	}

	return nil
}

func RoomExists(db *sql.DB, id int) (bool, error) {
	var exists bool
	query := `SELECT EXISTS(SELECT 1 FROM rooms WHERE id = $1);`

	err := db.QueryRow(query, id).Scan(&exists)
	return exists, err
}
