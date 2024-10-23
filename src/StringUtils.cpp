#include "StringUtils.h"
void nextSubstr(String &s, char delim, String &match, String &leftover) {
    for (int i = 0;; i++) {
        if (i >= static_cast<int>(s.length())) return;
        char next = s.charAt(i);
        if (next == delim) {
            // get rid of delimeter
            i++;
            // dump rest of string into leftover
            for (;; i++) {
                if (i >= static_cast<int>(s.length())) break;
                char next = s.charAt(i);
                leftover.concat(next);
            }
            return;
        }
        match.concat(next);
    }
}