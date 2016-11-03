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

function showEditCategory(name, description) {
	document.getElementById('editCategoryModal').style.display = "block";
}

function closeEditCategory(name, description) {
	document.getElementById('editCategoryModal').style.display = "none";
}

//creamos aplicacion angular con sus controllers
var jobify = angular.module('jobify', ['components']);
 
//controlador categorias
jobify.controller('CategoriesController', function($scope, $http) {
	//conseguimos las categories
  	var categoriesList = this;
  	$http.get('/categories')
  		.then(function(response) {
  			categoriesList.categories = response.data.categories;
  		});
  	$scope.categories = categoriesList.categories;
  	
  	$scope.newCategory = function() {
  		$http({
  			method:'POST',
  			url: '/categories',
  			headers: {'Content-Type': 'form-data'},
  			data: {'name':'prueba','description':'descripcion'}
  		}).then(
  			function (response) {
				console.log(response);
				console.log(response.data);
				console.log('creamos una category');
			}, function (rejection) {
				console.log(rejection);
				console.log(rejection.data);
				console.log('no creamos nada');
			}
  		);
  	};
});
  
