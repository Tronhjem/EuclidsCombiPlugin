//
//  TransportData.h
//  EuclidsCombinator
//
//  Created by Christian Tronhjem on 30.03.25.
//

#ifndef TransportData_h
#define TransportData_h

struct TransportData
{
    double bpm = 0.0;
    int64_t timeInSamples = 0;
    int sampleRate = 0;
    bool isPlaying = false;

//    double time = 0.0;
//    double ppq = 0.0;
//    int bar = 0;
//    int beat = 0;
};

#endif /* TransportData_h */
