package controller;

import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;

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
@RequestMapping("/user")
public class UserController {
    @RequestMapping(method = RequestMethod.GET)
    public String user(Model model, HttpServletRequest request) {
        HttpSession session = request.getSession();
        synchronized (session) {
            User user = (User) session.getAttribute("user");
            if (user.isAdmin()) {
                return "admin/users-list";
            }
            return "404";
        }
    }

    @RequestMapping(value = "/list", method = RequestMethod.GET)
    @ResponseBody
    public List<User> list() {
        return User.list();
    }

    @RequestMapping(value = "/add", method = RequestMethod.POST)
    @ResponseBody
    public ClientStatus add(@RequestBody User user, ModelMap model,
            HttpServletRequest request) {
        ClientStatus status = new ClientStatus();
        HttpSession session = request.getSession();
        synchronized (session) {
            if (user.isValid()) {
                User.add(user);
                status.setStatus("success");
            }
            else {
                status.setStatus("error");
            }
            return status;
        }
    }

    @RequestMapping(value = "/remove", method = RequestMethod.POST)
    @ResponseStatus(value = HttpStatus.OK)
    public void remove(@RequestBody User user, ModelMap model,
            HttpServletRequest request) {
        HttpSession session = request.getSession();
        synchronized (session) {
            User.remove(user);
        }
    }
}
