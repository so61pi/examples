<% Integer userId = (Integer) session.getAttribute("userId");
   if (userId == null) {
       %>
       <%@include file="/WEB-INF/pages/register.html"%>
       <%
   }
   else {
       response.sendRedirect("welcome.jsp");
   }
%> 