/*
    GanttSchedule：甘特图功能逻辑模块
    描述：监听处理单元格编辑，以及条形图拖拽事件。
*/

function clearTime (date) {
    if (!date) return null;
    return new Date(date.getFullYear(), date.getMonth(), date.getDate());
}
function maxTime(date) {
    if (!date) return null;
    return new Date(date.getFullYear(), date.getMonth(), date.getDate(), 23, 59, 59);
}
function clearHours(date) {
    if (!date) return null;
    return new Date(date.getFullYear(), date.getMonth(), date.getDate(), date.getHours());    
}
function maxHours(date) {
    if (!date) return null;
    return new Date(date.getFullYear(), date.getMonth(), date.getDate(), date.getHours(), 59, 59);
}

RGanttSchedule = function (gantt) {
    this.gantt = gantt;
    //处理单元格编辑，和条形图拖拽事件    
    gantt.on("taskdragstart", this.__OnTaskDragStart, this);
    gantt.on("taskdragcomplete", this.__OnTaskDragComplete, this);    
}
RGanttSchedule.prototype = {
    __OnTaskDragStart: function (e) {
        //        if (e.action == "start") {
        //            e.cancel = true;
        //        }
    },
    __OnTaskDragComplete: function (e) {
        var action = e.action, value = e.value, task = e.item;

        var gantt = this.gantt;

        if (gantt.OrderType == "day") {
            this.OrderDay(e);
        } else if (gantt.OrderType == "hour") {
            this.OrderHour(e);
        }


    },
    //以"天"为任务最小排程单位
    OrderDay: function (e) {
        var action = e.action, value = e.value, task = e.item;
        var gantt = this.gantt;

        if (action == "start") {

            task.Start = clearTime(value);
            if (task.Start > task.Finish) task.Start = clearTime(task.Finish);
            var days = parseInt((task.Finish - task.Start) / (3600 * 24 * 1000));
            task.Duration = (days + 1) * (3600 * 24);
        }
        if (action == "finish") {
            task.Finish = maxTime(value);
            if (task.Finish && task.Start) {
                var days = parseInt((task.Finish - task.Start) / (3600 * 24 * 1000));
                task.Duration = (days + 1) * (3600 * 24);
            }
        }
        if (action == "percentcomplete") {
            task.PercentComplete = value;
        }
        if (action == "move") {
            task.Start = clearTime(value);
            if (task.Start) {
                var d = new Date(task.Start.getTime() + task.Duration * 1000);
                d.setDate(d.getDate() - 1);
                task.Finish = maxTime(d);

                //task.Finish.setDate(task.Start.getDate() + task.Duration - 1);
            }

            var node = gantt.getOwnerNode(task), dropNode = e.dropNode;
            if (dropNode && dropNode != node) {
                node.Tasks.remove(task);
                if (!dropNode.Tasks) dropNode.Tasks = [];
                dropNode.Tasks.push(task);
            }
        }
        gantt.refresh();
    },
    //以"小时"为任务最小排程单位
    OrderHour: function (e) {
        var action = e.action, value = e.value, task = e.item;
        var gantt = this.gantt;

        if (action == "start") {
            task.Start = clearHours(value);
            if (task.Start > task.Finish) task.Start = clearHours(task.Finish);
            var hours = parseInt((task.Finish - task.Start) / (3600 * 1000));
            task.Duration = (days + 1) * (3600);
        }
        if (action == "finish") {
            task.Finish = maxHours(value);
            if (task.Finish && task.Start) {
                var days = parseInt((task.Finish - task.Start) / (3600 * 1000));
                task.Duration = (days + 1) * (3600);
            }
        }
        if (action == "percentcomplete") {
            task.PercentComplete = value;
        }
        if (action == "move") {
            task.Start = clearHours(value);
            if (task.Start) {
                var d = new Date(task.Start.getTime() + task.Duration * 1000);                
                d.setHours(d.getHours() - 1);
                task.Finish = maxHours(d);
            }

            var node = gantt.getOwnerNode(task), dropNode = e.dropNode;
            if (dropNode && dropNode != node) {
                node.Tasks.remove(task);
                if (!dropNode.Tasks) dropNode.Tasks = [];
                dropNode.Tasks.push(task);
            }
        }
        gantt.refresh();
    }
};