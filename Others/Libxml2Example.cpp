#include <cstdlib>
#include <iostream>
#include <string>

#include <libxml/parser.h>
#include <libxml/tree.h>


struct LibXmlRAII {
    LibXmlRAII() { LIBXML_TEST_VERSION }
    ~LibXmlRAII() { xmlCleanupParser(); }
};


class XmlDocRAII {
public:
    explicit XmlDocRAII(xmlDocPtr doc) : mDoc(doc) {}
    ~XmlDocRAII() { xmlFreeDoc(mDoc); }

private:
    xmlDocPtr mDoc;
};


void PrintElementNames(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr n) {
    if (!n) return;

    std::cout << "* Name     : " << n->name << std::endl;
    if (n->type == XML_TEXT_NODE) {
        auto content = xmlNodeGetContent(n);
        std::cout << "  Content  : " << content << std::endl;
    }
    std::cout << "  Type     : " << n->type << std::endl
              << "  Ns       : " << n->ns << " - " << (n->ns == ns ? "Recognized" : "Unrecognized") << std::endl
              << "  Children : " << xmlChildElementCount(n) << std::endl
              << std::endl;

    for (xmlNodePtr node = n->children; node; node = node->next) {
        PrintElementNames(doc, ns, node);
    }
}


int main(int argc, char *argv[]) {
    LibXmlRAII libXmlRAII;

    /// https://mail.gnome.org/archives/xml/2009-December/msg00020.html
    ///
    /// If libxml2 detected a non-blank text node at the same level
    /// it will keep all further text nodes, assuming a mixed content element.
    ///
    /// To avoid that, we can use xmlNextElementSibling and xmlFirstElementChild
    /// although the text between the elements now are ignored.
    std::string const data = R"(
            <a xmlns="http://example.org">
              <b1>
                <![CDATA[ cdata-1 ]]>
              </b1>
              <b2><![CDATA[ cdata-2 ]]></b2>
              <b3>text</b3>
              <![CDATA[Testing 0 < 0]]>
            </a>
        )";
    xmlDocPtr doc = xmlReadMemory(data.c_str(), data.size(), "noname.xml", "UTF-8",
                                  XML_PARSE_NOERROR | XML_PARSE_NOWARNING |
                                  XML_PARSE_NOBLANKS | XML_PARSE_NONET | XML_PARSE_NOCDATA);
    if (!doc) {
        std::cerr << "Malformed XML.\n";
        return EXIT_FAILURE;
    }
    XmlDocRAII docRaii{ doc };

    xmlNodePtr root = xmlDocGetRootElement(doc);
    xmlNsPtr ns = xmlSearchNsByHref(doc, root, BAD_CAST "http://example.org");
    PrintElementNames(doc, ns, root);
    return EXIT_SUCCESS;
}
