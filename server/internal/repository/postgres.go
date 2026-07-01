package repository

import (
	"database/sql"
	"fmt"
	"log"
	"time"

	_ "github.com/lib/pq"
)

func InitDB(host, port, user, password, dbname string) (*sql.DB, error) {
	connStr := fmt.Sprintf("host=%s port=%s user=%s password=%s dbname=%s sslmode=disable",
		host, port, user, password, dbname)

	var db *sql.DB
	var err error

	for i := 0; i < 5; i++ {
		db, err = sql.Open("postgres", connStr)
		if err == nil {
			err = db.Ping()
			if err == nil {
				log.Println("Успешное подключение к базе данных PostgreSQL!")
				return db, nil
			}
		}

		log.Printf("Не удалось подключиться к БД (попытка %d/5), ждем 2 секунды...", i+1)
		time.Sleep(2 * time.Second)
	}

	return nil, fmt.Errorf("все попытки подключения к БД исчерпаны: %w", err)
}
