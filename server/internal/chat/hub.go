package chat

import (
	"database/sql"
	"encoding/json"
	"log"
	"net/http"
	"strconv"

	"mewchat-server/internal/repository"

	"github.com/gorilla/websocket"
)

var upgrader = websocket.Upgrader{
	ReadBufferSize:  1024,
	WriteBufferSize: 1024,
	CheckOrigin: func(r *http.Request) bool {
		return true
	},
}

type Message struct {
	Sender string `json:"sender"`
	Text   string `json:"text"`
	RoomID int    `json:"room_id"`
}

type Client struct {
	Hub    *Hub
	Conn   *websocket.Conn
	Send   chan []byte
	RoomID int
}

type Hub struct {
	Rooms      map[int]map[*Client]bool
	Broadcast  chan Message
	Register   chan *Client
	Unregister chan *Client
}

func NewHub() *Hub {
	return &Hub{
		Rooms:      make(map[int]map[*Client]bool),
		Broadcast:  make(chan Message),
		Register:   make(chan *Client),
		Unregister: make(chan *Client),
	}
}

func (h *Hub) Run() {
	for {
		select {
		case client := <-h.Register:
			if h.Rooms[client.RoomID] == nil {
				h.Rooms[client.RoomID] = make(map[*Client]bool)
			}
			h.Rooms[client.RoomID][client] = true
			log.Printf("Клиент подключился к комнате %d", client.RoomID)

		case client := <-h.Unregister:
			if connections, exists := h.Rooms[client.RoomID]; exists {
				if _, ok := connections[client]; ok {
					delete(connections, client)
					close(client.Send)
					log.Printf("Клиент отключился от комнаты %d", client.RoomID)
					if len(connections) == 0 {
						delete(h.Rooms, client.RoomID)
					}
				}
			}

		case msg := <-h.Broadcast:
			connections := h.Rooms[msg.RoomID]
			for client := range connections {
				payload, err := json.Marshal(msg)
				if err != nil {
					log.Printf("Ошибка маршалинга: %v", err)
					continue
				}
				select {
				case client.Send <- payload:
				default:
					close(client.Send)
					delete(connections, client)
				}
			}
		}
	}
}

func (c *Client) ReadPump() {
	defer func() {
		c.Hub.Unregister <- c
		c.Conn.Close()
	}()

	for {
		_, message, err := c.Conn.ReadMessage()
		if err != nil {
			log.Printf("Ошибка чтения: %v", err)
			break
		}

		var msg Message
		if err := json.Unmarshal(message, &msg); err != nil {
			log.Printf("Неверный формат сообщения: %v", err)
			continue
		}

		msg.RoomID = c.RoomID

		c.Hub.Broadcast <- msg
	}
}

func (c *Client) WritePump() {
	defer func() {
		c.Conn.Close()
	}()
	for message := range c.Send {
		err := c.Conn.WriteMessage(websocket.TextMessage, message)
		if err != nil {
			break
		}
	}
}

func ServeWS(hub *Hub, db *sql.DB, w http.ResponseWriter, r *http.Request) {
	roomIDStr := r.URL.Query().Get("room_id")
	roomID, err := strconv.Atoi(roomIDStr)
	if err != nil || roomID <= 0 {
		http.Error(w, "Невалидный ID комнаты", http.StatusBadRequest)
		return
	}

	exists, err := repository.RoomExists(db, roomID)
	if err != nil {
		log.Printf("Ошибка проверки комнаты в БД: %v", err)
		http.Error(w, "Внутренняя ошибка сервера", http.StatusInternalServerError)
		return
	}

	if !exists {
		log.Printf("⚠️ Предупреждение безопасности: попытка подключения к несуществующей комнате ID %d", roomID)
		http.Error(w, "Доступ запрещен: комнаты создаются только администратором.", http.StatusForbidden)
		return
	}

	conn, err := upgrader.Upgrade(w, r, nil)
	if err != nil {
		log.Printf("Ошибка Upgrade: %v", err)
		return
	}

	client := &Client{
		Hub:    hub,
		Conn:   conn,
		Send:   make(chan []byte, 256),
		RoomID: roomID,
	}

	client.Hub.Register <- client

	go client.WritePump()
	go client.ReadPump()
}
