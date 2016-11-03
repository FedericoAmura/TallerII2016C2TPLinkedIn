//creamos el html propio
var components = angular.module('components',[]);

//componente html angular de items de categorias
components.directive('categorylist', function() {
  return {
  	restrict: 'E',
  	transclude: false,
  	scope: {
  		category: "="
  	},
  	controller: function ($scope, $element, $attrs, $http) {
  		var ctrl = this;
  		ctrl.category = $scope.category;
  		
  		$scope.delete = function() {
  			$http.delete('/categories/'+escape('outdoor activities')).then(
  				function (response) {
  					console.log(response);
  					console.log(response.data);
  					console.log('eliminamos outdoor activities');
  				}, function (rejection) {
  					console.log(rejection);
  					console.log(rejection.data);
  					console.log('no eliminamos nada');
  				}
  			);
  			console.log('frenamos el evento');
  			var e = window.event;
			e.cancelBubble = true;
			if (e.stopPropagation) e.stopPropagation();
		};
		
		$scope.edit = function() {
			showEditCategory('category','descripcion');
  			console.log('frenamos el evento');
  			var e = window.event;
			e.cancelBubble = true;
			if (e.stopPropagation) e.stopPropagation();
		};
  	},
  	template:
  	'<li>'+
  		'<div class="collapsible-header contentdetails"><a class="button btn-flat btn-small" ng-click="delete()"><i class="tiny material-icons">delete</i></a><a class="button btn-flat btn-small" ng-click="edit()"><i class="tiny material-icons">mode_edit</i></a>{{category.name}}</div>'+
  		'<div class="collapsible-body"><p>{{category.description}}</p></div>'+
  	'</li>',
  	replace: true
  };
});