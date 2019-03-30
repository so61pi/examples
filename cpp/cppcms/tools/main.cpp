#include <exception>
#include <iostream>

#include <cppcms/application.h>
#include <cppcms/applications_pool.h>
#include <cppcms/http_response.h>
#include <cppcms/service.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>

#include "templates/content.h"

#include "main.h"

class hello : public cppcms::application {
public:
    hello(cppcms::service& srv)
        : cppcms::application(srv)
    {
        dispatcher().assign("", &hello::intro, this);
        mapper().assign("");

        dispatcher().assign("/news", &hello::news, this);
        mapper().assign("news", "/news");

        dispatcher().assign("/page", &hello::page, this);
        mapper().assign("page", "/page");

        mapper().root("/hello");
    }

    // virtual void main(std::string url)
    // {
    //     content::message c;
    //     c.text=">>>Hello<<<";
    //     render("message",c);
    // }

    void ini(content::master& c, content::MenuIdentity active)
    {
        content::Menu menu;
        menu.emplace_back(content::MenuItem{ "HOME", url("/"), content::MenuIdentity::Home });
        menu.emplace_back(content::MenuItem{ "Page", url("/page"), content::MenuIdentity::Page });
        menu.emplace_back(content::MenuItem{ "News", url("/news"), content::MenuIdentity::News });
        menu.emplace_back(content::MenuItem{ "Item 3", "#", content::MenuIdentity::Invalid }
                          .Add(content::MenuItem{ "Item 1", "#", content::MenuIdentity::Invalid })
                          .Add(content::MenuItem{ "Item 2", "#", content::MenuIdentity::Invalid }
                               .Add(content::MenuItem{ "Item 1", "#", content::MenuIdentity::Invalid }))
                          .Add(content::MenuItem{ "Item 3", "#", content::MenuIdentity::Invalid }));
        menu.emplace_back(content::MenuItem{ "Item 4", "#", content::MenuIdentity::Invalid });

        c.title = "My Web Site";
        c.menu = menu;
        c.active = active;
    }

    void intro()
    {
        content::master c;
        ini(c, content::MenuIdentity::Home);
        c.panelTitle = "Home";
        render("intro", c);
    }

    void page()
    {
        content::page c;
        ini(c, content::MenuIdentity::Page);
        c.panelTitle = "About";
        c.page_content = "<p>A page about this web site</p>";
        render("page", c);
    }

    void news()
    {
        content::news c;
        ini(c, content::MenuIdentity::News);
        c.panelTitle = "News";
        c.news_list.push_back("This is the latest message!");
        c.news_list.push_back("This is the next message.");
        c.news_list.push_back("This is the last message!");
        render("news", c);
    }

private:
};

int main(int argc, char** argv)
{
    try {
        std::cout << "Server is starting" << std::endl;
        cppcms::service srv(argc, argv);
        srv.applications_pool().mount(cppcms::applications_factory<hello>());
        srv.run();
    } catch (std::exception const& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}
