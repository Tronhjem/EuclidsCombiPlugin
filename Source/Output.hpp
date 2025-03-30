//
//  Output.hpp
//
//  Created by Christian Tronhjem on 29.03.25.
//

#pragma once

#include "Track.hpp"

class Output
{
public:
    Output(int channel);
    
private:
    int mChannelOut;
    Track mTrack;
    
};
