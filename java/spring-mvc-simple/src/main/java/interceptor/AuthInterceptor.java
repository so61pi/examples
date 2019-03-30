package interceptor;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import model.User;

import org.springframework.web.servlet.handler.HandlerInterceptorAdapter;

public class AuthInterceptor extends HandlerInterceptorAdapter {
    @Override
    public boolean preHandle(HttpServletRequest request,
            HttpServletResponse response, Object handler) throws Exception {
        HttpSession session = request.getSession();
        synchronized (session) {
            // temporary hack
            if (User.getUserByName("admin") == null) {
                User u = new User();
                u.setName("admin");
                u.setPassword("admin");
                u.setEmail("admin@example.com");
                u.setRole("admin");
                User.add(u);
            }
            User u = (User) session.getAttribute("user");
            String redirect = go(request, u);
            if (redirect == null) {
                return true;
            }
            response.sendRedirect(redirect);
            return false;
        }
    }

    public static String go(HttpServletRequest request, User user) {
        if (user == null) {
            return anonymousGo(request);
        }
        else if (user.isAdmin()) {
            return adminGo(request);
        }
        else if (user.isCustomer()) {
            return customerGo(request);
        }
        return null;
    }

    public static String adminGo(HttpServletRequest request) {
        final String allowList[] = { "/product.*", "/user.*", "/logout",
                "/about" };
        if (isAllowed(request.getContextPath(), allowList,
                request.getRequestURI()))
            return null;
        return request.getContextPath() + "/product";
    }

    public static String customerGo(HttpServletRequest request) {
        final String allowList[] = { "/product.*", "/cart.*", "/logout",
                "/about" };
        if (isAllowed(request.getContextPath(), allowList,
                request.getRequestURI()))
            return null;
        return request.getContextPath() + "/product";
    }

    public static String anonymousGo(HttpServletRequest request) {
        final String allowList[] = { "/login", "/about" };
        if (isAllowed(request.getContextPath(), allowList,
                request.getRequestURI()))
            return null;
        return request.getContextPath() + "/login";
    }

    public static boolean isAllowed(String contextPath, String[] allowedPaths,
            String expect) {
        for (String path : allowedPaths) {
            if (expect.matches(contextPath + path))
                return true;
        }
        return false;
    }
}