package model

type Room struct {
	ID   int    `json:"id"`
	Name string `json:"name"`
}

type RoleRoom struct {
	RoleID int `json:"role_id"`
	RoomID int `json:"room_id"`
}
