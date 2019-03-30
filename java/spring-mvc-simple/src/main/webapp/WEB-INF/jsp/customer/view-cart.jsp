<%@ taglib uri="http://tiles.apache.org/tags-tiles" prefix="tiles" %>

<tiles:insertDefinition name="baselayout">
  <tiles:putAttribute name="title" value="Cart" type="string"/>
  <tiles:putAttribute name="body">
    <div ng-controller="ProductController" ng-init="LoadCart()">
      <button type="button" class="btn btn-success pull-right" ng-click="CheckOut()">Check out</button>
      <table class="table table-hover panel panel-primary col-md-12">
        <thead class="panel-heading">
          <tr>
            <th>#</th>
            <th>Name</th>
            <th>Description</th>
            <th>Price ($)</th>
            <th>Buy</th>
            <th>Total ($)</th>
            <th></th>
          </tr>
        </thead>
        <tbody class="panel-body">
          <tr ng-repeat="package in model.cart">
            <td>{{ $index + 1 }}</td>
            <td>{{ package.product.name }}</td>
            <td>{{ package.product.description }}</td>
            <td>{{ package.product.price }}</td>
            <td>{{ package.quality }}</td>
            <td>{{ (package.quality * package.product.price) }}</td>
            <td>
              <button type="button" class="btn btn-success" ng-click="AddToCart(package.product);LoadCart();">+1</button>
              <button type="button" class="btn btn-danger" ng-click="RemoveFromCart(package.product);LoadCart();">Remove</button>
            </td>
          </tr>
          <tr ng-repeat="(product, error) in model.status.errors">
            <td>{{ $index + 1 }}</td>
            <td>{{ product }}</td>
            <td>{{ error }}</td>
          </tr>
        </tbody>
      </table>
    </div>
  </tiles:putAttribute>
</tiles:insertDefinition>