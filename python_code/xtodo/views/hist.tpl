%include header title='index@ '

<h1>= 任务追踪(Bottle版) =</h1>

<div style="display:none">{{debug}}</div>

<h2>== 条目修订历史 ==</h2>
<ol>
%for item in histlist:
  <li><sub>{{item['stamp']}}</sub>
  {{item['txt']}}

%end
</ol>

%include footer

