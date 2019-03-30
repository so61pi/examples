<%@ taglib uri="/struts-tags"
    prefix="s"%>

<%@ page import="java.util.List" %>
<%@ page import="core.*" %>

<% List<Product> products = Product.getProductList(); %>
<% request.setAttribute("products", products); %>

<script type="text/javascript">
    $(document).ready(function() {
        $("#filter").keyup(function() {
            var rex = new RegExp($(this).val(), "i");
            $(".searchable tr").hide();
            $(".searchable tr").filter(function() {
                return rex.test($("#name", this).val());
            }).show();
        });
    });
</script>

<div class="input-group col-md-4">
    <span class="input-group-addon">Search</span>
    <input id="filter" type="text" class="form-control" placeholder="Name...">
</div>

<table id="product-list" class="table table-hover table-condensed">
	<thead>
		<tr>
			<th class="col-md-2">Product</th>
			<th class="col-md-4">Description</th>
			<th class="col-md-2">Price/unit ($)</th>
			<th class="col-md-2">In stock</th>
			<th class="col-md-2"></th>
		</tr>
		
		<tr>
            <s:form action="add-product" theme="bootstrap" cssClass="form-inline">
                <td>
                    <s:textfield name="name"/>
                </td>
                <td>
                    <s:textarea name="description" rows="1"/>
                </td>
                <td>
                    <s:textfield name="price" type="number"/>
                </td>
                <td>
                    <s:textfield name="quality" type="number"/>
                </td>
                <td>
                    <s:submit value="Add" cssClass="btn btn-success btn-block"/>
                </td>
            </s:form>
        </tr>
	</thead>
	<tbody class="searchable">
		<s:iterator value="#request.products">
		    <tr>
				<s:form theme="bootstrap" cssClass="form-inline">
					<td>
					    <s:hidden name="productId" value="%{productId}"/>
					    <s:textfield id="name" name="name" value="%{name}"/>
					</td>
					<td>
					    <s:textarea name="description" value="%{description}" rows="1"/>
					</td>
					<td>
					    <s:textfield name="price" value="%{price}" type="number"/>
					</td>
					<td>
					    <s:textfield name="quality" value="%{quality}" type="number"/>
					</td>
					<td>
					    <s:submit action="update-product" value="Update" cssClass="btn btn-primary btn-block"/>
					    <s:submit action="remove-product" value="Remove" cssClass="btn btn-danger btn-block"/>
					</td>
				</s:form>
			</tr>
		</s:iterator>
	</tbody>
</table>
