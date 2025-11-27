#ifndef COMPLETER_H_
#define COMPLETER_H_

#include <functional>
#include <vector>
#include <string>
#include "inputtable.h"
#include "candidate.h"

namespace McFoxIM {

class Completer {
public:
    using TableProvider = std::function<const InputTable&()>;

    Completer(TableProvider provider);

    /**
     * Completes the given prefix string.
     *
     * @param prefix The prefix to complete.
     * @returns A list of candidates.
     */
    std::vector<Candidate> complete(const std::string &prefix);

private:
    TableProvider provider_;
};

} // namespace McFoxIM

#endif // COMPLETER_H_
