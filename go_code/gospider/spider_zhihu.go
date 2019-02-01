package main

import (
	"fmt"
	"github.com/PuerkitoBio/goquery"
)

type Book struct {
	Title  string //标题
	Img    string //图片地址
	Author string //作者
	Sell   string //价格
	Url    string //访问链接
}

func main() {
	var (
		url string = "https://www.zhihu.com/publications"
	)
	//使用 goquery 创建 dom 对象
	query, query_err := goquery.NewDocument(url)
	if query_err != nil {
		fmt.Println(query_err)
	}

	data_arr := make(map[int]Book, 100)
	//查看元素发现 他是一个大的 ul 那么我们直接 找到这个对象就好了。
	//每一本书的数据 为一个 li 那么直接对 li的元素进行遍历就好了
	//需要设置两个参数 i 为当前是第几个元素 s 为当时元素对象
	query.Find("ul[class=\"publications-list  weekly\"]").Eq(0).Find("li").Each(func(i int, s *goquery.Selection) {
		a := s.Find("a").Eq(0)
		a.Next()
		r := Book{}
		//获取对应 跳转链接
		r.Url, _ = a.Attr("href")
		//获取书的名字
		r.Title = a.Find(".publications-item-title").Eq(0).Text()
		//获取书的图片
		r.Img, _ = a.Find(".publications-item-image").Eq(0).Attr("src")
		//获取书的作者
		r.Author = a.Find(".publications-item-author").Eq(0).Text()
		//获取书的售价
		r.Sell = a.Find(".publications-item-promotion").Eq(0).Find("span").Eq(0).Text()
		data_arr[i] = r
	})
	for k, v := range data_arr {
		fmt.Println(k, v)
	}

}
