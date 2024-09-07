#include "driver.h"
#include "parser.hh"
#include "scanner.h"

#include <fstream>
#include <string>


namespace Mreow
{
    // Parse a specific file using an absolute path.
    int Driver::parse(const char* filename)
    {
        // Tell the user what we're doing.
        std::cout << "-> parsing " << filename << "\n";

        // Open the file and verify we got a good filestream.
        std::ifstream input(filename);
        if (!input.good())
        {
            std::cerr << "couldn't open " << filename << "\n";
            return -1;
        }

        // Create a scanner object reading from the file and writing
        // parse issues directly to stderr.
        Scanner scanner{};
        scanner.switch_streams(input, std::cerr);

        // Create a parser using the scanner with the Driver as a context object.
        Parser parse(scanner, *this);

        // do the parsing.
        int result = parse();

        std::cout << "<- parsed " << filename << ", result is " << result << "\n";
        return result;
    }

}
