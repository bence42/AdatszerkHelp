//
// Created by rolan on 2019. 10. 09..
//

#ifndef KHF02_TRAIN_HPP
#define KHF02_TRAIN_HPP

#include <iostream>
#include <stack>
#include <queue>

bool is_orderable(const std::vector<short> &cars, bool stack, bool queue) {
    std::vector<short> outputV = {};
    std::vector<short> count = {};
    short curr = 1;
    for (auto i = 0; i < 4; ++i) {
        count.push_back(-1);
    }
    for (auto &car: cars) {
        count[car - 1] = 0;
    }
    for (auto &car : cars) {
        ++count[car - 1];
    }
    for (short i = 0; i < 4; ++i) {
        if (count[i] != 0 && count[i] != -1) {
            curr = i + 1;
            break;
        }
    }

    if (stack) {
        std::stack<short> myStack;
        for (auto &car : cars) {
            if (car == 1) {
                outputV.push_back(car);
                count[0]--;
                if (count[0] == 0) {
                    curr++;
                }
            } else if ((count[0] == 0 || count[0] == -1) && car == 2) {
                outputV.push_back(car);
                count[1]--;
                if (count[1] == 0) {
                    curr++;
                }
            } else if ((count[0] == 0 || count[0] == -1) && (count[1] == 0|| count[1] == -1) && car == 3) {
                outputV.push_back(car);
                count[2]--;
                if (count[2] == 0) {
                    curr++;
                }
            } else if ((count[0] == 0 || count[0] == -1) && (count[1] == 0|| count[1] == -1) && (count[2] == 0 || count[2] == -1) && car == 4) {
                outputV.push_back(car);
                count[3]--;
            } else {
                myStack.push(car);
                count[car - 1]--;
            }
            if (count[curr - 1] == -1) {
                curr++;
            }
            while (!myStack.empty() && myStack.top() == curr) {
                outputV.push_back(myStack.top());
                myStack.pop();
                if (!myStack.empty()) {
                    if (count[curr - 1] == 0 && myStack.top() != curr) {
                        curr++;
                    }
                }else if (curr != 4){
                    curr++;
                }
            }
        }
    } else if (queue) {
        std::queue<int> myQueue;
        for (auto &car : cars) {
            if (car == 1) {
                outputV.push_back(car);
                count[0]--;
                if (count[0] == 0) {
                    curr++;
                }
            } else if ((count[0] == 0 || count[0] == -1) && car == 2) {
                outputV.push_back(car);
                count[1]--;
                if (count[1] == 0) {
                    curr++;
                }
            } else if ((count[0] == 0 || count[0] == -1) && (count[1] == 0|| count[1] == -1) && car == 3) {
                outputV.push_back(car);
                count[2]--;
                if (count[2] == 0) {
                    curr++;
                }
            } else if ((count[0] == 0 || count[0] == -1) && (count[1] == 0|| count[1] == -1) && (count[2] == 0 || count[2] == -1) && car == 4) {
                outputV.push_back(car);
                count[3]--;
            } else {
                myQueue.push(car);
                count[car - 1]--;
            }
            if (count[curr - 1] == -1) {
                curr++;
            }
            while (!myQueue.empty() && myQueue.front() == curr) {
                outputV.push_back(myQueue.front());
                myQueue.pop();
                if (!myQueue.empty()) {
                    if (count[curr - 1] == 0 && myQueue.front() != curr) {
                        curr++;
                    }
                }else if (curr != 4){
                    curr++;
                }
            }
        }
    }
    return outputV.size() == cars.size();
}

#endif //KHF02_TRAIN_HPP
