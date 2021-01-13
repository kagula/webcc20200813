/**
 * Created by kagula on 2017-09-14.
 */

 var url_prefix="";//"http://127.0.0.1:8080/"

function initAugularConfig()
{
        app.config(['$routeProvider', function($routeProvider){
                $routeProvider
                    .when('/',{
                            templateUrl: 'page/warning.html',
                            controller: 'WarningController'})
                    .when('/Warning',{
                            templateUrl: 'page/warning.html',
                            controller: 'WarningController'})
                    .when('/warningHistory',{
                            templateUrl: 'page/warningHistory.html',
                            controller: 'warningHistoryController'})
                    .when('/mapEdit', {
                            templateUrl: 'page/mapEdit.html',
                            controller: 'mapEditController'})
                    .when('/vibrationFiber', {
                            templateUrl: 'page/vibrationFiber.html',
                            controller: 'vibrationFiberController'})
                    .when('/relayDevice', {
                            templateUrl: 'page/relayDevice.html',
                            controller: 'relayDeviceController'})
                    .when('/infraredSensor', {
                            templateUrl: 'page/infraredSensor.html',
                            controller: 'infraredSensorController'})
                    .when('/cameraDevice', {
                            templateUrl: 'page/cameraDevice.html',
                            controller: 'cameraDeviceController'})
                    .when('/userList', {
                            templateUrl: 'page/userList.html',
                            controller: 'userListController'})
                    .when('/userGroup', {
                            templateUrl: 'page/userGroup.html',
                            controller: 'userGroupController'})
                    .otherwise({
                            templateUrl: 'page/unknown.html',
                            controller: 'UnknownController'});
        }]);
}

initAugularConfig();