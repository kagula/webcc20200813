/**
 * Created by kagula on 2017-09-14.
 */

function initAngularUnknownController() {
    app.controller('UnknownController', function ($scope, $route) {
        console.log('unknown controller...')
        $scope.$route = $route;
    });
}

initAngularUnknownController();