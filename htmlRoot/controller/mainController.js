/**
 * Created by kagula on 2017-07-26.
 *
 * 对Angular JS的初始化移到了main.html
 */

var app;
var g_user;//当前用户信息
var g_user_authority;//当前用户的权限

function initAngular()
{
    console.log('main.html init');
    app = angular.module('routingDemoApp',['ngRoute','ngSanitize','ngDialog']);

//这里我很起奇怪 focusMe 为什么要映射成 focus-me 这个属性名字。
    app.directive('focusMe', ['$timeout', '$parse', function ($timeout, $parse) {
        return {
            //scope: true,   // optionally create a child scope
            link: function (scope, element, attrs) {
                var model = $parse(attrs.focusMe);
                scope.$watch(model, function (value) {
                    //console.log('value=', value);//debug purpose
                    if (value === true) {
                        $timeout(function () {
                            element[0].focus();
                        });
                    }
                });
                // to address @blesh's comment, set attribute value to 'false'
                // on blur event:
                element.bind('blur', function () {
                    //console.log('blur');//debug purpose
                    scope.$apply(model.assign(scope, false));
                });
            }
        };
    }]);
}

initAngular();

function ClearSideMenuChildNode()
{
    /*
    $("#sideMenu").each(function() {
        console.log($(this).val());
    });
*/
    $("#sideMenu").children(".nav-item").removeClass("active open");
    $("span.selected").remove();
}