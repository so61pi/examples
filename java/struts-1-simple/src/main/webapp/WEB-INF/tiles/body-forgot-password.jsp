<%@ taglib uri="http://struts.apache.org/tags-bean" prefix="bean" %>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>
<%@ taglib uri="http://struts.apache.org/tags-logic" prefix="logic" %>

<div class="container-fluid">
    <div class="row">
        <div class="col-md-3">
        </div>
        <div class="col-md-6">
            <html:form action="/doForgotPassword" styleClass="form-horizontal">
                <label><bean:message key="label.email" /></label>
                <html:text property="email" styleClass="form-control" />
                <html:errors property="email" />

                <center>
                <html:img src="captcha" />
                </center>
                
                <label><bean:message key="label.captcha" /></label>
                <html:text property="captcha" styleClass="form-control" />
                <html:errors property="captcha"/>
                
                <html:submit value="Help me" styleClass="btn btn-primary center-block"/>
            </html:form>
        </div>
        <div class="col-md-3">
        </div>
    </div>
</div>