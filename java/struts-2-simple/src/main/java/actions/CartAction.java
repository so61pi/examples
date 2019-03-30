package actions;

import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import org.apache.struts2.convention.annotation.Action;
import org.apache.struts2.convention.annotation.InterceptorRef;
import org.apache.struts2.convention.annotation.InterceptorRefs;
import org.apache.struts2.convention.annotation.Result;
import org.hibernate.Session;

import utils.HibernateUtil;
import utils.Tuple;

import com.opensymphony.xwork2.ActionContext;
import com.opensymphony.xwork2.ActionSupport;

import core.Product;

@InterceptorRefs({ @InterceptorRef("notLoggedinStack"),
    @InterceptorRef("adminStack") })
@Result(name = "error", location = "error.jsp")
public class CartAction extends ActionSupport {
    private int productId;
    private int num;

    //
    //
    //
    @Action(value = "check-cart", results = { @Result(name = "success", location = "check-cart.jsp") })
    public String checkCart() {
        // productId, name, quality, price, total/product
        List<Tuple<Integer, String, Integer, Double, Double>> list = new LinkedList<Tuple<Integer, String, Integer, Double, Double>>();
        double total = 0.0;

        Map session = (Map) ActionContext.getContext().get("session");
        Map request = (Map) ActionContext.getContext().get("request");

        Map<Integer, Integer> cart = (Map<Integer, Integer>) session
                .get("cart");
        Session sess = HibernateUtil.getSessionFactory().openSession();
        for (Map.Entry<Integer, Integer> entry : cart.entrySet()) {
            Product p = (Product) HibernateUtil.get(sess, Product.class,
                    entry.getKey());

            Tuple<Integer, String, Integer, Double, Double> x = new Tuple<Integer, String, Integer, Double, Double>(
                    p.getProductId(), p.getName(), entry.getValue(),
                    p.getPrice(), p.getPrice() * entry.getValue());
            total += p.getPrice() * entry.getValue();
            list.add(x);
        }
        sess.close();

        request.put("list", list);
        request.put("total", total);

        return SUCCESS;
    }

    //
    //
    //
    @Action(value = "remove-item", results = { @Result(name = "success", type = "redirect", location = "/check-cart") })
    public String removeItem() {
        Map session = (Map) ActionContext.getContext().get("session");
        Map<Integer, Integer> cart = (Map<Integer, Integer>) session
                .get("cart");

        cart.remove(productId);

        return SUCCESS;
    }

    //
    //
    //
    @Action(value = "add-item", results = { @Result(name = "success", type = "redirect", location = "/view-products") })
    public String addItem() {
        Map session = (Map) ActionContext.getContext().get("session");
        Map<Integer, Integer> cart = (Map<Integer, Integer>) session
                .get("cart");
        Integer n = cart.get(productId);
        if (n == null)
            n = 0;
        n += num;
        cart.put(productId, n);

        return SUCCESS;
    }

    //
    //
    //
    public int getProductId() {
        return productId;
    }

    public void setProductId(int productId) {
        if (productId <= 0)
            productId = 0;
        this.productId = productId;
    }

    public int getNum() {
        return num;
    }

    public void setNum(int num) {
        if (num < 0)
            num = 0;
        this.num = num;
    }

}
