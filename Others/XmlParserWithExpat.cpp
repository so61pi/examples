#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <memory>
#include <regex>
#include <string>
#include <utility>
#include <vector>

#define XML_NS
#include <expat.h>


bool IsValidName(std::string const& name) {
    if (name.empty()) return false;

    auto IsFirstValid = [](char c) -> bool { return std::isalpha(c) || c == '_'; };
    auto IsValid = [&](char c) -> bool { return IsFirstValid(c) || std::isdigit(c) || c == '.' || c == '-'; };

    if (!IsFirstValid(name[0])) return false;
    for (auto i = 1; i < name.size(); ++i) {
        if (!IsValid(name[i])) return false;
    }
    return true;
}


inline bool IsValidNsUri(std::string const& uri) {
    // old gcc version cannot handle this regex and throw exception, be careful
    static std::regex const rule(R"(^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?)");
    return std::regex_match(uri, rule);
}


inline bool IsValidNsPrefix(std::string const& prefix) {
    // TODO
    return prefix.empty() || IsValidName(prefix);
}


inline bool IsValidNamespace(std::string const& prefix, std::string const& uri) {
    return IsValidNsPrefix(prefix) && IsValidNsUri(uri);
}


inline bool IsValidNamespace(std::pair<std::string, std::string> const& ns) {
    return IsValidNamespace(ns.first, ns.second);
}


inline bool IsValidNamespaces(std::map<std::string, std::string> const& namespaces) {
    for (auto const& ns : namespaces) {
        if (!IsValidNamespace(ns)) return false;
    }
    return true;
}


std::string SimplifyString(std::string const& input) {
    std::string output;
    for (auto& c : input) {
        switch (c) {
        case '"':
            output += "&quot;";
            break;

        case '&':
            output += "&amp;";
            break;

        case '\'':
            output += "&apos;";
            break;

        case '<':
            output += "&lt;";
            break;

        case '>':
            output += "&gt;";
            break;

        default:
            output += c;
        }
    }
    return output;
}


enum class NodeType {
    Element,
    String,
};


class XmlParser;


class Node {
    friend class XmlParser;

public:
    /**
     */
    explicit Node(NodeType type, std::string input, Node* parent) : mType(type), mParent(parent) {
        switch (mType) {
        case NodeType::Element:
            assert(IsValidName(input));
            mName = std::move(input);
            break;

        case NodeType::String:
            mString = std::move(input);
            break;

        default:
            throw std::runtime_error("unknown node type");
        }
    }


    /**
     * @brief Constructor for node.
     */
    explicit Node(std::string name, std::string const& uri, Node* parent)
        : mType(NodeType::Element), mParent(parent), mName(std::move(name))
    {
        if (!IsValidName(mName)) throw std::runtime_error("invalid name");
        if (!IsValidNsUri(uri)) throw std::runtime_error("invalid uri");

        if (parent == nullptr) throw std::runtime_error("cannot look-up namespace prefix");
        auto prefixes = parent->LookupNsPrefix(uri);
        if (prefixes.empty()) throw std::runtime_error("cannot look-up namespace prefix");
        mNsPrefix = prefixes.front();
    }


    /**
     * @brief Constructor for node.
     */
    explicit Node(std::string name, std::pair<std::string, std::string> ns, Node* parent)
        : mType(NodeType::Element), mParent(parent), mName(std::move(name))
    {
        if (!IsValidName(mName)) throw std::runtime_error("invalid name");
        if (!IsValidNamespace(ns)) throw std::runtime_error("invalid namespace");

        mNsPrefix              = std::move(ns.first);
        mNamespaces[mNsPrefix] = std::move(ns.second);
    }


    /**
     */
    explicit Node(std::string name, std::map<std::string, std::string> namespaces, std::string const& uri, Node* parent)
        : mType(NodeType::Element), mParent(parent), mName(std::move(name)), mNamespaces(std::move(namespaces))
    {
        if (!IsValidName(mName)) throw std::runtime_error("invalid name");
        if (!IsValidNamespaces(mNamespaces)) throw std::runtime_error("invalid namespaces");
        if (!IsValidNsUri(uri)) throw std::runtime_error("invalid uri");

        auto prefixes = LookupNsPrefix(uri);
        if (prefixes.empty()) throw std::runtime_error("cannot find namespace");
        mNsPrefix = prefixes.front();
    }


