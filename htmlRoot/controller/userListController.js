/**
 * Created by kagula on 2017-09-14.
 */
var g_listUserGroup;

function initAugularUserListController()
{
    app.controller('userListController', function ($scope, $http, $route,ngDialog) {
        $scope.$route = $route;

        ClearSideMenuChildNode();
        $("#user_list_item").append("<span class=\"selected\"></span>");
        $("#user_list_item").addClass("active open");
/*
        $http.get("fakeData/listUser.json")
            .then(function (result) {
                $scope.listUser = result.data;
            });
*/
        $http.post("webccDemo/user/listAll.do")
            .then(function (result) {
                if(result.data.code==0)
                {
                    console.log("userListController .../webccDemo/user/listAll.do");
                    for(i=0;i<result.data.list.length;i++)
                    {
                        if(result.data.list[i].loginName=="admin")
                        {
                            result.data.list[i].isEditable = 0;//前端和后台各做控制，防止用户删除超级管理员账号。
                        } else
                        {
                            result.data.list[i].isEditable = result.data.isEditable;
                        }
                    }
                    $scope.listUser = result.data.list;

                    //判断当前账号是否是管理员账号
                    if( g_user_authority != null && g_user_authority.has('ROLE_ADMIN') )
                    {
                        $scope.isAdmin = true;
                    }else {
                        $scope.isAdmin = false;
                    }
                }
            });

        $scope.delUser = function(row, $event) {
            console.log("delUser="+row.id);
            $event.stopPropagation();//阻止冒泡

            bootbox.dialog({
                message: "你要删除["+row.loginName+" "+row.groupName+"]帐号吗?",
                title: "确认",
                buttons: {
                    danger: {
                        label: "删 除",
                        className: "red",
                        callback: function() {
                            var jsonReq={"iduserinfo":row.id};
                            $.ajax({
                                type: "POST",
                                contentType:"application/json",
                                url: "webccDemo/user/del.do",
                                data: JSON.stringify(jsonReq),
                                success: function (data) {
                                    if(data.code==0)
                                    {
                                        //删除操作成功，更新试图
                                        $scope.listUser.splice($scope.listUser.indexOf(row),1);//删除视图中row对象所在行的记录。
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

        $scope.editUser = function(row, $event) {
            $scope.value = true;

            $scope.myJSONObject = {title:'编辑用戶',
                operation:'edit',
                iduserinfo:row.id,
                account:row.loginName,
                isInputUserNameInvalid:true,
                isFocusUserName:false,
                isFocusPassword:true};

            ngDialog.openConfirm({
                template: '../dialog/userDetail.html',
                className: 'ngdialog-theme-plain',
                scope: $scope,
                data: $scope.myJSONObject,
                controller: ['$scope', function($scope) {
                    //model popup 前可以在这里初始化数据。

                    //用户组
                    $http.post("webccDemo/userGroup/list.do")
                        .then(function (result) {
                            if( result.data.code == 0 && result.data.list.length > 0 )
                            {
                                $scope.listGroup = result.data.list;
                                g_listUserGroup = result.data.list;
                                $scope.ngDialogData.myUserGroup = row.groupName;
                            }
                        });

                    //检查两次密码输入是否一样
                    $scope.changePassword = function() {
                        $("#idPassword2Style").attr("class", "form-group");

                        if( $scope.ngDialogData.myPassword !=  $scope.ngDialogData.myPassword2)
                        {
                            $("#idPassword2Style").addClass("has-error");
                            return;
                        }//if
                    };//function
                }],
                preCloseCallback:function(value){
                    //只有调用closeThisDialog关闭窗口，才会关闭窗口前回调这个函数
                    //形参表中最多只能跟一个参数。
                    if(value != "$closeButton")
                    {
                        //检查输入是否合法
                        if( value.myPassword == null || value.myPassword.length < 1)
                        {
                            bootbox.alert({
                                message: "密码不能为空!",
                                size: 'small'
                            });
                            return false;
                        }

                        if( value.myPassword != value.myPassword2)
                        {
                            bootbox.alert({
                                message: "你两次输入的密码必须要一致!",
                                size: 'small'
                            });
                            return false;
                        }

                        //if (confirm('确定要保存修改吗？')) {//这个confirm有点丑，不建议使用
                            //向服务端发送保存数据的请求。
                            console.log("value.iduserinfo=" + value.iduserinfo);

                            //取idGroupInfo
                            var idGroupInfo = -1;
                            for(var i=0;i<g_listUserGroup.length;i++)
                            {
                                if(g_listUserGroup[i].groupName==value.myUserGroup)
                                {
                                    idGroupInfo = g_listUserGroup[i].idGroupInfo;
                                    break;
                                }
                            }//for
                            console.log('value.myUserGroup=' + value.myUserGroup + " ,idGroupInfo=" + idGroupInfo);

                            var jsonReq={"iduserinfo":value.iduserinfo, "password":value.myPassword, "idGroupInfo":idGroupInfo};
                            $.ajax({
                                type: "POST",
                                contentType:"application/json",
                                url: "webccDemo/user/saveEdit.do",
                                data: JSON.stringify(jsonReq),
                                success: function (data) {
                                    if(data.code==0)
                                    {
                                        console.log("如果修改用户信息成功，刷新列表。");
                                        $http.post("webccDemo/user/listAll.do")
                                            .then(function (result) {
                                                if(result.data.code==0)
                                                {
                                                    for(i=0;i<result.data.list.length;i++)
                                                    {
                                                        if(result.data.list[i].loginName=="admin")
                                                        {
                                                            result.data.list[i].isEditable = 0;//前端和后台各做控制，防止用户删除超级管理员账号。
                                                        } else
                                                        {
                                                            result.data.list[i].isEditable = result.data.isEditable;
                                                        }
                                                    }
                                                    $scope.listUser = result.data.list;
                                                    $scope.$apply();//刷新angular js视图
                                                }
                                            });//post
                                    }
                                    else {
                                        console.log(data.message);
                                        bootbox.alert({
                                            message: '修改用户信息失败！',
                                            size: 'small'
                                        });
                                    }
                                },
                                eror: function (data) {
                                    console.log(data);
                                }
                            });

                            return true;
                        //}//confirm
                        return false;//不关闭窗口
                    }//if
                    return true;//关闭窗口
                }//function
            }).then(function(value){
                //console.log('调用confirm(x)函数关闭ngDialog，会跳到这里。 value=' + value );
            }, function(value){
                console.log('调用closeThisDialog(x)函数关闭ngDialog，会跳到这里。 value=' + value );
            });
        };

        $scope.viewUser = function(row, $event) {
            $scope.value = true;

            $scope.myJSONObject = {title:'查看用戶',
                operation:'view',
                account:row.loginName,
                isInputUserNameInvalid:true,
                isFocusUserName:true,
                isFocusPassword:false,
                isHidePassword:true};

            ngDialog.openConfirm({
                template: '../dialog/userDetail.html',
                className: 'ngdialog-theme-plain',
                scope: $scope,
                data: $scope.myJSONObject,
                controller: ['$scope', function($scope) {
                    //用户组
                    $http.post("webccDemo/userGroup/list.do")
                        .then(function (result) {
                            if( result.data.code == 0 && result.data.list.length > 0 )
                            {
                                $scope.listGroup = result.data.list;
                                g_listUserGroup = result.data.list;
                                $scope.ngDialogData.myUserGroup = row.groupName;
                            }
                        });
                }]
            }).then(function(value){
            }, function(value){
            });
        };

        $scope.addUser = function () {
            $scope.value = true;

            $scope.myJSONObject = {title:'添加用戶',
                operation:'add',
                isFocusUserName:true,
                isFocusPassword:false};

            ngDialog.openConfirm({
                template: '../dialog/userDetail.html',
                className: 'ngdialog-theme-plain',
                scope: $scope,
                data: $scope.myJSONObject,
                controller: ['$scope', function($scope) {
                    //here can not access html element.by JQuery selector

                    //用户组
                    $http.post("webccDemo/userGroup/list.do")
                        .then(function (result) {
                            if(result.data.code==0 && result.data.list.length>0)
                            {
                                console.log("userListController->添加用户->.../webccDemo/userGroup/list.do");
                                $scope.listGroup = result.data.list;
                                g_listUserGroup = result.data.list;
                                $scope.ngDialogData.myUserGroup = result.data.list[0].groupName;
                            }
                        });

                    //修改用户名
                    $scope.changeUserName = function() {
                        $("#idUserNameStyle").attr("class", "form-group");

                        if( $scope.ngDialogData.account.length > 12)
                        {
                            $("#idAccountPrompt").html("超出长度范围.");
                            $("#idAccountPrompt").css("display", "block");
                            $("#idUserNameStyle").addClass("has-warning");
                            return;
                        }

                        $.post('/webccDemo/user/isExist.do', 'loginName='+$scope.ngDialogData.account,function (result) {
                            objRes = JSON.parse(result);
                            if(objRes!=null && objRes.hasOwnProperty("code") && objRes.code==0)
                            {
                                console.log("exist " + $scope.ngDialogData.account);
                                $("#idAccountPrompt").html("帐号已经存在！")
                                $("#idAccountPrompt").css("display", "block");
                                $("#idUserNameStyle").addClass("has-error");
                            }
                            else
                            {
                                $("#idAccountPrompt").html("");
                                $("#idAccountPrompt").css("display", "none");
                                //$("#idUserNameStyle").addClass("has-success");
                            }
                        }).error(function() { console.log("network error"); });
                    };

                    //修改密码
                    $scope.changePassword = function() {
                        $("#idPassword2Style").attr("class", "form-group");

                        if( $scope.ngDialogData.myPassword !=  $scope.ngDialogData.myPassword2)
                        {
                            $("#idPassword2Style").addClass("has-error");
                            return;
                        }//if
                    };//function
                }],
                preCloseCallback:function(value){
                    if(value != "$closeButton")
                    {
                        console.log('只有调用closeThisDialog(x)函数关闭ngDialog，才会跳到这里');
                        console.log('value.account=' + value.account);
                        console.log('value.myPassword=' + value.myPassword);
                        //取idGroupInfo
                        var idGroupInfo = -1;
                        for(var i=0;i<g_listUserGroup.length;i++)
                        {
                            if(g_listUserGroup[i].groupName==value.myUserGroup)
                            {
                                idGroupInfo = g_listUserGroup[i].idGroupInfo;
                                break;
                            }
                        }//for
                        console.log('value.myUserGroup=' + value.myUserGroup + " ,idGroupInfo=" + idGroupInfo);

                        if(idGroupInfo < 0)
                        {
                            console.log('错误！ 未定义的idGroupInfo。');
                            return true;
                        }

                        //
                        //检查输入是否合法
                        if( value.account == null || value.myPassword.length < 1)
                        {
                            bootbox.alert({
                                message: "账户名不能为空!",
                                size: 'small'
                            });
                            return false;
                        }

                        if( value.myPassword == null || value.myPassword.length < 1)
                        {
                            bootbox.alert({
                                message: "密码不能为空!",
                                size: 'small'
                            });
                            return false;
                        }

                        if( value.myPassword != value.myPassword2)
                        {
                            bootbox.alert({
                                message: "你两次输入的密码必须要一致!",
                                size: 'small'
                            });
                            return false;
                        }

                        //向服务端发送添加用户的请求
                        var jsonReq={"loginname":value.account,"password":value.myPassword,"idGroupInfo":idGroupInfo};
                        $.ajax({
                            type: "POST",
                            contentType:"application/json",
                            url: "webccDemo/user/add.do",
                            data: JSON.stringify(jsonReq),
                            success: function (data) {
                                if(data.code==0)
                                {
                                    console.log("如果添加用户成功，刷新列表。");
                                    $http.post("webccDemo/user/listAll.do")
                                        .then(function (result) {
                                            if(result.data.code==0)
                                            {
                                                console.log("userListController .../webccDemo/user/listAll.do");
                                                for(i=0;i<result.data.list.length;i++)
                                                {
                                                    if(result.data.list[i].loginName=="admin")
                                                    {
                                                        result.data.list[i].isEditable = 0;//前端和后台各做控制，防止用户删除超级管理员账号。
                                                    } else
                                                    {
                                                        result.data.list[i].isEditable = result.data.isEditable;
                                                    }
                                                }
                                                $scope.listUser = result.data.list;
                                                $scope.$apply();//刷新angular js视图
                                            }
                                        });
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

                        //关闭窗口
                        return true;
                    } else {
                        console.log('用户点了右上角的close按钮。');
                        return true;
                    }
                }
            }).then(function(value){
            }, function(value){
                //关闭dialog后才会到这里。
            });
        };//add user
    });
}
initAugularUserListController();