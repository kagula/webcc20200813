/**
 * Created by kagula on 2017-09-14.
 */

function initAngularVibrationFiberController()
{
    app.controller('vibrationFiberController', function ($scope, $route) {
        console.log('vibrationFiberController...')
        $scope.$route = $route;

        ClearSideMenuChildNode();
        $("#vibration_fiber_item").append("<span class=\"selected\"></span>");
        $("#vibration_fiber_item").addClass("active open");
    });
}
initAngularVibrationFiberController();