    /**
     */
    explicit Node(std::string name, std::map<std::string, std::string> namespaces, Node* parent)
        : mType(NodeType::Element), mParent(parent), mName(std::move(name)), mNamespaces(std::move(namespaces))
    {
        if (!IsValidName(mName)) throw std::runtime_error("invalid name");
        if (!IsValidNamespaces(mNamespaces)) throw std::runtime_error("invalid namespaces");
    }


    NodeType           Type() const { return mType; }
    std::string const& Name() const { return mName; }
    std::string const& String() const { return mString; }
    std::string const& NsPrefix() const { return mNsPrefix; }
    Node const*        Parent() const { return mParent; }

private: // for XML parser
    Node* Parent() { return mParent; }


public:
    bool GetNsUri(std::string& uri) const {
        if (!IsValidNsUri(uri)) return false;

        if (LookupNsUri(mNsPrefix, uri) == false) {
            assert(mNsPrefix.empty());
            return true;
        }
        return true;
    }


    bool AddNamespace(std::string prefix, std::string uri) {
        if (!IsValidNamespace(prefix, uri)) return false;
        if (mNamespaces.find(prefix) != mNamespaces.end()) return false;

        mNamespaces[std::move(prefix)] = std::move(uri);
        return true;
    }


    bool AddNamespace(std::pair<std::string, std::string> ns) {
        return AddNamespace(std::move(ns.first), std::move(ns.second));
    }


    bool AddAttribute(std::string key, std::string value) {
        if (key.empty() || mAttrs.find(key) != mAttrs.end()) return false;

        mAttrs[std::move(key)] = std::move(value);
        return true;
    }


    /**
     */
    bool FindAttr(std::string const& key, std::string& value) const {
        auto it = mAttrs.find(key);
        if (it == mAttrs.end()) return false;
        value = it->second;
        return true;
    }


    bool LookupAttr(std::string const& key, std::string& value) const {
        if (FindAttr(key, value) == true) return true;
        if (mParent) return mParent->LookupAttr(key, value);
        return false;
    }


    /**
     */
    std::vector<std::string> FindNsPrefix(std::string const& uri) const {
        std::vector<std::string> prefixes;
        if (!IsValidNsUri(uri)) return prefixes;

        for (auto const& ns : mNamespaces) {
            if (ns.second == uri) prefixes.push_back(ns.first);
        }
        return prefixes;
    }


    std::vector<std::string> LookupNsPrefix(std::string const& uri) const {
        if (!IsValidNsUri(uri)) return std::vector<std::string>();

        auto prefixes = FindNsPrefix(uri);
        if (!prefixes.empty()) return prefixes;
        if (mParent) return mParent->LookupNsPrefix(uri);
        return prefixes; // prefixes is already empty
    }


    /**
     */
    bool FindNsUri(std::string const& prefix, std::string& uri) const {
        if (!IsValidNamespace(prefix, uri)) return false;

        auto it = mNamespaces.find(prefix);
        if (it == mNamespaces.end()) return false;
        uri = it->second;
        return true;
    }


    bool LookupNsUri(std::string const& prefix, std::string& uri) const {
        if (!IsValidNamespace(prefix, uri)) return false;

        if (FindNsUri(prefix, uri) == true) return true;
        if (mParent) return mParent->LookupNsUri(prefix, uri);
        return false;
    }


    /**
     */
    Node* AddChildNode(Node&& node) {
        if (mType == NodeType::String) return nullptr;
        if (node.Parent() != this) return nullptr;

        mVals.push_back(std::unique_ptr<Node>(new Node(std::move(node))));
        return mVals.back().get();
    }


    Node* AddChildNode(std::unique_ptr<Node>& pNode) {
        if (mType == NodeType::String) return nullptr;
        if (!pNode) return nullptr;
        if (pNode->Parent() != this) return nullptr;

        mVals.push_back(std::unique_ptr<Node>(nullptr));
        mVals.back().reset(pNode.release());
        return mVals.back().get();
    }


