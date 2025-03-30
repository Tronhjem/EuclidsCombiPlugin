//
//  Output.hpp
//
//  Created by Christian Tronhjem on 29.03.25.
//

#ifndef Output_hpp
#define Output_hpp

#include "Track.hpp"

class Output
{
public:
    Output(int channel);
    
private:
    int mChannelOut;
    Track mTrack;
    
};

#endif /* Output_hpp */
