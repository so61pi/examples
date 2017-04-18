#include <cstdlib>
#include <iostream>
#include <string>

#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>


// g++ -std=c++11 Libxml2Example.cpp `xml2-config --cflags` `xml2-config --libs`


struct libxml2RAII {
    libxml2RAII() { LIBXML_TEST_VERSION }
    ~libxml2RAII() { xmlCleanupParser(); }
};


class xmlDocRAII {
public:
    explicit xmlDocRAII(xmlDocPtr doc) : mDoc(doc) {}
    ~xmlDocRAII() { xmlFreeDoc(mDoc); }

private:
    xmlDocPtr mDoc;
};


class XmlStringRAII {
public:
    explicit XmlStringRAII(xmlChar* mem) : mMem(mem) {}
    ~XmlStringRAII() { xmlFree(mMem); }

private:
    xmlChar* mMem;
};


void ProcessDocument(xmlDocPtr doc) {
    // Get root element of document.
    xmlNodePtr root = xmlDocGetRootElement(doc);

    // Get root element name.
    std::cout << "Root element name : " << root->name << std::endl;
}


void ParseXmlFromMemory(char const* buffer, std::size_t size, char const* encoding) {
    xmlDocPtr doc = xmlReadMemory(buffer, size, nullptr, encoding, 0);
    xmlDocRAII xmlDocRaii(doc);
    if (doc == nullptr) {
        std::cerr << "Malformed XML." << std::endl;
        return;
    }
    ProcessDocument(doc);
}


void ParseXmlFromFile(char const* filename, char const* encoding) {
    xmlDocPtr doc = xmlReadFile(filename, "UTF-8", 0);
    xmlDocRAII xmlDocRaii(doc);
    if (doc == nullptr) {
        std::cerr << "Malformed XML." << std::endl;
        return;
    }
    ProcessDocument(doc);
}


xmlDocPtr BuildXml() {
    /// Create a document.
    xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");

    /// Create root node, set it to document.
    xmlNodePtr root = xmlNewNode(nullptr, BAD_CAST "root");
    xmlDocSetRootElement(doc, root);

    /// Create first child node under root name child.
    /// It has namespace xmlns="http://www.example.com" and property key="value".
    xmlNodePtr child = xmlNewNode(nullptr, BAD_CAST "child");
    xmlNewNs(child, BAD_CAST "http://www.example.com", nullptr);
    xmlNewProp(child, BAD_CAST "key", BAD_CAST "value");
    xmlAddChild(root, child);

    /// Create a second child node name child2.
    /// It has a text element inside it.
    xmlNodePtr child2 = xmlNewNode(nullptr, BAD_CAST "child2");
    xmlNodePtr text = xmlNewText(BAD_CAST "text");
    xmlAddChild(child2, text);
    xmlAddChild(root, child2);

    /// The XML tree looks like:
    ///
    /// <?xml version="1.0"?>
    /// <root>
    ///     <child xmlns="http://www.example.com" key="value"/>
    ///     <child2>text</child2>
    /// </root>

    return doc;
}


void DumpXmlToFile(char const* filename, xmlDocPtr doc) {
    xmlSaveFile(filename, doc);
}


void DumpXmlToStream(std::ostream& os, xmlDocPtr doc) {
    xmlChar* mem = nullptr;
    int size = 0;
    xmlDocDumpMemory(doc, &mem, &size);
    XmlStringRAII xmlStringRaii(mem);
    if (mem) {
        os << mem;
    }
}


int main(int argc, char* argv[]) {
    /// https://mail.gnome.org/archives/xml/2009-December/msg00020.html
    ///
    /// If libxml2 detected a non-blank text node at the same level
    /// it will keep all further text nodes, assuming a mixed content element.
    ///
    /// To avoid that, we can use xmlFirstElementChild and xmlNextElementSibling
    /// although the text between the elements now are ignored.
    libxml2RAII libXmlRaii;

    xmlDocPtr doc = BuildXml();
    xmlDocRAII xmlDocRaii(doc);
    DumpXmlToStream(std::cout, doc);
}
