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
  	var categoriesList = this;
  	var getCategories = function() {
  		console.log('actualizamos las categories');
  		$http.get('/categories')
  		.then(function(response) {
  			categoriesList.categories = response.data.categories;
  		});
  	}
  	getCategories();
  	$scope.categories = categoriesList.categories;
  	
  	$scope.newCategory = function() {
  		Materialize.toast('Category '+$scope.name+' created', 3000, 'rounded');
  		closeNewCategory();
  		$scope.name = '';
  		$scope.description = '';
  		/*$http({
  			method:'POST',
  			url: '/categories',
  			headers: {'Content-Type': 'application/json'},
  			data: {'name':'prueba','description':'descripcion'}
  		}).then(
  			function (response) {
				console.log('creamos una category');
				getCategories();
			}, function (rejection) {
				console.log('no creamos nada');
			}
  		);*/
		/*var config = {headers:{'Content-Type':'application/x-www-form-urlencoded'}};
		var data = $.param({
		    json: JSON.stringify({category:{name:$scope.name,description:$scope.description}})
		});*/

  		/*$http.post('/categories',data,config)
		.success(function (data, status, headers, config) {
			console.log('todo bien al hacer newCategory')
			getCategories();
		})
		.error(function (data, status, header, config) {
			console.log('error al hacer newCategory')
		});*/
	};
});
  
