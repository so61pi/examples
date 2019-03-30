package servlets;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import utils.HibernateUtil;
import cm.User;

/**
 * Servlet implementation class LoginServlet
 */
@WebServlet("/login")
public class LoginServlet extends HttpServlet {
    private static final long serialVersionUID = 1L;

    /**
     * @see HttpServlet#HttpServlet()
     */
    public LoginServlet() {
        super();
        // TODO Auto-generated constructor stub
    }

    /**
     * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse
     *      response)
     */
    @Override
    protected void doGet(HttpServletRequest request,
            HttpServletResponse response) throws ServletException, IOException {
        String userName = request.getParameter("username");
        String password = request.getParameter("password");
        String role = request.getParameter("role");

        HttpSession httpSession = request.getSession();
        synchronized (httpSession) {
            Integer userId = (Integer) httpSession.getAttribute("userId");
            if (userId != null) {
                // already logged in
                response.sendRedirect("welcome.jsp");
            }
            else if (userName != null && password != null && role != null) {
                int userRole = 0;
                if (role.equals("student"))
                    userRole = 0;
                else
                    userRole = 1;

                if (User.authenticate(userName, password, userRole)) {
                    User user = (User) HibernateUtil.findFirst(User.class,
                            "userName", userName);
                    httpSession.setAttribute("userId", user.getUserId());
                    response.sendRedirect("welcome.jsp");
                }
                else {
                    response.sendRedirect("login.jsp");
                }
            }
            else {
                response.sendRedirect("login.jsp");
            }
        }
    }

    /**
     * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse
     *      response)
     */
    @Override
    protected void doPost(HttpServletRequest request,
            HttpServletResponse response) throws ServletException, IOException {
        doGet(request, response);
    }

}
