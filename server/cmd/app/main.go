package main

import (
	"fmt"
	"log"
	"net/http"

	"mewchat-server/internal/config"
	"mewchat-server/internal/repository"
)

func main() {
	cfg := config.LoadConfig()

	db, err := repository.InitDB(cfg.DBHost, cfg.DBPort, cfg.DBUser, cfg.DBPassword, cfg.DBName)
	if err != nil {
		log.Fatalf("Критическая ошибка подключения к БД: %v", err)
	}
	defer db.Close()

	if err := repository.CreateSchema(db); err != nil {
		log.Fatalf("Ошибка инициализации схем БД: %v", err)
	}

	port := ":8080"
	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		fmt.Fprint(w, "Бэкенд MewChat работает, база данных полностью готова!")
	})

	log.Printf("Сервер MewChat стартует на порту %s...", port)
	if err := http.ListenAndServe(port, nil); err != nil {
		log.Fatalf("Ошибка запуска сервера: %v", err)
	}
}
