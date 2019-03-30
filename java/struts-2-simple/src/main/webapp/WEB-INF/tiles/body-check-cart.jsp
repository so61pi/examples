<%@ taglib uri="/struts-tags"
    prefix="s"%>

<%@ page import="java.util.List" %>
<%@ page import="core.*" %>

<table id="product-list" class="table table-hover table-condensed">
    <thead>
        <tr>
            <th class="col-md-2">Product</th>
            <th class="col-md-3">Quality</th>
            <th class="col-md-2">Price/unit ($)</th>
            <th class="col-md-2">TotalPrice</th>
            <th class="col-md-2"></th>
            <th class="col-md-1"></th>
        </tr>
    </thead>
    <tbody class="searchable">
        <s:iterator value="#request.list">
            <tr>
                <s:form action="remove-item" theme="bootstrap" cssClass="form-inline">
	                <td>
	                    <s:hidden name="productId" value="%{x}"/>
	                    <s:property value="%{y}" />
	                </td>
	                <td>
	                    <s:property value="%{z}"/>
	                </td>
	                <td>
	                    <s:property value="%{t}"/>
	                </td>
	                <td>
	                    <s:property value="%{u}"/>
	                </td>
	                <td>
	                    <s:submit value="Remove" cssClass="btn btn-danger btn-block"/>
	                </td>
	                <td>
	                </td>
                </s:form>
            </tr>
        </s:iterator>
    </tbody>
</table>
<s:form action="accept" theme="bootstrap">
	<div class="form-group col-md-2 col-md-offset-5 text-center">
		<label>Total: $<s:property value="#request.total"/></label>
	    <s:submit value="Accept" cssClass="btn btn-success btn-block"/>
	</div>
</s:form>
