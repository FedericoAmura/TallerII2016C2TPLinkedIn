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
		
		$scope.showEditCategory = function() {
			document.getElementById('editCategoryModal'+$scope.category.name).style.display = "block";
  			stopEventPropagation();
		};

		$scope.closeEditCategory = function() {
			document.getElementById('editCategoryModal'+$scope.category.name).style.display = "none";
		}
	},
  	template:
  	'<li>'+
  		'<div class="collapsible-header contentdetails"><a class="button btn-flat btn-small" ng-click="delete()"><i class="tiny material-icons">delete</i></a><a class="button btn-flat btn-small" ng-click="showEditCategory()"><i class="tiny material-icons">mode_edit</i></a>{{category.name}}</div>'+
  		'<div class="collapsible-body light-green lighten-1">'+
  			'<p>Description: {{category.description}}</p>'+
  		'</div>'+
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

//componente html angular de items de skills
components.directive('skilllist', function() {
  return {
  	restrict: 'E',
  	transclude: false,
  	scope: {
  		skill: "="
  	},
  	controller: function ($scope, $element, $attrs, $http) {
  		
  		$scope.delete = function() {
  			$http.delete('/skills/categories/'+escape($scope.skill.category)+'/'+escape($scope.skill.name)).then(
  				function (response) {
  					Materialize.toast($scope.skill.name+' deleted', 3000, 'rounded');
  					$scope.$parent.getSkills();
  				}, function (rejection) {
  					Materialize.toast('Error deleting '+$scope.skill.name, 3000, 'rounded');
  				}
  			);
  			stopEventPropagation();
		};
		
		$scope.edit = function() {
			$scope.closeEditSkill();
			data = {skill:{name:$scope.skill.newname,description:$scope.skill.newdescription,category:$scope.skill.newcategory}};
			$http.put('/skills/categories/'+escape($scope.skill.category)+'/'+escape($scope.skill.name),data).then(
  				function (response) {
  					Materialize.toast($scope.skill.name+' updated', 3000, 'rounded');
  					$scope.skill.name = $scope.skill.newname;
  					$scope.skill.description = $scope.skill.newdescription;
  					$scope.skill.newname = '';
  					$scope.skill.newdescription = '';
  					$scope.skill.newcategory = '';
  				}, function (rejection) {
  					Materialize.toast('Error updating '+$scope.skill.name, 3000, 'rounded');
  					$scope.skill.newname = '';
  					$scope.skill.newdescription = '';
  					$scope.skill.newcategory = '';
  				}
  			);
  			stopEventPropagation();
		}
		
		$scope.showEditSkill = function() {
			document.getElementById('editSkillModal'+$scope.skill.name).style.display = "block";
  			stopEventPropagation();
		};

		$scope.closeEditSkill = function() {
			document.getElementById('editSkillModal'+$scope.skill.name).style.display = "none";
		}
	},
  	template:
  	'<li>'+
  		'<div class="collapsible-header contentdetails"><a class="button btn-flat btn-small" ng-click="delete()"><i class="tiny material-icons">delete</i></a><a class="button btn-flat btn-small" ng-click="showEditSkill()"><i class="tiny material-icons">mode_edit</i></a>{{skill.name}}</div>'+
  		'<div class="collapsible-body light-green lighten-1">'+
  			'<p>Category: {{skill.category}}</br>'+
  			'Description: {{skill.description}}</p>'+
  		'</div>'+
  		'<!-- modal edit skill -->'+
  		'<div id="editSkillModal{{skill.name}}" class="modal-background">'+
  			'<div class="modal-content">'+
				'<div class="modal-header">'+
					'<span class="close" ng-click="closeEditSkill()">X</span>'+
					'<h5>Edit skill {{skill.name}} </h5>'+
				'</div>'+
				'<div class="modal-body">'+
					'<div>'+
						'<input type="text" class="active validate" ng-model="skill.newname" placeholder="New name"><br>'+
						'<input type="text" class="validate" ng-model="skill.newdescription" placeholder="New description"><br>'+
						'<input type="text" class="validate" ng-model="skill.newcategory" placeholder="New category"><br>'+
						'<input type="submit" value="Edit" class="btn waves-effect waves-green blue" ng-click="edit()"/>'+
					'</div>'+
				'</div>'+
			'</div>'+
  		'</div>'+
  	'</li>',
  	replace: true
  };
});

