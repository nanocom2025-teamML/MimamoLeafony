# -*- coding: utf-8 -*-


import os
from dotenv import load_dotenv

import assemblyai as aai


# .env の読み込み
load_dotenv()

# API キーの設定
aai.settings.api_key = os.getenv('ASSEMBLYAI_API_KEY')


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

    if transcript.status == "error":
        raise RuntimeError(f"Transcription failed: {transcript.error}")

    return transcript.text
