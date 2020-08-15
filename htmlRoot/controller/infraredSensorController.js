/**
 * Created by kagula on 2017-09-14.
 */

function initAngularInfraredSensorController()
{
    app.controller('infraredSensorController', function ($scope, $route) {
        console.log('infraredSensorController...')
        $scope.$route = $route;

        ClearSideMenuChildNode();
        $("#infrared_sensor_item").append("<span class=\"selected\"></span>");
        $("#infrared_sensor_item").addClass("active open");
    });
}

initAngularInfraredSensorController();
