<%@ taglib uri="http://tiles.apache.org/tags-tiles" prefix="tiles"%>

<%@ page import="model.User" %>

<html ng-app="app">
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
  <script src="js/jquery.min.js"></script>
  <link rel="stylesheet" href="css/jquery-ui.min.css">
  <script src="js/jquery-ui.min.js"></script>
  
  <link rel="stylesheet" href="css/bootstrap.min.css">
  <link rel="stylesheet" href="css/bootstrap-theme.min.css">
  <script src="js/bootstrap.min.js"></script>
  
  <script src="js/angular.min.js"></script>
  <script src="js/app.js"></script>
  
  <title><tiles:insertAttribute name="title"/></title>
  
  <style>
    html, body {
      height: 100%;
    }
    
    body {
      padding-bottom: 40px;
    }
    
    header {
      text-align: center;
      font-weight: bold;
      height: 40px
    }
    
    header p {
      position: relative;
      top: 50%;
      transform: translateY(-50%);
    }
    
    footer {
      font-weight: bold;
      text-align: center;
      position: relative;
      bottom: 0px;
      width: 100%;
      clear: both;
      position: absolute;
      height: 40px
    }
    
    footer p {
      position: relative;
      top: 50%;
      transform: translateY(-50%);
    }
  </style>
</head>
<body class="bg-info">
  <header class="bg-danger"><tiles:insertAttribute name="header"/></header>
  <%
  User u = (User) session.getAttribute("user");
  if (u == null) {
  %>
  <main class="container-fluid">
    <div class="row">
      <div class="col-md-2">
        <tiles:insertAttribute name="menu-anonymous"/>
      </div>
      <div class="col-md-10">
        <tiles:insertAttribute name="body"/>
      </div>
    </div>
  </main>
  <%
  }
  else if (u.isCustomer()) {
  %>
  <main class="container-fluid">
    <div class="row">
      <div class="col-md-2">
        <tiles:insertAttribute name="menu-customer"/>
      </div>
      <div class="col-md-10">
        <tiles:insertAttribute name="body"/>
      </div>
    </div>
  </main>
  <%
  }
  else {
  %>
  <main class="container-fluid">
    <div class="row">
      <div class="col-md-2">
        <tiles:insertAttribute name="menu-admin"/>
      </div>
      <div class="col-md-10">
        <tiles:insertAttribute name="body"/>
      </div>
    </div>
  </main>
  <%
  }
  %>
  <footer class="bg-danger"><tiles:insertAttribute name="footer"/></footer>
</body>
</html>