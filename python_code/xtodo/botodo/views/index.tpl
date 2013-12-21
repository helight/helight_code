%#template to generate a HTML table
%include head title=''
<h2 style="color:#f69">== ToDo ==</h2>
<div id="todolist">
</div>
<h2 style="color:#298A08">== Done ==</h2>
<div id="donelist">
</div>
<h2 style="color:#0174DF">== AddMore ==</h2>
<div class="input-append">
    <input type="text" id="txtadd" name="txtadd" class="span5" style="height: 30px;">
    <button onclick="add_item();" class="btn">ADD</button>
</div>
<script type="text/javascript">
function show(divid, data)
{
	$("#"+divid).html("");
	var list = "<ul style='color:#881100;'>";
	if (divid == "donelist")
		list = "<ul style='color:#298A08;'>";
	for (var i in data)
	{
		var task = data[i];
		if (divid == "donelist")
			list += "<li><del>" + task[1] + "</del><sup><a href='javascript:op_this("+ task[0] + ", \"redo\")'>重启</a> --- " + task[3] + "</sup>"
		else
			list += "<li>" + task[1] + "<sup><a href='javascript:op_this("+ task[0] + ", \"done\")'>完成</a> " 
					+ "|<a href='javascript:edit_this(" + task[0] + ")'>修改</a>"
					+ "|<a href='javascript:op_this(" + task[0] + ", \"del\")'>删除</a> ---" + task[3] + "</sup></li>"

	}
	list += "<ul>"
	$("#"+divid).html(list);
}
function add_item()
{
	var task = $("#txtadd").val();
	if (task != "")
	{
		var url = "/add";
		$.post(url,
			{
				txtadd:task	
			},
			function(data)
			{
				var ret = eval('('+data+')')
				var todolist = ret[0];
				var donelist = ret[1];
				show("todolist", todolist);
				show("donelist", donelist);
			});
	}
	
}
function op_this(itemid, op)
{
	var url = "/" + op + "/" + itemid;
	if (op == "index")
	{
		url = "/get_all_task";
	}
	$.get(url,
		function(data)
		{
			var ret = eval('('+data+')')
			var todolist = ret[0];
			var donelist = ret[1];
			show("todolist", todolist);
			show("donelist", donelist);
		});
}
op_this("", "index");
</script>
%include foot
