//
//  painting_table.cpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/3/24.
//

#include "painting_table.hpp"
#include "drawlines.hpp"
#include <iostream>
using namespace table;
int flushTheWindow(GLFWwindow* window){
    if (lines.size()&1) return -1;
    //std::cout<<"momery"<<std::endl;
    for (std::vector<fPoint>::iterator it = lines.begin(); it != lines.end(); it++){
        fPoint s = *it,t = *(it+1); ++it;
        //std::cout<<s.first<<" "<<s.second<<" "<<t.first<<" "<<t.second<<std::endl;
        drawLine(s.first, s.second, t.first, t.second);
    }
    return 0 ;
}
