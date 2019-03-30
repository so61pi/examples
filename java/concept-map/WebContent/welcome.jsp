<%@page import="utils.HibernateUtil"%>
<%@page import="org.hibernate.Session"%>
<%@page import="cm.User"%>

<% Integer userId = (Integer) session.getAttribute("userId");
   if (userId == null) {
       response.sendRedirect("login.jsp");
   }
   else {
       Session hibernateSession = HibernateUtil.getSessionFactory().openSession();
       
       User user = (User) HibernateUtil.get(hibernateSession, User.class, userId);
       if (user.getRole() == 0) {
	       %>
	       <%@include file="/WEB-INF/pages/student-welcome.html"%>
	       <%
       }
	   else {
	       %>
	       <%@include file="/WEB-INF/pages/lecturer-welcome.html"%>
	       <%
	   }
       
       hibernateSession.close();
   }
%> 
