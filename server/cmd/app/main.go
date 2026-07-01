package main

import (
	"fmt"
	"log"
	"net/http"
)

func main() {
	port := ":8080"

	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		fmt.Fprint(w, "server started")
	})

	log.Printf("port %s...", port)

	if err := http.ListenAndServe(port, nil); err != nil {
		log.Fatalf("Error: %v", err)
	}
}
