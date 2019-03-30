<%@ taglib uri="/struts-tags"
    prefix="s"%>

<table id="product-list" class="table table-hover table-condensed">
    <thead>
        <tr>
            <th class="col-md-2">Product</th>
            <th class="col-md-3">Quality</th>
            <th class="col-md-2">Status</th>
        </tr>
    </thead>
    <tbody class="searchable">
        <s:iterator value="#request.list">
            <tr>
                <td>
                    <s:property value="%{y}"/>
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
            </tr>
        </s:iterator>
    </tbody>
</table>
