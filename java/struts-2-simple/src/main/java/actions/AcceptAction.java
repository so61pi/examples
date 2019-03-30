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
@Action(value = "accept", results = {
        @Result(name = "success", location = "view-result.jsp"),
        @Result(name = "error", location = "error.jsp") })
public class AcceptAction extends ActionSupport {

    @Override
    public String execute() {
        Map session = (Map) ActionContext.getContext().get("session");
        Map request = (Map) ActionContext.getContext().get("request");

        // productId, name, quality, status
        List<Tuple<Integer, String, Integer, String, Double>> list = new LinkedList<Tuple<Integer, String, Integer, String, Double>>();

        Map<Integer, Integer> cart = (Map<Integer, Integer>) session
                .get("cart");

        Session sess = HibernateUtil.getSessionFactory().openSession();
        for (Map.Entry<Integer, Integer> entry : cart.entrySet()) {
            Product p = (Product) HibernateUtil.get(sess, Product.class,
                    entry.getKey());

            Tuple<Integer, String, Integer, String, Double> x;
            if (p.getQuality() < entry.getValue()) {
                x = new Tuple<Integer, String, Integer, String, Double>(
                        p.getProductId(), p.getName(), entry.getValue(), "Out",
                        null);
            }
            else {
                x = new Tuple<Integer, String, Integer, String, Double>(
                        p.getProductId(), p.getName(), entry.getValue(), "OK",
                        null);
                p.setQuality(p.getQuality() - entry.getValue());
                HibernateUtil.update(sess, p);
            }
            list.add(x);
        }
        sess.close();

        request.put("list", list);

        cart.clear();

        return SUCCESS;
    }
}
