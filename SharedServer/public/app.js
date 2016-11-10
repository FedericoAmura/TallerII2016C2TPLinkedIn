$(document).ready(function() {
	$(".dropdown-button").dropdown();
});

//funciones para mostrar los modales de categorias
function showNewCategory() {
	document.getElementById('newCategoryModal').style.display = "block";
}

function closeNewCategory() {
	document.getElementById('newCategoryModal').style.display = "none";
}

//creamos aplicacion angular con sus controllers
var jobify = angular.module('jobify', ['components']);
 
//controlador categorias
jobify.controller('CategoriesController', function($scope, $http) {
	$http.defaults.headers.post["Content-Type"] = "application/json";
	//conseguimos las categories
  	$scope.getCategories = function() {
  		console.log('actualizamos las categories');
  		$http.get('/categories')
  		.then(function(response) {
  			$scope.categories = response.data.categories;
  		});
  	}
  	$scope.getCategories();
  	
  	$scope.newCategory = function() {
  		closeNewCategory();
  		data = {category:{name:$scope.categoryname,description:$scope.description}};
  		$http.post('/categories',data).
  		then(function (response) {
  			Materialize.toast('Category '+$scope.categoryname+' created', 3000, 'rounded');
  			$scope.categories.push(response.data.category);
			$scope.categoryname = '';
			$scope.description = '';
  		}, function () {
			Materialize.toast('Could not create category '+$scope.categoryname, 3000, 'rounded');
			$scope.categoryname = '';
			$scope.description = '';
  		});
	};
});
  
