//
// Created by tang on 4/10/18.
//

#include <zconf.h>
#include <iostream>
#include "../NORM/NormManager.h"

using namespace std;
using namespace DCCast;

int main() {
    NormManager &instance = NormManager::get_instance();

    instance.receive(0, 54321);
    instance.send(1, "10.0.0.1", 54322, 30000);

    vector<unsigned int> receivers, senders;

    for (int i = 0; i < 10; i++) {
        receivers.clear();
        senders.clear();

        instance.active_transfers(senders, receivers);

        cout << "Receivers: ";
        for (auto id : receivers) {
            cout << id << ' ';
        }
        cout << endl;

        cout << "Senders ";
        for (auto id : senders) {
            cout << id << ' ';
        }
        cout << endl;

        sleep(1);

    }

    instance.terminate(0);

    for (int i = 0; i < 10; i++) {
        receivers.clear();
        senders.clear();

        instance.active_transfers(senders, receivers);

        cout << "Receivers: ";
        for (auto id : receivers) {
            cout << id << ' ';
        }
        cout << endl;

        cout << "Senders ";
        for (auto id : senders) {
            cout << id << ' ';
        }
        cout << endl;

        sleep(1);

    }
}