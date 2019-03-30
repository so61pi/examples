package actions;

import java.util.List;
import java.util.Map;

import org.apache.struts2.convention.annotation.Action;
import org.apache.struts2.convention.annotation.InterceptorRef;
import org.apache.struts2.convention.annotation.InterceptorRefs;
import org.apache.struts2.convention.annotation.Result;

import com.opensymphony.xwork2.ActionContext;
import com.opensymphony.xwork2.ActionSupport;

import core.Product;

@InterceptorRefs({ @InterceptorRef("notLoggedinStack") })
@Action(value = "view-products", results = {
        @Result(name = "success", location = "view-products.jsp"),
        @Result(name = "error", location = "error.jsp") })
public class ViewProductsAction extends ActionSupport {
    @Override
    public String execute() {
        Map request = (Map) ActionContext.getContext().get("request");

        List<Product> products = Product.getProductList();

        request.put("products", products);

        return SUCCESS;
    }
}
