<%@ taglib uri="http://tiles.apache.org/tags-tiles" prefix="tiles" %>

<tiles:insertDefinition name="baselayout">
  <tiles:putAttribute name="title" value="Products" type="string"/>
  <tiles:putAttribute name="body">
    <div ng-controller="ProductController" ng-init="LoadProductsList()">
      <table class="table table-hover panel panel-primary col-md-12">
        <thead class="panel-heading">
          <tr>
            <th>#</th>
            <th>Name</th>
            <th>Description</th>
            <th>Price ($)</th>
            <th>Quality</th>
            <th></th>
          </tr>
        </thead>
        <tbody class="panel-body">
          <tr ng-repeat="product in model.productsList" ng-click="model.product=product">
            <td>{{ $index + 1 }}</td>
            <td>{{ product.name }}</td>
            <td>{{ product.description }}</td>
            <td>{{ product.price }}</td>
            <td>{{ product.quality }}</td>
            <td>
              <button class="btn btn-success center-block" ng-click="AddToCart(product)">Add to cart</button>
            </td>
          </tr>
        </tbody>
      </table>
    </div>
  </tiles:putAttribute>
</tiles:insertDefinition>