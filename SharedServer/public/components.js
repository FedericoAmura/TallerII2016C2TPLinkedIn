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
  	controller: function ($scope, $element, $attrs) {
  		var ctrl = this;
  		console.log($scope.category);
  		ctrl.category = $scope.category;
  	},
  	template:
  	'<li>'+
  		'<div class="collapsible-header">{{category.name}}</div>'+
  		'<div class="collapsible-body"><p>{{category.description}}</p></div>'+
  	'</li>',
  	replace: true
  };
});