var RGanttTaskMenu = function () {
    RGanttTaskMenu.superclass.constructor.call(this);

}
mini.extend(RGanttTaskMenu, mini.Menu, {
    _create: function () {
        RGanttTaskMenu.superclass._create.call(this);

        var menuItems = [
            { type: "menuitem", iconCls: "icon-split", text: mini.Gantt.Split_Text, name: "split" },
            { type: "menuitem", iconCls: "icon-edit", text: mini.Gantt.Edit_Text, name: "edit" },
            { type: "menuitem", iconCls: "icon-remove", text: mini.Gantt.Remove_Text, name: "remove" },
            '-',
            { type: "menuitem", iconCls: "", text: mini.Gantt.Deselect_Text, name: "deselect" }
        ];
        this.setItems(menuItems);

        this.split = mini.getbyName("split", this);
        this.edit = mini.getbyName("edit", this);
        this.remove = mini.getbyName("remove", this);
        this.deselect = mini.getbyName("deselect", this);

        this.split.on("click", this.__OnSplit, this);
        this.edit.on("click", this.__OnEdit, this);
        this.remove.on("click", this.__OnRemove, this);
        this.deselect.on("click", this.__OnDeSelect, this);


        this.on("opening", this.__OnOpening, this);
    },
    __OnOpening: function (e) {
        var gantt = this.owner;
        var tasks = gantt.getSelectedTasks();

        this.edit.hide();
        this.split.hide();
        if (tasks.length == 1) {
            this.split.show(); //仅当选中一个任务时，才显示拆分项
            this.edit.show(); //仅当选中一个任务时，才可以点击编辑项
        }
    },
    __OnSplit: function (e) {
        var gantt = this.owner;
        var task = gantt.getSelectedTask();
        if (task) {
            alert("拆分任务：" + task.Name);


        } else {
            alert("请选择一个任务");
        }
    },
    __OnEdit: function (e) {
        //        var gantt = this.owner;
        //        var task = gantt.getSelectedTask();
        //        if (task) {
        //            alert("弹出面板，修改任务：" + task.Name);
        //            

        //        } else {
        //            alert("请选择一个任务");
        //        }
    },
    __OnRemove: function (e) {
        var gantt = this.owner;
        var tasks = gantt.getSelectedTasks();
        if (tasks.length > 0) {
            if (confirm("确定删除任务？")) {
                gantt.removeTasks(tasks);
            }
        } else {
            alert("请选择要删除的任务");
        }
    },
    __OnDeSelect: function (e) {
        var gantt = this.owner;
        var tasks = gantt.getSelectedTasks();
        gantt.deselectTasks(tasks);
    }

});


var RGanttContextMenu = function () {
    RGanttContextMenu.superclass.constructor.call(this);

}
mini.extend(RGanttContextMenu, mini.Menu, {
    _create: function () {
        RGanttTaskMenu.superclass._create.call(this);

        var menuItems = [
            { type: "menuitem", icon: "icon-add", text: mini.Gantt.Add_Text, name: "add" },
            '-',
            { type: "menuitem", icon: "icon-zoomin", text: mini.Gantt.ZoomIn_Text, name: "zoomin" },
            { type: "menuitem", icon: "icon-zoomout", text: mini.Gantt.ZoomOut_Text, name: "zoomout" }
        ];
        this.setItems(menuItems);

        this.add = mini.getbyName("add", this);
        this.zoomIn = mini.getbyName("zoomin", this);
        this.zoomOut = mini.getbyName("zoomout", this);

        this.add.on("click", this.__OnAdd, this);
        this.zoomIn.on("click", this.__OnZoomIn, this);
        this.zoomOut.on("click", this.__OnZoomOut, this);
    },
    __OnAdd: function (e) {
        var gantt = this.owner;

    },
    __OnZoomIn: function (e) {
        var gantt = this.owner;
        gantt.zoomIn();
    },
    __OnZoomOut: function (e) {
        var gantt = this.owner;
        gantt.zoomOut();
    }

});