    Node* AddChildString(std::string str) {
        if (mType == NodeType::String) return nullptr;

        mVals.push_back(std::unique_ptr<Node>(new Node(NodeType::String, std::move(str), this)));
        return mVals.back().get();
    }


    /**
     */
    std::size_t size() const {
        switch (mType) {
        case NodeType::Element:
            return mVals.size();

        case NodeType::String:
            return mString.size();

        default: // WTF
            std::abort();
        }
    }


    bool empty() const {
        switch (mType) {
        case NodeType::Element:
            return mVals.empty();

        case NodeType::String:
            return mString.empty();

        default: // WTF
            std::abort();
        }
    }


    /**
     */
    std::size_t asize() const { return mAttrs.size(); }
    bool        aempty() const { return mAttrs.empty(); }

    std::size_t nsize() const { return mNamespaces.size(); }
    bool        nempty() const { return mNamespaces.empty(); }


    /**
     */
    std::vector<std::unique_ptr<Node>>::const_iterator begin() const { return mVals.begin(); }
    std::vector<std::unique_ptr<Node>>::const_iterator end() const { return mVals.end(); }

    std::map<std::string, std::string>::const_iterator abegin() const { return mAttrs.begin(); }
    std::map<std::string, std::string>::const_iterator aend() const { return mAttrs.end(); }

    std::map<std::string, std::string>::const_iterator nbegin() const { return mNamespaces.begin(); }
    std::map<std::string, std::string>::const_iterator nend() const { return mNamespaces.end(); }


private:
    NodeType mType;
    Node*    mParent;

    std::string                        mName;
    std::string                        mString;
    std::vector<std::unique_ptr<Node>> mVals;

    std::string mNsPrefix;
    std::map<std::string, std::string> mNamespaces; //!< std::map<prefix, uri>
    std::map<std::string, std::string> mAttrs;      //!< std::map<key, value>
};


/**
 */
bool xmlize(Node const& node, std::ostream& os, std::size_t level = 0) {
    if (node.Type() == NodeType::String) {
        os << std::string(level, ' ') << SimplifyString(node.String()) << '\n';
        return true;
    }

    auto CreateNodeName = [](Node const& node) -> std::string {
        if (node.NsPrefix().empty()) {
            return node.Name();
        } else {
            return node.NsPrefix() + ':' + node.Name();
        }
    };

    std::string nodeName = CreateNodeName(node);

    // node name
    os << std::string(level, ' ') << '<' << nodeName;

    // add namespaces
    for (auto it = node.nbegin(); it != node.nend(); ++it) {
        if (it->first.empty()) {
            os << " xmlns=\"" << it->second << '"';
        } else {
            os << " xmlns:" << it->first << "=\"" << it->second << '"';
        }
    }

    // add attrs
    for (auto it = node.abegin(); it != node.aend(); ++it) {
        os << ' ' << it->first << "=\"" << it->second << '"';
    }

    // node end
    if (node.empty()) {
        os << " />\n";
    } else {
        os << ">\n";

        for (auto it = node.begin(); it != node.end(); ++it) {
            if (xmlize(*(it->get()), os, level + 4) == false) return false;
        }

        os << std::string(level, ' ') << "</" << nodeName << ">\n";
    }
    return true;
}


/**
 */
class XmlParser {
private:
    enum class StopReason {
        InvalidXmlFormat,
        InvalidParserDataState,
        Exception
    };

    static const XML_Char NamespaceSeparator = 0xFF;

    /**
     */
    class XmlParserRAII {
    public:
        XmlParserRAII(XML_Char namespaceSeparator) {
            mParser = XML_ParserCreateNS(nullptr, namespaceSeparator);
            if (!mParser) throw std::runtime_error("cannot create xml parser");
        }

        XmlParserRAII(XML_Parser parser) : mParser(parser) {}

