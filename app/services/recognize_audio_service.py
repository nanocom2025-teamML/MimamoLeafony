# -*- coding: utf-8 -*-


import os, csv
import assemblyai as aai
import numpy as np
from scipy.io.wavfile import write

from app.config import ASSEMBLYAI_API_KEY


# API キーの設定
aai.settings.api_key = ASSEMBLYAI_API_KEY


def recognize_voice(audio_file: str) -> str:
    """
    音声を認識するための関数
    :param audio_file: 音声ファイル
    :return: 音声認識結果
    """
    config = aai.TranscriptionConfig(
        speech_model=aai.SpeechModel.best,
        language_code="ja",  # 日本語
    )

    transcript = aai.Transcriber(config=config).transcribe(audio_file)

    if transcript.error == aai.TranscriptStatus.error:
        print(transcript.error)

    if transcript.status == "error":
        raise RuntimeError(f"Transcription failed: {transcript.error}")

    return transcript.text


def convert_to_wav() -> None:
    # パラメータ
    path = "./audio_files/mic_record.wav"
    fs = 3000  # サンプリング周波数（Hz）
    bits = 10  # ADC分解能（ESP32: 12bit(0-4095)）

    adc_max = 2**bits - 1
    adc_center = 2**(bits - 1)

    mic_data = np.loadtxt("./audio_files/mic_record.csv", delimiter=",")

    # numpy.ndarray 型に変換
    audio_data = np.array(mic_data, dtype=np.int32)

    audio_data = audio_data - adc_center
    audio_data = np.int16(audio_data * (32767 / adc_center))

    os.makedirs(os.path.dirname(path), exist_ok=True)

    write(path, fs, audio_data)
    
    return(path)

def append_csv(mic_data: list[int]):
    """
    音声データをCSVに追記する関数
    """
    path = "./audio_files/mic_record.csv"
    os.makedirs(os.path.dirname(path), exist_ok=True)

    with open(path, mode="a", newline="") as f:
        writer = csv.writer(f)
        for sample in mic_data:
            writer.writerow([sample])

def delete_csv():
    path = "./audio_files/mic_record.csv"
    os.remove(path)
