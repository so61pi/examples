<%@ taglib uri="http://tiles.apache.org/tags-tiles" prefix="tiles" %>

<tiles:insertDefinition name="baselayout">
  <tiles:putAttribute name="title" value="Login" type="string"/>
  <tiles:putAttribute name="body">
    <div ng-controller="LoginController">
      <form class="col-md-offset-4 col-md-4 text-center">
        <label class="control-label">Default account: admin - admin</label>
        <div class="form-group {{model.status.errors.name ? 'has-error' : ''}}">
          <input name="name" type="text" class="form-control" placeholder="User name..." required ng-model="model.user.name"/>
          <label class="control-label">{{ model.status.errors.name }}</label>
        </div>
        <div class="form-group {{model.status.errors.password ? 'has-error' : ''}}">
          <input name="password" type="password" class="form-control" placeholder="Password..." required ng-model="model.user.password"/>
          <label class="control-label">{{ model.status.errors.password }}</label>
        </div>
        <button type="submit" class="btn btn-primary center-block" ng-click="Login()">Login</button>
      </form>
    </div>
  </tiles:putAttribute>
</tiles:insertDefinition>
