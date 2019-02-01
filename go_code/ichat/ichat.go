package main

import (
	"fmt"
	"context"
)

func GetNews() {
	//先进入gocn的每日新闻列表
	doc, err := goquery.NewDocument("https://gocn.vip/explore/category-14")

	if err != nil {
		ctx.Error("New index document fail, err :%v\n", err)
		return
	}

	list := doc.Find("a:contains(GoCN)")
	if list == nil {
		fmt.Printf("list is empty")
		return
	}

	//获取每日新闻列表中的第一个 & title contains todayDate
	var newUrl string
	var title string
	list.Each(func(i int, selection *goquery.Selection) {
		if len(newUrl) != 0 {
			return
		}
		val, _ := selection.Attr("href")
		title = selection.Text()

		todayDate := time.Now().Format("2006-01-02")
		if strings.Contains(val, "gocn.vip/question") == true && strings.Contains(title, todayDate){
			newUrl = val
		}
	})

	ctx.Info("newUrl :%s\n", newUrl)

	//检测newUrl是否已经用过
	/*used, err := isUrlUsed(ctx, newUrl)

	if err != nil {
		ctx.Error("Check url if_used fail, err :%v, url :%s", err, newUrl)
		//todo monitor
		return
	}

	if used {
		ctx.Info("Url is used, url :%s", newUrl)
		return
	}*/


	//进入新url，获取内容
	newDoc, err := goquery.NewDocument(newUrl)
	if err != nil {
		ctx.Error("open new url fail, err :%v, newUrl :%s", err, newUrl)
		return
	}

	modClass := newDoc.Find("div.content").Find("li")
	if modClass == nil {
		ctx.Error("mod class is nil, newUrl :%s", newUrl)
		return
	}

	contentBuf := []byte{}

	counter := 1 //计算每个内容的序号

	modClass.Each(func(i int, selection * goquery.Selection) {
		urlIndex := strings.Index(selection.Text(), "http")
		aTag := selection.Find("a")
		if aTag == nil {
			ctx.Error("A tag not found, selection Text :%s", selection.Text())
			return
		}

		urlStr, exist := aTag.Attr("href") //获取a标签中的href

		if exist  {
			contentStr := selection.Text()[0:urlIndex]
			contentBuf = append(contentBuf, []byte(fmt.Sprintf(">%d. [%s](%s)\n", counter,strings.TrimSpace(contentStr), urlStr))...)
		}
		/*else {
			contentBuf = append(contentBuf, []byte(fmt.Sprintf(">%d. %s\n", counter, selection.Text()))...)
		}*/

		counter ++

	})


	//发送企业微信消息
	err = notify_data.NotifyData(handler.NewContext(context.Background(), os.Stdout), notify_data.KEY_TYPE_BINGO, title+"\n"+string(contentBuf))
	if err != nil {
		ctx.Error("Notify wx fail, err :%v", err)
		//todo monitor
	}
}

func main() int {
	return 0
}