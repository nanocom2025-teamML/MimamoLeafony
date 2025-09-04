from fastapi import APIRouter

from app.services.recognize_audio_service import convert_to_wav, recognize_voice


router = APIRouter(prefix="/api/device", tags=["device"])


@router.post("/messages")
def post_device_message(audio_data: list[int]):
    text: str = recognize_voice(convert_to_wav(audio_data))
    print(text)

    return text


@router.post("/touch")
def post_device_touch():
    pass
