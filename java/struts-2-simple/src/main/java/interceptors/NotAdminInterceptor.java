package interceptors;

import java.util.Map;

import com.opensymphony.xwork2.ActionInvocation;
import com.opensymphony.xwork2.interceptor.AbstractInterceptor;

import core.User;

public class NotAdminInterceptor extends AbstractInterceptor {

    @Override
    public String intercept(ActionInvocation invocation) throws Exception {
        Map session = (Map) invocation.getInvocationContext().get("session");
        User user = (User) session.get("user");
        if (!user.getUserName().equals("administrator")) {
            return "to-home-page";
        }

        return invocation.invoke();
    }
}