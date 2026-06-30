package model

import "time"

type Message struct {
	ID        int       `json:"id"`
	SenderID  int       `json:"sender_id"`
	RoomID    int       `json:"room_id"`
	Text      string    `json:"text"`
	CreatedAt time.Time `json:"created_at"`
}
