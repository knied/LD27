//
//  Sound.cpp
//  LD27
//
//  Created by Kristof Niederholtmeyer on 19.08.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Sound.h"

////////////////////////////////////////////////////////////////////////////////

Sound::Sound(const std::string& name) {
    std::string resource_name = ":/" + name + ".wav";
    _sound_effect.setSource(QUrl::fromLocalFile(resource_name.c_str()));
}

void Sound::setGain(float gain) {
    _sound_effect.setVolume(gain);
}

float Sound::gain() const {
    return _sound_effect.volume();
}

void Sound::play(int loops) {
    _sound_effect.setLoopCount(loops);
    _sound_effect.play();
}

bool Sound::is_playing() const {
    return _sound_effect.isPlaying();
}

////////////////////////////////////////////////////////////////////////////////
