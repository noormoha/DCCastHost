//
// Created by tang on 4/10/18.
//

#include <unistd.h>
#include "../NORM/NormManager.h"

using namespace DCCast;
using namespace std;

int main() {
    NormManager &instance = NormManager::get_instance();

    instance.send(0, "10.0.0.2", 54321, 30000);
    instance.receive(1, 54322);

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