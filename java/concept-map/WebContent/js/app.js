var app = angular.module("cmApp", []);

app.controller("Controller", function($scope, $http) {
    // global variables
	$scope.name = "Map name";
	$scope.repoId = 0;
	$scope.description = "Description";
	$scope.deadline = 1;
	
    $scope.nodes = [];
    $scope.edges = [];
    graph = new ConceptMap(window.d3.select("#drawing-area").select("svg"));
    graph.setData($scope.nodes, $scope.edges);
    graph.updateGraph();
    
    $scope.update = function() {
        $scope.$digest();
    }
    
    $scope.save = function() {
        $http({
                url: "save",
                method: "POST",
                data: {
                	name: $scope.name,
                	repoId: $scope.repoId,
                	description: $scope.description,
                	deadline: $scope.deadline,
                	cm: {
	                	concepts: $scope.nodes,
	                	relationships: $scope.edges
                	}
            	}
            })
            .success(function(data, status, headers, config) {
            	$scope.repoId = data.repoId;
            	$scope.update();
                console.log(data);
            })
            .error(function(data, status, headers, config) {
                console.log(data);
            });
    }
    
    $scope.load = function(cmId) {
        $http({
                url: "load",
                method: "POST",
                data: {cmId: cmId}
            })
            .success(function(data, status, headers, config) {
                $scope.nodes = data.concepts;
                $scope.edges = data.relationships;
                graph.setData($scope.nodes, $scope.edges);
                graph.updateGraph();
            })
            .error(function(data, status, headers, config) {
                console.log(data);
            });
    }
    
});
