$(document).ready(function() {
	$(".dropdown-button").dropdown();
});

//creamos aplicacion angular con sus controllers
var jobify = angular.module('jobify', ['components']);
 
//controlador categorias
jobify.controller('CategoriesController', function($scope, $http) {
  	var categoriesList = this;
  	$http.get('/categories')
  		.then(function(response) {
  			categoriesList.categories = response.data.categories;
  		});
  	$scope.categories = categoriesList.categories;
  });
  
