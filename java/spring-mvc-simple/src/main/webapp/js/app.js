var app = angular.module("app", []);

app.controller("ProductController", function($scope, $http, $window) {
    $scope.model = {};
    $scope.model.status = {};
    $scope.model.productsList = [];
    $scope.model.product = {};
    $scope.model.cart = [];
    
    $scope.Add = function(product) {
        $http({
            url: "product/add",
            method: "POST",
            data: product
        })
        .success(function(data, status, headers, config) {
            $scope.model.status = data;
            if ($scope.model.status.status === "success") {
            	$scope.LoadProductsList();
            }
            console.log(data);
        })
        .error(function(data, status, headers, config) {
            console.log(data);
        });
    };
    
    $scope.Remove = function(product) {
    	if (!confirm('Are you sure you want to delete this product, this action CANNOT be undone?')) {
    	    return;
    	}
        $http({
            url: "product/remove",
            method: "POST",
            data: product
        })
        .success(function(data, status, headers, config) {
            $scope.LoadProductsList();
            console.log(data);
        })
        .error(function(data, status, headers, config) {
            console.log(data);
        });
    };
    
    $scope.LoadProductsList = function() {
        $http({
            url: "product/list",
            method: "GET"
        })
        .success(function(data, status, headers, config) {
            $scope.model.productsList = data;
            $scope.model.product = {};
            $scope.model.status = {};
            console.log(data);
        })
        .error(function(data, status, headers, config) {
            console.log(data);
        });
    };
    
    $scope.New = function(product) {
    	$scope.model.status = {};
    	return {};
    };
    
    $scope.LoadCart = function() {
        $http({
            url: "cart/list",
            method: "GET"
        })
        .success(function(data, status, headers, config) {
            $scope.model.cart = data;
            $scope.model.status = {};
            console.log(data);
        })
        .error(function(data, status, headers, config) {
            console.log(data);
        });
    };
    
    $scope.AddToCart = function(product) {
        $http({
            url: "cart/add",
            method: "POST",
            data: product
        })
        .success(function(data, status, headers, config) {
            console.log(data);
        })
        .error(function(data, status, headers, config) {
            console.log(data);
        });
    };
    
    $scope.RemoveFromCart = function(product) {
        $http({
            url: "cart/remove",
            method: "POST",
            data: product
        })
        .success(function(data, status, headers, config) {
            console.log(data);
        })
        .error(function(data, status, headers, config) {
            console.log(data);
        });
    };
    
    $scope.CheckOut = function() {
        $http({
            url: "cart/check-out",
            method: "POST"
        })
        .success(function(data, status, headers, config) {
            $scope.model.status = data;
            $scope.model.cart = [];
            console.log(data);
        })
        .error(function(data, status, headers, config) {
            console.log(data);
        });
    };
});

app.controller("UserController", function($scope, $http, $window) {
    $scope.model = {};
    $scope.model.status = {};
    $scope.model.usersList = [];
    $scope.model.user = {};
    
    $scope.Add = function(user) {
        $http({
            url: "user/add",
            method: "POST",
            data: user
        })
        .success(function(data, status, headers, config) {
            $scope.model.status = data;
            if ($scope.model.status.status === "success") {
            	$scope.LoadUsersList();
            }
            console.log(data);
        })
        .error(function(data, status, headers, config) {
            console.log(data);
        });
    };
    
    $scope.Remove = function(user) {
    	if (!confirm('Are you sure you want to delete this user, this action CANNOT be undone?')) {
    	    return;
    	}
        $http({
            url: "user/remove",
            method: "POST",
            data: user
        })
        .success(function(data, status, headers, config) {
            $scope.LoadUsersList();
            console.log(data);
        })
        .error(function(data, status, headers, config) {
            console.log(data);
        });
    };
    
    $scope.LoadUsersList = function() {
        $http({
            url: "user/list",
            method: "GET"
        })
        .success(function(data, status, headers, config) {
            $scope.model.usersList = data;
            $scope.model.user = {};
            $scope.model.status = {};
            console.log(data);
        })
        .error(function(data, status, headers, config) {
            console.log(data);
        });
    };
    
    $scope.New = function() {
    	$scope.model.status = {};
    	return {};
    };
});

app.controller("LoginController", function($scope, $http, $window) {
    $scope.model = {};
    $scope.model.status = {};
    $scope.model.user = {};
    
    $scope.Login = function() {
        $http({
            url: "login",
            method: "POST",
            data: $scope.model.user
        })
        .success(function(data, status, headers, config) {
            $scope.model.status = data;
            if ($scope.model.status.status === "success") {
                $window.location.href = "product";
            }
            console.log(data);
        })
        .error(function(data, status, headers, config) {
            console.log(data);
        });
    };
});
