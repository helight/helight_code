package gomockdemo

import (
	"fmt"
	"testing"
	"github.com/golang/mock/gomock"
	"gomockdemo/mock"
)

func TestGetPeopleName(t *testing.T) {
	mockCtl := gomock.NewController(t)
    defer mockCtl.Finish()

    // 构造mock类， mock_gomockdemo就是生成的mock代码，以包的形式存在
    mockpeople := mock_gomockdemo.NewMockIpeople(mockCtl)   

    // m := new(Student)
    // mockCtl.RecordCall(m,"GetName").Times(2)
    // mockCtl.Call(m,"GetName")

    //注入期望的返回值
    // call := mockpeople.EXPECT().GetName().Return("helight1")
    // mockpeople.EXPECT().GetName().Return("helight").After(call)   
    // gomock.InOrder(
        mockpeople.EXPECT().GetName().Return("helight")
        mockpeople.EXPECT().GetName().Return("helight")
    // )
	
    mockedName := GetPeopleName(mockpeople)

    if "helight" != mockedName {
        t.Error("Get wrong name1: ", mockedName)
    }
   
    fmt.Println("gomock.Any: ", gomock.Any())

    //指定输入参数，返回指定结果
    mockpeople.EXPECT().SetName(gomock.Eq("he")).Return("ok")
    // .Do(func(format string) {   
    //  fmt.Println("recv param1 :", format)
    // }).Return("ok")
    
    //输出参不做指定，但是指定返回结果
    mockpeople.EXPECT().SetName(gomock.Any()).Do(func(format string) {    
        fmt.Println("recv param2 :", format)
    }).Return("ok1")

    fmt.Println("gomock.Eq: ", gomock.Eq("he"))
	fmt.Println("gomock.Any: ", gomock.Any())
    
    mockedSetName := SetPeopleName(mockpeople,"he")
    fmt.Println("mockedSetName: ", mockedSetName)

    if "ok" != mockedSetName{
        t.Error("Set wrong name2: ", mockedSetName)
    }

    mockedSetName = SetPeopleName(mockpeople,"al222")
    fmt.Println("mockedSetName: ", mockedSetName)

    if "ok1" != mockedSetName{
        t.Error("Set wrong name2: ", mockedSetName)
    }
    
}
