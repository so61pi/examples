<%@ taglib uri="http://tiles.apache.org/tags-tiles" prefix="tiles" %>

<tiles:insertDefinition name="baselayout">
  <tiles:putAttribute name="title" value="User management" type="string"/>
  <tiles:putAttribute name="body">
    <div ng-controller="UserController" ng-init="LoadUsersList()">
      <form class="form-inline {{model.status.status === 'error' ? 'has-error' : ''}}" class="col-md-12">
        <div class="input-group">
          <span class="input-group-addon">Name</span>
          <input name="name" type="text" class="form-control" required ng-model="model.user.name"/>
        </div>
        <div class="input-group">
          <span class="input-group-addon">Pass</span>
          <input name="password" type="password" class="form-control" required ng-model="model.user.password"/>
        </div>
        <div class="input-group">
          <span class="input-group-addon">Email</span>
          <input name="email" type="email" class="form-control" required ng-model="model.user.email"/>
        </div>
        <div class="input-group">
          <label><input name="role" type="radio" value="customer" checked required ng-model="model.user.role"/>Customer</label>
          <label><input name="role" type="radio" value="admin" ng-model="model.user.role"/>Admin</label>
        </div>
        <div class="input-group pull-right">
          <button type="submit" class="btn btn-success" ng-click="Add(model.user)">Add/Update</button>
          <button type="button" class="btn btn-warning" ng-click="model.user = New()">New</button>
          <button type="button" class="btn btn-info" ng-click="LoadUsersList()">Reload</button>
          <button type="button" class="btn btn-danger" ng-click="Remove(model.user)">Remove</button>
        </div>
      </form>
      <table class="table table-hover panel panel-primary col-md-12">
        <thead class="panel-heading">
          <tr>
            <th>#</th>
            <th>Name</th>
            <th>Password</th>
            <th>Email</th>
            <th>Role</th>
          </tr>
        </thead>
        <tbody class="panel-body">
          <tr ng-repeat="user in model.usersList" ng-click="model.user=user">
            <td>{{ $index + 1 }}</td>
            <td>{{ ::user.name }}</td>
            <td>{{ ::user.password }}</td>
            <td>{{ ::user.email }}</td>
            <td>{{ ::user.role }}</td>
          </tr>
        </tbody>
      </table>
    </div>
  </tiles:putAttribute>
</tiles:insertDefinition>