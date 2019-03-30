<%@ taglib uri="http://struts.apache.org/tags-bean" prefix="bean" %>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>
<%@ taglib uri="http://struts.apache.org/tags-logic" prefix="logic" %>


<div class="container-fluid">
    <div class="row">
        <div class="col-md-3">
        </div>
        <div class="col-md-6">
            <html:form action="/doChangePassword" styleClass="form-horizontal">
                <label><bean:message key="label.oldPassword" /></label>
                <html:password property="oldPassword" styleClass="form-control" />
                <html:errors property="oldPassword" />

                <label><bean:message key="label.newPassword" /></label>
                <html:password property="newPassword" styleClass="form-control" />
                <html:errors property="newPassword"/>
                
                <label><bean:message key="label.retypeNewPassword" /></label>
                <html:password property="newPasswordConfirmed" styleClass="form-control" />
                <html:errors property="newPasswordConfirmed"/>

                <html:submit value="Change" styleClass="btn btn-primary center-block"/>
            </html:form>
	</div>
        <div class="col-md-3">
        </div>
    </div>
</div>