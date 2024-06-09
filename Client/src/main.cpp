//
//  Created by Thanh Hoang on 5/6/2024.
//  Copyright © 20124 Thanh Hoang. All rights reserved.
//
#include "PCH.h"
int main(int argc, char *argv[]) {
    auto app = Piero::Application::GetInstance();
    app->Run([]() {});
    app->ShutDown();
    return 0;
}