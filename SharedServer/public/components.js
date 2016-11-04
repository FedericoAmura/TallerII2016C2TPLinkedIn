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
  		
  		function stopEventPropagation() {
  			var e = window.event;
			e.cancelBubble = true;
			if (e.stopPropagation) e.stopPropagation();
  		}
  		
  		$scope.delete = function() {
  			$http.delete('/categories/'+escape('outdoor activities')).then(
  				function (response) {
  					Materialize.toast(ctrl.category.name+' deleted', 3000, 'rounded');
  				}, function (rejection) {
  					Materialize.toast('Error deleting '+ctrl.category.name, 3000, 'rounded');
  				}
  			);
  			stopEventPropagation();
		};
		
		$scope.edit = function() {
			document.getElementById('editCategoryModal'+ctrl.category.name).style.display = "block";
  			stopEventPropagation();
		};

		$scope.closeEditCategory = function() {
			document.getElementById('editCategoryModal'+ctrl.category.name).style.display = "none";
		}
	},
  	template:
  	'<li>'+
  		'<div class="collapsible-header contentdetails"><a class="button btn-flat btn-small" ng-click="delete()"><i class="tiny material-icons">delete</i></a><a class="button btn-flat btn-small" ng-click="edit()"><i class="tiny material-icons">mode_edit</i></a>{{category.name}}</div>'+
  		'<div class="collapsible-body"><p>{{category.description}}</p></div>'+
  		'<!-- modal edit category -->'+
  		'<div id="editCategoryModal{{category.name}}" class="modal-background">'+
  			'<div class="modal-content">'+
				'<div class="modal-header">'+
					'<span class="close" ng-click="closeEditCategory()">X</span>'+
					'<h5>Edit category</h5>'+
				'</div>'+
				'<div class="modal-body">'+
					'<form method="put" action="/categories">'+
						'<input type="text" class="active validate" name="name" placeholder="Name" value="{{category.name}}" required><br>'+
						'<input type="text" class="validate" name="description" placeholder="Description" value="{{category.description}}" required><br>'+
						'<button type="submit" value="Add" class="btn waves-effect waves-green blue">Edit</button>'+
					'</form>'+
				'</div>'+
			'</div>'+
  		'</div>'+
  	'</li>',
  	replace: true
  };
});