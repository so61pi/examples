<%@ taglib uri="http://struts.apache.org/tags-bean" prefix="bean" %>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>
<%@ taglib uri="http://struts.apache.org/tags-logic" prefix="logic" %>

<div class="container-fluid">
	<div class="row">
		<div class="col-md-3">
		</div>
		<div class="col-md-6">
                    <html:form action="/doLogin" styleClass="form-horizontal">
                        <label><bean:message key="label.userName" /></label>
                        <html:text property="userName" styleClass="form-control" />
                        <html:errors property="userName" />
                        
                        <label><bean:message key="label.password" /></label>
                        <html:password property="password" styleClass="form-control" />
                        <html:errors property="password"/>
                        
                        <html:submit value="Login" styleClass="btn btn-primary center-block"/>
                    </html:form>
		</div>
		<div class="col-md-3">
		</div>
	</div>
</div>

