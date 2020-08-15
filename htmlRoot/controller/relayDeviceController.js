/**
 * Created by kagula on 2017-09-14.
 */

function initAngularRelayDeviceController()
{
    app.controller('relayDeviceController', function ($scope, $route) {
        console.log('relayDeviceController...')
        $scope.$route = $route;

        ClearSideMenuChildNode();
        $("#relay_device_item").append("<span class=\"selected\"></span>");
        $("#relay_device_item").addClass("active open");
    });
}

initAngularRelayDeviceController();