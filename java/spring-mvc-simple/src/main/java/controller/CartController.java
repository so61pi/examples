package controller;

import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;

import model.Product;
import model.User;

import org.springframework.http.HttpStatus;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.bind.annotation.ResponseStatus;

import util.ClientStatus;

@Controller
@RequestMapping("/cart")
public class CartController {
    @RequestMapping(method = RequestMethod.GET)
    public String get(Model model, HttpServletRequest request) {
        HttpSession session = request.getSession();
        synchronized (session) {
            User user = (User) session.getAttribute("user");
            if (user.isCustomer()) {
                return "customer/view-cart";
            }
            return "login";
        }
    }

    @RequestMapping(value = "/list", method = RequestMethod.GET)
    @ResponseBody
    public List<model.Package> checkCart(ModelMap model,
            HttpServletRequest request) {
        HttpSession session = request.getSession();
        synchronized (session) {
            User user = (User) session.getAttribute("user");
            return user.getCart().getPackages();
        }
    }

    @RequestMapping(value = "/add", method = RequestMethod.POST)
    @ResponseStatus(value = HttpStatus.OK)
    public void add(@RequestBody Product product, ModelMap model,
            HttpServletRequest request) {
        HttpSession session = request.getSession();
        synchronized (session) {
            User user = (User) session.getAttribute("user");
            user.getCart().addProduct(product, 1);
        }
    }

    @RequestMapping(value = "/remove", method = RequestMethod.POST)
    @ResponseStatus(value = HttpStatus.OK)
    public void remove(@RequestBody Product product, ModelMap model,
            HttpServletRequest request) {
        HttpSession session = request.getSession();
        synchronized (session) {
            User user = (User) session.getAttribute("user");
            user.getCart().removeProduct(product);
        }
    }

    @RequestMapping(value = "/check-out", method = RequestMethod.POST)
    @ResponseBody
    public ClientStatus checkout(ModelMap model, HttpServletRequest request) {
        ClientStatus status = new ClientStatus();
        HttpSession session = request.getSession();
        synchronized (session) {
            User user = (User) session.getAttribute("user");
            status.setStatus("success");
            status.setErrors(user.getCart().checkOut());
            return status;
        }
    }
}
