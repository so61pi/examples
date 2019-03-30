<%@ taglib uri="/struts-tags" prefix="s" %>
<%@ taglib uri="/struts-bootstrap-tags" prefix="sb" %>


<div class="container-fluid">
    <div class="row">
        <div class="col-md-3">
        </div>
        <div class="col-md-6">
			<s:form action="change-password" theme="bootstrap" cssClass="form-horizontal">
			    <s:password name="oldPass" label="Old password"/>
			    <s:password name="newPass" label="New password"/>
			    <s:password name="confirmedNewPass" label="Retype new password"/>
			    <s:submit value="Change" cssClass="btn btn-primary center-block"/>
			</s:form>
		</div>
        <div class="col-md-3">
        </div>
    </div>
</div>