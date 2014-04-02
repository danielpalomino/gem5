/**
 * An output visitor class like Text, but it writes the stats to a stringstream.
 *
 * @author Sascha Friedmann <sascha.friedmann@student.kit.edu>
 */
#ifndef __BASE_STATS_TEXTSTREAM_HH__
#define __BASE_STATS_TEXTSTREAM_HH__

#include <sstream>

#include "base/stats/text.hh"

namespace Stats {

class TextStream : public Output
{
public:
    TextStream() : text(stream) {};

    virtual void begin() {
        text.begin();
    }

    virtual void end() {
        text.end();
    }

    virtual bool valid() const {
        return text.valid();
    }

    virtual void visit(const ScalarInfo &info) {
        text.visit(info);
    }

    virtual void visit(const VectorInfo &info) {
        text.visit(info);
    }

    virtual void visit(const DistInfo &info) {
        text.visit(info);
    }

    virtual void visit(const VectorDistInfo &info) {
        text.visit(info);
    }

    virtual void visit(const Vector2dInfo &info) {
        text.visit(info);
    }

    virtual void visit(const FormulaInfo &info) {
        text.visit(info);
    }

    virtual void visit(const SparseHistInfo &info) {
        text.visit(info);
    }

    virtual std::string str() {
        std::string str = stream.str();
        stream.str(""); // clear stream
        return str;
    }

private:
    std::ostringstream stream;
    Text text;
};

TextStream *initTextStream();

}

#endif /* __BASE_STATS_TEXTSTREAM_HH__ */
