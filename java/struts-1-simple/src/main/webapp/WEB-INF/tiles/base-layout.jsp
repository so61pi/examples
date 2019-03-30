<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>

<%@ taglib uri="http://struts.apache.org/tags-tiles" prefix="tiles" %>

<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>

<%@ taglib uri="http://struts.apache.org/tags-bean" prefix="bean" %>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>
<%@ taglib uri="http://struts.apache.org/tags-logic" prefix="logic" %>

<!DOCTYPE html>
<html>
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
    
    <link rel="stylesheet" href="css/bootstrap.min.css">
    <link rel="stylesheet" href="css/bootstrap-theme.min.css">
    <script src="js/bootstrap.min.js"></script>

    <title><tiles:insert attribute="title" /></title>
    
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
<body>
    <header class="bg-success">
        <p>Struts 1 Simple</p>
    </header>
    
    <main>
        <c:choose>
            <c:when test="${sessionScope.userId == null}">
                <tiles:insert attribute="menu-guest" />
            </c:when>
            <c:otherwise>
                <tiles:insert attribute="menu-user" />
            </c:otherwise>
        </c:choose>
	<tiles:insert attribute="body" />
    </main>
    
    <footer class="bg-success">
        <tiles:insert attribute="footer" />
    </footer>
</body>
</html>