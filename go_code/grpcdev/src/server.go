package main

import (
	"xgrpcd"
	"log"
	"net"
	"runtime"
	"strconv"
	"golang.org/x/net/context"
	"google.golang.org/grpc"
)

const (
	port = "50001"
)

type Data struct{}

func main() {
	runtime.GOMAXPROCS(runtime.NumCPU())

	// start
	lis, err := net.Listen("tcp", ":" + port)
	if err != nil {
		log.Fatalf("failed to listen: %v", err)
	}
	s := grpc.NewServer()
	xgrpcd.RegisterDataServer(s, &Data{})

	s.Serve(lis)

	log.Println("grpc server in : %s", port)
}

// define function
func (t *Data) GetUser(ctx context.Context, request *xgrpcd.UserRq) (response *xgrpcd.UserRp, err error) {
	response = &xgrpcd.UserRp {
		Name: strconv.Itoa(int(request.Id)) + ":test",
	}

	return response, err
}
