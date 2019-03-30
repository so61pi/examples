package actions;

import org.apache.struts2.convention.annotation.Action;
import org.apache.struts2.convention.annotation.InterceptorRef;
import org.apache.struts2.convention.annotation.InterceptorRefs;
import org.apache.struts2.convention.annotation.Result;
import org.apache.struts2.convention.annotation.Results;

import com.opensymphony.xwork2.ActionSupport;

import core.Product;

@InterceptorRefs({ @InterceptorRef("notLoggedinStack"),
    @InterceptorRef("notAdminStack") })
@Results({
    @Result(name = "success", type = "redirect", location = "/update-products"),
    @Result(name = "error", location = "error.jsp") })
public class ProductAction extends ActionSupport {
    private int productId;
    private String name;
    private String description;
    private double price;
    private int quality;

    @Action("add-product")
    public String addProduct() {
        Product.addProduct(name, description, quality, price);
        return SUCCESS;
    }

    @Action("update-product")
    public String updateProduct() {
        Product.updateProduct(productId, name, description, quality, price);
        return SUCCESS;
    }

    @Action("remove-product")
    public String removeProduct() {
        Product.removeProduct(productId);
        return SUCCESS;
    }

    //
    //
    //
    public int getProductId() {
        return productId;
    }

    public void setProductId(int productId) {
        this.productId = productId;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public double getPrice() {
        return price;
    }

    public void setPrice(double price) {
        if (price < 0)
            price = 0;
        this.price = price;
    }

    public int getQuality() {
        return quality;
    }

    public void setQuality(int quality) {
        if (quality < 0)
            quality = 0;
        this.quality = quality;
    }
}
