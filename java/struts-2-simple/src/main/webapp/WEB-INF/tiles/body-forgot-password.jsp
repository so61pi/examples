<%@ taglib uri="/struts-tags" prefix="s" %>
<%@ taglib uri="/struts-bootstrap-tags" prefix="sb" %>

<div class="container-fluid">
    <div class="row">
        <div class="col-md-3">
        </div>
        <div class="col-md-6">
			<s:form action="forgot-password" theme="bootstrap" cssClass="form-horizontal">
			    <s:textfield name="email" label="Email"/>
			    <center>
                    <img src="captcha.jpg"/>
                </center>
			    <s:textfield name="captcha" label="Captcha"/>
			    <s:submit value="Help me" cssClass="btn btn-primary center-block"/>
			</s:form>
        </div>
        <div class="col-md-3">
        </div>
    </div>
</div>