from fastapi import APIRouter, Depends
from sqlalchemy.orm import Session
from datetime import datetime

from app.database import get_db
from app.models import Message
from app.services.recognize_audio_service import convert_to_wav, recognize_voice


router = APIRouter(prefix="/api/device", tags=["device"])


@router.post("/messages")
def post_device_message(data: dict):
    audio_data = data["audio_data"]
    text: str = recognize_voice(convert_to_wav(audio_data))
    print(text)

    return text


@router.post("/touch")
def post_device_touch(db: Session = Depends(get_db)):
    # 未読メッセージの取得
    unread_messages = db.query(Message).filter(Message.read_at.is_(None)).all()

    if not unread_messages:
        return {"messages": []}

    # レスポンス用のデータ
    response_data = [
        {
            "id": msg.id,
            "target_id": msg.target_id,
            "content": msg.content,
            "read_at": msg.read_at,
            "created_at": msg.created_at,
            "updated_at": msg.updated_at,
        }
        for msg in unread_messages
    ]

    # 既読に変更する
    now = datetime.utcnow()
    for msg in unread_messages:
        msg.read_at = now
    db.commit()

    return {"messages": response_data}
