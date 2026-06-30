package model

import "time"

type Role struct {
	ID   int    `json:"id"`
	Name string `json:"name"`
}

type User struct {
	ID           int       `json:"id"`
	Username     string    `json:"username"`
	PasswordHash string    `json:"-"`
	RoleID       int       `json:"role_id"`
	CreatedAt    time.Time `json:"created_at"`
}
