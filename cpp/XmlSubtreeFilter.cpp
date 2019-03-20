#include <algorithm>
#include <cstring>
#include <iostream>
#include <iterator>
#include <vector>

#include "pugixml.hpp"

using namespace pugi;


size_t child_size(xml_node node) {
    return std::distance(node.begin(), node.end());
}


void stfilter(xml_node filter, xml_node data, bool& rm_data) {
    if (child_size(filter) == 0) {
        return;
    }

    std::vector<xml_node> dcnodes;
    for (xml_node dc = data.first_child(); dc; dc = dc.next_sibling()) {
        dcnodes.push_back(dc);
    }

    std::vector<xml_node> ithis;
    std::vector<xml_node> iall;
    for (xml_node fc = filter.first_child(); fc; fc = fc.next_sibling()) {
        if (fc.first_child().type() == node_pcdata) {
            xml_node dc = data.child(fc.name());
            if (dc.first_child().type() == node_pcdata && std::strcmp(dc.first_child().value(), fc.first_child().value()) == 0) {
                iall.push_back(dc);
            } else {
                // remove-this-data-node = true
                rm_data = true;
                return;
            }
        } else {
            for (xml_node dc = data.child(fc.name()); dc; dc = dc.next_sibling(fc.name())) {
                bool rm_dc = false;
                stfilter(fc, dc, rm_dc);
                if (!rm_dc) {
                    ithis.push_back(dc);
                }
            }
        }
    }

    if (ithis.empty()) {
        if (iall.empty()) {
            // remove everything
            for (auto it = dcnodes.begin(); it != dcnodes.end(); ++it) {
                data.remove_child(*it);
            }

            // remove-this-data-node = true
            rm_data = true;
            return;
        } else {
            return;
        }
    } else {
        auto it = std::remove_if(dcnodes.begin(), dcnodes.end(),
            [&](xml_node node) -> bool {
                return std::find(ithis.begin(), ithis.end(), node) != ithis.end() ||
                       std::find(iall.begin(), iall.end(), node) != iall.end();
            });

        dcnodes.erase(it, dcnodes.end());

        for (auto it = dcnodes.begin(); it != dcnodes.end(); ++it) {
            data.remove_child(*it);
        }
    }
}


int main() {
    xml_document filter_doc;
    xml_document text_doc;

    filter_doc.load_string(
        R"(
            <users>
                <user>
                    <name>abc</name>
                    <info/>
                </user>
            </users>
        )");
    text_doc.load_string(
        R"(
            <users>
                <user>
                    <name>abc</name>
                    <age>10</age>
                    <info>
                        <mother>mom</mother>
                        <father>dad</father>
                    </info>
                </user>
                <user>
                    <name>def</name>
                    <age>20</age>
                    <info>
                        <brother>bro</brother>
                        <sister>sis</sister>
                    </info>
                </user>
            </users>
        )");

    bool kill = false;
    stfilter(filter_doc, text_doc, kill);

    text_doc.save(std::cout);
}
