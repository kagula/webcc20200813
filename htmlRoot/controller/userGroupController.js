/**
 * Created by kagula on 2017-09-14.
 */
var g_listMap;
function initAngularUserGroupController()
{
    app.controller('userGroupController', function ($scope, $http, $route, ngDialog) {
        console.log('userGroupController...')
        $scope.$route = $route;

        ClearSideMenuChildNode();
        $("#user_group_item").append("<span class=\"selected\"></span>");
        $("#user_group_item").addClass("active open");

        /*
        $http.get("fakeData/listUserGroup.json")
            .then(function (result) {
                $scope.listGroup = result.data;
            });
        */
        $http.post("DAS2017/userGroup/list.do")
            .then(function (result) {
                if(result.data.code==0)
                {
                    $scope.listGroup = result.data.list;

                    //判断当前账号是否是管理员账号，如果不是不显示“添加用户组”按钮。
                    if( g_user_authority != null && g_user_authority.has('ROLE_ADMIN') )
                    {
                        $scope.isAdmin = true;
                    }else {
                        $scope.isAdmin = false;
                    }
                }
            });

        $scope.delGroup = function(row, $event) {
            console.log("delGroup id=" + row.id);
            $event.stopPropagation();//阻止冒泡

            bootbox.dialog({
                message: "你要删除[" + row.groupName + "]用户组吗?",
                title: "确认",
                buttons: {
                    danger: {
                        label: "删 除",
                        className: "red",
                        callback: function() {
                            var jsonReq={"idGroupInfo":row.idGroupInfo};
                            $.ajax({
                                type: "POST",
                                contentType:"application/json",
                                url: "DAS2017/userGroup/del.do",
                                data: JSON.stringify(jsonReq),
                                success: function (data) {
                                    if(data.code==0)
                                    {
                                        //删除操作成功，更新试图
                                        $scope.listGroup.splice($scope.listGroup.indexOf(row),1);//删除视图中row对象所在行的记录。
                                        $scope.$apply();//刷新angular js视图
                                    }
                                    else {
                                    }
                                },
                                eror: function (data) {
                                    console.log(data);
                                }
                            });
                        }
                    },
                    main: {
                        label: "取 消",
                        className: "green",
                        callback: function() {
                        }
                    }
                }
            });
        };//function

        $scope.editGroup = function(row, $event) {
            $scope.myJSONObject = {title:'编辑用戶组',
                idGroupInfo:row.idGroupInfo,
                groupName:row.groupName,
                mapName:row.mapName,
                operation:'edit',
                isFocusGroupName:true,
                isFocusMapName:false};

            ngDialog.openConfirm({
                template: '../dialog/userGroupDetail.html',
                className: 'ngdialog-theme-plain',
                scope: $scope,
                data: $scope.myJSONObject,
                controller: ['$scope', function($scope) {
                    $scope.changeGroupName = function() {
                        $("#idGroupNameStyle").attr("class", "form-group");

                        if( $scope.ngDialogData.groupName.length > 12)
                        {
                            $("#idGroupNamePrompt").html("超出长度范围.");
                            $("#idGroupNamePrompt").css("display", "block");
                            $("#idGroupNameStyle").addClass("has-warning");
                            return;
                        }

                        $.post('/DAS2017/userGroup/isExist.do', 'groupName='+$scope.ngDialogData.groupName,function (result) {
                            objRes = JSON.parse(result);
                            if(objRes!=null && objRes.hasOwnProperty("code") && objRes.code==0 && objRes.isExist==1)
                            {
                                console.log("exist " + $scope.ngDialogData.account);
                                //重名也有可能是用户没有修改原来的名字，所以这里改为简单提示下。
                                $("#idGroupNameStyle").addClass("has-error");
                            }
                            else
                            {
                                $("#idGroupNamePrompt").html("");
                                $("#idGroupNamePrompt").css("display", "none");
                                //$("#idUserNameStyle").addClass("has-success");
                            }
                        }).error(function() { console.log("network error"); });
                    };
                    //初始化map select控件
                    $http.post("DAS2017/map/list.do")
                        .then(function (result) {
                            if(result.data.code==0 && result.data.list.length>0)
                            {
                                $scope.listMap = result.data.list;
                                g_listMap = result.data.list;
                                $scope.ngDialogData.myMap= row.mapName;
                            }
                        });//post
                }],
                preCloseCallback:function(value){
                    if(value != "$closeButton")
                    {
                        console.log('value.idGroupInfo=' + value.idGroupInfo);
                        console.log('value.groupName=' + value.groupName);
                        console.log('value.myMap=' + value.myMap);

                        if(value.groupName == null || value.groupName.length < 1)
                        {
                            bootbox.alert({
                                message: "组名不能为空!",
                                size: 'small'
                            });
                            return false;
                        }//if

                        //get idMapInfo
                        var idMapInfo=-1;
                        for(var i=0;i<g_listMap.length;i++)
                        {
                            if(g_listMap[i].mapName==value.myMap)
                            {
                                idMapInfo = g_listMap[i].idMapInfo;
                                break;
                            }
                        }//for
                        console.log("idMapInfo=" + idMapInfo);

                        var jsonReq={"idGroupInfo":value.idGroupInfo, "groupName":value.groupName, "idMapInfo":idMapInfo};
                        $.ajax({
                            type: "POST",
                            contentType:"application/json",
                            url: "DAS2017/userGroup/update.do",
                            data: JSON.stringify(jsonReq),
                            success: function (data) {
                                if(data.code==0)
                                {
                                    $http.post("DAS2017/userGroup/list.do")
                                        .then(function (result) {
                                            if(result.data.code==0)
                                            {
                                                $scope.listGroup = result.data.list;

                                                //判断当前账号是否是管理员账号，如果不是不显示“添加用户组”按钮。
                                                if( g_user_authority != null && g_user_authority.has('ROLE_ADMIN') )
                                                {
                                                    $scope.isAdmin = true;
                                                }else {
                                                    $scope.isAdmin = false;
                                                }
                                                //不知道为什么这里使用$scope.$apply();会出错。
                                                //但是不调用apply的话也能正确刷新视图。
                                            }
                                        });//post
                                }
                                else {
                                    console.log(data.message);
                                    bootbox.alert({
                                        message: '修改用户组失败！',
                                        size: 'small'
                                    });
                                }
                            },
                            eror: function (data) {
                                console.log(data);
                            }
                        });
                    }//if not close button fired
                    //close dialog
                    return true;
                }//function
            }).then(function(value){
            }, function(value){
            });
        };//editGroup

        $scope.addGroup = function(row, $event) {
            $scope.myJSONObject = {title:'添加用戶组',
                operation:'add',
                isFocusGroupName:true,
                isFocusMapName:false};

            ngDialog.openConfirm({
                template: '../dialog/userGroupDetail.html',
                className: 'ngdialog-theme-plain',
                scope: $scope,
                data: $scope.myJSONObject,
                controller: ['$scope', function($scope) {

                    $scope.changeGroupName = function() {
                        $("#idGroupNameStyle").attr("class", "form-group");

                        if( $scope.ngDialogData.groupName.length > 12)
                        {
                            $("#idGroupNamePrompt").html("超出长度范围.");
                            $("#idGroupNamePrompt").css("display", "block");
                            $("#idGroupNameStyle").addClass("has-warning");
                            return;
                        }

                        $.post('/DAS2017/userGroup/isExist.do', 'groupName='+$scope.ngDialogData.groupName,function (result) {
                            objRes = JSON.parse(result);
                            if(objRes!=null && objRes.hasOwnProperty("code") && objRes.code==0 && objRes.isExist==1)
                            {
                                console.log("exist " + $scope.ngDialogData.account);
                                $("#idGroupNamePrompt").html("用户组已经存在！")
                                $("#idGroupNamePrompt").css("display", "block");
                                $("#idGroupNameStyle").addClass("has-error");
                            }
                            else
                            {
                                $("#idGroupNamePrompt").html("");
                                $("#idGroupNamePrompt").css("display", "none");
                                //$("#idUserNameStyle").addClass("has-success");
                            }
                        }).error(function() { console.log("network error"); });
                    };

                    //初始化map select控件
                    $http.post("DAS2017/map/list.do")
                        .then(function (result) {
                            if(result.data.code==0 && result.data.list.length>0)
                            {
                                $scope.listMap = result.data.list;
                                g_listMap = result.data.list;
                                $scope.ngDialogData.myMap = result.data.list[0].mapName;
                            }
                        });//post
                }],
                preCloseCallback:function(value){
                    if(value != "$closeButton")
                    {
                        console.log('value.groupName=' + value.groupName);
                        console.log('value.myMap=' + value.myMap);

                        if(value.groupName==null || value.groupName.length<1)
                        {
                            bootbox.alert({
                                message: "组名不能为空!",
                                size: 'small'
                            });
                            return false;
                        }

                        //get idMapInfo
                        var idMapInfo=-1;
                        for(var i=0;i<g_listMap.length;i++)
                        {
                            if(g_listMap[i].mapName==value.myMap)
                            {
                                idMapInfo = g_listMap[i].idMapInfo;
                                break;
                            }
                        }//for
                        console.log("idMapInfo=" + idMapInfo);
                        //submit data
                        var jsonReq={"groupName":value.groupName, "idMapInfo":idMapInfo};
                        $.ajax({
                            type: "POST",
                            contentType:"application/json",
                            url: "DAS2017/userGroup/add.do",
                            data: JSON.stringify(jsonReq),
                            success: function (data) {
                                if(data.code==0)
                                {
                                    $http.post("DAS2017/userGroup/list.do")
                                        .then(function (result) {
                                            if(result.data.code==0)
                                            {
                                                $scope.listGroup = result.data.list;

                                                //判断当前账号是否是管理员账号，如果不是不显示“添加用户组”按钮。
                                                if( g_user_authority != null && g_user_authority.has('ROLE_ADMIN') )
                                                {
                                                    $scope.isAdmin = true;
                                                }else {
                                                    $scope.isAdmin = false;
                                                }
                                                //不知道为什么这里使用$scope.$apply();会出错。
                                                //但是不调用apply的话也能正确刷新视图。
                                            }
                                        });//post
                                }
                                else {
                                    console.log(data.message);
                                    bootbox.alert({
                                        message: '添加用户失败，请检查账号是否已经存在！',
                                        size: 'small'
                                    });
                                }
                            },
                            eror: function (data) {
                                console.log(data);
                            }
                        });
                    }//if not close button fired
                    //close dialog
                    return true;
                }//function
            }).then(function(value){
            }, function(value){
            });
        };//add group
    });
}

initAngularUserGroupController();