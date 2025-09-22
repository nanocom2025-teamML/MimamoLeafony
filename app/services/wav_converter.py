import wave


CHANNELS = 1
SAMPLE_WIDTH = 2
FRAME_RATE = 16000


def bin2wav(bin_path, wav_path):
    with open(bin_path, "rb") as f:
        pcm_data = f.read()

    with wave.open(wav_path, "wb") as wf:
        wf.setnchannels(CHANNELS)
        wf.setsampwidth(SAMPLE_WIDTH)
        wf.setframerate(FRAME_RATE)
        wf.writeframes(pcm_data)