//componente html angular de items de jobs
components.directive('joblist', function() {
  return {
  	restrict: 'E',
  	transclude: false,
  	scope: {
  		job: "="
  	},
  	controller: function ($scope, $element, $attrs, $http) {
  		
  		$scope.delete = function() {
  			$http.delete('/job_positions/categories/'+escape($scope.job.category)+'/'+escape($scope.job.name)).then(
  				function (response) {
  					Materialize.toast($scope.job.name+' deleted', 3000, 'rounded');
  					$scope.$parent.getJobs();
  				}, function (rejection) {
  					Materialize.toast('Error deleting '+$scope.job.name, 3000, 'rounded');
  				}
  			);
  			stopEventPropagation();
		};
		
		$scope.edit = function() {
			$scope.closeEditJob();
			data = {job_position:{name:$scope.job.newname,description:$scope.job.newdescription,category:$scope.job.newcategory}};
			$http.put('/job_positions/categories/'+escape($scope.job.category)+'/'+escape($scope.job.name),data).then(
  				function (response) {
  					Materialize.toast($scope.job.name+' updated', 3000, 'rounded');
  					$scope.job.name = $scope.job.newname;
  					$scope.job.description = $scope.job.newdescription;
  					$scope.job.newname = '';
  					$scope.job.newdescription = '';
  					$scope.job.newcategory = '';
  				}, function (rejection) {
  					Materialize.toast('Error updating '+$scope.job.name, 3000, 'rounded');
  					$scope.job.newname = '';
  					$scope.job.newdescription = '';
  					$scope.job.newcategory = '';
  				}
  			);
  			stopEventPropagation();
		}
		
		$scope.showEditJob = function() {
			document.getElementById('editSkillModal'+$scope.job.name).style.display = "block";
  			stopEventPropagation();
		};

		$scope.closeEditJob = function() {
			document.getElementById('editSkillModal'+$scope.job.name).style.display = "none";
		}
	},
  	template:
  	'<li>'+
  		'<div class="collapsible-header contentdetails"><a class="button btn-flat btn-small" ng-click="delete()"><i class="tiny material-icons">delete</i></a><a class="button btn-flat btn-small" ng-click="showEditJob()"><i class="tiny material-icons">mode_edit</i></a>{{job.name}}</div>'+
  		'<div class="collapsible-body light-green lighten-1">'+
  			'<p>Category: {{job.category}}</br>'+
  			'Description: {{job.description}}</p>'+
  		'</div>'+
  		'<!-- modal edit job -->'+
  		'<div id="editSkillModal{{job.name}}" class="modal-background">'+
  			'<div class="modal-content">'+
				'<div class="modal-header">'+
					'<span class="close" ng-click="closeEditJob()">X</span>'+
					'<h5>Edit job {{job.name}} </h5>'+
				'</div>'+
				'<div class="modal-body">'+
					'<div>'+
						'<input type="text" class="active validate" ng-model="job.newname" placeholder="New name"><br>'+
						'<input type="text" class="validate" ng-model="job.newdescription" placeholder="New description"><br>'+
						'<input type="text" class="validate" ng-model="job.newcategory" placeholder="New category"><br>'+
						'<input type="submit" value="Edit" class="btn waves-effect waves-green blue" ng-click="edit()"/>'+
					'</div>'+
				'</div>'+
			'</div>'+
  		'</div>'+
  	'</li>',
  	replace: true
  };
});