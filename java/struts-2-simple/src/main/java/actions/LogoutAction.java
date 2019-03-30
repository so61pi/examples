package actions;

import java.util.Map;

import org.apache.struts2.convention.annotation.Action;
import org.apache.struts2.convention.annotation.InterceptorRef;
import org.apache.struts2.convention.annotation.InterceptorRefs;
import org.apache.struts2.convention.annotation.Result;

import com.opensymphony.xwork2.ActionContext;
import com.opensymphony.xwork2.ActionSupport;

@InterceptorRefs({ @InterceptorRef("notLoggedinStack") })
@Action(value = "logout", results = {
        @Result(name = "success", type = "redirect", location = "/login"),
        @Result(name = "error", location = "error.jsp") })
public class LogoutAction extends ActionSupport {
    @Override
    public String execute() {
        Map session = (Map) ActionContext.getContext().get("session");
        session.remove("user");

        return SUCCESS;
    }
}
