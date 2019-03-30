<%@ taglib uri="http://struts.apache.org/tags-bean" prefix="bean" %>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>
<%@ taglib uri="http://struts.apache.org/tags-logic" prefix="logic" %>

<div class="container-fluid">
    <div class="row">
        <div class="col-md-3">
        </div>
        <div class="col-md-6">
            <html:form action="/doRegister" styleClass="form-horizontal">
                <label><bean:message key="label.userName" /></label>
                <html:text property="userName" styleClass="form-control" />
                <html:errors property="userName" />

                <label><bean:message key="label.password" /></label>
                <html:password property="password" styleClass="form-control" />
                <html:errors property="password"/>
                
                <label><bean:message key="label.retypePassword" /></label>
                <html:password property="passwordConfirmed" styleClass="form-control" />
                <html:errors property="passwordConfirmed"/>
                
                <label><bean:message key="label.email" /></label>
                <html:text property="email" styleClass="form-control" />
                <html:errors property="email"/>
                
                <label><bean:message key="label.retypeEmail" /></label>
                <html:text property="emailConfirmed" styleClass="form-control" />
                <html:errors property="emailConfirmed"/>
                
                <center>
                <html:img src="captcha" />
                </center>

                <label><bean:message key="label.captcha" /></label>
                <html:text property="captcha" styleClass="form-control" />
                <html:errors property="captcha"/>
                
                <html:submit value="Register" styleClass="btn btn-primary center-block"/>
            </html:form>
        </div>
        <div class="col-md-3">
        </div>
    </div>
</div>