<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>

<%@ taglib uri="http://struts.apache.org/tags-bean" prefix="bean" %>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>
<%@ taglib uri="http://struts.apache.org/tags-logic" prefix="logic" %>
    
<div class="container-fluid">
    <div class="row">
        <label class="col-md-2 control-label">User name:</label>
        <div class="col-md-10">
            <bean:write name="user" property="userName" />
        </div>
    </div>
    <div class="row">
        <label class="col-md-2 control-label">Email:</label>
        <div class="col-md-10">
            <bean:write name="user" property="email" />
        </div>
    </div>
    <div class="row">
        <label class="col-md-2 control-label">Status:</label>
        <div class="col-md-10">
            <c:choose>
                <c:when test="${user.isActivated}">
                    Activated.
                </c:when>
                <c:otherwise>
                    Not activated.
                </c:otherwise>
            </c:choose>
        </div>
    </div>
    <div class="row">
        <label class="col-md-2 control-label">Role:</label>
        <div class="col-md-10">
            <c:choose>
                <c:when test="${user.role == 0}">
                    Customer.
                </c:when>
                <c:otherwise>
                    Unknown.
                </c:otherwise>
            </c:choose>
        </div>
    </div>
</div>