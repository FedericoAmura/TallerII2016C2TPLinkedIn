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
  		
  		function stopEventPropagation() {
  			var e = window.event;
			e.cancelBubble = true;
			if (e.stopPropagation) e.stopPropagation();
  		}
  		
  		$scope.delete = function() {
  			$http.delete('/categories/'+escape($scope.category.name)).then(
  				function (response) {
  					Materialize.toast($scope.category.name+' deleted', 3000, 'rounded');
  					$scope.$parent.getCategories();
  				}, function (rejection) {
  					Materialize.toast('Error deleting '+$scope.category.name, 3000, 'rounded');
  				}
  			);
  			stopEventPropagation();
		};
		
		$scope.edit = function() {
			$scope.closeEditCategory();
			data = {category:{name:$scope.category.newname,description:$scope.category.newdescription}};
			$http.put('/categories/'+escape($scope.category.name),data).then(
  				function (response) {
  					Materialize.toast($scope.category.name+' updated', 3000, 'rounded');
  					$scope.category.name = $scope.category.newname;
  					$scope.category.description = $scope.category.newdescription;
  					$scope.category.newname = '';
  					$scope.category.newdescription = '';
  				}, function (rejection) {
  					Materialize.toast('Error updating '+$scope.category.name, 3000, 'rounded');
  					$scope.category.newname = '';
  					$scope.category.newdescription = '';
  				}
  			);
  			stopEventPropagation();
		}
		
		$scope.showEdit = function() {
			document.getElementById('editCategoryModal'+$scope.category.name).style.display = "block";
  			stopEventPropagation();
		};

		$scope.closeEditCategory = function() {
			document.getElementById('editCategoryModal'+$scope.category.name).style.display = "none";
		}
	},
  	template:
  	'<li>'+
  		'<div class="collapsible-header contentdetails"><a class="button btn-flat btn-small" ng-click="delete()"><i class="tiny material-icons">delete</i></a><a class="button btn-flat btn-small" ng-click="showEdit()"><i class="tiny material-icons">mode_edit</i></a>{{category.name}}</div>'+
  		'<div class="collapsible-body"><p>{{category.description}}</p></div>'+
  		'<!-- modal edit category -->'+
  		'<div id="editCategoryModal{{category.name}}" class="modal-background">'+
  			'<div class="modal-content">'+
				'<div class="modal-header">'+
					'<span class="close" ng-click="closeEditCategory()">X</span>'+
					'<h5>Edit category {{category.name}} </h5>'+
				'</div>'+
				'<div class="modal-body">'+
					'<div>'+
						'<input type="text" class="active validate" ng-model="category.newname" placeholder="New name"><br>'+
						'<input type="text" class="validate" ng-model="category.newdescription" placeholder="New description"><br>'+
						'<input type="submit" value="Edit" class="btn waves-effect waves-green blue" ng-click="edit()"/>'+
					'</div>'+
				'</div>'+
			'</div>'+
  		'</div>'+
  	'</li>',
  	replace: true
  };
});