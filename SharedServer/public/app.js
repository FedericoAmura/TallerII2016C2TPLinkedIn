$(document).ready(function() {
	$(".dropdown-button").dropdown();
});

function stopEventPropagation() {
	var e = window.event;
	e.cancelBubble = true;
	if (e.stopPropagation) e.stopPropagation();
}

//funciones para mostrar los modales para creacion
function showNewCategory() {
	document.getElementById('newCategoryModal').style.display = "block";
}
function closeNewCategory() {
	document.getElementById('newCategoryModal').style.display = "none";
}

function showNewSkill() {
	document.getElementById('newSkillModal').style.display = "block";
}
function closeNewSkill() {
	document.getElementById('newSkillModal').style.display = "none";
}

function showNewJob() {
	document.getElementById('newJobModal').style.display = "block";
}
function closeNewJob() {
	document.getElementById('newJobModal').style.display = "none";
}

//funciones para switchear las views
function showJobs() {
	document.getElementById('jobsDiv').style.display = "flex";
	document.getElementById('jobsTab').classList.add('active');
	document.getElementById('skillsDiv').style.display = "none";
	document.getElementById('skillsTab').classList.remove('active');
	document.getElementById('categoriesDiv').style.display = "none";
	document.getElementById('categoriesTab').classList.remove('active');
}

function showSkills() {
	document.getElementById('jobsDiv').style.display = "none";
	document.getElementById('jobsTab').classList.remove('active');
	document.getElementById('skillsDiv').style.display = "flex";
	document.getElementById('skillsTab').classList.add('active');
	document.getElementById('categoriesDiv').style.display = "none";
	document.getElementById('categoriesTab').classList.remove('active');
}

function showCategories() {
	document.getElementById('jobsDiv').style.display = "none";
	document.getElementById('jobsTab').classList.remove('active');
	document.getElementById('skillsDiv').style.display = "none";
	document.getElementById('skillsTab').classList.remove('active');
	document.getElementById('categoriesDiv').style.display = "flex";
	document.getElementById('categoriesTab').classList.add('active');
}

//creamos aplicacion angular con sus controllers
var jobify = angular.module('jobify', ['components']);
 
//controlador categorias
jobify.controller('CategoriesController', function($scope, $http) {
	$http.defaults.headers.post["Content-Type"] = "application/json";
	
	//para conseguir las categories
  	$scope.getCategories = function() {
  		$http.get('/categories')
  		.then(function(response) {
  			$scope.categories = response.data.categories;
  		});
  	}
  	
  	//conseguimos todo
  	$scope.getCategories();
  	
  	//para crear una nueva category
  	$scope.newCategory = function() {
  		closeNewCategory();
  		data = {category:{name:$scope.categoryname,description:$scope.categorydescription}};
  		$http.post('/categories',data).
  		then(function (response) {
  			Materialize.toast('Category '+$scope.categoryname+' created', 3000, 'rounded');
  			$scope.getCategories();
			$scope.categoryname = '';
			$scope.categorydescription = '';
  		}, function () {
			Materialize.toast('Could not create category '+$scope.categoryname, 3000, 'rounded');
			$scope.categoryname = '';
			$scope.categorydescription = '';
  		});
	};
});

//controlador categorias
jobify.controller('SkillsController', function($scope, $http) {
	$http.defaults.headers.post["Content-Type"] = "application/json";
	
  	//para conseguir las skills
  	$scope.getSkills = function() {
  		$http.get('/skills')
  		.then(function(response) {
  			$scope.skills = response.data.skills;
  		});
  	}
  	
  	//conseguimos todo
  	$scope.getSkills();
  	
	//para crear una nueva skill
  	$scope.newSkill = function() {
  		closeNewSkill();
  		data = {skill:{name:$scope.skillname,description:$scope.skilldescription}};
  		$http.post('/skills/categories/'+escape($scope.skillcategory),data).
  		then(function (response) {
  			Materialize.toast('Skill '+$scope.skillname+' created', 3000, 'rounded');
  			$scope.getSkills();
			$scope.skillname = '';
			$scope.skilldescription = '';
  		}, function () {
			Materialize.toast('Could not create skill '+$scope.skillname, 3000, 'rounded');
			$scope.skillname = '';
			$scope.skilldescription = '';
  		});
	};
});

//controlador categorias
jobify.controller('JobsController', function($scope, $http) {
	$http.defaults.headers.post["Content-Type"] = "application/json";
	
  	//para conseguir los jobs
  	$scope.getJobs = function() {
  		$http.get('/job_positions')
  		.then(function(response) {
  			$scope.jobs = response.data.job_positions;
  		});
  	}
  	
  	//conseguimos todo
  	$scope.getJobs();
  	
	//para crear un nuevo job
  	$scope.newJob = function() {
  		closeNewJob();
  		data = {job_position:{name:$scope.jobname,description:$scope.jobdescription}};
  		$http.post('/job_positions/categories/'+escape($scope.jobcategory),data).
  		then(function (response) {
  			Materialize.toast('Job '+$scope.jobname+' created', 3000, 'rounded');
  			$scope.getJobs();
			$scope.jobname = '';
			$scope.jobdescription = '';
  		}, function () {
			Materialize.toast('Could not create job '+$scope.jobname, 3000, 'rounded');
			$scope.jobname = '';
			$scope.jobdescription = '';
  		});
	};
});
  
