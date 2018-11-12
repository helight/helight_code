/* 定义一些常用、通用的方法，供外部调用
-----------------------------------------------------------------------------*/

function CreateGantt() {

    var gantt = new PlusGantt();

    //创建甘特图调度插件
    new GanttSchedule(gantt);

    gantt.setStyle("width:100%;height:400px");

    gantt.setColumns([
        { header: "", field: "ID", width: 30, cellCls: "mini-indexcolumn", align: "center" },
        { header: mini.Gantt.Name_Text, field: "Name", width: 200, name: "taskname",
            editor: { type: "textbox" }
        },
        { header: mini.Gantt.PredecessorLink_Text, field: "PredecessorLink",
            editor: {
                type: "textbox"
            },
            oncellbeginedit: function (e) {
                var table = e.source, gantt = table.owner;
                var links = e.value;
                e.value = gantt.getLinkString(links);
            }
        },
        { header: mini.Gantt.Duration_Text, field: "Duration",
            editor: {
                type: "spinner", minValue: 0, maxValue: 1000
            }
        },
        { header: mini.Gantt.Start_Text, field: "Start",
            editor: {
                type: "datepicker"
            }
        },
        { header: mini.Gantt.Finish_Text, field: "Finish",
            editor: {
                type: "datepicker"
            }
        },
        { header: mini.Gantt.PercentComplete_Text, field: "PercentComplete",
            editor: {
                type: "spinner", minValue: 0, maxValue: 100
            }
        },
        { header: "备注", field: "Note", width: 200, enterCommit: false,
            editor: { type: "textarea", minWidth: 200 }
        }
    ]);
    //设置节点列
    gantt.setTreeColumn("taskname");

    gantt.on("drawcell", function (e) {
        if (e.column.field == "Name" && e.record.Summary) {
            e.cellHtml = '<b>' + e.cellHtml + '</b>';
        }
        if (e.column.field == "Start" || e.column.field == "Finish") {
            var d = e.value;
            if (d) {
                e.cellHtml = d.getFullYear() + "-" + (d.getMonth() + 1) + "-" + d.getDate();
            }
        }
        if (e.column.field == "PredecessorLink") {
            e.cellHtml = gantt.getLinkString(e.value);
        }
    });

    return gantt;
}

