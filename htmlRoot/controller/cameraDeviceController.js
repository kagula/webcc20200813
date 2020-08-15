/**
 * Created by kagula on 2017-09-14.
 */

function initAngularCameraDeviceController()
{
    app.controller('cameraDeviceController', function ($scope, $route) {
        console.log('cameraDeviceController...')
        $scope.$route = $route;

        ClearSideMenuChildNode();
        $("#camera_item").append("<span class=\"selected\"></span>");
        $("#camera_item").addClass("active open");
    });
}

initAngularCameraDeviceController();