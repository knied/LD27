//
//  Sound.h
//  LD27
//
//  Created by Kristof Niederholtmeyer on 19.08.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//

#ifndef __LD27__Sound__
#define __LD27__Sound__

#include <iostream>
#include <QtMultimedia>

class Sound {
    QSoundEffect _sound_effect;
    
public:
    Sound(const std::string& name) {
        std::string resource_name = ":/" + name + ".wav";
        _sound_effect.setSource(QUrl::fromLocalFile(resource_name.c_str()));
    }
    
    void setGain(float gain) {
        _sound_effect.setVolume(gain);
    }
    float gain() const {
        return _sound_effect.volume();
    }
    
    void play(int loops) {
        _sound_effect.setLoopCount(loops);
        _sound_effect.play();
    }
    bool is_playing() const {
        return _sound_effect.isPlaying();
    }
    
};

#endif /* defined(__LD27__Sound__) */
