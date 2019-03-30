<%@ taglib uri="http://tiles.apache.org/tags-tiles" prefix="tiles" %>

<tiles:insertDefinition name="baselayout">
  <tiles:putAttribute name="title" value="Product management" type="string"/>
  <tiles:putAttribute name="body">
    <div ng-controller="ProductController" ng-init="LoadProductsList()">
      <form class="form-inline {{model.status.status === 'error' ? 'has-error' : ''}}">
        <div class="input-group">
          <span class="input-group-addon">Name</span>
          <input name="name" type="text" class="form-control" required ng-model="model.product.name"/>
        </div>
        <div class="input-group">
          <span class="input-group-addon">Desc</span>
          <input name="description" type="text" class="form-control" required ng-model="model.product.description"/>
        </div>
        <div class="input-group">
          <span class="input-group-addon">$</span>
          <input name="price" type="number" class="form-control" min="0" step="any" required ng-model="model.product.price"/>
        </div>
        <div class="input-group">
          <span class="input-group-addon">#</span>
          <input name="quality" type="number" class="form-control" min="0" required ng-model="model.product.quality"/>
        </div>
        <div class="input-group pull-right">
          <button type="submit" class="btn btn-success" ng-click="Add(model.product)">Add/Update</button>
          <button type="button" class="btn btn-warning" ng-click="model.product = New()">New</button>
          <button type="button" class="btn btn-info" ng-click="LoadProductsList()">Reload</button>
          <button type="button" class="btn btn-danger" ng-click="Remove(model.product)">Remove</button>
        </div>
      </form>
      <table class="table table-hover panel panel-primary">
        <thead class="panel-heading">
          <tr>
            <th>#</th>
            <th>Name</th>
            <th>Description</th>
            <th>Price ($)</th>
            <th>Quality</th>
          </tr>
        </thead>
        <tbody class="panel-body">
          <tr ng-repeat="product in model.productsList" ng-click="model.product=product">
            <td>{{ $index + 1 }}</td>
            <td>{{ ::product.name }}</td>
            <td>{{ ::product.description }}</td>
            <td>{{ ::product.price }}</td>
            <td>{{ ::product.quality }}</td>
          </tr>
        </tbody>
      </table>
    </div>
  </tiles:putAttribute>
</tiles:insertDefinition>