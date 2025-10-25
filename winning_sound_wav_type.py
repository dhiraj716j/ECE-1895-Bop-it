# pip install numpy soundfile

import numpy as np
import soundfile as sf

# Define frequencies for notes (C major)
notes = {
    "C5": 523.25, "D5": 587.33, "E5": 659.25, "F5": 698.46,
    "G5": 783.99, "A5": 880.00, "B5": 987.77, "C6": 1046.50
}

# Helper: create sine-wave notes
def make_note(note, duration=0.3, volume=0.3, sample_rate=44100):
    t = np.linspace(0, duration, int(sample_rate * duration), endpoint=False)
    wave = np.sin(2 * np.pi * notes[note] * t) * volume
    # Fade in/out
    fade_len = int(sample_rate * 0.02)
    wave[:fade_len] *= np.linspace(0, 1, fade_len)
    wave[-fade_len:] *= np.linspace(1, 0, fade_len)
    return wave

# Melody sequence
melody_notes = [
    "C5","E5","G5","C6",
    "G5","E5","C5",
    "E5","G5","A5","G5",
    "F5","E5","D5","C5",
    "C6","G5","C6"
]

# Combine notes
song = np.concatenate([make_note(n) for n in melody_notes])
# Add pause + final chord
song = np.concatenate([song, np.zeros(int(44100 * 0.3))])
for n in ["C6", "E5", "G5"]:
    song = np.concatenate([song, make_note(n, duration=0.4, volume=0.4)])

# Export
sf.write("win_theme_8bit.wav", song, 44100)
print("✅ Created 'win_theme_8bit.wav' in your current folder!")

