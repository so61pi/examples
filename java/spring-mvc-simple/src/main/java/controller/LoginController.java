package controller;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;

import model.User;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import util.ClientStatus;
import util.Wrap;

@Controller
public class LoginController {
    @RequestMapping(value = { "/login", "/" }, method = RequestMethod.GET)
    public String get(Model model, HttpServletRequest request) {
        model.addAttribute("user", new User());
        return "login";
    }

    @RequestMapping(value = "/login", method = RequestMethod.POST)
    @ResponseBody
    public ClientStatus login(@RequestBody User user, ModelMap model,
            HttpServletRequest request) {
        ClientStatus status = new ClientStatus();
        HttpSession session = request.getSession();
        synchronized (session) {
            Wrap<User> wrap = new Wrap<User>(user);
            switch (User.authenticate(wrap)) {
            case SUCCESS:
                session.setAttribute("user", wrap.get());
                status.setStatus("success");
                break;

            case USER_NOT_EXIST:
                status.setStatus("error");
                status.addError("name", "User does NOT exist.");
                break;

            case WRONG_PASSWORD:
                status.setStatus("error");
                status.addError("password", "Wrong password.");
                break;

            default:
                break;
            }
            return status;
        }
    }

    @RequestMapping(value = "/logout", method = RequestMethod.GET)
    public String logout(Model model, HttpServletRequest request) {
        HttpSession session = request.getSession();
        synchronized (session) {
            session.removeAttribute("user");
        }
        return "redirect:login";
    }
}
