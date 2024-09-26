#include <iostream>
#include <string>
#include "CLI/CLI.hpp"
#include "spdlog/spdlog.h"

int main(int argc, char** argv) {

    CLI::App app("CLI11 Logging Example");
    
    std::string trace_message;
    std::string debug_message;
    std::string info_message;
    std::string warn_message;
    std::string error_message;
    std::string fatal_message;
    
    // add options for each logging level
    app.add_option("--trace", trace_message, "Log a trace message");
    app.add_option("--debug", debug_message, "Log a debug message");
    app.add_option("--info", info_message, "Log an info message");
    app.add_option("--warn", warn_message, "Log a warn message");
    app.add_option("--error", error_message, "Log an error message");
    app.add_option("--fatal", fatal_message, "Log a fatal message");

    // add option to set the log level
    std::vector<std::string> allowed_log_levels = {"trace", "debug", "info", "warn", "error", "fatal"};
    std::string log_level = "info";
    app.add_option("--log-level", log_level, "Set the log level")->check(CLI::IsMember(allowed_log_levels))->default_val(log_level);

    // parse the command line arguments
    CLI11_PARSE(app, argc, argv);

    // configure the logger
    //auto logger = spdlog::stderr_color_mt("console");  // deprecated??
    spdlog::set_level(spdlog::level::from_str(log_level));

    // log the message at the appropriate level
    if (!trace_message.empty()) {
        spdlog::trace(trace_message);
    }
    if (!debug_message.empty()) {
        spdlog::debug(debug_message);
    }
    if (!info_message.empty()) {
        spdlog::info(debug_message);
    }
    if (!warn_message.empty()) {
        spdlog::warn(debug_message);
    }
    if (!error_message.empty()) {
        spdlog::error(debug_message);
    }
    if (!fatal_message.empty()) {
        spdlog::critical(debug_message);
    }

    return 0;
}