        XmlParserRAII(XmlParserRAII const&) = delete;
        XmlParserRAII(XmlParserRAII&&) = delete;
        XmlParserRAII& operator=(XmlParserRAII const&) = delete;
        XmlParserRAII& operator=(XmlParserRAII&&) = delete;

        XML_Parser Parser() const {
            return mParser;
        }

        ~XmlParserRAII() {
            XML_ParserFree(mParser);
        }

    private:
        XML_Parser mParser;
    };


    /**
     */
    struct ParserData {
        explicit ParserData(XML_Parser parser, std::unique_ptr<Node>& root)
            : parser(parser), root(root), parent(nullptr), reason(StopReason::InvalidXmlFormat) {}

        XML_Parser             parser;
        std::unique_ptr<Node>& root;

        Node* parent;
        std::map<std::string, std::string> namespaces; //!< prefix, uri

        StopReason reason;
    };


    /**
     * @brief Get namespace uri and node name from full node name.
     */
    static void separate(char const* fullName, char separator, std::string& uri, std::string& name) {
        auto len   = std::strlen(fullName);
        auto first = fullName;
        auto last  = fullName + len;
        auto xpos  = std::find(first, last, separator);
        if (xpos == last) {
            name = std::string(first, last);
        } else {
            uri  = std::string(first, xpos);
            name = std::string(xpos + 1, last);
        }
    }


    /**
     */
    static void StartElementHandler(void* userData, XML_Char const* fullName, XML_Char const** attrs) {
        auto pData = reinterpret_cast<ParserData*>(userData);
        try {
            std::string uri;
            std::string name;
            separate(fullName, NamespaceSeparator, uri, name);

            std::unique_ptr<Node> pNode;
            if (uri.empty()) {
                pNode = std::unique_ptr<Node>(new Node(name, std::move(pData->namespaces), pData->parent));
            } else {
                pNode = std::unique_ptr<Node>(new Node(name, std::move(pData->namespaces), uri, pData->parent));
            }

            // reset container state after moving
            pData->namespaces.clear();

            for (auto i = 0; attrs[i]; i += 2)
                pNode->AddAttribute(attrs[i], attrs[i + 1]);

            if (pData->parent == nullptr) {
                pData->root   = std::move(pNode);
                pData->parent = pData->root.get();
            } else {
                pData->parent = pData->parent->AddChildNode(pNode);
                if (pData->parent == nullptr) {
                    XML_StopParser(pData->parser, XML_FALSE);
                    pData->reason = StopReason::InvalidParserDataState;
                    return;
                }
            }
        } catch (...) {
            XML_StopParser(pData->parser, XML_FALSE);
            pData->reason = StopReason::Exception;
        }
    }


    static void EndElementHandler(void* userData, XML_Char const* name) {
        auto pData = reinterpret_cast<ParserData*>(userData);
        try {
            if (!pData->namespaces.empty()) {
                XML_StopParser(pData->parser, XML_FALSE);
                pData->reason = StopReason::InvalidParserDataState;
                return;
            }

            if (pData->parent) pData->parent = pData->parent->Parent();
        } catch (...) {
            XML_StopParser(pData->parser, XML_FALSE);
            pData->reason = StopReason::Exception;
        }
    }


    /**
     */
    static void CharacterDataHandler(void* userData, XML_Char const* str, int len) {
        auto pData = reinterpret_cast<ParserData*>(userData);
        try {
            if (!pData->namespaces.empty()) {
                XML_StopParser(pData->parser, XML_FALSE);
                pData->reason = StopReason::InvalidParserDataState;
                return;
            }

            bool ignore = true;
            for (auto i = str; i != str + len; ++i) {
                if (*i != ' ' && *i != '\t' && *i != '\n' /* 0x0A */) {
                    ignore = false;
                    break;
                }
            }
            if (ignore) return;

            if (pData->parent->AddChildString(std::string(str, str + len)) == nullptr) {
                XML_StopParser(pData->parser, XML_FALSE);
                pData->reason = StopReason::InvalidParserDataState;
                return;
            }
        } catch (...) {
            XML_StopParser(pData->parser, XML_FALSE);
            pData->reason = StopReason::Exception;
        }
    }


