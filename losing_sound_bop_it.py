import numpy as np
from scipy.io.wavfile import write

#define note frequencies
notes = {
    "C5": 523.25, "D5": 587.33, "D#5": 622.25, "E5": 659.25,
    "F5": 698.46, "G5": 783.99, "A4": 440.00, "A#4": 466.16, "B4": 493.88,
    "C4": 261.63
}

def make_square_wave(note, duration=0.3, volume=0.4, sample_rate=44100):
    """Generate a retro 8-bit style square wave note."""
    t = np.linspace(0, duration, int(sample_rate * duration), endpoint=False)
    wave = np.sign(np.sin(2 * np.pi * notes[note] * t)) * volume
    fade_len = int(sample_rate * 0.02)
    wave[:fade_len] *= np.linspace(0, 1, fade_len)
    wave[-fade_len:] *= np.linspace(1, 0, fade_len)
    return wave

#short "sad fall" melody (descending minor tones)
melody_notes = [
    ("E5", 0.25),
    ("D#5", 0.25),
    ("D5", 0.25),
    ("C5", 0.35),
    ("A4", 0.5),
    ("F5", 0.4),
    ("C4", 0.8)
]

#build the melody
song = np.concatenate([make_square_wave(note, dur) for note, dur in melody_notes])

#add a quiet “fade out” tail
song = np.concatenate([song, np.zeros(int(44100 * 0.3))])

#export as WAV
write("lose_theme_8bit.wav", 44100, (song * 32767).astype(np.int16))


