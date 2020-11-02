#include <iostream>

using namespace std;

//
// Created by Victor MENDELE on 13/10/2020.
//

#ifndef MASK_MESSAGEREPLACER_H
#define MASK_MESSAGEREPLACER_H


class MessageReplacer {
private:
    string lastMessage;

public:
    MessageReplacer(): lastMessage() {}
    void replace(const string & message) {
        const auto messageLength = lastMessage.length();

        for (auto i = 0; i < messageLength; i += 1)
            cout << "\b" << flush;

        cout << message;
        lastMessage = message;
    }

    void clear() {
        replace("");
    }
};


#endif //MASK_MESSAGEREPLACER_H
