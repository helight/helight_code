package main

import (
	"xgrpcd"
	"log"
	"runtime"
	"strconv"
	"strings"
	"sync"
	"time"
	"math/rand"
	"golang.org/x/net/context"
	"google.golang.org/grpc"
)

var (
	wg sync.WaitGroup
)

const (
	networkType	= "tcp"
	server		= "127.0.0.1"
	port		= "50001"
	parallel	= 50   //
	times		= 100000	//
)

func main() {
	runtime.GOMAXPROCS(runtime.NumCPU())
	currTime := time.Now()

	for i := 0; i < int(parallel); i++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			exe()
		}()
	}

	wg.Wait()

	log.Println("time taken : %.2f", time.Now().Sub(currTime).Seconds())
}

// define function
func exe() {
	conn, err := grpc.Dial(server + ":" + port, grpc.WithInsecure())
	if err != nil {
		log.Fatalf("did not connnet: %v", err)
	}
	defer conn.Close()
	client := xgrpcd.NewDataClient(conn)

	for i := 0; i < int(times); i++ {
		getUser(client)
	}
}

func getUser(client xgrpcd.DataClient) {
	var request xgrpcd.UserRq
	r := rand.Intn(parallel)
	request.Id = int32(r)

	response, _ := client.GetUser(context.Background(), &request)

	if id, _ := strconv.Atoi(strings.Split(response.Name, ":")[0]); id != r {
		log.Printf("response error %#v", response)
	}
}

