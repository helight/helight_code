/* 标准方法：加载、保存、调试项目，弹出任务面板、日历面板。
-----------------------------------------------------------------------------*/

var ServicesPath = PATH + "scripts/plusproject/services/";    //Ajax交互路径（根据实际项目部署路径，需要修改）

var LoadProjectUrl = ServicesPath + 'load.php';
var SaveProjectUrl = ServicesPath + 'save.php';

function LoadProject(params, project, callback) {
    if (typeof params != "object") params = { projectuid: params };

    project.loading();
    $.ajax({
        url: LoadProjectUrl,
        data: params,
        cache: false,
        success: function (text) {
            var dataProject = mini.decode(text);
            project.loadData(dataProject);
            if (callback) callback(project);
            project.unmask();
        },
        error: function (jqXHR, textStatus, errorThrown) {
            alert("加载失败, 错误码：" + textStatus);
            project.unmask();
        }
    });
}

function SaveProject(project, callback, params) {

    project.mask("数据保存中，请稍后...");
    var dataProject = project.getData();
    dataProject.RemovedTasks = project.getRemovedTasks();
    dataProject.pid = 100;
    var json = mini.encode(dataProject);

    if (!params) params = {};
    params.project = json;

    $.ajax({
        url: SaveProjectUrl,
        type: "post",
        data: params,
        success: function (text) {
            alert("保存成功,项目UID：" + text);
            project.acceptChanges();
            if (callback) callback(project);
            project.unmask();
        },
        error: function (jqXHR, textStatus, errorThrown) {
            alert("保存失败, 错误码：" + textStatus);
            project.unmask();
        }
    });
}

function TrackProject(project) {
    var dataProject = project.getData();
    var json = mini.encode(dataProject);
    document.write(json);
    //把生成的项目JSON数据发送给技术支持人员，方便技术人员进行调试定位项目问题
}

function LoadJSONProject(url, project, callback) {
    project.loading();
    $.ajax({
        url: url,
        cache: false,
        success: function (text) {
            var dataProject = mini.decode(text);

            project.loadData(dataProject);
            if (callback) callback(project);
            project.unmask();

        },
        error: function (jqXHR, textStatus, errorThrown) {
            alert("加载失败, 错误码：" + textStatus);
            project.unmask();
        }
    });
}

//创建任务面板

var taskWindow = null;

function ShowTaskWindow(project) {
    var task = project.getSelected();
    if (task) {
        if (!taskWindow) {
            taskWindow = new TaskWindow();
        }
        taskWindow.setTitle("编辑任务");
        taskWindow.show();
        taskWindow.setData(task, project,
            function (action) {
                
                if (action == 'ok') {
                    try {
                        var taskData = taskWindow.getData();
                        project.updateTask(task, taskData);
                    } catch (ex) {
                        alert("error:"+ex.message);
                        return false;
                    }
                }
            }
        );
    } else {
        alert("请先选择任务");
    }
}
var cpWindow = null;
function ShowCreateProjectWindow(project) {
        if (!cpWindow) {
            cpWindow = new CreateProjectWindow();
        }
        cpWindow.setTitle("新建项目");
        cpWindow.show();
        cpWindow.setData(task, project,
            function (action) {
                
                if (action == 'ok') {
                    try {
                        var taskData = cpWindow.getData();
                        project.updateTask(task, taskData);
                    } catch (ex) {
                        alert("error:"+ex.message);
                        return false;
                    }
                }
            }
        ); 
}

//日历面板

var calendarWindow = null;

function ShowCalendarWindow(project) {
    if (!calendarWindow) {
        calendarWindow = new CalendarWindow();
    }
    calendarWindow.show();
    calendarWindow.setData(project.getCalendars(), project,
        function (action) {
            if (action == "ok") {
            
                var calendars = calendarWindow.getData();
                project.setCalendars(calendars);
            }
        }
    );
}
