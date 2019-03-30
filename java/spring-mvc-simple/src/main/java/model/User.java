package model;

import java.io.Serializable;
import java.util.List;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.Transient;

import org.hibernate.Session;

import util.HibernateUtil;
import util.Status;
import util.Wrap;

import com.fasterxml.jackson.annotation.JsonIgnore;

@Entity
public class User implements Serializable {
    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "ID")
    private int id;

    @Column(name = "NAME")
    private String name;

    @Column(name = "PASSWORD")
    private String password;

    @Column(name = "EMAIL")
    private String email;

    @Column(name = "ROLE")
    private String role;

    @JsonIgnore
    @Transient
    private Cart cart = new Cart();

    //
    //
    //
    public void cloneFrom(User user) {
        name = user.name;
        password = user.password;
        email = user.email;
        role = user.role;
    }

    //
    //
    //
    @JsonIgnore
    public boolean isCustomer() {
        return role.equals("customer");
    }

    @JsonIgnore
    public boolean isAdmin() {
        return role.equals("admin");
    }

    @JsonIgnore
    public boolean isValid() {
        return (name != null && !name.equals(""))
                && (password != null && !password.equals(""))
                && (email != null && !email.equals(""))
                && (role != null && (role.equals("customer") || role
                        .equals("admin")));
    }

    //
    //
    //
    public static List<User> list() {
        Session session = HibernateUtil.open();
        List<User> r = session.createCriteria(User.class).list();
        session.close();
        return r;
    }

    public static void add(User user) {
        Session session = HibernateUtil.open();
        User u = (User) HibernateUtil.get(session, User.class, user.id);
        if (u == null) {
            HibernateUtil.save(session, user);
        }
        else {
            u.cloneFrom(user);
            HibernateUtil.update(session, u);
        }
        session.close();
    }

    public static void remove(User user) {
        Session session = HibernateUtil.open();
        HibernateUtil.delete(session,
                HibernateUtil.get(session, User.class, user.id));
        session.close();
    }

    public static Status authenticate(Wrap<User> wrap) {
        User u = getUserByName(wrap.get().getName());
        if (u == null) {
            return Status.USER_NOT_EXIST;
        }
        if (!u.password.equals(wrap.get().password)) {
            return Status.WRONG_PASSWORD;
        }
        wrap.set(u);
        return Status.SUCCESS;
    }

    public static User getUserByName(String name) {
        Session session = HibernateUtil.open();
        User r = (User) HibernateUtil.findFirst(session, User.class, "name",
                name);
        session.close();
        return r;
    }

    //
    //
    //
    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public String getRole() {
        return role;
    }

    public void setRole(String role) {
        this.role = role;
    }

    public Cart getCart() {
        return cart;
    }

    public void setCart(Cart cart) {
        this.cart = cart;
    }
}
