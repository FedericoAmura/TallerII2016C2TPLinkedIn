//creamos aplicacion angular con sus controllers
var jobifyLogin = angular.module('jobifyLogin', []);

//controlador de login
jobifyLogin.controller('LoginController', function($scope, $http, $window) {
	$scope.login = function() {
		console.log('hacemos el login');
		console.log('usuario '+$scope.username);
		data = {username:$scope.username,password:$scope.password};
		$http.post('/login',data).
		then(function (response) {
			$window.location.href = response.data.redirect;
		}, function (response) {
			$scope.username = '';
			$scope.password = '';
			Materialize.toast('Wrong username or password', 3000, 'rounded');
		});
	}
});


$(document).ready(function() {
	$(".dropdown-button").dropdown();
});
