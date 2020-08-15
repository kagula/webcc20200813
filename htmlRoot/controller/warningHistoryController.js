/**
 * Created by kagula on 2017-09-14.
 */

function initAngularWarningHistoryController()
{
    app.controller('warningHistoryController', function ($scope, $route) {
        console.log('warningHistoryController...')
        $scope.$route = $route;

        ClearSideMenuChildNode();
        $("#waning_history_item").append("<span class=\"selected\"></span>");
        $("#waning_history_item").addClass("active open");
    });
}
initAngularWarningHistoryController();