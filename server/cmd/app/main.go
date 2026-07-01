package main

import (
	"flag"
	"fmt"
	"log"
	"net/http"
	"os"

	"mewchat-server/internal/chat"
	"mewchat-server/internal/config"
	"mewchat-server/internal/repository"
)

func main() {
	createRoomFlag := flag.String("create-room", "", "Имя новой комнаты чата для создания")
	flag.Parse()

	cfg := config.LoadConfig()

	db, err := repository.InitDB(cfg.DBHost, cfg.DBPort, cfg.DBUser, cfg.DBPassword, cfg.DBName)
	if err != nil {
		log.Fatalf("Критическая ошибка подключения к БД: %v", err)
	}
	defer db.Close()

	if err := repository.CreateSchema(db); err != nil {
		log.Fatalf("Ошибка инициализации схем БД: %v", err)
	}

	if *createRoomFlag != "" {
		err := repository.CreateRoom(db, *createRoomFlag)
		if err != nil {
			log.Fatalf("❌ Ошибка при создании комнаты: %v", err)
		}
		fmt.Printf("🚀 Админ-панель: Комната '%s' успешно создана/проверена в БД!\n", *createRoomFlag)
		os.Exit(0)
	}

	hub := chat.NewHub()
	go hub.Run()

	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		fmt.Fprint(w, "Бэкенд MewChat готов к WebSocket-соединениям!")
	})

	http.HandleFunc("/ws", func(w http.ResponseWriter, r *http.Request) {
		chat.ServeWS(hub, db, w, r)
	})

	port := ":8080"
	log.Printf("Сервер MewChat стартует на порту %s...", port)
	if err := http.ListenAndServe(port, nil); err != nil {
		log.Fatalf("Ошибка запуска сервера: %v", err)
	}
}
