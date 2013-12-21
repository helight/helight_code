
CreateProjectWindow = function () {
    CreateProjectWindow.superclass.constructor.call(this);    
    this.initControls();
    this.initEvents();
}
mini.extend(CreateProjectWindow, mini.Window, {
    url: mini_JSPath + "../plusproject/js/CreatePoject.html",
    width: 580,
    height: 380,
    showFooter: true,
    showModal: true,
    initControls: function () {
        //body        
        var bodyEl = this.getBodyEl();
        mini.update(this.url, bodyEl);      

        //footer buttons
        var footerEl = this.getFooterEl();
        mini.setStyle(footerEl, "padding:8px;padding-right:10px;text-align:right;");
        footerEl.innerHTML = '<a name="ok" class="mini-button" width="60" style="margin-right:10px;">确定</a> <a name="cancel" class="mini-button" width="60">取消</a>';
        mini.parse(footerEl);

        //获取控件对象

        this.ok = mini.getbyName("ok", this);
        this.cancel = mini.getbyName("cancel", this);

 
        this.Name = mini.get("project_name");
        this.Devmember = mini.get("project_member");
        this.Devteam = mini.get("project_team");

    },
    initEvents: function () {
        //绑定事件处理函数        

        //////////////////////////
        this.ok.on("click", function (e) {
            var ret = true;
            if (this.callback) ret = this.callback('ok');
            if (ret !== false) {
                this.hide();
            }
        }, this);
        this.cancel.on("click", function (e) {
            var ret = true;
            if (this.callback) ret = this.callback('cancel');
            if (ret !== false) {
                this.hide();
            }
        }, this);
    },
    //如果是摘要任务, 并且不固定工期, 则禁止摘要任务操作日期(开始/完成/工期)
    editEnabled: function () {

    },
    setData: function (task, project, callback) {

        this.callback = callback;

        this.project = project;
        this.dataProject = project.getData();

    },
    getData: function () {

        var task = {
            DateChangeAction: this.DateChangeAction,

            Name: this.Name.getValue(),
            Department: this.Department.getValue(),
            Principal: this.Principal.getValue(),

            PercentComplete: this.PercentComplete.getValue(),
            Duration: this.Duration.getValue(),
            Work: this.Work.getValue(),

            Start: this.Start.getValue(),
            Finish: this.Finish.getValue(),

            ActualStart: this.ActualStart.getValue(),
            ActualFinish: this.ActualFinish.getValue(),

            ConstraintType: this.ConstraintType.getValue(),
            ConstraintDate: this.ConstraintDate.getValue(),
            Notes: this.Notes.getValue(),
            WBS: this.WBS.getValue(),

            FixedDate: this.FixedDate.getChecked() ? 1 : 0,
            Milestone: this.Milestone.getChecked() ? 1 : 0,
            Critical2: this.Critical2.getChecked() ? 1 : 0
        };

        return task;
    },
    /////////////////////
    getResources: function () {
        var data = this.dataProject.Resources || []
        return data;
    },
    getDepartments: function () {
        var data = this.dataProject.Departments || []
        return data;
    },
    getPrincipals: function () {
        var data = this.dataProject.Principals || []
        return data;
    },
    getPrincipalsByDepartment: function (dept) {
        var psAll = this.getPrincipals();
        var ps = [];
        for (var i = 0, l = psAll.length; i < l; i++) {
            var p = psAll[i];
            if (p.Department == dept) {
                ps.push(p);
            }
        }
        return ps;
    },
    getTaskByID: function (taskID) {
        return this.project.getTaskByID(taskID);
    },
    getTaskByUID: function (taskUID) {
        return this.project.getTask(taskUID);
    },
    getResourceByUID: function (reUID) {
        return this.project.getResource(reUID);
    }
});
