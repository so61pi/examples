package actions;

import java.util.Map;

import org.apache.struts2.convention.annotation.Action;
import org.apache.struts2.convention.annotation.InterceptorRef;
import org.apache.struts2.convention.annotation.InterceptorRefs;
import org.apache.struts2.convention.annotation.Result;

import com.opensymphony.xwork2.ActionContext;
import com.opensymphony.xwork2.ActionSupport;

@InterceptorRefs({ @InterceptorRef("notLoggedinStack") })
@Action(value = "check-info", results = {
        @Result(name = "success", location = "check-info.jsp"),
        @Result(name = "error", location = "error.jsp") })
public class CheckInfoAction extends ActionSupport {

    @Override
    public String execute() {
        Map session = (Map) ActionContext.getContext().get("session");
        Map request = (Map) ActionContext.getContext().get("request");

        request.put("user", session.get("user"));

        return SUCCESS;
    }
}
