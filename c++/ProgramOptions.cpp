#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include <boost/program_options.hpp>
#include <boost/variant.hpp>

namespace po = boost::program_options;

// g++ ProgramOptions.cpp -lboost_program_options

template<typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T> const& v) {
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(os, " "));
    return os;
}


struct Configuration {
    int Optimization;
    std::vector<std::string> IncludePaths;
    std::vector<std::string> InputFiles;
};

std::ostream& operator<<(std::ostream& os, Configuration const& c) {
    os << "Optimization : " << c.Optimization << std::endl
       << "Include paths : " << c.IncludePaths << std::endl
       << "Input files : " << c.InputFiles << std::endl;
    return os;
}


boost::variant<bool, Configuration> ParseConfig(int ac, char const* const* av) {
    std::string configFile;

    po::options_description cmdOnlyOpts("Command line only options");
    cmdOnlyOpts.add_options()
        ("help,h", "Print help message")
        ("config,c", po::value<std::string>(&configFile)->default_value("/etc/example.cfg"), "Configuration file path")
        ;

    po::options_description commonOpts("Options");
    commonOpts.add_options()
        ("optimization", po::value<int>()->default_value(10), "Optimization level")
        ("include-path,I", po::value<std::vector<std::string>>()->composing(), "Include path")
        ;

    po::options_description hiddenOpts("Hidden options");
    hiddenOpts.add_options()
        ("input-file", po::value<std::vector<std::string>>(), "Input files")
    ;

    po::options_description cmdOpts;
    cmdOpts.add(cmdOnlyOpts).add(commonOpts).add(hiddenOpts);

    po::options_description configFileOpts;
    configFileOpts.add(commonOpts).add(hiddenOpts);

    po::options_description visibleOpts;
    visibleOpts.add(cmdOnlyOpts).add(commonOpts);

    // positional_options_description& add(const char * name, int max_count);
    // Species that up to 'max_count' next positional options should be given the 'name'.
    // The value of '-1' means 'unlimited'. No calls to 'add' can be made after call with 'max_value' equal to '-1'.
    po::positional_options_description p;
    p.add("input-file", -1);

    // Usually, the value stored first is preferred.
    // This is what happens for the "--optimization" option.
    // For "composing" options, like "include-file", the values are merged.
    po::variables_map vm;
    store(po::command_line_parser(ac, av).options(cmdOpts).positional(p).run(), vm);
    notify(vm);

    if (vm.count("help")) {
        std::cout << "Usage: " << av[0] << " [options] input-files" << std::endl
                  << visibleOpts << std::endl;
        return true;
    }

    if (!configFile.empty()) {
        std::ifstream ifs(configFile);
        store(parse_config_file(ifs, commonOpts), vm);
    }

    Configuration config;
    if (vm.count("optimization")) {
        config.Optimization = vm["optimization"].as<int>();
    }

    if (vm.count("include-path")) {
        config.IncludePaths = vm["include-path"].as<std::vector<std::string>>();
    }

    if (vm.count("input-file")) {
        config.InputFiles = vm["input-file"].as<std::vector<std::string>>();
    } else {
        std::cerr << "Missing input file" << std::endl;
        return false;
    }

    return config;
}


int main(int argc, char* argv[]) {
    try {
        boost::variant<bool, Configuration> tmpconfig = ParseConfig(argc, argv);
        if (bool* result = boost::get<bool>(&tmpconfig)) {
            return *result ? EXIT_SUCCESS : EXIT_FAILURE;
        }

        Configuration const config = boost::get<Configuration>(tmpconfig);
        std::cout << config << std::endl;
    } catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
    }
}
