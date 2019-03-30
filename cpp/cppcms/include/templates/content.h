#ifndef CONTENT_H
#define CONTENT_H


#include <string>
#include <vector>

#include <cppcms/view.h>

namespace content {


    enum class MenuIdentity {
        Invalid,
        Home,
        Page,
        News
    };

    struct MenuItem;
    using Menu = std::vector<MenuItem>;

    class MenuItem {
    public:
        MenuItem() = default;

        explicit MenuItem(std::string const& name, std::string const& link, MenuIdentity identity)
            : mName(name)
            , mLink(link)
            , mIdentity(identity)
            {}

        MenuIdentity Identity() const { return mIdentity; }
        std::string const& Name() const { return mName; }
        std::string const& Link() const { return mLink; }
        Menu const& ChildItems() const { return mChildItems; }

        MenuItem& Add(MenuItem child) {
            mChildItems.emplace_back(std::move(child));
            return *this;
        }

    private:
        MenuIdentity mIdentity;
        std::string mName;
        std::string mLink;
        Menu mChildItems;
    };

    struct master : public cppcms::base_content {
        bool const cfgDemo = false;
        bool const cfgSearch = false;

        std::string  title;
        std::string  panelTitle;
        Menu         menu;
        MenuIdentity active;
    };

    struct news : public master {
        std::list<std::string> news_list;
    };

    struct page : public master {
        std::string page_title, page_content;
    };
}

#endif // CONTENT_H
