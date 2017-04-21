package zkutil 

/**
客户端doc地址：github.com/samuel/go-zookeeper/zk
**/
import (
    "fmt"
    zk "github.com/samuel/go-zookeeper/zk"
    "time"
)

const (
	timeOut =3 
)

var hosts []string = []string{"127.0.0.1:2181"} // the zk server list

/**
 * 获取一个zk连接
 * @return {[type]}
 */
func GetConnect() (conn *zk.Conn, err error) {
    conn, _, err = zk.Connect(hosts, timeOut*time.Second)
    if err != nil {
        fmt.Println(err)
    }
    return
}


func RegistServer(conn *zk.Conn, host string) (err error) {
    _, err = conn.Create("/go_servers/"+host, nil, zk.FlagEphemeral, zk.WorldACL(zk.PermAll))
    return
}

func GetServerList(conn *zk.Conn) (list []string, err error) {
    list, _, err = conn.Children("/go_servers")
    return
}