    /**
     */
    static void StartNsDeclHandler(void* userData, XML_Char const* prefix, XML_Char const* uri) {
        auto pData = reinterpret_cast<ParserData*>(userData);
        try {
            if (prefix == nullptr)
                pData->namespaces[""] = uri;
            else
                pData->namespaces[prefix] = uri;
        } catch (...) {
            XML_StopParser(pData->parser, XML_FALSE);
            pData->reason = StopReason::Exception;
        }
    }


    static void EndNsDeclHandler(void* userData, XML_Char const* prefix) {
        auto pData = reinterpret_cast<ParserData*>(userData);
        try {
            if (!pData->namespaces.empty()) {
                XML_StopParser(pData->parser, XML_FALSE);
                pData->reason = StopReason::InvalidParserDataState;
                return;
            }
        } catch (...) {
            XML_StopParser(pData->parser, XML_FALSE);
            pData->reason = StopReason::Exception;
        }
    }


public:
    std::unique_ptr<Node> Parse(std::string const& data) {
        auto parser = XML_ParserCreateNS(nullptr, NamespaceSeparator);
    if (!parser) return std::unique_ptr<Node>();
        XmlParserRAII raii(parser);

        std::unique_ptr<Node> root;
        ParserData            parserData(raii.Parser(), root);

        XML_SetUserData(raii.Parser(), reinterpret_cast<void*>(&parserData));
        XML_SetElementHandler(raii.Parser(), StartElementHandler, EndElementHandler);
        XML_SetNamespaceDeclHandler(raii.Parser(), StartNsDeclHandler, EndNsDeclHandler);
        XML_SetCharacterDataHandler(raii.Parser(), CharacterDataHandler);

        if (XML_Parse(raii.Parser(), data.c_str(), data.size(), 1) != XML_STATUS_OK) {
            switch (parserData.reason) {
            case StopReason::InvalidXmlFormat:
                std::cerr << "\n"
                      << "********************************\n"
                      << "[Invalid XML format]\n"
                      << "Failed at (" << XML_GetCurrentLineNumber(parser) << ", " << XML_GetCurrentColumnNumber(parser) << ")\n"
                      << "********************************\n";
                break;

            case StopReason::InvalidParserDataState:
                std::cerr << "\n"
                      << "********************************\n"
                      << "[Invalid Parser State Data]\n"
                      << "Failed at (" << XML_GetCurrentLineNumber(parser) << ", " << XML_GetCurrentColumnNumber(parser) << ")\n"
                      << "********************************\n";
                break;

            case StopReason::Exception:
                std::cerr << "\n"
                      << "********************************\n"
                      << "[Exception]\n"
                      << "Failed at (" << XML_GetCurrentLineNumber(parser) << ", " << XML_GetCurrentColumnNumber(parser) << ")\n"
                      << "********************************\n";
                break;

            default:
                std::cerr << "\n"
                      << "********************************\n"
                      << "[Unknown reason]\n"
                      << "Failed at (" << XML_GetCurrentLineNumber(parser) << ", " << XML_GetCurrentColumnNumber(parser) << ")\n"
                      << "********************************\n";
            }
        }

        return root;
    }
};


int main() {
    char const* text =
        R"(
<_.abc xmlns="default:default" xmlns:u="kupi:so61pix" xmlns:h="kupi:so61pix" x="y" y="z">
    <u:l0 xmlns:h="kupi:so61pix" xmlns:k="kupi:so61pi">
        <u:k>ddd:ddd</u:k>
    </u:l0>
    <u:def>
        <u:k>ddd:<foo/>ddd</u:k>
        <exampleOfACDATA>
        <![CDATA[
            Since this is a CDATA section
            I can use all sorts of reserved characters
            like > < " and &
            or write things like
            <foo></bar>
            but my document is still well formed!
        ]]>
        </exampleOfACDATA>
    </u:def>
</_.abc>
)";

    std::cout << XML_ExpatVersion() << std::endl;

    XmlParser p;
    if (auto n = p.Parse(text)) {
        xmlize(*n, std::cout);
    }
}
