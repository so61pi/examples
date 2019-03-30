<%@ taglib uri="/struts-tags" prefix="s" %>
<%@ taglib uri="/struts-bootstrap-tags" prefix="sb" %>

<div class="container-fluid">
    <div class="row">
        <div class="col-md-3">
        </div>
        <div class="col-md-6">
			<s:form action="register" theme="bootstrap" cssClass="form-horizontal">
			    <s:textfield name="userName" label="User name"/>
			    <s:password name="pass" label="Password"/>
			    <s:password name="confirmedPass" label="Retype password"/>
			    <s:textfield name="email" label="Email"/>
			    <s:textfield name="confirmedEmail" label="Retype email"/>
			    <center>
                    <img src="captcha.jpg"/>
                </center>
			    <s:textfield name="captcha" label="Captcha"/>
			    <s:submit value="Register" cssClass="btn btn-primary center-block"/>
			</s:form>
        </div>
        <div class="col-md-3">
        </div>
    </div>
</div>