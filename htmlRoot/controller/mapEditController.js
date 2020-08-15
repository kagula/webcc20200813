/**
 * Created by kagula on 2017-09-14.
 */

function intitAngularMapEditController()
{
    app.controller('mapEditController', function ($scope, $route) {
        console.log('mapEditController...')
        $scope.$route = $route;

        ClearSideMenuChildNode();
        $("#map_edit_item").append("<span class=\"selected\"></span>");
        $("#map_edit_item").addClass("active open");
    });
}
intitAngularMapEditController();