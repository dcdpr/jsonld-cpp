#include "IriUtils.h"

#pragma clang diagnostic push
#pragma GCC diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#include "uri.hpp"
#pragma clang diagnostic pop
#pragma GCC diagnostic pop

#include <vector>

namespace {

    std::vector<std::string> split(std::string input, char separator) {
        std::vector<std::string> segments;
        std::vector<std::string>::size_type i;

        while((i = input.find(separator)) != std::string::npos) {
            segments.push_back(input.substr(0, i));
            input.erase(0, i+1);
        }
        segments.push_back(input);
        return segments;
    }

    std::string join(const std::vector<std::string>& input, char separator) {
        std::string result;
        auto it = input.cbegin();
        auto end = input.cend();
        if(it != end) {
            result.append(*it);
            ++it;
        }
        for(; it != end; ++it) {
            result.append(1, separator);
            result.append(*it);
        }
        return result;
    }

}

std::string IriUtils::removeDotSegments(const std::string& path, bool hasAuthority) {

    std::vector<std::string> segments = split(path, '/');
    std::vector<std::string> result;

    std::string leadingSlash;
    if(path.find('/') == 0) {
        leadingSlash = "/";
    }

    while(!segments.empty()) {
        std::string s = segments.front();

        if (s == "." || s.empty()) {
            segments.erase(segments.begin());
            // if "." is the last thing in the path, add "" to force a trailing "/"
            //if(s == "." && segments.empty())
            if(segments.empty())
                result.emplace_back("");
            continue;
        }
        if (s == "..") {
            segments.erase(segments.begin());
//            if(hasAuthority || (!result.empty() && result.back() != "..")) {
//            if(!result.empty() && result.back() != "..") {
            if(!result.empty()) {
                result.pop_back();
            }
            // if ".." is the last thing in the path, add "" to force a trailing "/"
            if(s == ".." && segments.empty())
                result.emplace_back("");
            continue;
        }
        result.push_back(segments.front());
        segments.erase(segments.begin());
    }

    return leadingSlash + join(result, '/');
}

std::string IriUtils::prependBase(std::string base, std::string iri) {

    if(base.empty()) {
        return iri;
    }

    if(iri.empty()) {
        return base;
    }

    std::string finalIriScheme;
    std::string finalIriAuthority;
    std::string finalIriPath;
    std::string finalIriQuery;
    std::string finalIriFragment;

    uri parsedBase(base);

    finalIriScheme = parsedBase.get_scheme();

    uri parsedIri(iri);

    if(!parsedIri.get_scheme().empty()) {
        finalIriScheme = parsedIri.get_scheme();
        finalIriAuthority = parsedIri.get_host();
        finalIriPath = parsedIri.get_path();
        finalIriQuery = parsedIri.get_query();
        finalIriFragment = parsedIri.get_fragment();
    }
    else {
        if (!parsedIri.get_host().empty()) {
            finalIriAuthority = parsedIri.get_host();
            finalIriPath = parsedIri.get_path();
            finalIriQuery = parsedIri.get_query();
        } else {

            if (parsedIri.get_path().empty()) {
                finalIriPath = parsedBase.get_path();
                if (!parsedIri.get_query().empty())
                    finalIriQuery = parsedIri.get_query();
                else
                    finalIriQuery = parsedBase.get_query();
            } else {
                if (parsedIri.get_path_is_rooted()) {
                    // IRI represents an absolute path
                    finalIriPath = parsedIri.get_path();
                } else {
                    // merge paths
                    std::string basePath = parsedBase.get_path();
                    std::string iriPath = parsedIri.get_path();

                    // append relative path to the end of the last directory from base
                    if (!iriPath.empty()) {
                        // remove last segment from basePath
                        std::string::size_type i = basePath.find_last_of('/');
                        basePath = basePath.substr(0, i);

                        if (!basePath.empty() && basePath.back() != '/') {
                            basePath.push_back('/');
                        }

                        basePath += iriPath;
                    }
                    finalIriPath = basePath;
                }
                finalIriQuery = parsedIri.get_query();
            }
            finalIriAuthority = parsedBase.get_host();
        }
        finalIriFragment = parsedIri.get_fragment();
    }

    finalIriPath = removeDotSegments(finalIriPath, false);

    std::string result;

    if(!finalIriScheme.empty())
        result += finalIriScheme + ":";

    if(!finalIriAuthority.empty())
        result += "//" + finalIriAuthority;

    if(!finalIriPath.empty()) {
        if (finalIriPath[0] == '/')
            result += finalIriPath;
        else
            result += "/" + finalIriPath;
    }
    else {
        // if finalIriPath is empty after removingDots, but the original was not, then we need a trailing slash
        if(!parsedIri.get_path().empty())
            result += "/";
    }

    if(!finalIriQuery.empty())
        result += "?" + finalIriQuery;

    if(!finalIriFragment.empty())
        result += "#" + finalIriFragment;

    return result;
}

