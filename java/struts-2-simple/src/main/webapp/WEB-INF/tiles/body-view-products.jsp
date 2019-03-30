<%@ taglib uri="/struts-tags"
    prefix="s"%>

<script type="text/javascript">
    $(document).ready(function() {
        $("#filter").keyup(function() {
            var rex = new RegExp($(this).val(), "i");
            $(".searchable tr").hide();
            $(".searchable tr").filter(function() {
                return rex.test($("#name", this).text());
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
            <th class="col-md-3">Description</th>
            <th class="col-md-2">Price/unit ($)</th>
            <th class="col-md-2">In stock</th>
            <th class="col-md-2"></th>
            <th class="col-md-1"></th>
        </tr>
    </thead>
    <tbody class="searchable">
        <s:iterator value="#request.products">
            <tr>
                <s:form action="add-item" theme="bootstrap" cssClass="form-inline">
                    <td>
                        <s:hidden name="productId" value="%{productId}"/>
                        <label id="name"><s:property value="%{name}"/></label>
                    </td>
                    <td>
                        <s:property value="%{description}"/>
                    </td>
                    <td>
                        <s:property value="%{price}"/>
                    </td>
                    <td>
                        <s:property value="%{quality}"/>
                    </td>
                    <td>
                        <s:textfield name="num" type="number" value="1"/>
                    </td>
                    <td>
                        <s:submit value="Add to Cart" cssClass="btn btn-primary btn-block"/>
                    </td>
                </s:form>
            </tr>
        </s:iterator>
    </tbody>
</table>
