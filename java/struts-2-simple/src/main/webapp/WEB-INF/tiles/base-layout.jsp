<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ taglib uri="http://tiles.apache.org/tags-tiles"
    prefix="tiles"%>
<%@ taglib uri="/struts-tags"
    prefix="s"%>

<!DOCTYPE html>
<html>
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
    
    <script src="js/jquery-2.1.1.min.js"></script>
    <link rel="stylesheet" href="css/jquery-ui.min.css">
    <script src="js/jquery-ui.min.js"></script>
    
    <link rel="stylesheet" href="css/bootstrap.min.css">
    <link rel="stylesheet" href="css/bootstrap-theme.min.css">
    <script src="js/bootstrap.min.js"></script>

    <title><tiles:insertAttribute name="title" ignore="true" /></title>
    
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
        <p>Struts 2 Simple</p>
    </header>
    
    <main>
		<s:if test="%{#session.user == null}">
		    <tiles:insertAttribute name="menu-guest" />
		</s:if>
		<s:elseif test="%{#session.user.userName == 'administrator'}">
		    <tiles:insertAttribute name="menu-admin" />
		</s:elseif>
		<s:else>
            <tiles:insertAttribute name="menu-user" />
        </s:else>
	    <tiles:insertAttribute name="body" />
    </main>
    
    <footer class="bg-success">
        <tiles:insertAttribute name="footer" />
    </footer>
</body>
</html>