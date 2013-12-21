%include header title='index@ '

<h1>= 任务追踪(Bottle版) =</h1>

<div style="display:none">{{debug}}</div>

<h2>== 修订 ==</h2>
<form action="/fix" method="POST">
<input type="text" name="crtid" value="{{crtid}}" style="display:none"/>

<input type="text" name="txtadd" size="100" maxlength="280" 
    value="{{itemtxt}}"/>
<button type="submit">添加</button>
</form>

