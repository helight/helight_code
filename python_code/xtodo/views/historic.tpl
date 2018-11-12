%include header title='index@ '

<h1>= 任务追踪(Bottle版) =</h1>

<div style="display:none">{{debug}}</div>

<h2>== TODO变更历史 ==</h2>
<ol id="historic">
%for item in historiclist:
  <li class="state{{item['scode']}}"><sup>{{item['state']}}</sup>
  <sub>{{item['stamp']}}~</sub>
  {{item['txt']}}

%end
</ol>


%include footer

