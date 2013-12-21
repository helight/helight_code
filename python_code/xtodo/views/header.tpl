<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="zh-CN" lang="zh-CN">
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" >
  <title>{{title}} zqbtodo.py~简明TODO示例(by Zoom.Quiet)</title>
    <style type="text/css">
  body { background: #fefefe; color: #343434; margin: 1em; padding: 0; font-size:105%;}
  img { border: none }
  hr {
      width : 100%;
      height : 2px;
      background : #efefef;
      border : 1px solid #ddd;
      clear : both;
  }
  div#poweredby{
      float : right;
  }

  #mainbody a {color:#aaa;}
  #mainbody a:hover {color:blue;}

  del {font-size:95%; color:#088A08;}
  sub,sup {font-size:14px;color:#aaa}

  li {font-size:120%;
      border-bottom:1px dotted #aaa;
      width:90%;
	  color:#810;
      }

  #historic li{
      border-bottom:1px dashed dimgray;
      font-size:14px;line-height:20px;height:20px;
      padding:2px 3px;
      }
  li.stateC{ background-color:azure;    }
  li.stateN{ background-color:moccasin;    }
  li.stateR{ background-color:palegreen;    }
  li.stateE{ background-color:pink;    }
  li.stateD{ background-color:#ccc;}
  /*from 42qu.com*/
  input{  font-size:18px;line-height:22px;height:26px;
      padding:1px 3px;
      border:1px solid #ddd;
      width:37em;
      }

  button {    
          font-weight:normal;color:#444;
          font-size:16px;line-height:22px;height:28px;
          text-decoration:center;
      width:50px;overflow:visible;
          margin:0;
          padding:1px 3px;
      background-color:#feeefe;
      border:1px solid #ddd;
      cursor:pointer;
      }
  button:hover{
      background-color:#fff;border:1px solid #aaa;color:#000
      -moz-box-shadow:-2px 2px 2px #ccc;
      box-shadow:-2px 2px 2px #ccc;
      -webkit-box-shadow:-2px 2px 2px #ccc;
      }

    </style>
</head>
<body>
<div id="mainbody">
<h4>导航:<a href="/">首页</a>
|<a href="/historic">历史</a>
</h4>

