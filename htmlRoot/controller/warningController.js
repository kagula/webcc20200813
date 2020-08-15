/**
 * Created by kagula on 2017-09-14.
 */

function initAngularWarningController()
{
    app.factory('WarningInfo', function($rootScope) {
        var Service = {};
        var ws;
        var warningList = [];
        /*
         也可以初始条数据。
         var warningList = [
         {
         "id":"5",
         "dateTime":"2017-09-15 01:01:26",
         "locate":"3,49m",
         "device":"OTDR-02",
         "eventType":"线路不通"
         }
         ];
         */
        /*
         Service.onMessage = function (message) {
         console.log('message.data='+message.data);
         //warningList.push(JSON.parse(message.data));
         };
         */

        Service.onClose = function () {
            console.log('onClose');
        };
        Service.onOpen = function () {
            console.log('onOpen');
        };
        Service.onError = function (error) {
            console.log('opError');
        };

        Service.connect = function (url,callback) {
            // (Re)connect
            ws = new WebSocket(url);
            // Reattaching handlers to object
            //ws.onmessage = Service.onMessage
            ws.onmessage = function (evt) {
                callback(evt);
            };;
            ws.onclose = Service.onClose;
            ws.onopen = Service.onOpen;
            ws.onerror = Service.onError;
        }

        Service.get =  function () {
            return warningList;
        };

        return Service;
    });

    app.controller('WarningController', function ($scope, $http, $route,WarningInfo) {
        console.log('WarningController...')
        $scope.$route = $route;

        ClearSideMenuChildNode();
        $("#warning_menu_item").append("<span class=\"selected\"></span>");
        $("#warning_menu_item").addClass("active open");

        $scope.listWarnings = WarningInfo.get();//不初始的话，就是个空对象，没有任何成员方法。
        WarningInfo.connect("ws://127.0.0.1:9002/webccDemo/ws/WarningInfo.do",function (evt) {
            $scope.$apply(function () {
                console.log("From WS:" + evt.data);
                $scope.listWarnings.push(JSON.parse(evt.data));
            });
        });

        //$scope.listWarnings = WarningInfo;
        /*
         $http.get("fakeData/listWarnings.json")
         .then(function (result) {
         $scope.listWarnings = result.data;
         });
         */

        $scope.warningDetail = function(row, $event) {
            console.log("warningDetail="+row.id);
            $event.stopPropagation();//阻止冒泡
        };

        //手动添加warningInfo行的测试
        /*
         $scope.addWarning = function() {
         console.log('addWarning');
         $scope.listWarnings.push(  {
         "id":"1000",
         "dateTime":"2017-09-14 10:45:01",
         "locate":"227m",
         "device":"OTDR-01",
         "eventType":"低于阈值"
         });
         };
         */
    });
}

initAngularWarningController();