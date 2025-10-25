import numpy as np
import soundfile as sf

#Frequencies
NOTES = {
    "C5": 523.25, "D5": 587.33, "E5": 659.25, "F5": 698.46,
    "G5": 783.99, "A5": 880.00, "B5": 987.77, "C6": 1046.50,
    "D6": 1174.66, "E6": 1318.51, "G6": 1567.98
}

SAMPLE_RATE = 44100

def make_note(note, duration=0.25, volume=0.35, waveform="square"):
    """Generate a waveform note with fade in/out."""
    freq = NOTES[note]
    t = np.linspace(0, duration, int(SAMPLE_RATE * duration), endpoint=False)

 
    if waveform == "square":
        wave = np.sign(np.sin(2 * np.pi * freq * t))
    elif waveform == "triangle":
        wave = 2 * np.abs(2 * (t * freq - np.floor(t * freq + 0.5))) - 1
    elif waveform == "saw":
        wave = 2 * (t * freq - np.floor(0.5 + t * freq))
    else:
        wave = np.sin(2 * np.pi * freq * t)

    wave *= volume


    fade_len = int(SAMPLE_RATE * 0.02)
    wave[:fade_len] *= np.linspace(0, 1, fade_len)
    wave[-fade_len:] *= np.linspace(1, 0, fade_len)
    return wave

def make_drum(duration=0.12, volume=0.5):
    """Generate a short percussive pop (like a sushi chop)."""
    t = np.linspace(0, duration, int(SAMPLE_RATE * duration), endpoint=False)
    noise = np.random.uniform(-1, 1, len(t)) * volume
    envelope = np.exp(-10 * t)
    return noise * envelope


intro = ["C5", "E5", "G5", "C6"]               
main  = ["E5", "G5", "A5", "E5", "F5", "A5", "D6", "C6"]  
bridge = ["C5", "D5", "E5", "G5", "A5", "C6", "E6", "D6"] 
ending = ["G5", "A5", "B5", "C6", "E6", "G6", "C6"]       

# combine sections
melody = intro + main + bridge + ending

#guild the song
song = np.array([], dtype=np.float32)

for i, note in enumerate(melody):
    tone = make_note(note, duration=0.22, waveform="square")
    # add percussive "chops" every other note
    if i % 2 == 1:
        chop = make_drum(duration=0.08, volume=0.4)
        tone = np.concatenate([tone, chop])
    song = np.concatenate([song, tone])

song = np.concatenate([song, np.zeros(int(SAMPLE_RATE * 0.3))])


flourish_notes = ["C6", "E6", "G6"]
flourish = sum(make_note(n, duration=0.6, volume=0.25, waveform="triangle") for n in flourish_notes)
flourish /= len(flourish_notes)
song = np.concatenate([song, flourish])


song /= np.max(np.abs(song))


sf.write("sushi_chef_start_theme_longer.wav", song, SAMPLE_RATE)
