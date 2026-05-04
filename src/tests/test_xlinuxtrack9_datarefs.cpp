// test_xlinuxtrack9_datarefs.cpp
//
// Phase 0 invariant tests for the X-Plane plugin DataRef surface.
//
// xlinuxtrack9 cannot be unit-tested directly (it links the X-Plane SDK and
// runs inside X-Plane), but the registration/publish symmetry is a pure
// source-level invariant. If a DataRef is registered via
// XPLMRegisterDataAccessor() but not advertised to DataRefEditor via
// MSG_ADD_DATAREF, it silently disappears from the editor — a very common
// drift bug. This test guards the symmetry by parsing the source file.
//
// See doc/GAZE_TRACKING_ROADMAP.md for the gaze work this guards.

#include "catch2/catch_amalgamated.hpp"

#include <algorithm>
#include <fstream>
#include <iterator>
#include <regex>
#include <set>
#include <sstream>
#include <string>

namespace {

std::string read_file(const std::string &path) {
    std::ifstream in(path);
    std::stringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

std::set<std::string> match_all(const std::string &src, const std::regex &re) {
    std::set<std::string> names;
    auto begin = std::sregex_iterator(src.begin(), src.end(), re);
    auto end = std::sregex_iterator();
    for (auto it = begin; it != end; ++it) {
        names.insert((*it)[1].str());
    }
    return names;
}

std::string join(const std::set<std::string> &s) {
    std::string out;
    for (const auto &n : s) {
        if (!out.empty()) out += ", ";
        out += n;
    }
    return out;
}

} // namespace

TEST_CASE("xlinuxtrack9 DataRef registration / publish symmetry",
          "[xplane][datarefs]") {
    const std::string src = read_file(XLINUXTRACK9_SOURCE_PATH);
    REQUIRE_FALSE(src.empty());

    // XPLMRegisterDataAccessor("linuxtrack/<name>", ...
    // Custom delimiter R"RE(...)RE" because the patterns contain )" which
    // would terminate a default raw-string literal early.
    const std::regex register_re(
        R"RE(XPLMRegisterDataAccessor\(\s*"(linuxtrack/[A-Za-z_][A-Za-z0-9_]*)")RE");
    // XPLMSendMessageToPlugin(..., MSG_ADD_DATAREF, (void*)"linuxtrack/<name>")
    const std::regex publish_re(
        R"RE(MSG_ADD_DATAREF,\s*\(void\*\)\s*"(linuxtrack/[A-Za-z_][A-Za-z0-9_]*)")RE");

    const auto registered = match_all(src, register_re);
    const auto published  = match_all(src, publish_re);

    REQUIRE_FALSE(registered.empty());
    REQUIRE_FALSE(published.empty());

    SECTION("every registered DataRef is advertised to DataRefEditor") {
        std::set<std::string> only_registered;
        std::set_difference(registered.begin(), registered.end(),
                            published.begin(), published.end(),
                            std::inserter(only_registered,
                                          only_registered.begin()));
        INFO("registered but not advertised: " << join(only_registered));
        REQUIRE(only_registered.empty());
    }

    SECTION("every advertised DataRef is actually registered") {
        std::set<std::string> only_published;
        std::set_difference(published.begin(), published.end(),
                            registered.begin(), registered.end(),
                            std::inserter(only_published,
                                          only_published.begin()));
        INFO("advertised but not registered: " << join(only_published));
        REQUIRE(only_published.empty());
    }
}

TEST_CASE("Phase 0 gaze DataRefs are present and gated correctly",
          "[xplane][datarefs][gaze]") {
    const std::string src = read_file(XLINUXTRACK9_SOURCE_PATH);
    REQUIRE_FALSE(src.empty());

    SECTION("gaze_yaw and gaze_pitch are registered") {
        REQUIRE(src.find("\"linuxtrack/gaze_yaw\"")   != std::string::npos);
        REQUIRE(src.find("\"linuxtrack/gaze_pitch\"") != std::string::npos);
    }

    SECTION("LTR_GAZE_MOCK is defined and used") {
        REQUIRE(src.find("#define LTR_GAZE_MOCK")  != std::string::npos);
        REQUIRE(src.find("#if LTR_GAZE_MOCK")      != std::string::npos);
    }

    SECTION("mock writes to current_gaze_yaw / current_gaze_pitch") {
        // The mock must update the storage that the DataRef accessors point
        // at, not some other variable. Drift here means the DataRefs sit at
        // 0.0 even with the mock enabled.
        REQUIRE(src.find("current_gaze_yaw =")   != std::string::npos);
        REQUIRE(src.find("current_gaze_pitch =") != std::string::npos);
    }
}
