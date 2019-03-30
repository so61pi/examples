<%@ taglib uri="/struts-tags"
    prefix="s"%>
    
<div class="container-fluid">
    <div class="row">
        <label class="col-md-2 control-label">User name:</label>
        <div class="col-md-10">
            <s:property value="#request.user.userName"/>
        </div>
    </div>
    <div class="row">
        <label class="col-md-2 control-label">Email:</label>
        <div class="col-md-10">
            <s:property value="#request.user.email"/>
        </div>
    </div>
    <div class="row">
        <label class="col-md-2 control-label">Status:</label>
        <div class="col-md-10">
	        <s:if test="%{#request.user.isActivated}">
		        Activated
		    </s:if>
		    <s:else>
		        Not activated
		    </s:else>
        </div>
    </div>
    <div class="row">
        <label class="col-md-2 control-label">Role:</label>
        <div class="col-md-10">
            <s:if test="%{#request.user.role == 0}">
                Customer
            </s:if>
            <s:else>
                Unknown
            </s:else>
        </div>
    </div>
</div>